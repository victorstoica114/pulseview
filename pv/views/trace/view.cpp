/*
 * This file is part of the PulseView project.
 *
 * Copyright (C) 2012 Joel Holdsworth <joel@airwebreathe.org.uk>
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

#ifdef ENABLE_DECODE
#include <libsigrokdecode/libsigrokdecode.h>
#endif

#include <extdef.h>

#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <iostream>
#include <iterator>

#include <QApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <QEvent>
#include <QFontMetrics>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <QVBoxLayout>

#include <libsigrokcxx/libsigrokcxx.hpp>

#include "view.hpp"

#include "pv/globalsettings.hpp"
#include "pv/metadata_obj.hpp"
#include "pv/session.hpp"
#include "pv/util.hpp"
#include "pv/data/logic.hpp"
#include "pv/data/logicsegment.hpp"
#include "pv/data/signalbase.hpp"
#include "pv/devices/device.hpp"
#include "pv/views/trace/mathsignal.hpp"
#include "pv/views/trace/analogsignal.hpp"
#include "pv/views/trace/header.hpp"
#include "pv/views/trace/logicsignal.hpp"
#include "pv/views/trace/ruler.hpp"
#include "pv/views/trace/signal.hpp"
#include "pv/views/trace/tracegroup.hpp"
#include "pv/views/trace/triggermarker.hpp"
#include "pv/views/trace/viewport.hpp"

#ifdef ENABLE_DECODE
#include "pv/views/trace/decodetrace.hpp"
#endif

using pv::data::SignalBase;
using pv::data::SignalData;
using pv::data::Segment;
using pv::util::TimeUnit;
using pv::util::Timestamp;

using std::back_inserter;
using std::copy_if;
using std::count_if;
using std::inserter;
using std::lock_guard;
using std::max;
using std::make_pair;
using std::make_shared;
using std::min;
using std::numeric_limits;
using std::pair;
using std::set;
using std::set_difference;
using std::shared_ptr;
using std::vector;

namespace pv {
namespace views {
namespace trace {

const Timestamp View::MaxScale("1e9");
const Timestamp View::MinScale("1e-14");

const int View::MaxScrollValue = INT_MAX / 2;

/* Area at the top and bottom of the view that can't be scrolled out of sight */
const int View::ViewScrollMargin = 50;

const int View::ScaleUnits[3] = {1, 2, 5};

CustomScrollArea::CustomScrollArea(QWidget *parent) :
	QAbstractScrollArea(parent)
{
}

bool CustomScrollArea::viewportEvent(QEvent *event)
{
	switch (event->type()) {
	case QEvent::Paint:
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
	case QEvent::MouseButtonDblClick:
	case QEvent::MouseMove:
	case QEvent::Wheel:
	case QEvent::TouchBegin:
	case QEvent::TouchUpdate:
	case QEvent::TouchEnd:
		return false;
	default:
		return QAbstractScrollArea::viewportEvent(event);
	}
}

View::View(Session &session, bool is_main_view, QMainWindow *parent) :
	ViewBase(session, is_main_view, parent),

	// Note: Place defaults in View::reset_view_state(), not here
	splitter_(new QSplitter()),
	header_was_shrunk_(false),  // The splitter remains unchanged after a reset, so this goes here
	sticky_scrolling_(false),  // Default setting is set in MainWindow::setup_ui()
	scroll_needs_defaults_(true)
{
	QVBoxLayout *root_layout = new QVBoxLayout(this);
	root_layout->setContentsMargins(0, 0, 0, 0);
	root_layout->addWidget(splitter_);

	viewport_ = new Viewport(*this);
	scrollarea_ = new CustomScrollArea(splitter_);
	scrollarea_->setViewport(viewport_);
	scrollarea_->setFrameShape(QFrame::NoFrame);

	ruler_ = new Ruler(*this);

	header_ = new Header(*this);
	header_->setMinimumWidth(10);  // So that the arrow tips show at least

	// We put the header into a simple layout so that we can add the top margin,
	// allowing us to make it line up with the bottom of the ruler
	QWidget *header_container = new QWidget();
	header_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QVBoxLayout *header_layout = new QVBoxLayout(header_container);
	header_layout->setContentsMargins(0, ruler_->sizeHint().height(), 0, 0);
	header_layout->addWidget(header_);

	// To let the ruler and scrollarea be on the same split pane, we need a layout
	QWidget *trace_container = new QWidget();
	trace_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QVBoxLayout *trace_layout = new QVBoxLayout(trace_container);
	trace_layout->setSpacing(0);  // We don't want space between the ruler and scrollarea
	trace_layout->setContentsMargins(0, 0, 0, 0);
	trace_layout->addWidget(ruler_);
	trace_layout->addWidget(scrollarea_);

	splitter_->addWidget(header_container);
	splitter_->addWidget(trace_container);
	splitter_->setHandleWidth(1);  // Don't show a visible rubber band
	splitter_->setCollapsible(0, false);  // Prevent the header from collapsing
	splitter_->setCollapsible(1, false);  // Prevent the traces from collapsing
	splitter_->setStretchFactor(0, 0);  // Prevent the panes from being resized
	splitter_->setStretchFactor(1, 1);  // when the entire view is resized
	splitter_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	viewport_->installEventFilter(this);
	ruler_->installEventFilter(this);
	header_->installEventFilter(this);

	// Set up settings and event handlers
	GlobalSettings settings;
	colored_bg_ = settings.value(GlobalSettings::Key_View_ColoredBG).toBool();
	snap_distance_ = settings.value(GlobalSettings::Key_View_SnapDistance).toInt();

	GlobalSettings::add_change_handler(this);

	// Set up metadata objects and event handlers
	if (is_main_view) {
		session_.metadata_obj_manager()->create_object(MetadataObjMainViewRange);
		session_.metadata_obj_manager()->create_object(MetadataObjMousePos);
	}

	// Set up UI event handlers
	connect(scrollarea_->horizontalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(h_scroll_value_changed(int)));
	connect(scrollarea_->verticalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(v_scroll_value_changed()));

	connect(header_, SIGNAL(selection_changed()),
		ruler_, SLOT(clear_selection()));
	connect(ruler_, SIGNAL(selection_changed()),
		header_, SLOT(clear_selection()));

	connect(header_, SIGNAL(selection_changed()),
		this, SIGNAL(selection_changed()));
	connect(ruler_, SIGNAL(selection_changed()),
		this, SIGNAL(selection_changed()));

	connect(splitter_, SIGNAL(splitterMoved(int, int)),
		this, SLOT(on_splitter_moved()));

	connect(&lazy_event_handler_, SIGNAL(timeout()),
		this, SLOT(process_sticky_events()));
	lazy_event_handler_.setSingleShot(true);
	lazy_event_handler_.setInterval(1000 / ViewBase::MaxViewAutoUpdateRate);

	// Set up local keyboard shortcuts
	zoom_in_shortcut_ = new QShortcut(QKeySequence(Qt::Key_Plus), this,
		SLOT(on_zoom_in_shortcut_triggered()), nullptr, Qt::WidgetWithChildrenShortcut);
	zoom_in_shortcut_->setAutoRepeat(false);

	zoom_out_shortcut_ = new QShortcut(QKeySequence(Qt::Key_Minus), this,
		SLOT(on_zoom_out_shortcut_triggered()), nullptr, Qt::WidgetWithChildrenShortcut);
	zoom_out_shortcut_->setAutoRepeat(false);

	zoom_in_shortcut_2_ = new QShortcut(QKeySequence(Qt::Key_Up), this,
		SLOT(on_zoom_in_shortcut_triggered()), nullptr, Qt::WidgetWithChildrenShortcut);
	zoom_out_shortcut_2_ = new QShortcut(QKeySequence(Qt::Key_Down), this,
		SLOT(on_zoom_out_shortcut_triggered()), nullptr, Qt::WidgetWithChildrenShortcut);

	home_shortcut_ = new QShortcut(QKeySequence(Qt::Key_Home), this,
		SLOT(on_scroll_to_start_shortcut_triggered()), nullptr, Qt::WidgetWithChildrenShortcut);
	home_shortcut_->setAutoRepeat(false);

	end_shortcut_ = new QShortcut(QKeySequence(Qt::Key_End), this,
		SLOT(on_scroll_to_end_shortcut_triggered()), nullptr, Qt::WidgetWithChildrenShortcut);
	end_shortcut_->setAutoRepeat(false);

	grab_ruler_left_shortcut_ = new QShortcut(QKeySequence(Qt::Key_1), this,
		nullptr, nullptr, Qt::WidgetWithChildrenShortcut);
	connect(grab_ruler_left_shortcut_, &QShortcut::activated,
		this, [=]{on_grab_ruler(1);});
	grab_ruler_left_shortcut_->setAutoRepeat(false);

	grab_ruler_right_shortcut_ = new QShortcut(QKeySequence(Qt::Key_2), this,
		nullptr, nullptr, Qt::WidgetWithChildrenShortcut);
	connect(grab_ruler_right_shortcut_, &QShortcut::activated,
		this, [=]{on_grab_ruler(2);});
	grab_ruler_right_shortcut_->setAutoRepeat(false);

	cancel_grab_shortcut_ = new QShortcut(QKeySequence(Qt::Key_Escape), this,
		nullptr, nullptr, Qt::WidgetWithChildrenShortcut);
	connect(cancel_grab_shortcut_, &QShortcut::activated,
		this, [=]{grabbed_widget_ = nullptr;});
	cancel_grab_shortcut_->setAutoRepeat(false);

	// Trigger the initial event manually. The default device has signals
	idle_scan_timer_.setInterval(0);
	connect(&idle_scan_timer_, &QTimer::timeout, this, &View::process_idle_scan);
	connect(this, SIGNAL(time_mapping_changed()),
		ruler_, SLOT(invalidate_tick_position_cache()));

	// Trigger the initial event manually. The default device has signals
	// which were created before this object came into being
	signals_changed();

	// Make sure the transparent widgets are on the top
	ruler_->raise();
	header_->raise();

	reset_view_state();
}

View::~View()
{
	GlobalSettings::remove_change_handler(this);
}

ViewType View::get_type() const
{
	return ViewTypeTrace;
}

void View::reset_view_state()
{
	reset_idle_compression();
	ViewBase::reset_view_state();

	segment_display_mode_ = Trace::ShowLastSegmentOnly;
	segment_selectable_ = false;
	scale_ = 1e-3;
	offset_ = 0;
	ruler_offset_ = 0;
	zero_offset_ = 0;
	custom_zero_offset_set_ = false;
	updating_scroll_ = false;
	restoring_state_ = false;
	always_zoom_to_fit_ = false;
	tick_period_ = 0;
	tick_prefix_ = pv::util::SIPrefix::yocto;
	tick_precision_ = 0;
	time_unit_ = util::TimeUnit::Time;
	show_cursors_ = false;
	cursors_ = make_shared<CursorPair>(*this);
	next_flag_text_ = 'A';
	trigger_markers_.clear();
	hover_widget_ = nullptr;
	grabbed_widget_ = nullptr;
	hover_point_ = QPoint(-1, -1);
	scroll_needs_defaults_ = true;
	scale_at_acq_start_ = 0;
	offset_at_acq_start_ = 0;

	show_cursors_ = false;
	cursor_state_changed(show_cursors_);
	flags_.clear();

	// Update the zoom state
	calculate_tick_spacing();

	// Make sure the standard bar's segment selector is in sync
	set_segment_display_mode(segment_display_mode_);

	scrollarea_->verticalScrollBar()->setRange(-100000000, 100000000);
}

Session& View::session()
{
	return session_;
}

const Session& View::session() const
{
	return session_;
}

vector< shared_ptr<Signal> > View::signals() const
{
	return signals_;
}

shared_ptr<Signal> View::get_signal_by_signalbase(shared_ptr<data::SignalBase> base) const
{
	shared_ptr<Signal> ret_val;

	for (const shared_ptr<Signal>& s : signals_)
		if (s->base() == base) {
			ret_val = s;
			break;
		}

	return ret_val;
}

void View::clear_signalbases()
{
	reset_idle_compression();
	ViewBase::clear_signalbases();
	signals_.clear();
}

void View::add_signalbase(const shared_ptr<data::SignalBase> signalbase)
{
	ViewBase::add_signalbase(signalbase);

	shared_ptr<Signal> signal;

	switch (signalbase->type()) {
	case SignalBase::LogicChannel:
		signal = shared_ptr<Signal>(new LogicSignal(session_, signalbase));
		break;

	case SignalBase::AnalogChannel:
		signal = shared_ptr<Signal>(new AnalogSignal(session_, signalbase));
		break;

	case SignalBase::MathChannel:
		signal = shared_ptr<Signal>(new MathSignal(session_, signalbase));
		break;

	default:
		qDebug() << "Unknown signalbase type:" << signalbase->type();
		assert(false);
		break;
	}

	signals_.push_back(signal);

	signal->set_segment_display_mode(segment_display_mode_);
	signal->set_current_segment(current_segment_);

	// Secondary views use the signal's settings in the main view
	if (!is_main_view()) {
		shared_ptr<View> main_tv = dynamic_pointer_cast<View>(session_.main_view());
		shared_ptr<Signal> main_signal = main_tv->get_signal_by_signalbase(signalbase);
		if (main_signal)
			signal->restore_settings(main_signal->save_settings());
	}

	connect(signal->base().get(), SIGNAL(name_changed(const QString&)),
		this, SLOT(on_signal_name_changed()));
}

void View::remove_signalbase(const shared_ptr<data::SignalBase> signalbase)
{
	ViewBase::remove_signalbase(signalbase);

	shared_ptr<Signal> signal = get_signal_by_signalbase(signalbase);

	if (signal)
		remove_trace(signal);
}

#ifdef ENABLE_DECODE
void View::clear_decode_signals()
{
	ViewBase::clear_decode_signals();
	decode_traces_.clear();
}

void View::add_decode_signal(shared_ptr<data::DecodeSignal> signal)
{
	ViewBase::add_decode_signal(signal);

	shared_ptr<DecodeTrace> d(
		new DecodeTrace(session_, signal, decode_traces_.size()));
	decode_traces_.push_back(d);

	d->set_segment_display_mode(segment_display_mode_);
	d->set_current_segment(current_segment_);

	connect(signal.get(), SIGNAL(name_changed(const QString&)),
		this, SLOT(on_signal_name_changed()));
}

void View::remove_decode_signal(shared_ptr<data::DecodeSignal> signal)
{
	for (auto i = decode_traces_.begin(); i != decode_traces_.end(); i++)
		if ((*i)->base() == signal) {
			decode_traces_.erase(i);
			break;
		}

	ViewBase::remove_decode_signal(signal);
}
#endif

void View::remove_trace(shared_ptr<Trace> trace)
{
	TraceTreeItemOwner *const owner = trace->owner();
	assert(owner);
	owner->remove_child_item(trace);

	for (auto i = signals_.begin(); i != signals_.end(); i++)
		if ((*i) == trace) {
			signals_.erase(i);
			break;
		}

	if (!header_was_shrunk_)
		resize_header_to_fit();

	update_layout();

	header_->update();
	viewport_->update();
}

shared_ptr<Signal> View::get_signal_under_mouse_cursor() const
{
	return signal_under_mouse_cursor_;
}

View* View::view()
{
	return this;
}

const View* View::view() const
{
	return this;
}

Viewport* View::viewport()
{
	return viewport_;
}

const Viewport* View::viewport() const
{
	return viewport_;
}

QAbstractScrollArea* View::scrollarea() const
{
	return scrollarea_;
}

const Ruler* View::ruler() const
{
	return ruler_;
}

void View::save_settings(QSettings &settings) const
{
	// Save the ordinary viewport; folding is a transient inspection mode.
	settings.setValue("scale", idle_compression_enabled_ ? idle_plain_scale_ : scale_);
	settings.setValue("idle_threshold", idle_threshold_);
	settings.setValue("v_offset",
		scrollarea_->verticalScrollBar()->sliderPosition());

	settings.setValue("splitter_state", splitter_->saveState());
	settings.setValue("segment_display_mode", segment_display_mode_);

	GlobalSettings::store_timestamp(settings, "offset",
		idle_compression_enabled_ ? idle_plain_offset_ : offset_);

	if (custom_zero_offset_set_)
		GlobalSettings::store_timestamp(settings, "zero_offset", -zero_offset_);
	else
		settings.remove("zero_offset");

	for (const shared_ptr<Signal>& signal : signals_) {
		settings.beginGroup(signal->base()->internal_name());
		signal->save_settings(settings);
		settings.endGroup();
	}
}

void View::restore_settings(QSettings &settings)
{
	set_idle_threshold(settings.value("idle_threshold", 0.001).toDouble());
	// Note: It is assumed that this function is only called once,
	// immediately after restoring a previous session.

	if (settings.contains("scale"))
		set_scale(settings.value("scale").toDouble());

	if (settings.contains("offset")) {
		// This also updates ruler_offset_
		set_offset(GlobalSettings::restore_timestamp(settings, "offset"));
	}

	if (settings.contains("zero_offset"))
		set_zero_position(GlobalSettings::restore_timestamp(settings, "zero_offset"));

	if (settings.contains("splitter_state"))
		splitter_->restoreState(settings.value("splitter_state").toByteArray());

	if (settings.contains("segment_display_mode"))
		set_segment_display_mode(
			(Trace::SegmentDisplayMode)(settings.value("segment_display_mode").toInt()));

	for (shared_ptr<Signal> signal : signals_) {
		settings.beginGroup(signal->base()->internal_name());
		signal->restore_settings(settings);
		settings.endGroup();
	}

	if (settings.contains("v_offset")) {
		// Note: see eventFilter() for additional information
		saved_v_offset_ = settings.value("v_offset").toInt();
		scroll_needs_defaults_ = false;
	}

	restoring_state_ = true;

	// Update the ruler so that it uses the new scale
	calculate_tick_spacing();
}

vector< shared_ptr<TimeItem> > View::time_items() const
{
	const vector<shared_ptr<Flag>> f(flags());
	vector<shared_ptr<TimeItem>> items(f.begin(), f.end());

	if (cursors_) {
		items.push_back(cursors_);
		items.push_back(cursors_->first());
		items.push_back(cursors_->second());
	}

	for (auto& trigger_marker : trigger_markers_)
		items.push_back(trigger_marker);

	return items;
}

double View::scale() const
{
	return scale_;
}

double View::time_to_pixel(const Timestamp& time) const
{
	return ((idle_time_map_.to_display(time) -
		idle_time_map_.to_display(offset_)) / scale_).convert_to<double>();
}

Timestamp View::pixel_to_time(double x) const
{
	return idle_time_map_.to_real(idle_time_map_.to_display(offset_) + x * scale_);
}

vector<const IdleTimeMap::Gap*> View::visible_idle_gaps(int width) const
{
	vector<const IdleTimeMap::Gap*> result;
	const auto left = pixel_to_time(0), right = pixel_to_time(width);
	const auto& gaps = idle_time_map_.gaps();
	auto it = std::lower_bound(gaps.begin(), gaps.end(), left,
		[](const IdleTimeMap::Gap& gap, const Timestamp& t) { return gap.end < t; });
	for (; it != gaps.end() && it->begin <= right; ++it) result.push_back(&*it);
	return result;
}

QString View::idle_gap_text(double x) const
{
	const auto time = pixel_to_time(x);
	const auto& gaps = idle_time_map_.gaps();
	const auto it = std::upper_bound(gaps.begin(), gaps.end(), time,
		[](const Timestamp& t, const IdleTimeMap::Gap& gap) { return t < gap.begin; });
	if (it == gaps.begin() || time > std::prev(it)->end) return QString();
	const auto& gap = *std::prev(it);
	return tr("Idle: %1 (display compressed; real time preserved)").arg(
		util::format_time_si(gap.end - gap.begin, util::SIPrefix::unspecified, 6, "s", false));
}

Timestamp View::snapped_time(int64_t sample) const
{
	if (!signal_under_mouse_cursor_) return Timestamp(0);
	const double rate = signal_under_mouse_cursor_->base()->get_samplerate();
	if (rate <= 0) return Timestamp(0);
	Timestamp origin(0);
	const auto logic = std::dynamic_pointer_cast<LogicSignal>(signal_under_mouse_cursor_);
	if (logic && logic->get_logic_segment_to_paint())
		origin = logic->get_logic_segment_to_paint()->start_time();
	return origin + Timestamp(sample) / rate;
}

void View::idle_mapping_updated()
{
	time_mapping_changed();
	ruler_->updateGeometry();
	if (header_->parentWidget() && header_->parentWidget()->layout())
		header_->parentWidget()->layout()->setContentsMargins(0, ruler_->sizeHint().height(), 0, 0);
	calculate_tick_spacing();
	update_scroll();
	ruler_->update();
	viewport_->update();
}

void View::reset_idle_compression(const QString& status)
{
	const bool was_active = idle_compression_enabled_ || !idle_time_map_.empty();
	idle_scan_timer_.stop();
	idle_scan_.reset();
	idle_scan_segment_.reset();
	idle_time_map_.clear();
	pending_idle_map_.clear();
	idle_map_position_ = 0;
	idle_ranges_ready_ = false;
	idle_compression_enabled_ = false;
	if (was_active) {
		if (idle_plain_scale_ > 0)
			set_scale_offset(idle_plain_scale_, idle_plain_offset_);
		idle_mapping_updated();
		idle_compression_changed(false, status);
	}
}

void View::set_idle_threshold(double seconds)
{
	if (!std::isfinite(seconds) || seconds < 0.000001 || seconds > 1000)
		return;
	idle_threshold_ = seconds;
	if (idle_compression_enabled_) set_idle_compression(true);
}

void View::set_idle_compression(bool enabled)
{
	reset_idle_compression();
	if (!enabled) return;
	idle_plain_scale_ = scale_;
	idle_plain_offset_ = offset_;
	auto reject = [this](const QString& text) {
		idle_compression_changed(false, text);
	};
	if (session_.get_capture_state() != Session::Stopped) {
		reject(tr("Stop capture before compressing idle time"));
		return;
	}
	shared_ptr<data::LogicSegment> segment;
	for (const auto& signal : signals_) {
		if (!signal->enabled()) continue;
		const auto logic = std::dynamic_pointer_cast<LogicSignal>(signal);
		if (!logic) {
			reject(tr("Idle compression requires digital traces only"));
			return;
		}
		const auto current = logic->get_logic_segment_to_paint();
		if (!current) continue;
		if (segment && segment != current) {
			reject(tr("Idle compression requires one synchronized digital segment"));
			return;
		}
		segment = current;
	}
	if (!segment || segment->get_sample_count() < 2 || segment->samplerate() <= 0) {
		reject(tr("No timed digital capture to compress"));
		return;
	}
	determine_time_unit();
	const double minimum = std::ceil(idle_threshold_ * segment->samplerate());
	if (!std::isfinite(minimum) || minimum >= double(UINT64_MAX)) {
		reject(tr("Idle threshold exceeds the capture range"));
		return;
	}
	try {
		idle_scan_.reset(new data::IdleScan(segment->unit_size(),
			std::max<uint64_t>(2, uint64_t(minimum))));
	} catch (const std::exception& e) {
		reject(QString::fromUtf8(e.what()));
		return;
	}
	idle_scan_segment_ = segment;
	idle_scan_count_ = segment->get_sample_count();
	idle_compression_enabled_ = true;
	idle_compression_changed(true, tr("Finding idle intervals..."));
	idle_scan_timer_.start();
}

void View::process_idle_scan()
{
	if (!idle_scan_ || !idle_scan_segment_) return;
	try {
		if (session_.get_capture_state() != Session::Stopped ||
			idle_scan_segment_->get_sample_count() != idle_scan_count_) {
			reset_idle_compression(tr("Capture changed; idle compression disabled"));
			return;
		}
		QElapsedTimer budget;
		budget.start();
		std::vector<uint8_t> bytes;
		do {
			if (!idle_ranges_ready_) {
				const auto position = idle_scan_->position();
				const auto count = std::min<uint64_t>(65536, idle_scan_count_ - position);
				bytes.resize(count * idle_scan_segment_->unit_size());
				if (count) {
					idle_scan_segment_->get_samples(position, position + count, bytes.data());
					idle_scan_->append(bytes.data(), count);
				}
				if (idle_scan_->position() == idle_scan_count_) {
					idle_scan_->finish();
					idle_ranges_ready_ = true;
				}
			}
			if (idle_ranges_ready_) {
				const double rate = idle_scan_segment_->samplerate();
				const auto origin = idle_scan_segment_->start_time();
				const auto& ranges = idle_scan_->ranges();
				uint64_t folded_samples = 0;
				for (const auto& range : ranges) folded_samples += range.second - range.first;
				// Reserve readable space for break labels when fitting rare
				// bursts, without ever expanding a short original interval.
				const Timestamp display_duration = ranges.empty() ? Timestamp(idle_threshold_) / 100 :
					max(Timestamp(idle_threshold_) / 100,
						Timestamp(idle_scan_count_ - folded_samples) / rate * 2 / 3 / ranges.size());
				while (idle_map_position_ < ranges.size() && budget.elapsed() < 10) {
					const auto& range = ranges[idle_map_position_++];
					pending_idle_map_.append(origin + Timestamp(range.first) / rate,
						origin + Timestamp(range.second) / rate,
						min(display_duration, Timestamp(range.second - range.first) / rate / 10));
				}
				if (idle_map_position_ == ranges.size()) {
					idle_scan_timer_.stop();
					idle_scan_.reset();
					idle_scan_segment_.reset();
					idle_time_map_ = std::move(pending_idle_map_);
					pending_idle_map_.clear();
					idle_mapping_updated();
					if (!idle_time_map_.empty()) zoom_fit(false);
					idle_compression_changed(true, idle_time_map_.empty() ?
						tr("No idle intervals at this threshold") :
						tr("%1 folded pauses").arg(idle_time_map_.gaps().size()));
					return;
				}
			}
		} while (budget.elapsed() < 10);
	} catch (const std::exception& e) {
		reset_idle_compression(QString::fromUtf8(e.what()));
	}
}

void View::set_scale(double scale)
{
	if (scale_ != scale) {
		scale_ = scale;

		update_view_range_metaobject();

		scale_changed();
	}
}

void View::set_offset(const pv::util::Timestamp& offset, bool force_update)
{
	if ((offset_ != offset) || force_update) {
		offset_ = offset;
		ruler_offset_ = offset_ + zero_offset_;

		update_view_range_metaobject();

		offset_changed();
	}
}

const Timestamp& View::offset() const
{
	return offset_;
}

const Timestamp& View::ruler_offset() const
{
	return ruler_offset_;
}

void View::set_zero_position(const pv::util::Timestamp& position)
{
	zero_offset_ = -position;
	custom_zero_offset_set_ = true;

	// Force an immediate update of the offsets
	set_offset(offset_, true);
	ruler_->update();
}

void View::reset_zero_position()
{
	zero_offset_ = 0;

	// When enabled, the first trigger for this segment is used as the zero position
	GlobalSettings settings;
	bool trigger_is_zero_time = settings.value(GlobalSettings::Key_View_TriggerIsZeroTime).toBool();

	if (trigger_is_zero_time) {
		vector<util::Timestamp> triggers = session_.get_triggers(current_segment_);

		if (triggers.size() > 0)
			zero_offset_ = triggers.front();
	}

	custom_zero_offset_set_ = false;

	// Force an immediate update of the offsets
	set_offset(offset_, true);
	ruler_->update();
}

pv::util::Timestamp View::zero_offset() const
{
	return zero_offset_;
}

int View::owner_visual_v_offset() const
{
	return -scrollarea_->verticalScrollBar()->sliderPosition();
}

void View::set_v_offset(int offset)
{
	scrollarea_->verticalScrollBar()->setSliderPosition(offset);
	header_->update();
	viewport_->update();
}

void View::set_h_offset(int offset)
{
	scrollarea_->horizontalScrollBar()->setSliderPosition(offset);
	header_->update();
	viewport_->update();
}

int View::get_h_scrollbar_maximum() const
{
	return scrollarea_->horizontalScrollBar()->maximum();
}

unsigned int View::depth() const
{
	return 0;
}

uint32_t View::current_segment() const
{
	return current_segment_;
}

pv::util::SIPrefix View::tick_prefix() const
{
	return tick_prefix_;
}

void View::set_tick_prefix(pv::util::SIPrefix tick_prefix)
{
	if (tick_prefix_ != tick_prefix) {
		tick_prefix_ = tick_prefix;
		tick_prefix_changed();
	}
}

unsigned int View::tick_precision() const
{
	return tick_precision_;
}

void View::set_tick_precision(unsigned tick_precision)
{
	if (tick_precision_ != tick_precision) {
		tick_precision_ = tick_precision;
		tick_precision_changed();
	}
}

const pv::util::Timestamp& View::tick_period() const
{
	return tick_period_;
}

unsigned int View::minor_tick_count() const
{
	return minor_tick_count_;
}

void View::set_tick_period(const pv::util::Timestamp& tick_period)
{
	if (tick_period_ != tick_period) {
		tick_period_ = tick_period;
		tick_period_changed();
	}
}

TimeUnit View::time_unit() const
{
	return time_unit_;
}

void View::set_time_unit(pv::util::TimeUnit time_unit)
{
	if (time_unit_ != time_unit) {
		time_unit_ = time_unit;
		time_unit_changed();
	}
}

void View::set_current_segment(uint32_t segment_id)
{
	if (current_segment_ != segment_id) reset_idle_compression();
	current_segment_ = segment_id;

	for (const shared_ptr<Signal>& signal : signals_)
		signal->set_current_segment(current_segment_);
#ifdef ENABLE_DECODE
	for (shared_ptr<DecodeTrace>& dt : decode_traces_)
		dt->set_current_segment(current_segment_);
#endif

	vector<util::Timestamp> triggers = session_.get_triggers(current_segment_);

	trigger_markers_.clear();
	for (util::Timestamp timestamp : triggers)
		trigger_markers_.push_back(make_shared<TriggerMarker>(*this, timestamp));

	if (!custom_zero_offset_set_)
		reset_zero_position();

	viewport_->update();

	segment_changed(segment_id);
}

bool View::segment_is_selectable() const
{
	return segment_selectable_;
}

Trace::SegmentDisplayMode View::segment_display_mode() const
{
	return segment_display_mode_;
}

void View::set_segment_display_mode(Trace::SegmentDisplayMode mode)
{
	if (segment_display_mode_ != mode) reset_idle_compression();
	segment_display_mode_ = mode;

	for (const shared_ptr<Signal>& signal : signals_)
		signal->set_segment_display_mode(mode);

	uint32_t last_segment = session_.get_highest_segment_id();

	switch (mode) {
	case Trace::ShowLastSegmentOnly:
		if (current_segment_ != last_segment)
			set_current_segment(last_segment);
		break;

	case Trace::ShowLastCompleteSegmentOnly:
		// Do nothing if we only have one segment so far
		if (last_segment > 0) {
			// If the last segment isn't complete, the previous one must be
			uint32_t segment_id =
				(session_.all_segments_complete(last_segment)) ?
				last_segment : last_segment - 1;

			if (current_segment_ != segment_id)
				set_current_segment(segment_id);
		}
		break;

	case Trace::ShowSingleSegmentOnly:
	case Trace::ShowAllSegments:
	case Trace::ShowAccumulatedIntensity:
	default:
		// Current segment remains as-is
		break;
	}

	segment_selectable_ = true;

	if ((mode == Trace::ShowAllSegments) || (mode == Trace::ShowAccumulatedIntensity))
		segment_selectable_ = false;

	viewport_->update();

	segment_display_mode_changed((int)mode, segment_selectable_);
}

void View::zoom(double steps)
{
	zoom(steps, viewport_->width() / 2);
}

void View::zoom(double steps, int offset)
{
	set_zoom(scale_ * pow(3.0 / 2.0, -steps), offset);
}

void View::zoom_fit(bool gui_state)
{
	// Act as one-shot when stopped, toggle along with the GUI otherwise
	if (session_.get_capture_state() == Session::Stopped) {
		always_zoom_to_fit_ = false;
		always_zoom_to_fit_changed(false);
	} else {
		always_zoom_to_fit_ = gui_state;
		always_zoom_to_fit_changed(gui_state);
	}

	const pair<Timestamp, Timestamp> extents = get_time_extents();
	const Timestamp delta = idle_time_map_.to_display(extents.second) -
		idle_time_map_.to_display(extents.first);
	if (delta < Timestamp("1e-12"))
		return;

	assert(viewport_);
	const int w = viewport_->width();
	if (w <= 0)
		return;

	const Timestamp scale = max(min(delta / w, MaxScale), MinScale);
	set_scale_offset(scale.convert_to<double>(), extents.first);
}

void View::focus_on_range(uint64_t start_sample, uint64_t end_sample)
{
	assert(viewport_);
	const uint64_t w = viewport_->width();
	if (w <= 0)
		return;

	const double samplerate = session_.get_samplerate();
	if (!idle_time_map_.empty() && samplerate > 0) {
		const Timestamp first = Timestamp(start_sample) / samplerate;
		const Timestamp last = Timestamp(end_sample) / samplerate;
		const Timestamp span = idle_time_map_.to_display(last) - idle_time_map_.to_display(first);
		const Timestamp fit_scale = max(MinScale, min(MaxScale, span * 1.25 / w));
		set_scale_offset(fit_scale.convert_to<double>(), idle_time_map_.to_real(
			idle_time_map_.to_display(first) - span * 0.2));
		return;
	}
	const double samples_per_pixel = samplerate * scale_;
	const uint64_t viewport_samples = w * samples_per_pixel;

	const uint64_t sample_delta = (end_sample - start_sample);

	// Note: We add 20% margin on the left and 5% on the right
	const uint64_t ext_sample_delta = sample_delta * 1.25;

	// Check if we can keep the zoom level and just center the supplied range
	if (viewport_samples >= ext_sample_delta) {
		// Note: offset is the left edge of the view so to center, we subtract half the view width
		const int64_t sample_offset = (start_sample + (sample_delta / 2) - (viewport_samples / 2));
		const Timestamp offset = sample_offset / samplerate;
		set_scale_offset(scale_, offset);
	} else {
		const Timestamp offset = (start_sample - sample_delta * 0.20) / samplerate;
		const Timestamp delta = ext_sample_delta / samplerate;
		const Timestamp scale = max(min(delta / w, MaxScale), MinScale);
		set_scale_offset(scale.convert_to<double>(), offset);
	}
}

void View::set_scale_offset(double scale, const Timestamp& offset)
{
	// Disable sticky scrolling / always zoom to fit when acquisition runs
	// and user drags the viewport
	if ((scale_ == scale) && (offset_ != offset) &&
		(session_.get_capture_state() == Session::Running)) {

		if (sticky_scrolling_) {
			sticky_scrolling_ = false;
			sticky_scrolling_changed(false);
		}

		if (always_zoom_to_fit_) {
			always_zoom_to_fit_ = false;
			always_zoom_to_fit_changed(false);
		}
	}

	set_scale(scale);
	set_offset(offset);

	calculate_tick_spacing();

	update_scroll();
	ruler_->update();
	viewport_->update();
}

vector< shared_ptr<SignalData> > View::get_visible_data() const
{
	// Make a set of all the visible data objects
	vector< shared_ptr<SignalData> > visible_data;
	for (const shared_ptr<Signal>& sig : signals_)
		if (sig->enabled())
			visible_data.push_back(sig->base()->data());

	return visible_data;
}

pair<Timestamp, Timestamp> View::get_time_extents() const
{
	boost::optional<Timestamp> left_time, right_time;

	vector< shared_ptr<SignalData> > data;
	if (signals_.size() == 0)
		return make_pair(0, 0);

	for (const shared_ptr<Signal>& s : signals_)
		if (s->base()->data() && (s->base()->data()->segments().size() > 0))
			data.push_back(s->base()->data());

	for (const shared_ptr<SignalData>& d : data) {
		const vector< shared_ptr<Segment> > segments = d->segments();
		for (const shared_ptr<Segment>& s : segments) {
			double samplerate = s->samplerate();
			samplerate = (samplerate <= 0.0) ? 1.0 : samplerate;

			const Timestamp start_time = s->start_time();
			left_time = left_time ?
				min(*left_time, start_time) :
				                start_time;
			right_time = right_time ?
				max(*right_time, start_time + d->max_sample_count() / samplerate) :
				                 start_time + d->max_sample_count() / samplerate;
		}
	}

	if (!left_time || !right_time)
		return make_pair(0, 0);

	assert(*left_time <= *right_time);
	return make_pair(*left_time, *right_time);
}

bool View::colored_bg() const
{
	return colored_bg_;
}

bool View::cursors_shown() const
{
	return show_cursors_;
}

void View::show_cursors(bool show)
{
	if (show_cursors_ != show) {
		show_cursors_ = show;

		cursor_state_changed(show);
		ruler_->update();
		viewport_->update();
	}
}

void View::set_cursors(pv::util::Timestamp& first, pv::util::Timestamp& second)
{
	assert(cursors_);

	cursors_->first()->set_time(first);
	cursors_->second()->set_time(second);

	ruler_->update();
	viewport_->update();
}

void View::center_cursors()
{
	assert(cursors_);

	cursors_->first()->set_time(pixel_to_time(viewport_->width() * 0.4));
	cursors_->second()->set_time(pixel_to_time(viewport_->width() * 0.6));

	ruler_->update();
	viewport_->update();
}

shared_ptr<CursorPair> View::cursors() const
{
	return cursors_;
}

shared_ptr<Flag> View::add_flag(const Timestamp& time)
{
	shared_ptr<Flag> flag =
		make_shared<Flag>(*this, time, QString("%1").arg(next_flag_text_));
	flags_.push_back(flag);

	next_flag_text_ = (next_flag_text_ >= 'Z') ? 'A' :
		(next_flag_text_ + 1);

	time_item_appearance_changed(true, true);
	return flag;
}

void View::remove_flag(shared_ptr<Flag> flag)
{
	flags_.remove(flag);
	time_item_appearance_changed(true, true);
}

vector< shared_ptr<Flag> > View::flags() const
{
	vector< shared_ptr<Flag> > flags(flags_.begin(), flags_.end());
	stable_sort(flags.begin(), flags.end(),
		[](const shared_ptr<Flag> &a, const shared_ptr<Flag> &b) {
			return a->time() < b->time();
		});

	return flags;
}

const QPoint& View::hover_point() const
{
	return hover_point_;
}

const QWidget* View::hover_widget() const
{
	return hover_widget_;
}

int64_t View::get_nearest_level_change(const QPoint &p)
{
	// Is snapping disabled?
	if (snap_distance_ == 0)
		return -1;

	struct entry_t {
		entry_t(shared_ptr<Signal> s) :
			signal(s), delta(numeric_limits<int64_t>::max()), sample(-1), is_dense(false) {}
		shared_ptr<Signal> signal;
		int64_t delta;
		int64_t sample;
		bool is_dense;
	};

	vector<entry_t> list;

	// Create list of signals to consider
	if (signal_under_mouse_cursor_)
		list.emplace_back(signal_under_mouse_cursor_);
	else
		for (shared_ptr<Signal> s : signals_) {
			if (!s->enabled())
				continue;

			list.emplace_back(s);
		}

	// Get data for listed signals
	for (entry_t &e : list) {
		// Calculate sample number from cursor position
		const double rate = e.signal->base()->get_samplerate();
		if (rate <= 0) continue;
		Timestamp origin(0);
		const auto logic = std::dynamic_pointer_cast<LogicSignal>(e.signal);
		if (logic && logic->get_logic_segment_to_paint())
			origin = logic->get_logic_segment_to_paint()->start_time();
		const int64_t sample_num = max(Timestamp(0),
			(pixel_to_time(p.x()) - origin) * rate).convert_to<int64_t>();

		vector<data::LogicSegment::EdgePair> edges =
			e.signal->get_nearest_level_changes(sample_num);

		if (edges.empty())
			continue;

		// Check first edge
		const int64_t first_delta = std::abs(time_to_pixel(
			origin + Timestamp(edges.front().first) / rate) - p.x());
		e.delta = first_delta;
		e.sample = edges.front().first;

		// Check second edge if available
		if (edges.size() == 2) {
			// Note: -1 because this is usually the right edge and sample points are left-aligned
			const int64_t second_delta = std::abs(time_to_pixel(
				origin + Timestamp(edges.back().first + 1) / rate) - p.x());

			// If both edges are too close, we mark this signal as being dense
			if ((first_delta + second_delta) <= snap_distance_)
				e.is_dense = true;

			if (second_delta < first_delta) {
				e.delta = second_delta;
				e.sample = edges.back().first;
			}
		}
	}

	// Look for the best match: non-dense first, then dense
	entry_t *match = nullptr;

	for (entry_t &e : list) {
		if (e.delta > snap_distance_ || e.is_dense)
			continue;

		if (match) {
			if (e.delta < match->delta)
				match = &e;
		} else
			match = &e;
	}

	if (!match) {
		for (entry_t &e : list) {
			if (!e.is_dense)
				continue;

			if (match) {
				if (e.delta < match->delta)
					match = &e;
			} else
				match = &e;
		}
	}

	if (match) {
		// Somewhat ugly hack to make TimeItem::drag_by() work
		signal_under_mouse_cursor_ = match->signal;

		return match->sample;
	}

	return -1;
}

void View::restack_all_trace_tree_items()
{
	// Make a list of owners that is sorted from deepest first
	const vector<shared_ptr<TraceTreeItem>> items(
		list_by_type<TraceTreeItem>());
	set< TraceTreeItemOwner* > owners;
	for (const auto &r : items)
		owners.insert(r->owner());
	vector< TraceTreeItemOwner* > sorted_owners(owners.begin(), owners.end());
	sort(sorted_owners.begin(), sorted_owners.end(),
		[](const TraceTreeItemOwner* a, const TraceTreeItemOwner *b) {
			return a->depth() > b->depth(); });

	// Restack the items recursively
	for (auto &o : sorted_owners)
		o->restack_items();

	// Animate the items to their destination
	for (const auto &i : items)
		i->animate_to_layout_v_offset();
}

int View::header_width() const
{
	 return header_->extended_size_hint().width();
}

void View::on_setting_changed(const QString &key, const QVariant &value)
{
	GlobalSettings settings;

	if (key == GlobalSettings::Key_View_ColoredBG) {
		colored_bg_ = settings.value(GlobalSettings::Key_View_ColoredBG).toBool();
		viewport_->update();
	}

	if ((key == GlobalSettings::Key_View_ShowSamplingPoints) ||
	   (key == GlobalSettings::Key_View_ShowAnalogMinorGrid))
		viewport_->update();

	if (key == GlobalSettings::Key_View_TriggerIsZeroTime)
		on_settingViewTriggerIsZeroTime_changed(value);

	if (key == GlobalSettings::Key_View_SnapDistance)
		snap_distance_ = settings.value(GlobalSettings::Key_View_SnapDistance).toInt();
}

void View::trigger_event(int segment_id, util::Timestamp location)
{
	// TODO This doesn't work if we're showing multiple segments at once
	if ((uint32_t)segment_id != current_segment_)
		return;

	if (!custom_zero_offset_set_)
		reset_zero_position();

	trigger_markers_.push_back(make_shared<TriggerMarker>(*this, location));
}

void View::get_scroll_layout(double &length, Timestamp &offset) const
{
	const pair<Timestamp, Timestamp> extents = get_time_extents();
	length = ((idle_time_map_.to_display(extents.second) -
		idle_time_map_.to_display(extents.first)) / scale_).convert_to<double>();
	offset = idle_time_map_.empty() ? offset_ / scale_ :
		(idle_time_map_.to_display(offset_) - idle_time_map_.to_display(extents.first)) / scale_;
}

void View::set_zoom(double scale, int offset)
{
	// Reset the "always zoom to fit" feature as the user changed the zoom
	always_zoom_to_fit_ = false;
	always_zoom_to_fit_changed(false);

	const Timestamp cursor_offset = idle_time_map_.to_display(pixel_to_time(offset));
	const Timestamp new_scale = max(min(Timestamp(scale), MaxScale), MinScale);
	const Timestamp new_offset = idle_time_map_.to_real(cursor_offset - new_scale * offset);
	set_scale_offset(new_scale.convert_to<double>(), new_offset);
}

void View::calculate_tick_spacing()
{
	const double SpacingIncrement = 10.0f;
	const double MinValueSpacing = 40.0f;

	// Figure out the highest numeric value visible on a label
	const QSize areaSize = viewport_->size();
	const Timestamp max_time = max(fabs(offset_),
		fabs(pixel_to_time(areaSize.width())));

	double min_width = SpacingIncrement;
	double label_width, tick_period_width;

	QFontMetrics m(QApplication::font());

	// Copies of the member variables with the same name, used in the calculation
	// and written back afterwards, so that we don't emit signals all the time
	// during the calculation.
	pv::util::Timestamp tick_period = tick_period_;
	pv::util::SIPrefix tick_prefix = tick_prefix_;
	unsigned tick_precision = tick_precision_;

	do {
		const double min_period = scale_ * min_width;

		const int order = (int)floorf(log10f(min_period));
		const pv::util::Timestamp order_decimal =
			pow(pv::util::Timestamp(10), order);

		// Allow for a margin of error so that a scale unit of 1 can be used.
		// Otherwise, for a SU of 1 the tick period will almost always be below
		// the min_period by a small amount - and thus skipped in favor of 2.
		// Note: margin assumes that SU[0] and SU[1] contain the smallest values
		double tp_margin = (ScaleUnits[0] + ScaleUnits[1]) / 2.0;
		double tp_with_margin;
		unsigned int unit = 0;

		do {
			tp_with_margin = order_decimal.convert_to<double>() *
				(ScaleUnits[unit++] + tp_margin);
		} while (tp_with_margin < min_period && unit < countof(ScaleUnits));

		minor_tick_count_ = (unit == 2) ? 4 : 5;
		tick_period = order_decimal * ScaleUnits[unit - 1];
		tick_prefix = static_cast<pv::util::SIPrefix>(
			(order - pv::util::exponent(pv::util::SIPrefix::yocto)) / 3);

		// Precision is the number of fractional digits required, not
		// taking the prefix into account (and it must never be negative)
		tick_precision = max(ceil(log10(1 / tick_period)).convert_to<int>(), 0);

		tick_period_width = (tick_period / scale_).convert_to<double>();

		const QString label_text = Ruler::format_time_with_distance(
			tick_period, max_time, tick_prefix, time_unit_, tick_precision);

		label_width = m.boundingRect(0, 0, INT_MAX, INT_MAX,
			Qt::AlignLeft | Qt::AlignTop, label_text).width() +
				MinValueSpacing;

		min_width += SpacingIncrement;
	} while (tick_period_width < label_width);

	set_tick_period(tick_period);
	set_tick_prefix(tick_prefix);
	set_tick_precision(tick_precision);
}

void View::adjust_top_margin()
{
	assert(viewport_);

	const QSize areaSize = viewport_->size();

	const pair<int, int> extents = v_extents();
	const int top_margin = owner_visual_v_offset() + extents.first;
	const int trace_bottom = owner_visual_v_offset() + extents.first + extents.second;

	// Do we have empty space at the top while the last trace goes out of screen?
	if ((top_margin > 0) && (trace_bottom > areaSize.height())) {
		const int trace_height = extents.second - extents.first;

		// Center everything vertically if there is enough space
		if (areaSize.height() >= trace_height)
			set_v_offset(extents.first -
				((areaSize.height() - trace_height) / 2));
		else
			// Remove the top margin to make as many traces fit on screen as possible
			set_v_offset(extents.first);
	}
}

void View::update_scroll()
{
	assert(viewport_);
	QScrollBar *hscrollbar = scrollarea_->horizontalScrollBar();
	QScrollBar *vscrollbar = scrollarea_->verticalScrollBar();

	const QSize areaSize = viewport_->size();

	// Set the horizontal scroll bar
	double length = 0;
	Timestamp offset;
	get_scroll_layout(length, offset);
	length = max(length - areaSize.width(), 0.0);

	int major_tick_distance = (tick_period_ / scale_).convert_to<int>();

	hscrollbar->setPageStep(areaSize.width() / 2);
	hscrollbar->setSingleStep(major_tick_distance);

	updating_scroll_ = true;

	if (length < MaxScrollValue) {
		hscrollbar->setRange(0, length);
		hscrollbar->setSliderPosition(offset.convert_to<double>());
	} else {
		hscrollbar->setRange(0, MaxScrollValue);
		hscrollbar->setSliderPosition(
			(idle_time_map_.empty() ? offset_ * MaxScrollValue / (scale_ * length) :
			offset * MaxScrollValue / length).convert_to<double>());
	}

	updating_scroll_ = false;

	// Set the vertical scrollbar
	vscrollbar->setPageStep(areaSize.height());
	vscrollbar->setSingleStep(areaSize.height() / 8);

	const pair<int, int> extents = v_extents();

	// Don't change the scrollbar range if there are no traces
	if (extents.first != extents.second) {
		int top_margin = ViewScrollMargin;
		int btm_margin = ViewScrollMargin;

		vscrollbar->setRange(extents.first - areaSize.height() + top_margin,
			extents.second - btm_margin);
	}

	if (scroll_needs_defaults_) {
		set_scroll_default();
		scroll_needs_defaults_ = false;
	}
}

void View::reset_scroll()
{
	scrollarea_->verticalScrollBar()->setRange(0, 0);
}

void View::set_scroll_default()
{
	assert(viewport_);

	const QSize areaSize = viewport_->size();

	const pair<int, int> extents = v_extents();
	const int trace_height = extents.second - extents.first;

	// Do all traces fit in the view?
	if (areaSize.height() >= trace_height)
		// Center all traces vertically
		set_v_offset(extents.first -
			((areaSize.height() - trace_height) / 2));
	else
		// Put the first trace at the top, letting the bottom ones overflow
		set_v_offset(extents.first);
}

void View::determine_if_header_was_shrunk()
{
	const int header_pane_width =
		splitter_->sizes().front();  // clazy:exclude=detaching-temporary

	// Allow for a slight margin of error so that we also accept
	// slight differences when e.g. a label name change increased
	// the overall width
	header_was_shrunk_ = (header_pane_width < (header_width() - 10));
}

void View::resize_header_to_fit()
{
	// Setting the maximum width of the header widget doesn't work as
	// expected because the splitter would allow the user to make the
	// pane wider than that, creating empty space as a result.
	// To make this work, we stricly enforce the maximum width by
	// expanding the header unless the user shrunk it on purpose.
	// As we're then setting the width of the header pane, we set the
	// splitter to the maximum allowed position.

	int splitter_area_width = 0;
	for (int w : splitter_->sizes())  // clazy:exclude=range-loop
		splitter_area_width += w;

	// Make sure the header has enough horizontal space to show all labels fully
	QList<int> pane_sizes;
	pane_sizes.push_back(header_->extended_size_hint().width());
	pane_sizes.push_back(splitter_area_width - header_->extended_size_hint().width());
	splitter_->setSizes(pane_sizes);
}

void View::update_layout()
{
	update_scroll();

	update_view_range_metaobject();
}

TraceTreeItemOwner* View::find_prevalent_trace_group(
	const shared_ptr<sigrok::ChannelGroup> &group,
	const map<shared_ptr<data::SignalBase>, shared_ptr<Signal> > &signal_map)
{
	assert(group);

	set<TraceTreeItemOwner*> owners;
	vector<TraceTreeItemOwner*> owner_list;

	// Make a set and a list of all the owners
	for (const auto& channel : group->channels()) {
		for (auto& entry : signal_map) {
			if (entry.first->channel() == channel) {
				TraceTreeItemOwner *const o = (entry.second)->owner();
				owner_list.push_back(o);
				owners.insert(o);
			}
		}
	}

	// Iterate through the list of owners, and find the most prevalent
	size_t max_prevalence = 0;
	TraceTreeItemOwner *prevalent_owner = nullptr;
	for (TraceTreeItemOwner *owner : owners) {
		const size_t prevalence = count_if(
			owner_list.begin(), owner_list.end(),
			[&](TraceTreeItemOwner *o) { return o == owner; });
		if (prevalence > max_prevalence) {
			max_prevalence = prevalence;
			prevalent_owner = owner;
		}
	}

	return prevalent_owner;
}

vector< shared_ptr<Trace> > View::extract_new_traces_for_channels(
	const vector< shared_ptr<sigrok::Channel> > &channels,
	const map<shared_ptr<data::SignalBase>, shared_ptr<Signal> > &signal_map,
	set< shared_ptr<Trace> > &add_list)
{
	vector< shared_ptr<Trace> > filtered_traces;

	for (const auto& channel : channels) {
		for (auto& entry : signal_map) {
			if (entry.first->channel() == channel) {
				shared_ptr<Trace> trace = entry.second;
				const auto list_iter = add_list.find(trace);
				if (list_iter == add_list.end())
					continue;

				filtered_traces.push_back(trace);
				add_list.erase(list_iter);
			}
		}
	}

	return filtered_traces;
}

void View::determine_time_unit()
{
	// Check whether we know the sample rate and hence can use time as the unit
	if (time_unit_ == util::TimeUnit::Samples) {
		// Check all signals but...
		for (const shared_ptr<Signal>& signal : signals_) {
			const shared_ptr<SignalData> data = signal->base()->data();

			// ...only check first segment of each
			const vector< shared_ptr<Segment> > segments = data->segments();
			if (!segments.empty())
				if (segments[0]->samplerate()) {
					set_time_unit(util::TimeUnit::Time);
					break;
				}
		}
	}
}

bool View::eventFilter(QObject *object, QEvent *event)
{
	const QEvent::Type type = event->type();

	if (type == QEvent::MouseMove) {

		if (object)
			hover_widget_ = qobject_cast<QWidget*>(object);

		const QMouseEvent *const mouse_event = (QMouseEvent*)event;
		if (object == viewport_)
			hover_point_ = mouse_event->pos();
		else if (object == ruler_)
			hover_point_ = mouse_event->pos();
		else if (object == header_)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
			hover_point_ = QPoint(0, mouse_event->pos().y());
#else
			hover_point_ = QPoint(0, mouse_event->y());
#endif
		else
			hover_point_ = QPoint(-1, -1);

		update_hover_point();
		if (object == viewport_ || object == ruler_) {
			const QString text = idle_gap_text(hover_point_.x());
			viewport_->setToolTip(text);
			ruler_->setToolTip(text);
		}

		if (grabbed_widget_) {
			int64_t nearest = get_nearest_level_change(hover_point_);
			pv::util::Timestamp mouse_time = pixel_to_time(hover_point_.x());

			if (nearest == -1) {
				grabbed_widget_->set_time(mouse_time);
			} else {
				grabbed_widget_->set_time(snapped_time(nearest));
			}
		}

	} else if (type == QEvent::MouseButtonPress) {
		grabbed_widget_ = nullptr;

		const QMouseEvent *const mouse_event = (QMouseEvent*)event;
		if ((object == viewport_) && (mouse_event->button() & Qt::LeftButton)) {
			// Send event to all trace tree items
			const vector<shared_ptr<TraceTreeItem>> trace_tree_items(
				list_by_type<TraceTreeItem>());
			for (const shared_ptr<TraceTreeItem>& r : trace_tree_items)
				r->mouse_left_press_event(mouse_event);
		}
	} else if (type == QEvent::Leave) {
		hover_point_ = QPoint(-1, -1);
		update_hover_point();
	} else if (type == QEvent::Show) {

		// This is somewhat of a hack, unfortunately. We cannot use
		// set_v_offset() from within restore_settings() as the view
		// at that point is neither visible nor properly sized.
		// This is the least intrusive workaround I could come up
		// with: set the vertical offset (or scroll defaults) when
		// the view is shown, which happens after all widgets were
		// resized to their final sizes.
		update_layout();

		if (restoring_state_)
			determine_if_header_was_shrunk();
		else
			resize_header_to_fit();

		if (scroll_needs_defaults_) {
			set_scroll_default();
			scroll_needs_defaults_ = false;
		}

		if (restoring_state_)
			set_v_offset(saved_v_offset_);
	}

	return QObject::eventFilter(object, event);
}

void View::contextMenuEvent(QContextMenuEvent *event)
{
	QPoint pos = event->pos() - QPoint(0, ruler_->sizeHint().height());

	const shared_ptr<ViewItem> r = viewport_->get_mouse_over_item(pos);

	QMenu* menu = nullptr;

	if (!r) {
		context_menu_x_pos_ = pos.x();

		// No view item under cursor, use generic menu
		menu = new QMenu(this);

		QAction *const create_marker_here = new QAction(tr("Create marker here"), this);
		connect(create_marker_here, SIGNAL(triggered()), this, SLOT(on_create_marker_here()));
		menu->addAction(create_marker_here);
	} else {
		menu = r->create_view_context_menu(this, pos);
	}

	if (menu)
		menu->popup(event->globalPos());
}

void View::resizeEvent(QResizeEvent* event)
{
	// Only adjust the top margin if we shrunk vertically
	if (event->size().height() < event->oldSize().height())
		adjust_top_margin();

	update_layout();
}

void View::update_view_range_metaobject() const
{
	const int w = viewport_->width();
	if (w > 0) {
		const double samplerate = session_.get_samplerate();
		// Note: sample_num = time * samplerate
		// Note: samples_per_pixel = samplerate * scale
		const int64_t start_sample = (offset_ * samplerate).convert_to<int64_t>();
		const int64_t end_sample = (pixel_to_time(w) * samplerate).convert_to<int64_t>();

		MetadataObject* md_obj =
			session_.metadata_obj_manager()->find_object_by_type(MetadataObjMainViewRange);

		const int64_t old_start_sample = md_obj->value(MetadataValueStartSample).toLongLong();
		const int64_t old_end_sample = md_obj->value(MetadataValueEndSample).toLongLong();

		if (start_sample != old_start_sample)
			md_obj->set_value(MetadataValueStartSample, QVariant((qlonglong)start_sample));

		if (end_sample != old_end_sample)
			md_obj->set_value(MetadataValueEndSample, QVariant((qlonglong)end_sample));
	}
}

void View::update_hover_point()
{
	// Determine signal that the mouse cursor is hovering over
	signal_under_mouse_cursor_.reset();
	if (hover_widget_ == this) {
		for (const shared_ptr<Signal>& s : signals_) {
			const pair<int, int> extents = s->v_extents();
			const int top = s->get_visual_y() + extents.first;
			const int btm = s->get_visual_y() + extents.second;
			if ((hover_point_.y() >= top) && (hover_point_.y() <= btm)
				&& s->base()->enabled())
				signal_under_mouse_cursor_ = s;
		}
	}

	// Update all trace tree items
	const vector<shared_ptr<TraceTreeItem>> trace_tree_items(
		list_by_type<TraceTreeItem>());
	for (const shared_ptr<TraceTreeItem>& r : trace_tree_items)
		r->hover_point_changed(hover_point_);

	// Notify this view's listeners
	hover_point_changed(hover_widget_, hover_point_);

	// Hover point is -1 when invalid and 0 for the header
	if (hover_point_.x() > 0) {
		// Notify global listeners
		pv::util::Timestamp mouse_time = pixel_to_time(hover_point_.x());
		int64_t sample_num = (mouse_time * session_.get_samplerate()).convert_to<int64_t>();

		MetadataObject* md_obj =
			session_.metadata_obj_manager()->find_object_by_type(MetadataObjMousePos);
		md_obj->set_value(MetadataValueStartSample, QVariant((qlonglong)sample_num));
	}
}

void View::row_item_appearance_changed(bool label, bool content)
{
	if (label)
		header_->update();
	if (content)
		viewport_->update();
}

void View::time_item_appearance_changed(bool label, bool content)
{
	if (label) {
		ruler_->update();

		// Make sure the header pane width is updated, too
		update_layout();
	}

	if (content)
		viewport_->update();
}

void View::extents_changed(bool horz, bool vert)
{
	if (horz && idle_compression_enabled_)
		for (const auto& signal : signals_)
			if (signal->enabled() && !std::dynamic_pointer_cast<LogicSignal>(signal)) {
				reset_idle_compression(tr("Idle compression requires digital traces only"));
				break;
			}
	sticky_events_ |=
		(horz ? TraceTreeItemHExtentsChanged : 0) |
		(vert ? TraceTreeItemVExtentsChanged : 0);

	if (!lazy_event_handler_.isActive())
		lazy_event_handler_.start();
}

void View::on_signal_name_changed()
{
	if (!header_was_shrunk_)
		resize_header_to_fit();
}

void View::on_splitter_moved()
{
	// The header can only shrink when the splitter is moved manually
	determine_if_header_was_shrunk();

	if (!header_was_shrunk_)
		resize_header_to_fit();
}

void View::on_zoom_in_shortcut_triggered()
{
	zoom(1);
}

void View::on_zoom_out_shortcut_triggered()
{
	zoom(-1);
}

void View::on_scroll_to_start_shortcut_triggered()
{
	set_h_offset(0);
}

void View::on_scroll_to_end_shortcut_triggered()
{
	set_h_offset(get_h_scrollbar_maximum());
}

void View::h_scroll_value_changed(int value)
{
	if (updating_scroll_)
		return;

	// Disable sticky scrolling when user moves the horizontal scroll bar
	// during a running acquisition
	if (sticky_scrolling_ && (session_.get_capture_state() == Session::Running)) {
		sticky_scrolling_ = false;
		sticky_scrolling_changed(false);
	}

	const int range = scrollarea_->horizontalScrollBar()->maximum();
	if (!idle_time_map_.empty()) {
		double length = 0; Timestamp ignored;
		get_scroll_layout(length, ignored);
		const auto origin = idle_time_map_.to_display(get_time_extents().first);
		const double pixels = range < MaxScrollValue ? double(value) :
			std::max(0.0, length - viewport_->width()) * value / MaxScrollValue;
		set_offset(idle_time_map_.to_real(origin + scale_ * pixels));
	} else if (range < MaxScrollValue)
		set_offset(scale_ * value);
	else {
		double length = 0;
		Timestamp offset;
		get_scroll_layout(length, offset);
		set_offset(scale_ * length * value / MaxScrollValue);
	}

	ruler_->update();
	viewport_->update();
}

void View::v_scroll_value_changed()
{
	header_->update();
	viewport_->update();
}

void View::on_grab_ruler(int ruler_id)
{
	if (!cursors_shown()) {
		center_cursors();
		show_cursors();
	}

	// Release the grabbed widget if its trigger hotkey was pressed twice
	if (ruler_id == 1)
		grabbed_widget_ = (grabbed_widget_ == cursors_->first().get()) ?
			nullptr : cursors_->first().get();
	else
		grabbed_widget_ = (grabbed_widget_ == cursors_->second().get()) ?
			nullptr : cursors_->second().get();

	if (grabbed_widget_)
		grabbed_widget_->set_time(ruler_->get_absolute_time_from_x_pos(
			mapFromGlobal(QCursor::pos()).x() - header_width()));
}

void View::signals_changed()
{
	reset_idle_compression();
	using sigrok::Channel;

	lock_guard<mutex> lock(signal_mutex_);

	vector< shared_ptr<Channel> > channels;
	shared_ptr<sigrok::Device> sr_dev;
	bool signals_added_or_removed = false;

	// Do we need to set the vertical scrollbar to its default position later?
	// We do if there are no traces, i.e. the scroll bar has no range set
	bool reset_scrollbar =
		(scrollarea_->verticalScrollBar()->minimum() ==
			scrollarea_->verticalScrollBar()->maximum());

	if (!session_.device()) {
		reset_scroll();
		signals_.clear();
	} else {
		sr_dev = session_.device()->device();
		assert(sr_dev);
		channels = sr_dev->channels();
	}

	vector< shared_ptr<TraceTreeItem> > new_top_level_items;

	// Make a list of traces that are being added, and a list of traces
	// that are being removed. The set_difference() algorithms require
	// both sets to be in the exact same order, which means that PD signals
	// must always be last as they interrupt the sort order otherwise
	const vector<shared_ptr<Trace>> prev_trace_list = list_by_type<Trace>();
	const set<shared_ptr<Trace>> prev_traces(
		prev_trace_list.begin(), prev_trace_list.end());

	set< shared_ptr<Trace> > traces(signals_.begin(), signals_.end());

#ifdef ENABLE_DECODE
	traces.insert(decode_traces_.begin(), decode_traces_.end());
#endif
	set< shared_ptr<Trace> > add_traces;
	set_difference(traces.begin(), traces.end(),
		prev_traces.begin(), prev_traces.end(),
		inserter(add_traces, add_traces.begin()));

	set< shared_ptr<Trace> > remove_traces;
	set_difference(prev_traces.begin(), prev_traces.end(),
		traces.begin(), traces.end(),
		inserter(remove_traces, remove_traces.begin()));

	// Make a look-up table of sigrok Channels to pulseview Signals
	map<shared_ptr<data::SignalBase>, shared_ptr<Signal> > signal_map;
	for (const shared_ptr<Signal>& sig : signals_)
		signal_map[sig->base()] = sig;

	// Populate channel groups
	if (sr_dev)
		for (auto& entry : sr_dev->channel_groups()) {
			const shared_ptr<sigrok::ChannelGroup> &group = entry.second;

			if (group->channels().size() <= 1)
				continue;

			// Find best trace group to add to
			TraceTreeItemOwner *owner = find_prevalent_trace_group(
				group, signal_map);

			// If there is no trace group, create one
			shared_ptr<TraceGroup> new_trace_group;
			if (!owner) {
				new_trace_group.reset(new TraceGroup());
				owner = new_trace_group.get();
			}

			// Extract traces for the trace group, removing them from
			// the add list
			const vector< shared_ptr<Trace> > new_traces_in_group =
				extract_new_traces_for_channels(group->channels(),
					signal_map, add_traces);

			// Add the traces to the group
			const pair<int, int> prev_v_extents = owner->v_extents();
			int offset = prev_v_extents.second - prev_v_extents.first;
			for (const shared_ptr<Trace>& trace : new_traces_in_group) {
				assert(trace);
				owner->add_child_item(trace);

				const pair<int, int> extents = trace->v_extents();
				if (trace->enabled())
					offset += -extents.first;
				trace->force_to_v_offset(offset);
				if (trace->enabled())
					offset += extents.second;
			}

			if (new_trace_group) {
				// Assign proper vertical offsets to each channel in the group
				new_trace_group->restack_items();

				// If this is a new group, enqueue it in the new top level
				// items list
				if (!new_traces_in_group.empty())
					new_top_level_items.push_back(new_trace_group);
			}
		}

	// Enqueue the remaining logic channels in a group
	vector< shared_ptr<Channel> > logic_channels;
	copy_if(channels.begin(), channels.end(), back_inserter(logic_channels),
		[](const shared_ptr<Channel>& c) {
			return c->type() == sigrok::ChannelType::LOGIC; });

	const vector< shared_ptr<Trace> > non_grouped_logic_signals =
		extract_new_traces_for_channels(logic_channels,	signal_map, add_traces);

	if (non_grouped_logic_signals.size() > 0) {
		const shared_ptr<TraceGroup> non_grouped_trace_group(
			make_shared<TraceGroup>());
		for (const shared_ptr<Trace>& trace : non_grouped_logic_signals)
			non_grouped_trace_group->add_child_item(trace);

		non_grouped_trace_group->restack_items();
		new_top_level_items.push_back(non_grouped_trace_group);
	}

	// Enqueue the remaining channels as free ungrouped traces
	const vector< shared_ptr<Trace> > new_top_level_signals =
		extract_new_traces_for_channels(channels, signal_map, add_traces);
	new_top_level_items.insert(new_top_level_items.end(),
		new_top_level_signals.begin(), new_top_level_signals.end());

	// Enqueue any remaining traces i.e. decode traces
	new_top_level_items.insert(new_top_level_items.end(),
		add_traces.begin(), add_traces.end());

	// Remove any removed traces
	for (const shared_ptr<Trace>& trace : remove_traces) {
		TraceTreeItemOwner *const owner = trace->owner();
		assert(owner);
		owner->remove_child_item(trace);
		signals_added_or_removed = true;
	}

	// Remove any empty trace groups
	for (shared_ptr<TraceGroup> group : list_by_type<TraceGroup>())
		if (group->child_items().size() == 0) {
			remove_child_item(group);
			group.reset();
		}

	// Add and position the pending top levels items
	int offset = v_extents().second;
	for (shared_ptr<TraceTreeItem> item : new_top_level_items) {
		// items may already have gained an owner when they were added to a group above
		if (item->owner())
			continue;

		add_child_item(item);

		// Position the item after the last item or at the top if there is none
		const pair<int, int> extents = item->v_extents();

		if (item->enabled())
			offset += -extents.first;

		item->force_to_v_offset(offset);

		if (item->enabled())
			offset += extents.second;
		signals_added_or_removed = true;
	}


	if (signals_added_or_removed && !header_was_shrunk_)
		resize_header_to_fit();

	update_layout();

	header_->update();
	viewport_->update();

	if (reset_scrollbar)
		set_scroll_default();
}

void View::capture_state_updated(int state)
{
	if (state != Session::Stopped) reset_idle_compression();
	GlobalSettings settings;

	if (state == Session::Running) {
		set_time_unit(util::TimeUnit::Samples);

		trigger_markers_.clear();
		if (!custom_zero_offset_set_)
			set_zero_position(0);

		scale_at_acq_start_ = scale_;
		offset_at_acq_start_ = offset_;

		// Activate "always zoom to fit" if the setting is enabled and we're
		// the main view of this session (other trace views may be used for
		// zooming and we don't want to mess them up)
		bool state = settings.value(GlobalSettings::Key_View_ZoomToFitDuringAcq).toBool();
		if (is_main_view_ && state && !restoring_state_) {
			always_zoom_to_fit_ = true;
			always_zoom_to_fit_changed(always_zoom_to_fit_);
		}

		// Enable sticky scrolling if the setting is enabled
		sticky_scrolling_ = !restoring_state_ &&
			settings.value(GlobalSettings::Key_View_StickyScrolling).toBool();

		// Reset all traces to segment 0
		current_segment_ = 0;
		set_current_segment(current_segment_);
	}

	if (state == Session::Stopped) {
		// After acquisition has stopped we need to re-calculate the ticks once
		// as it's otherwise done when the user pans or zooms, which is too late
		calculate_tick_spacing();

		// Reset "always zoom to fit", the acquisition has stopped
		if (always_zoom_to_fit_) {
			// Perform a final zoom-to-fit before disabling
			zoom_fit(always_zoom_to_fit_);
			always_zoom_to_fit_ = false;
			always_zoom_to_fit_changed(always_zoom_to_fit_);
		}

		bool zoom_to_fit_after_acq =
			settings.value(GlobalSettings::Key_View_ZoomToFitAfterAcq).toBool();

		// Only perform zoom-to-fit if the user hasn't altered the viewport and
		// we didn't restore settings in the meanwhile
		if (zoom_to_fit_after_acq &&
			!restoring_state_ &&
			(scale_ == scale_at_acq_start_) &&
			(sticky_scrolling_ || (offset_ == offset_at_acq_start_))) {
			zoom_fit(false);  // We're stopped, so the GUI state doesn't matter
		}

		restoring_state_ = false;
	}
}

void View::on_new_segment(int new_segment_id)
{
	on_segment_changed(new_segment_id);
}

void View::on_segment_completed(int segment_id)
{
	on_segment_changed(segment_id);
}

void View::on_segment_changed(int segment)
{
	reset_idle_compression();
	switch (segment_display_mode_) {
	case Trace::ShowLastSegmentOnly:
	case Trace::ShowSingleSegmentOnly:
		set_current_segment(segment);
		break;

	case Trace::ShowLastCompleteSegmentOnly:
		// Only update if all segments are complete
		if (session_.all_segments_complete(segment))
			set_current_segment(segment);
		break;

	case Trace::ShowAllSegments:
	case Trace::ShowAccumulatedIntensity:
	default:
		break;
	}
}

void View::on_create_marker_here()
{
	const QPoint p = ruler_->mapFrom(this, QPoint(context_menu_x_pos_, 0));

	add_flag(ruler_->get_absolute_time_from_x_pos(p.x()));
}

void View::on_settingViewTriggerIsZeroTime_changed(const QVariant new_value)
{
	(void)new_value;

	if (!custom_zero_offset_set_)
		reset_zero_position();
}

void View::perform_delayed_view_update()
{
	if (always_zoom_to_fit_) {
		zoom_fit(true);
	} else if (sticky_scrolling_) {
		// Make right side of the view sticky
		double length = 0;
		Timestamp offset;
		get_scroll_layout(length, offset);

		const QSize areaSize = viewport_->size();
		length = max(length - areaSize.width(), 0.0);

		set_offset(scale_ * length);
	}

	determine_time_unit();
	update_scroll();
	ruler_->update();
	viewport_->update();
}

void View::process_sticky_events()
{
	if (sticky_events_ & TraceTreeItemHExtentsChanged)
		update_layout();
	if (sticky_events_ & TraceTreeItemVExtentsChanged) {
		restack_all_trace_tree_items();
		update_scroll();
	}

	// Clear the sticky events
	sticky_events_ = 0;
}

} // namespace trace
} // namespace views
} // namespace pv
