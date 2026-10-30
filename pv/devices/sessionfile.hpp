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

#ifndef PULSEVIEW_PV_DEVICES_SESSIONFILE_HPP
#define PULSEVIEW_PV_DEVICES_SESSIONFILE_HPP

#include <memory>
#include <atomic>
#include <mutex>
#include <pv/capturecollection.hpp>
#include <QTemporaryDir>

#include "file.hpp"

using std::shared_ptr;
using std::string;

namespace sigrok {
class Context;
} // sigrok

namespace pv {
namespace devices {

class SessionFile final : public File
{
public:
	SessionFile(const shared_ptr<sigrok::Context> context,
		const string &file_name);

	void open();

	void close();
	void connect_datafeed(FeedCallback callback) override;
	void start() override;
	void run() override;
	void stop() override;
	std::vector<std::string> frame_triggers() const override;
	bool frame_origin(uint64_t& origin) const override;

private:
	const shared_ptr<sigrok::Context> context_;
	std::unique_ptr<CaptureCollection> collection_;
	std::unique_ptr<QTemporaryDir> temporary_;
	std::atomic_bool abort_{false};
	std::recursive_mutex playback_mutex_;
	shared_ptr<sigrok::Session> playback_;
	size_t current_frame_ = 0;
	struct RecordedChannel { unsigned index; int type; std::string name; bool enabled; };
	std::vector<RecordedChannel> recorded_channels_;
};

} // namespace devices
} // namespace pv

#endif // PULSEVIEW_PV_DEVICES_SESSIONFILE_HPP
