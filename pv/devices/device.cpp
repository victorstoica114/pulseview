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

#include <cassert>
#include <type_traits>

#include <QApplication>
#include <QDebug>
#include <QString>

#include <libsigrokcxx/libsigrokcxx.hpp>

#include "device.hpp"

using std::is_same;
using std::shared_ptr;

using sigrok::ConfigKey;
using sigrok::Capability;

using Glib::VariantBase;
using Glib::Variant;

namespace pv {
namespace devices {

Device::~Device()
{
	if (session_)
		session_->remove_datafeed_callbacks();
}

shared_ptr<sigrok::Session> Device::session() const
{
	return session_;
}

shared_ptr<sigrok::Device> Device::device() const
{
	return device_;
}

template uint64_t Device::read_config(const sigrok::ConfigKey*, const uint64_t);

template<typename T>
T Device::read_config(const ConfigKey *key, const T default_value)
{
	assert(key);

	if (!device_)
		return default_value;

	if (!device_->config_check(key, Capability::GET)) {
		qWarning() << QApplication::tr("Querying config key %1 is not allowed")
			.arg(QString::fromStdString(key->identifier()));
		return default_value;
	}

	VariantBase value;
	try {
		value = device_->config_get(key);
	} catch (const sigrok::Error &e) {
		qWarning() << QApplication::tr("Querying config key %1 resulted in %2")
			.arg(QString::fromStdString(key->identifier()), e.what());
		return default_value;
	}

	if (is_same<T, uint32_t>::value)
		return VariantBase::cast_dynamic<Glib::Variant<guint32>>(value).get();
	if (is_same<T, int32_t>::value)
		return VariantBase::cast_dynamic<Glib::Variant<gint32>>(value).get();
	if (is_same<T, uint64_t>::value)
		return VariantBase::cast_dynamic<Glib::Variant<guint64>>(value).get();
	if (is_same<T, int64_t>::value)
		return VariantBase::cast_dynamic<Glib::Variant<gint64>>(value).get();

	qWarning() << QApplication::tr("Unknown type supplied when attempting to query %1")
		.arg(QString::fromStdString(key->identifier()));
	return default_value;
}

bool Device::can_stream_continuously() const
{
	return device_ && device_->config_check(ConfigKey::CONTINUOUS, Capability::GET) &&
		device_->config_check(ConfigKey::CONTINUOUS, Capability::SET);
}

std::function<void()> Device::suspend_capture_limits()
{
	if (!can_stream_continuously()) throw std::runtime_error("Device does not expose continuous acquisition controls.");
	const auto source = device_;
	const bool continuous = g_variant_get_boolean(source->config_get(ConfigKey::CONTINUOUS).gobj());
	const auto samples = read_config<uint64_t>(ConfigKey::LIMIT_SAMPLES);
	const auto msec = read_config<uint64_t>(ConfigKey::LIMIT_MSEC);
	std::function<void()> restore = [source, continuous, samples, msec] {
		source->config_set(ConfigKey::CONTINUOUS, Glib::Variant<bool>::create(continuous));
		if (!continuous) {
			if (msec) source->config_set(ConfigKey::LIMIT_MSEC, Glib::Variant<guint64>::create(msec));
			else if (samples) source->config_set(ConfigKey::LIMIT_SAMPLES, Glib::Variant<guint64>::create(samples));
		}
	};
	try { source->config_set(ConfigKey::CONTINUOUS, Glib::Variant<bool>::create(true)); }
	catch (...) { try { restore(); } catch (...) { } throw; }
	return restore;
}

void Device::connect_datafeed(FeedCallback callback)
{
	datafeed_callback_ = std::move(callback);
	session_->add_datafeed_callback(datafeed_callback_);
}

void Device::disconnect_datafeed()
{
	if (session_) session_->remove_datafeed_callbacks();
	datafeed_callback_ = {};
	frame_callback_ = {};
}

void Device::start()
{
	assert(session_);
	session_->start();
}

void Device::run()
{
	assert(device_);
	assert(session_);
	session_->run();
}

void Device::stop()
{
	assert(session_);
	session_->stop();
}

} // namespace devices
} // namespace pv
