/*
 * This file is part of the PulseView project.
 *
 * Copyright (C) 2015 Joel Holdsworth <joel@airwebreathe.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <libsigrokcxx/libsigrokcxx.hpp>

#include <boost/filesystem.hpp>

#include "sessionfile.hpp"
#include <QFile>
#include <stdexcept>

using std::shared_ptr;
using std::string;

namespace pv {
namespace devices {

SessionFile::SessionFile(const shared_ptr<sigrok::Context> context,
	const string &file_name) :
	File(file_name),
	context_(context)
{
}

void SessionFile::open()
{
	if (session_)
		close();

	collection_.reset();
	temporary_.reset();
	if (CaptureCollection::is_collection(QString::fromStdString(file_name_))) {
		collection_.reset(new CaptureCollection(CaptureCollection::read(QString::fromStdString(file_name_))));
		temporary_.reset(new QTemporaryDir);
		if (!temporary_->isValid()) throw sigrok::Error(SR_ERR_MALLOC);
		abort_ = false;
		collection_->extract(QString::fromStdString(file_name_), 0, temporary_->filePath("first.sr"), abort_);
		session_ = context_->load_session(temporary_->filePath("first.sr").toStdString());
	} else
		session_ = context_->load_session(file_name_);
	device_ = session_->devices()[0];
	recorded_channels_.clear();
	if (collection_)
		for (const auto& channel : device_->channels())
			recorded_channels_.push_back({channel->index(), channel->type()->id(), channel->name(), channel->enabled()});
}

void SessionFile::close()
{
	stop();
	if (session_)
		session_->remove_devices();
	device_.reset();
	session_.reset();
	collection_.reset();
	temporary_.reset();
	recorded_channels_.clear();
}

void SessionFile::connect_datafeed(FeedCallback callback)
{
	if (!collection_) Device::connect_datafeed(std::move(callback));
	else datafeed_callback_ = std::move(callback);
}

void SessionFile::start()
{
	if (!collection_) { Device::start(); return; }
	abort_ = false;
}

void SessionFile::stop()
{
	if (!collection_) { if (session_) Device::stop(); return; }
	abort_ = true;
	std::lock_guard<std::recursive_mutex> lock(playback_mutex_);
	if (playback_) playback_->stop();
}

std::vector<std::string> SessionFile::frame_triggers() const
{
	std::vector<std::string> result;
	if (collection_)
		for (const auto& t : collection_->frames.at(current_frame_).triggers)
			result.push_back(t.toStdString());
	return result;
}

bool SessionFile::frame_origin(uint64_t& origin) const
{
	if (!collection_ || current_frame_ >= collection_->frames.size() ||
		!collection_->frames[current_frame_].has_origin) return false;
	origin = collection_->frames[current_frame_].origin;
	return true;
}

void SessionFile::run()
{
	if (!collection_) { Device::run(); return; }
	using sigrok::ConfigKey;
	using sigrok::Packet;
	using sigrok::PacketType;
	const auto send_outcome = [this](const QString& status, const QString& error) {
		datafeed_callback_(device_, context_->create_meta_packet({
			{ConfigKey::CAPTURE_STATUS, Glib::Variant<Glib::ustring>::create(status.toStdString())},
			{ConfigKey::CAPTURE_ERROR, Glib::Variant<Glib::ustring>::create(error.toStdString())}}));
	};
	try {
		for (current_frame_ = 0; current_frame_ < collection_->frames.size() && !abort_; ++current_frame_) {
			if (frame_callback_) frame_callback_(true);
			const auto& frame = collection_->frames[current_frame_];
			const auto path = current_frame_ ? temporary_->filePath("next.sr") : temporary_->filePath("first.sr");
			// Recheck each checksum, including the first frame, when replay starts.
			collection_->extract(QString::fromStdString(file_name_), current_frame_, path, abort_);
			auto playback = context_->load_session(path.toStdString());
			const auto device = playback->devices().at(0);
			const auto channels = device->channels();
			// Compare with the recorded layout, not UI edits applied by a .pvs
			// setup (which may rename or hide a channel before playback starts).
			if (channels.size() != recorded_channels_.size()) throw std::runtime_error("Collection channel layout changed.");
			for (size_t c = 0; c < channels.size(); ++c)
				if (channels[c]->type() != sigrok::ChannelType::LOGIC ||
					channels[c]->type()->id() != recorded_channels_[c].type ||
					channels[c]->index() != recorded_channels_[c].index || channels[c]->name() != recorded_channels_[c].name ||
					channels[c]->enabled() != recorded_channels_[c].enabled)
					throw std::runtime_error("Inconsistent digital collection channels.");
			uint64_t received = 0;
			bool ended = false;
			std::exception_ptr packet_error;
			playback->add_datafeed_callback([&](shared_ptr<sigrok::Device>, shared_ptr<Packet> packet) {
				// libsigrok's C callback trampoline is noexcept. Defer failures
				// until run() returns rather than unwind through that boundary.
				if (packet_error) return;
				try {
				switch (packet->type()->id()) {
				case SR_DF_HEADER:
					if (!current_frame_) datafeed_callback_(device_, packet);
					datafeed_callback_(device_, context_->create_meta_packet({
						{ConfigKey::SAMPLERATE, Glib::Variant<guint64>::create(frame.samplerate)}}));
					break;
				case SR_DF_LOGIC: {
					const auto logic = std::dynamic_pointer_cast<sigrok::Logic>(packet->payload());
					if (!logic->unit_size() || logic->data_length() % logic->unit_size())
						throw std::runtime_error("Invalid collection sample alignment.");
					received += logic->data_length() / logic->unit_size();
					if (received > frame.samples) throw std::runtime_error("Collection sample count exceeded.");
					datafeed_callback_(device_, packet);
					break;
				}
				case SR_DF_END: ended = true; break;
				case SR_DF_META: break; // manifest owns samplerate and outcome
				default: throw std::runtime_error("Non-digital packet in capture collection.");
				}
				} catch (...) {
					packet_error = std::current_exception();
					try { playback->stop(); } catch (...) { }
				}
			});
			{
				std::lock_guard<std::recursive_mutex> lock(playback_mutex_);
				if (abort_) break;
				playback_ = playback;
				playback_->start();
			}
			playback->run();
			{
				std::lock_guard<std::recursive_mutex> lock(playback_mutex_);
				playback_.reset();
			}
			playback->remove_datafeed_callbacks();
			playback->remove_devices();
			if (packet_error) std::rethrow_exception(packet_error);
			if (!abort_ && (!ended || received != frame.samples))
				throw std::runtime_error("Truncated capture collection frame.");
			send_outcome(abort_ ? "stopped" : frame.status, abort_ ? QString() : frame.error);
			if (frame_callback_) frame_callback_(false);
		}
		send_outcome(abort_ ? "stopped" : collection_->status, abort_ ? QString() : collection_->error);
		datafeed_callback_(device_, context_->create_end_packet());
	} catch (...) {
		std::lock_guard<std::recursive_mutex> lock(playback_mutex_);
		if (playback_) {
			playback_->stop();
			playback_->remove_datafeed_callbacks();
			playback_->remove_devices();
			playback_.reset();
		}
		if (!abort_) throw;
		send_outcome("stopped", QString());
		if (frame_callback_) frame_callback_(false);
		datafeed_callback_(device_, context_->create_end_packet());
	}
}

} // namespace devices
} // namespace pv
