/*
 * This file is part of the PulseView project.
 *
 * Copyright (C) 2012-14 Joel Holdsworth <joel@airwebreathe.org.uk>
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

#ifndef PULSEVIEW_PV_SESSION_HPP
#define PULSEVIEW_PV_SESSION_HPP

#ifdef ENABLE_FLOW
#include <atomic>
#include <condition_variable>
#endif

#include <deque>
#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include <glibmm/datetime.h>

#include <QObject>
#include <QSettings>
#include <pv/data/streamwindows.hpp>
#include <QString>
#include <QElapsedTimer>

#ifdef ENABLE_FLOW
#include <gstreamermm.h>
#include <libsigrokflow/libsigrokflow.hpp>
#endif

#include "metadata_obj.hpp"
#include "util.hpp"
#include "views/viewbase.hpp"

using std::deque;
using std::function;
using std::map;
using std::mutex;
using std::recursive_mutex;
using std::shared_ptr;
using std::string;
using std::unordered_set;

#ifdef ENABLE_FLOW
using Glib::RefPtr;
using Gst::AppSink;
using Gst::Element;
using Gst::Pipeline;
#endif

struct srd_decoder;
struct srd_channel;

namespace sigrok {
class Analog;
class Channel;
class Device;
class InputFormat;
class Logic;
class Meta;
class Option;
class OutputFormat;
class Packet;
class Session;
class Trigger;
}  // namespace sigrok

using sigrok::Option;

namespace pv {

struct HardwareTriggerOptions;

class DeviceManager;

namespace data {
class Analog;
class AnalogSegment;
class DecodeSignal;
class Logic;
class LogicSegment;
class SignalBase;
class SignalData;
class SignalGroup;
}

namespace devices {
class Device;
}

namespace toolbars {
class MainBar;
}

namespace views {
class ViewBase;
}

using pv::views::ViewType;

class Session : public QObject
{
	Q_OBJECT

public:
	enum capture_state {
		Stopped,
		AwaitingTrigger,
		Running
	};

	static shared_ptr<sigrok::Context> sr_context;

public:
	Session(DeviceManager &device_manager, QString name);

	~Session();

	DeviceManager& device_manager();

	const DeviceManager& device_manager() const;

	shared_ptr<sigrok::Session> session() const;
	void set_trigger(shared_ptr<sigrok::Trigger> trigger);
	void set_trigger(shared_ptr<sigrok::Trigger> trigger, const HardwareTriggerOptions &options);
	bool has_extended_trigger() const;

	shared_ptr<devices::Device> device() const;

	QString name() const;
	void set_name(QString name);

	QString save_path() const;
	void set_save_path(QString path);

	const vector< shared_ptr<views::ViewBase> > views() const;

	shared_ptr<views::ViewBase> main_view() const;
	shared_ptr<pv::toolbars::MainBar> main_bar() const;
	void set_main_bar(shared_ptr<pv::toolbars::MainBar> main_bar);

	/**
	 * Indicates whether the captured data was saved to disk already or not
	 */
	bool data_saved() const;

	void save_setup(QSettings &settings) const;
	void save_settings(QSettings &settings) const;
	void restore_setup(QSettings &settings);
	void restore_settings(QSettings &settings);

	/**
	 * Attempts to set device instance, may fall back to demo if needed
	 */
	void select_device(shared_ptr<devices::Device> device);

	/**
	 * Sets device instance that will be used in the next capture session.
	 */
	void set_device(shared_ptr<devices::Device> device);
	void set_default_device();
	bool using_file_device() const;

	void load_init_file(const string &file_name, const string &format,
		const string &setup_file_name);

	void load_file(QString file_name, QString setup_file_name = QString(),
		shared_ptr<sigrok::InputFormat> format = nullptr,
		const map<string, Glib::VariantBase> &options =
			map<string, Glib::VariantBase>());

	capture_state get_capture_state() const;
	QString capture_outcome() const;
	QString capture_error() const;
	bool device_needs_reconnect() const;
	void start_capture(function<void (const QString)> error_handler);
	void stop_capture();
	void request_stop_capture();
	bool stop_requested() const;
	unsigned captures_per_run() const { return captures_per_run_.load(); }
	bool set_captures_per_run(unsigned count);
	bool stream_windows_enabled() const { return stream_windows_enabled_.load(); }
	bool set_stream_windows_enabled(bool enabled);
	unsigned window_pretrigger_percent() const { return window_pretrigger_percent_.load(); }
	bool set_window_pretrigger_percent(unsigned percent);
	bool segment_sample_origin(uint32_t segment, uint64_t& origin) const;
	uint32_t captured_segment_count() const;
	QString segment_capture_outcome(uint32_t segment) const;
	QString segment_capture_error(uint32_t segment) const;
	void restore_segment_triggers(const vector<util::Timestamp>& triggers);

	double get_samplerate() const;
	Glib::DateTime get_acquisition_start_time() const;

	uint32_t get_highest_segment_id() const;
	uint64_t get_segment_sample_count(uint32_t segment_id) const;

	vector<util::Timestamp> get_triggers(uint32_t segment_id) const;

	void register_view(shared_ptr<views::ViewBase> view);
	void deregister_view(shared_ptr<views::ViewBase> view);
	bool has_view(shared_ptr<views::ViewBase> view);

	const vector< shared_ptr<data::SignalBase> > signalbases() const;
	uint32_t get_signal_count(data::SignalBase::ChannelType type) const;
	uint32_t get_next_signal_index(data::SignalBase::ChannelType type);

	void add_generated_signal(shared_ptr<data::SignalBase> signal);
	void remove_generated_signal(shared_ptr<data::SignalBase> signal);

#ifdef ENABLE_DECODE
	shared_ptr<data::DecodeSignal> add_decode_signal();

	void remove_decode_signal(shared_ptr<data::DecodeSignal> signal);
#endif

	bool all_segments_complete(uint32_t segment_id) const;

	MetadataObjManager* metadata_obj_manager();

private:
	void set_capture_state(capture_state state);

	void update_signals();

	shared_ptr<data::SignalBase> signalbase_from_channel(
		shared_ptr<sigrok::Channel> channel) const;

	static map<string, Glib::VariantBase> input_format_options(
		vector<string> user_spec,
		map<string, shared_ptr<Option>> fmt_opts);

	void sample_thread_proc(function<void (const QString)> error_handler);

	void free_unused_memory();

	void signal_new_segment();
	void signal_segment_completed();

#ifdef ENABLE_FLOW
	bool on_gst_bus_message(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message);

	Gst::FlowReturn on_gst_new_sample();
#endif

	void feed_in_header();
	void feed_in_meta(shared_ptr<sigrok::Meta> meta);
	void feed_in_trigger();
	void feed_in_frame_begin();
	void feed_in_frame_end();
	void record_segment_outcome();
	void prepare_stream_windows();
	void sample_stream_windows();
	void accept_stream_window(data::StreamWindows::Window&& window);
	void set_segment_origin(uint64_t origin);
	void feed_in_logic(shared_ptr<sigrok::Logic> logic);
	void feed_in_logic_payload(shared_ptr<sigrok::Logic> logic);
	void feed_in_analog(shared_ptr<sigrok::Analog> analog);

	void data_feed_in(shared_ptr<sigrok::Device> device,
		shared_ptr<sigrok::Packet> packet);

Q_SIGNALS:
	void captures_per_run_changed(unsigned count);
	void stream_windows_changed();
	void capture_series_progress(unsigned current, unsigned total);
	void capture_state_changed(int state);
	void device_changed();

	void signals_changed();

	void name_changed();

	void trigger_event(int segment_id, util::Timestamp location);
	void trigger_changed();

	void new_segment(int new_segment_id);
	void segment_completed(int segment_id);

	void data_received();

	void add_view(ViewType type, Session *session);
	void session_error_raised(const QString text, const QString info_text);

public Q_SLOTS:
	void on_data_saved();

#ifdef ENABLE_DECODE
	void on_new_decoders_selected(vector<const srd_decoder*> decoders);
#endif

private:
	bool shutting_down_;

	DeviceManager &device_manager_;
	shared_ptr<devices::Device> device_;
	QString default_name_, name_, save_path_;

	vector< shared_ptr<views::ViewBase> > views_;
	shared_ptr<pv::views::ViewBase> main_view_;

	shared_ptr<pv::toolbars::MainBar> main_bar_;

	mutable mutex sampling_mutex_; //!< Protects access to capture_state_
	capture_state capture_state_;
	bool stop_requested_; //!< Protected by sampling_mutex_
	std::atomic_uint captures_per_run_{1};
	std::atomic_bool stream_windows_enabled_{false};
	std::atomic_uint window_pretrigger_percent_{50};
	bool active_window_mode_ = false;
	uint64_t window_depth_ = 0, window_pre_ = 0, window_rate_ = 0;
	std::vector<data::WindowTriggerStage> window_stages_;
	std::unique_ptr<data::StreamWindows> window_extractor_;
	QString window_packet_error_;
	bool window_target_stop_ = false;
	struct SegmentOrigin { bool known = false; uint64_t sample = 0; };
	vector<SegmentOrigin> segment_origins_; // Protected by data_mutex_.
	unsigned active_capture_count_ = 1;
	bool series_frame_ = false; // Sampling-thread only; intermediate END seals a segment.
	bool acquisition_received_samples_ = false;
	bool acquisition_end_received_ = false;
	struct SegmentOutcome { QString status, error; };
	vector<SegmentOutcome> segment_outcomes_; // Protected by data_mutex_.
	QString capture_outcome_, capture_error_;
	std::atomic_bool device_reconnect_needed_{false};

	vector< shared_ptr<data::SignalBase> > signalbases_;
	unordered_set< shared_ptr<data::SignalData> > all_signal_data_;
	deque<data::SignalGroup*> signal_groups_;
	map<uint8_t, uint32_t> next_index_list_; // signal type -> index counter

	/// trigger_list_ contains pairs of <segment_id, timestamp> values
	vector< std::pair<uint32_t, util::Timestamp> > trigger_list_;

	mutable recursive_mutex data_mutex_;
	shared_ptr<data::Logic> logic_data_;
	uint64_t cur_samplerate_;
	shared_ptr<data::LogicSegment> cur_logic_segment_;
	map< shared_ptr<sigrok::Channel>, shared_ptr<data::AnalogSegment> >
		cur_analog_segments_;
	int32_t highest_segment_id_;
	vector<uint64_t> segment_sample_count_;

	std::thread sampling_thread_;

	bool out_of_memory_;
	bool data_saved_;
	bool frame_began_;

	QElapsedTimer acq_time_;
	Glib::DateTime acq_start_time_;

	MetadataObjManager metadata_obj_manager_;

#ifdef ENABLE_FLOW
	RefPtr<Pipeline> pipeline_;
	RefPtr<Element> source_;
	RefPtr<AppSink> sink_;

	mutable mutex pipeline_done_mutex_;
	mutable condition_variable pipeline_done_cond_;
	atomic<bool> pipeline_done_interrupt_;
#endif
};

} // namespace pv

#endif // PULSEVIEW_PV_SESSION_HPP
