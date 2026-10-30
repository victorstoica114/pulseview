/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Real demo sessions and real Qt widgets; no physical device discovery. */
#include "test/test.hpp"
#include <boost/test/unit_test.hpp>
#include <atomic>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QEvent>
#include <QElapsedTimer>
#include <QDir>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <zip.h>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QPointer>
#include <QKeyEvent>
#include <QSettings>
#include <QScreen>
#include <QSpinBox>
#include <QTableWidget>
#include <QTemporaryDir>
#include <thread>
#include <libsigrokcxx/libsigrokcxx.hpp>
#include <pv/dialogs/triggereditor.hpp>
#include <pv/application.hpp>
#include <pv/globalsettings.hpp>
#include <pv/triggersettings.hpp>
#include <pv/triggersequence.hpp>
#include <pv/devicemanager.hpp>
#include <pv/devices/device.hpp>
#include <pv/devices/inputfile.hpp>
#include <pv/session.hpp>
#include <pv/storesession.hpp>
#include <pv/capturecollection.hpp>
#include <pv/devices/sessionfile.hpp>
#include <pv/devices/hardwaredevice.hpp>
#include <pv/toolbars/mainbar.hpp>
#include <pv/views/trace/logicsignal.hpp>
#include <pv/views/trace/view.hpp>
#include <pv/views/trace/flag.hpp>
#include <pv/views/trace/cursor.hpp>
#include <pv/views/trace/standardbar.hpp>
#include <pv/views/trace/viewport.hpp>
#include <pv/data/logic.hpp>
#include <pv/data/logicsegment.hpp>
#include <pv/prop/int.hpp>
#ifdef ENABLE_DECODE
#include <libsigrokdecode/libsigrokdecode.h>
#include <pv/data/decodesignal.hpp>
#include <pv/data/decode/decoder.hpp>
#include <pv/data/decode/annotation.hpp>
#endif

namespace {
void wait_capture(pv::Session& session)
{
	QElapsedTimer timer; timer.start();
	while (session.get_capture_state() != pv::Session::Stopped && timer.elapsed() < 10000) {
		QApplication::processEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	const bool stopped = session.get_capture_state() == pv::Session::Stopped;
	session.stop_capture();
	BOOST_REQUIRE(stopped);
	QApplication::processEvents();
}

void application()
{
	static int argc = 1;
	static char executable[] = "pulseview-test";
	static char *argv[] = {executable, nullptr};
	static QTemporaryDir settings_directory;
	static Application app(argc, argv);
#ifdef ENABLE_DECODE
	struct DecoderRuntime {
		DecoderRuntime() {
			if (srd_init(nullptr) != SRD_OK || srd_decoder_load("uart") != SRD_OK)
				throw std::runtime_error("Cannot initialize UART test decoder");
		}
		~DecoderRuntime() { srd_exit(); }
	};
	static DecoderRuntime decoder_runtime;
#endif
	static const bool initialized = [&] {
		QCoreApplication::setOrganizationName("PulseViewTests");
		QCoreApplication::setApplicationName("CrashRecovery");
		QSettings::setDefaultFormat(QSettings::IniFormat);
		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, settings_directory.path());
		pv::GlobalSettings settings;
		settings.setValue(pv::GlobalSettings::Key_General_Language, "en");
		settings.save_internal_defaults();
		settings.set_defaults_where_needed();
		return true;
	}();
	(void)initialized;
}

struct Fixture {
	std::shared_ptr<sigrok::Context> context = sigrok::Context::create();
	std::shared_ptr<sigrok::HardwareDevice> device;
	std::shared_ptr<sigrok::Session> session;
	Fixture()
	{
		application();
		device = context->drivers().at("demo")->scan({
			{sigrok::ConfigKey::NUM_LOGIC_CHANNELS, Glib::Variant<gint32>::create(32)},
			{sigrok::ConfigKey::NUM_ANALOG_CHANNELS, Glib::Variant<gint32>::create(1)},
		}).at(0);
		device->open();
		session = context->create_session();
		session->add_device(device);
	}
	~Fixture()
	{
		session->remove_devices();
		session.reset();
		device->close();
	}

	pv::TriggerSequence sequence()
	{
		pv::TriggerSequence s;
		s.enabled = true;
		s.name = "address then edge";
		s.stages = {{{31, SR_TRIGGER_ONE}, {7, SR_TRIGGER_ZERO}},
			{{0, SR_TRIGGER_FALLING}}, {{32, SR_TRIGGER_RISING, .75f}}};
		return s;
	}
};

template<typename T> T *widget(QObject &parent, const char *name)
{
	auto *result = parent.findChild<T *>(name);
	BOOST_REQUIRE(result);
	return result;
}

void click(QObject &parent, const char *name)
{
	widget<QPushButton>(parent, name)->click();
}

void check_same(const pv::TriggerSequence &a, const pv::TriggerSequence &b)
{
	BOOST_CHECK_EQUAL(a.enabled, b.enabled);
	BOOST_CHECK_EQUAL(a.name, b.name);
	BOOST_REQUIRE_EQUAL(a.stages.size(), b.stages.size());
	for (size_t i = 0; i < a.stages.size(); ++i) {
		BOOST_REQUIRE_EQUAL(a.stages[i].size(), b.stages[i].size());
		for (size_t j = 0; j < a.stages[i].size(); ++j) {
			const auto &x = a.stages[i][j], &y = b.stages[i][j];
			BOOST_CHECK_EQUAL(x.channel, y.channel);
			BOOST_CHECK_EQUAL(x.type, y.type);
			if (std::isnan(x.value)) BOOST_CHECK(std::isnan(y.value));
			else BOOST_CHECK_EQUAL(x.value, y.value);
		}
	}
}
}

BOOST_AUTO_TEST_SUITE(TriggerSequence)

BOOST_AUTO_TEST_CASE(unavailable_integer_control_recovers_without_reopening_form)
{
	application();
	bool available = false;
	unsigned int writes = 0;
	guint64 current = 50;
	auto getter = [&]() -> Glib::VariantBase {
		if (!available) throw sigrok::Error(SR_ERR_NA);
		return Glib::Variant<guint64>::create(current);
	};
	auto setter = [&](Glib::VariantBase value) {
		current = g_variant_get_uint64(value.gobj()); ++writes;
	};
	QWidget parent;
	int64_t upper = 90;
	auto range = [&]() -> boost::optional<std::pair<int64_t, int64_t>> {
		return std::make_pair<int64_t, int64_t>(1, int64_t(upper));
	};
	pv::prop::Int property("Capture ratio", "", "%",
		std::make_pair<int64_t, int64_t>(0, 100), getter, setter, "", range);
	auto *spin = qobject_cast<QSpinBox *>(property.get_widget(&parent, true));
	BOOST_REQUIRE(spin);
	BOOST_CHECK(!spin->isEnabled()); BOOST_CHECK_EQUAL(writes, 0);
	BOOST_CHECK_EQUAL(spin->specialValueText().toStdString(), "Unavailable");
	available = true; property.update_widget();
	BOOST_CHECK(spin->isEnabled()); BOOST_CHECK_EQUAL(spin->value(), 50);
	BOOST_CHECK_EQUAL(spin->minimum(), 1); BOOST_CHECK_EQUAL(spin->maximum(), 90);
	BOOST_CHECK(spin->specialValueText().isEmpty());
	BOOST_CHECK_EQUAL(writes, 0);
	spin->setValue(75); BOOST_CHECK_EQUAL(current, 75); BOOST_CHECK_EQUAL(writes, 1);
	available = false; property.update_widget();
	BOOST_CHECK(!spin->isEnabled());
	spin->setValue(80); BOOST_CHECK_EQUAL(writes, 1);
	available = true; property.update_widget();
	BOOST_CHECK(spin->isEnabled()); BOOST_CHECK_EQUAL(spin->value(), 75);
	BOOST_CHECK_EQUAL(writes, 1);
	upper = 60; property.update_widget();
	BOOST_CHECK_EQUAL(spin->maximum(), 60); BOOST_CHECK_EQUAL(spin->value(), 60);
	BOOST_CHECK_EQUAL(writes, 1); BOOST_CHECK_EQUAL(current, 75);
}

BOOST_AUTO_TEST_CASE(channel_name_restore_after_popup_destruction)
{
	Fixture f;
	pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "popup lifetime");
	auto base = std::make_shared<pv::data::SignalBase>(f.device->channels()[0], pv::data::SignalBase::LogicChannel);
	pv::views::trace::LogicSignal signal(session, base);
	{
		QWidget popup;
		auto *form = new QFormLayout(&popup);
		static_cast<pv::views::trace::Signal &>(signal).populate_popup_form(&popup, form);
		signal.set_name("Before close");
	}
	// Session setup restoration emits name_changed after the popup is gone.
	base->set_name("Restored name");
	signal.set_name("Renamed without popup");
	BOOST_CHECK_EQUAL(base->name().toStdString(), "Renamed without popup");
}

BOOST_AUTO_TEST_CASE(hardware_options_roundtrip_and_boundaries)
{
	pv::HardwareTriggerOptions options;
	options.mode = "parallel"; options.stages = {{1, false}, {255, true}};
	auto copy = pv::parse_hardware_trigger(pv::encode_hardware_trigger(options));
	BOOST_REQUIRE_EQUAL(copy.stages.size(), 2);
	BOOST_CHECK_EQUAL(copy.stages[1].count, 255); BOOST_CHECK(copy.stages[1].continuous);
	for (const unsigned int count : {0U, 256U}) {
		options.stages[0].count = count;
		BOOST_CHECK_THROW(pv::parse_hardware_trigger(pv::encode_hardware_trigger(options)), std::runtime_error);
	}
	options = {}; options.mode = "serial"; options.clock = 17; options.data = 18;
	options.bits = 16; options.value = 0xa55a; options.falling = true;
	copy = pv::parse_hardware_trigger(pv::encode_hardware_trigger(options));
	BOOST_CHECK_EQUAL(copy.value, 0xa55a); BOOST_CHECK(copy.falling);
	options.bits = 8;
	BOOST_CHECK_THROW(pv::parse_hardware_trigger(pv::encode_hardware_trigger(options)), std::runtime_error);
	options.bits = 16; options.data = options.clock;
	BOOST_CHECK_THROW(pv::parse_hardware_trigger(pv::encode_hardware_trigger(options)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(unsupported_hardware_restore_preserves_live_trigger)
{
	Fixture f;
	const auto original = pv::build_trigger_sequence(f.context, f.device, f.sequence());
	f.session->set_trigger(original);
	QTemporaryDir directory;
	QSettings settings(directory.filePath("extended.ini"), QSettings::IniFormat);
	pv::save_trigger_settings(settings, f.session, f.device);
	settings.setValue("trigger/enabled", false);
	settings.setValue("trigger/hardware", "{'mode': <'serial'>, 'clock': <uint32 0>, 'data': <uint32 1>, 'edge': <'rising'>, 'bits': <uint32 8>, 'value': <uint32 165>}");
	BOOST_CHECK_THROW(pv::restore_trigger_settings(settings, f.context, f.device, f.session), std::exception);
	BOOST_CHECK(f.session->trigger() == original);
	settings.setValue("trigger/hardware", "{'mode': <'serial'>}");
	BOOST_CHECK_THROW(pv::restore_trigger_settings(settings, f.context, f.device, f.session), std::exception);
	BOOST_CHECK(f.session->trigger() == original);
	pv::dialogs::TriggerEditor editor(f.context, f.device, original);
	BOOST_CHECK_EQUAL(editor.hardware_options().mode, "default");
	BOOST_CHECK(widget<QComboBox>(editor, "triggerMode")->isHidden());
}

BOOST_AUTO_TEST_CASE(physical_indices_stage_order_and_values)
{
	Fixture f;
	const auto s = f.sequence();
	auto t = pv::build_trigger_sequence(f.context, f.device, s);
	check_same(s, pv::read_trigger_sequence(t));
	BOOST_CHECK(!f.session->trigger());
}

BOOST_AUTO_TEST_CASE(invalid_candidates_never_replace_live_trigger)
{
	Fixture f;
	f.session->set_trigger(pv::build_trigger_sequence(f.context, f.device, f.sequence()));
	const auto original = f.session->trigger();
	auto s = f.sequence();
	s.stages[0].clear();
	BOOST_CHECK_THROW(pv::build_trigger_sequence(f.context, f.device, s), std::runtime_error);
	s = f.sequence(); s.stages[0].push_back(s.stages[0][0]);
	BOOST_CHECK_THROW(pv::build_trigger_sequence(f.context, f.device, s), std::runtime_error);
	s = f.sequence(); s.stages[0][0].channel = 1000;
	BOOST_CHECK_THROW(pv::build_trigger_sequence(f.context, f.device, s), std::runtime_error);
	s = f.sequence(); s.stages[0][0].type = SR_TRIGGER_OVER;
	BOOST_CHECK_THROW(pv::build_trigger_sequence(f.context, f.device, s), std::runtime_error);
	s = f.sequence(); s.stages.back()[0].value = std::numeric_limits<float>::infinity();
	BOOST_CHECK_THROW(pv::build_trigger_sequence(f.context, f.device, s), std::runtime_error);
	s = f.sequence(); f.device->channels().at(31)->set_enabled(false);
	BOOST_CHECK_THROW(pv::build_trigger_sequence(f.context, f.device, s), std::runtime_error);
	BOOST_CHECK(f.session->trigger() == original);
	BOOST_CHECK(!f.device->channels().at(31)->enabled());
}

BOOST_AUTO_TEST_CASE(editor_stage_duplicate_reorder_remove)
{
	Fixture f;
	const auto source = f.sequence();
	pv::dialogs::TriggerEditor editor(f.context, f.device,
		pv::build_trigger_sequence(f.context, f.device, source));
	check_same(source, pv::read_trigger_sequence(editor.trigger()));
	auto *list = widget<QListWidget>(editor, "triggerStages");
	list->setCurrentRow(0);
	click(editor, "duplicateTriggerStage");
	BOOST_CHECK_EQUAL(editor.trigger()->stages().size(), 4);
	click(editor, "moveTriggerStageDown");
	auto result = pv::read_trigger_sequence(editor.trigger());
	BOOST_CHECK_EQUAL(result.stages[1][0].channel, 0);
	BOOST_CHECK_EQUAL(result.stages[2][0].channel, 31);
	click(editor, "moveTriggerStageUp");
	click(editor, "removeTriggerStage");
	check_same(source, pv::read_trigger_sequence(editor.trigger()));
}

BOOST_AUTO_TEST_CASE(editor_new_stage_conditions_and_duplicate_validation)
{
	Fixture f;
	pv::dialogs::TriggerEditor editor(f.context, f.device, nullptr);
	BOOST_CHECK(!editor.trigger());
	click(editor, "addTriggerStage");
	BOOST_CHECK_THROW(editor.trigger(), std::runtime_error);
	click(editor, "addTriggerCondition");
	click(editor, "addTriggerCondition");
	auto *table = widget<QTableWidget>(editor, "triggerConditions");
	auto *channel = qobject_cast<QComboBox *>(table->cellWidget(1, 0));
	BOOST_REQUIRE(channel);
	channel->setCurrentIndex(channel->findData(31u));
	BOOST_CHECK_EQUAL(editor.trigger()->stages().at(0)->matches().at(1)->channel()->index(), 31);
	channel = qobject_cast<QComboBox *>(table->cellWidget(1, 0));
	channel->setCurrentIndex(channel->findData(0u));
	BOOST_CHECK_THROW(editor.trigger(), std::runtime_error);
	BOOST_CHECK(!widget<QDialogButtonBox>(editor, "triggerButtons")->button(QDialogButtonBox::Ok)->isEnabled());
	table->selectRow(1);
	click(editor, "removeTriggerCondition");
	BOOST_CHECK_EQUAL(editor.trigger()->stages().at(0)->matches().size(), 1);
	click(editor, "addTriggerStage");
	click(editor, "addTriggerCondition");
	BOOST_CHECK_EQUAL(editor.trigger()->stages().size(), 2);
}

BOOST_AUTO_TEST_CASE(editor_analog_text_validation_and_precision)
{
	Fixture f;
	const auto initial = pv::build_trigger_sequence(f.context, f.device, f.sequence());
	pv::dialogs::TriggerEditor editor(f.context, f.device, initial);
	widget<QListWidget>(editor, "triggerStages")->setCurrentRow(2);
	auto *table = widget<QTableWidget>(editor, "triggerConditions");
	auto *value = qobject_cast<QLineEdit *>(table->cellWidget(0, 2));
	BOOST_REQUIRE(value);
	value->setText("not a number");
	BOOST_CHECK_THROW(editor.trigger(), std::runtime_error);
	value->setText("inf");
	BOOST_CHECK_THROW(editor.trigger(), std::runtime_error);
	value->setText("-0.123456789");
	BOOST_CHECK_EQUAL(editor.trigger()->stages().at(2)->matches().at(0)->value(), -.123456789f);
	value->clear();
	BOOST_CHECK(std::isnan(editor.trigger()->stages().at(2)->matches().at(0)->value()));
	BOOST_CHECK_EQUAL(initial->stages().at(2)->matches().at(0)->value(), .75f);
}

BOOST_AUTO_TEST_CASE(editor_cancel_and_explicit_disable)
{
	Fixture f;
	const auto original = pv::build_trigger_sequence(f.context, f.device, f.sequence());
	f.session->set_trigger(original);
	pv::dialogs::TriggerEditor editor(f.context, f.device, original);
	click(editor, "addTriggerStage");
	editor.reject();
	BOOST_CHECK(f.session->trigger() == original);
	widget<QCheckBox>(editor, "triggerEnabled")->setChecked(false);
	BOOST_CHECK(!editor.trigger());
	BOOST_CHECK(f.session->trigger() == original);
}

BOOST_AUTO_TEST_CASE(editor_disabled_channel_is_preserved_and_rejected)
{
	Fixture f;
	const auto original = pv::build_trigger_sequence(f.context, f.device, f.sequence());
	f.device->channels().at(31)->set_enabled(false);
	pv::dialogs::TriggerEditor editor(f.context, f.device, original);
	BOOST_CHECK_THROW(editor.trigger(), std::runtime_error);
	BOOST_CHECK(!f.device->channels().at(31)->enabled());
	f.device->channels().at(31)->set_enabled(true);
	check_same(f.sequence(), pv::read_trigger_sequence(editor.trigger()));
}

BOOST_AUTO_TEST_CASE(editor_sequence_settings_roundtrip)
{
	Fixture f;
	QTemporaryDir directory;
	const auto sequence = f.sequence();
	pv::dialogs::TriggerEditor editor(f.context, f.device,
		pv::build_trigger_sequence(f.context, f.device, sequence));
	widget<QLineEdit>(editor, "triggerName")->setText("three-stage capture");
	f.session->set_trigger(editor.trigger());
	const auto expected = pv::read_trigger_sequence(f.session->trigger());
	{
		QSettings settings(directory.filePath("sequence.ini"), QSettings::IniFormat);
		pv::save_trigger_settings(settings, f.session);
		settings.sync();
		BOOST_REQUIRE(settings.status() == QSettings::NoError);
	}
	f.session->set_trigger(nullptr);
	QSettings settings(directory.filePath("sequence.ini"), QSettings::IniFormat);
	BOOST_CHECK(pv::restore_trigger_settings(settings, f.context, f.device, f.session));
	pv::dialogs::TriggerEditor reopened(f.context, f.device, f.session->trigger());
	check_same(expected, pv::read_trigger_sequence(reopened.trigger()));
}

BOOST_AUTO_TEST_CASE(editor_focus_selects_condition_for_removal)
{
	Fixture f;
	pv::dialogs::TriggerEditor editor(f.context, f.device,
		pv::build_trigger_sequence(f.context, f.device, f.sequence()));
	auto *table = widget<QTableWidget>(editor, "triggerConditions");
	QEvent focus(QEvent::FocusIn);
	QApplication::sendEvent(table->cellWidget(1, 1), &focus);
	BOOST_CHECK_EQUAL(table->currentRow(), 1);
	click(editor, "removeTriggerCondition");
	BOOST_CHECK_EQUAL(editor.trigger()->stages()[0]->matches().size(), 1);
	BOOST_CHECK_EQUAL(editor.trigger()->stages()[0]->matches()[0]->channel()->index(), 31);
}

BOOST_AUTO_TEST_CASE(editor_visible_focus_preserves_condition_column)
{
	Fixture f;
	pv::dialogs::TriggerEditor editor(f.context, f.device,
		pv::build_trigger_sequence(f.context, f.device, f.sequence()));
	// A hidden widget receiving a synthetic FocusIn misses Qt's item-view
	// focus changes. Exercise the real focus chain, including column changes.
	editor.move(QApplication::screens().back()->availableGeometry().topLeft() + QPoint(50, 50));
	editor.show();
	QApplication::setActiveWindow(&editor);
	QApplication::processEvents();
	auto *table = widget<QTableWidget>(editor, "triggerConditions");
	for (int repeat = 0; repeat < 3; ++repeat)
		for (int row : {1, 0})
			for (int column : {1, 0, 1}) {
				auto *field = table->cellWidget(row, column);
				BOOST_REQUIRE(field);
				field->setFocus(Qt::MouseFocusReason);
				QApplication::processEvents();
				BOOST_CHECK(QApplication::focusWidget() == field);
				BOOST_CHECK_EQUAL(table->currentRow(), row);
				BOOST_CHECK_EQUAL(table->currentColumn(), column);
			}
	widget<QListWidget>(editor, "triggerStages")->setCurrentRow(2);
	auto *threshold = table->cellWidget(0, 2);
	BOOST_REQUIRE(threshold);
	threshold->setFocus(Qt::TabFocusReason);
	QApplication::processEvents();
	BOOST_CHECK(QApplication::focusWidget() == threshold);
	BOOST_CHECK_EQUAL(table->currentColumn(), 2);
	check_same(f.sequence(), pv::read_trigger_sequence(editor.trigger()));
}

BOOST_AUTO_TEST_CASE(editor_rejects_foreign_device_with_same_channel_indices)
{
	Fixture f;
	const auto original = pv::build_trigger_sequence(f.context, f.device, f.sequence());
	auto other = f.context->drivers().at("demo")->scan({
		{sigrok::ConfigKey::NUM_LOGIC_CHANNELS, Glib::Variant<gint32>::create(32)},
		{sigrok::ConfigKey::NUM_ANALOG_CHANNELS, Glib::Variant<gint32>::create(1)},
	}).at(0);
	BOOST_CHECK_THROW(pv::dialogs::TriggerEditor(f.context, other, original), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(idle_display_preserves_pulses_cursors_export_and_toggle)
{
	Fixture f;
	pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "idle display");
	auto view = std::make_shared<pv::views::trace::View>(session, true);
	session.register_view(view);
	view->resize(1100, 500);
	view->move(QApplication::screens().back()->availableGeometry().topLeft() + QPoint(30, 30));
	view->show();
	QApplication::processEvents();
	QTemporaryDir directory;
	QByteArray bytes(1000000, '\x01'); // UART idle HIGH; three frames at 10 kbaud.
	const std::array<int, 3> starts{100, 500100, 900100};
	const std::array<uint8_t, 3> values{0x55, 0xa6, 0x3c};
	for (size_t frame = 0; frame < starts.size(); ++frame)
		for (int bit = 0; bit < 10; ++bit) {
			const bool high = bit == 9 || (bit && ((values[frame] >> (bit - 1)) & 1));
			for (int sample = 0; sample < 100; ++sample)
				bytes[starts[frame] + bit * 100 + sample] = high ? 1 : 0;
		}
	bytes[450000] = '\x81'; // Single-sample pulse on a different recorded input.
	const auto source = directory.filePath("idle.bin");
	{
		QFile input(source); BOOST_REQUIRE(input.open(QIODevice::WriteOnly));
		BOOST_REQUIRE_EQUAL(input.write(bytes), bytes.size());
	}
	session.set_device(std::make_shared<pv::devices::InputFile>(f.context, source.toStdString(),
		f.context->input_formats().at("binary"), std::map<std::string, Glib::VariantBase>{
			{"numchannels", Glib::Variant<gint32>::create(8)},
			{"samplerate", Glib::Variant<guint64>::create(1000000)}}));
	QString error;
	session.start_capture([&](QString message) { error = message; });
	QElapsedTimer deadline; deadline.start();
	while (deadline.elapsed() < 10000) {
		QApplication::processEvents();
		if (session.get_capture_state() == pv::Session::Stopped &&
			!session.signalbases().empty() && session.signalbases()[0]->has_samples()) break;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	session.stop_capture();
	QApplication::processEvents();
	BOOST_REQUIRE(error.isEmpty());
	BOOST_REQUIRE(!session.signalbases().empty());
	const auto logic = session.signalbases()[0]->logic_data();
	BOOST_REQUIRE(logic && !logic->logic_segments().empty());
	const auto segment = logic->logic_segments()[0];
	BOOST_REQUIRE_EQUAL(segment->get_sample_count(), 1000000);
	std::vector<uint8_t> original(bytes.size());
	segment->get_samples(0, bytes.size(), original.data());
	BOOST_CHECK_EQUAL(std::memcmp(original.data(), bytes.constData(), bytes.size()), 0);
	for (unsigned channel = 1; channel < 7; ++channel)
		session.signalbases()[channel]->set_enabled(false);
	QMetaObject::invokeMethod(view.get(), "perform_delayed_view_update", Qt::DirectConnection);
	view->zoom_fit(false);
	const double plain_scale = view->scale();
	const auto plain_offset = view->offset();
	pv::util::Timestamp first("0.0001"), second("0.5001");
	view->set_cursors(first, second);
	const auto flag = view->add_flag(pv::util::Timestamp("0.45"));
#ifdef ENABLE_DECODE
	auto decoded = session.add_decode_signal();
	decoded->stack_decoder(srd_decoder_get_by_id("uart"), false);
	const auto uart = decoded->decoder_stack().front();
	uart->set_option("baudrate", g_variant_new_int64(10000));
	for (const auto& channel : decoded->get_channels())
		if (std::strcmp(channel.pdch_->id, "rx") == 0)
			decoded->assign_signal(channel.id, session.signalbases()[0]);
	deadline.restart();
	while (deadline.elapsed() < 10000 && decoded->get_decoded_sample_count(0, true) < bytes.size()) {
		QApplication::processEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	BOOST_REQUIRE(decoded->get_decoded_sample_count(0, true) >= bytes.size());
	BOOST_REQUIRE_EQUAL(decoded->get_binary_data_chunk_count(0, uart.get(), 0), 3);
	for (unsigned i = 0; i < 3; ++i) {
		const std::vector<uint8_t>* chunk; uint64_t size;
		decoded->get_binary_data_chunk(0, uart.get(), 0, i, &chunk, &size);
		BOOST_REQUIRE_EQUAL(size, 1);
		BOOST_CHECK_EQUAL(chunk->at(0), values[i]);
	}
	auto annotations = [&] {
		std::deque<const pv::data::decode::Annotation*> all;
		decoded->get_annotation_subset(all, 0, 0, bytes.size());
		std::vector<std::string> result;
		for (const auto ann : all)
			result.push_back(std::to_string(ann->start_sample()) + ":" +
				std::to_string(ann->end_sample()) + ":" + ann->longest_annotation().toStdString());
		std::sort(result.begin(), result.end());
		return result;
	};
	const auto original_annotations = annotations();
	BOOST_REQUIRE(!original_annotations.empty());
#endif
	pv::views::trace::StandardBar bar(session, nullptr, view.get());
	view->show_cursors(true);
	qobject_cast<QVBoxLayout*>(view->layout())->insertWidget(0, &bar);
	QApplication::processEvents();
	const QString screenshot_dir = qEnvironmentVariable("PV_IDLE_SCREENSHOT_DIR");
	auto screenshot = [&](const char *name) {
		QElapsedTimer settled; settled.start();
		while (settled.elapsed() < 200) {
			QApplication::processEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		if (!screenshot_dir.isEmpty()) {
			QDir().mkpath(screenshot_dir);
			BOOST_CHECK(view->grab().save(screenshot_dir + "/" + name));
		}
	};
	screenshot("linear.png");
	auto *toggle = widget<QAction>(bar, "actionCompressIdle");
	for (int repeat = 0; repeat < 3; ++repeat) {
		toggle->setChecked(true);
		deadline.restart();
		while (deadline.elapsed() < 5000 && view->idle_compression_pending())
			QApplication::processEvents();
		BOOST_REQUIRE(!view->idle_compression_pending());
		BOOST_REQUIRE(view->idle_compression_enabled());
		BOOST_CHECK(view->time_unit() == pv::util::TimeUnit::Time);
		BOOST_REQUIRE(!view->idle_time_map().empty());
		BOOST_CHECK(view->scale() < plain_scale / 100);
		BOOST_CHECK(std::abs(flag->get_x() - view->time_to_pixel(flag->time())) <= 1);
		BOOST_CHECK_EQUAL(flag->time(), pv::util::Timestamp("0.45"));
		BOOST_CHECK_EQUAL(view->cursors()->second()->time() - view->cursors()->first()->time(),
			pv::util::Timestamp("0.5"));
		for (double t : {0.0001, 0.2, 0.45, 0.5001, 0.9001})
			BOOST_CHECK(abs(view->pixel_to_time(view->time_to_pixel(t)) - t) < pv::util::Timestamp("1e-12"));
		for (const auto& gap : view->idle_time_map().gaps())
			BOOST_CHECK(!(gap.begin <= pv::util::Timestamp("0.45") && gap.end > pv::util::Timestamp("0.45")));
		BOOST_CHECK(!view->viewport()->grab().isNull());
		if (!repeat) screenshot("folded.png");
#ifdef ENABLE_DECODE
		BOOST_CHECK(annotations() == original_annotations);
#endif
		QSettings setup(directory.filePath("folded-setup.ini"), QSettings::IniFormat);
		view->save_settings(setup);
		BOOST_CHECK_EQUAL(setup.value("scale").toDouble(), plain_scale);
		BOOST_CHECK_EQUAL(pv::GlobalSettings::restore_timestamp(setup, "offset"), plain_offset);
		toggle->setChecked(false);
		BOOST_CHECK(view->idle_time_map().empty());
		BOOST_CHECK_EQUAL(view->scale(), plain_scale);
		BOOST_CHECK_EQUAL(view->offset(), plain_offset);
		if (!repeat) screenshot("restored.png");
	}
	view->set_idle_compression(true);
	while (view->idle_compression_pending()) QApplication::processEvents();
	const auto target = directory.filePath("unchanged.bin");
	{
		pv::StoreSession saved(target.toStdString(), f.context->output_formats().at("binary"), {}, {0, 0}, session);
		BOOST_REQUIRE(saved.start()); saved.wait(); BOOST_CHECK(saved.error().isEmpty());
	}
	QFile exported(target); BOOST_REQUIRE(exported.open(QIODevice::ReadOnly));
	BOOST_CHECK(exported.readAll() == bytes);
	const auto selected_target = directory.filePath("selected-real-time.bin");
	{
		pv::StoreSession selected(selected_target.toStdString(), f.context->output_formats().at("binary"),
			{}, {100, 500100}, session);
		BOOST_REQUIRE(selected.start()); selected.wait(); BOOST_CHECK(selected.error().isEmpty());
	}
	QFile selected(selected_target); BOOST_REQUIRE(selected.open(QIODevice::ReadOnly));
	BOOST_CHECK(selected.readAll() == bytes.mid(100, 500000));
	// Cancellation must retire timer callbacks before the view/data disappear.
	view->set_idle_compression(true);
	view->set_idle_compression(false);
	QApplication::processEvents();
	BOOST_CHECK(!view->idle_compression_pending());
	BOOST_CHECK(!view->idle_compression_enabled());
	{
		auto retired = std::make_shared<pv::views::trace::View>(session);
		session.register_view(retired);
		retired->set_idle_compression(true);
		BOOST_REQUIRE(retired->idle_compression_pending());
		session.deregister_view(retired);
		retired.reset();
		QApplication::processEvents();
	}
}

namespace {

class SeriesDevice : public pv::devices::Device {
	std::shared_ptr<sigrok::Context> context_;
public:
	std::atomic_uint starts{0}, stops{0};
	std::atomic_bool hold_start{false}, release_start{false};
	unsigned fail_start = 0, empty_run = 0, timeout_run = 0, missing_end = 0;
	std::function<void()> after_data;
	explicit SeriesDevice(Fixture& f) : context_(f.context) {
		device_ = f.device; session_ = f.session;
		for (const auto& channel : device_->channels())
			channel->set_enabled(channel->type() == sigrok::ChannelType::LOGIC);
		device_->config_set(sigrok::ConfigKey::LIMIT_SAMPLES, Glib::Variant<guint64>::create(32));
		device_->config_set(sigrok::ConfigKey::SAMPLERATE, Glib::Variant<guint64>::create(1000000));
	}
	std::string full_name() const override { return "scripted finite captures"; }
	std::string display_name(const pv::DeviceManager&) const override { return full_name(); }
	void open() override {}
	void close() override {}
	void start() override {
		++starts;
		while (hold_start && !release_start) std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (starts == fail_start) throw std::runtime_error("scripted rearm failure");
	}
	void run() override {
		if (starts != empty_run) {
			std::array<uint32_t, 32> samples;
			samples.fill(starts.load());
			datafeed_callback_(device_, context_->create_logic_packet(samples.data(), sizeof(samples), 4));
			if (after_data) after_data();
		}
		const bool failed = starts == timeout_run;
		datafeed_callback_(device_, context_->create_meta_packet({
			{sigrok::ConfigKey::CAPTURE_STATUS, Glib::Variant<Glib::ustring>::create(failed ? "timeout" : "complete")},
			{sigrok::ConfigKey::CAPTURE_ERROR, Glib::Variant<Glib::ustring>::create(failed ? "scripted timeout" : "")}}));
		if (starts != missing_end) datafeed_callback_(device_, context_->create_end_packet());
	}
	void stop() override { ++stops; }
};

void check_tag(pv::Session& session, unsigned index, uint32_t tag)
{
	const auto segments = session.signalbases().at(0)->logic_data()->logic_segments();
	BOOST_REQUIRE(index < segments.size());
	std::array<uint32_t, 32> data;
	BOOST_REQUIRE_EQUAL(segments[index]->get_sample_count(), data.size());
	segments[index]->get_samples(0, data.size(), reinterpret_cast<uint8_t*>(data.data()));
	for (const auto value : data) BOOST_CHECK_EQUAL(value, tag);
}
}

BOOST_AUTO_TEST_CASE(repeated_captures_preserve_all_segments_and_setup)
{
	Fixture f; pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "series");
	auto device = std::make_shared<SeriesDevice>(f);
	session.set_device(device);
	auto view = std::make_shared<pv::views::trace::View>(session, true);
	session.register_view(view);
	pv::toolbars::MainBar bar(session, nullptr, view.get());
	auto count = bar.findChild<QSpinBox*>("captures_per_run");
	BOOST_REQUIRE(count);
	BOOST_CHECK_EQUAL(session.captures_per_run(), 1);
	BOOST_CHECK(!session.set_captures_per_run(0));
	BOOST_CHECK(!session.set_captures_per_run(10001));
	BOOST_REQUIRE(session.set_captures_per_run(3));
	BOOST_CHECK_EQUAL(count->value(), 3);
	QString error;
	session.start_capture([&](QString text) { error = text; });
	wait_capture(session);
	BOOST_REQUIRE(error.isEmpty());
	BOOST_CHECK_EQUAL(device->starts.load(), 3);
	BOOST_REQUIRE_EQUAL(session.captured_segment_count(), 3);
	for (unsigned i = 0; i < 3; ++i) {
		check_tag(session, i, i + 1);
		BOOST_CHECK_EQUAL(session.segment_capture_outcome(i).toStdString(), "complete");
	}
	QTemporaryDir dir;
	QSettings settings(dir.filePath("setup.ini"), QSettings::IniFormat);
	session.save_setup(settings);
	BOOST_CHECK_EQUAL(settings.value("captures_per_run").toUInt(), 3);
	auto selector = bar.findChild<QSpinBox*>("captured_segment");
	BOOST_REQUIRE(selector);
	BOOST_CHECK_EQUAL(selector->maximum(), 3);
	selector->setValue(1);
	BOOST_CHECK_EQUAL(view->current_segment(), 0);
	session.deregister_view(view);
}

BOOST_AUTO_TEST_CASE(repeated_capture_failure_keeps_earlier_outcomes)
{
	for (unsigned scenario = 0; scenario < 4; ++scenario) {
		Fixture f; pv::DeviceManager manager(f.context, "", false);
		pv::Session session(manager, "series failures");
		auto device = std::make_shared<SeriesDevice>(f);
		if (scenario == 0) device->fail_start = 2;
		if (scenario == 1) device->timeout_run = 2;
		if (scenario == 2) device->empty_run = 2;
		if (scenario == 3) device->missing_end = 2;
		session.set_device(device); session.set_captures_per_run(4);
		session.start_capture([](QString) {}); wait_capture(session);
		BOOST_CHECK_EQUAL(device->starts.load(), 2);
		BOOST_REQUIRE_EQUAL(session.captured_segment_count(), scenario == 1 || scenario == 3 ? 2 : 1);
		check_tag(session, 0, 1);
		BOOST_CHECK_EQUAL(session.segment_capture_outcome(0).toStdString(), "complete");
		BOOST_CHECK(session.capture_outcome() != "complete");
		if (scenario == 1) {
			check_tag(session, 1, 2);
			BOOST_CHECK_EQUAL(session.segment_capture_outcome(1).toStdString(), "timeout");
		}
	}
}

BOOST_AUTO_TEST_CASE(series_stop_during_start_and_before_rearm)
{
	Fixture f; pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "series stop");
	auto device = std::make_shared<SeriesDevice>(f);
	session.set_device(device); session.set_captures_per_run(4);
	device->hold_start = true;
	session.start_capture([](QString) {});
	QElapsedTimer timer; timer.start();
	while (!device->starts && timer.elapsed() < 2000) std::this_thread::sleep_for(std::chrono::milliseconds(1));
	BOOST_CHECK(!session.set_captures_per_run(2));
	session.request_stop_capture(); device->release_start = true;
	wait_capture(session);
	BOOST_CHECK_EQUAL(device->starts.load(), 1);
	BOOST_CHECK(device->stops >= 2);
	device->hold_start = false;
	const auto connection = QObject::connect(&session, &pv::Session::capture_series_progress, &session,
		[&](unsigned current, unsigned) { if (current == 2) session.request_stop_capture(); }, Qt::DirectConnection);
	session.start_capture([](QString) {}); wait_capture(session);
	QObject::disconnect(connection);
	BOOST_CHECK_EQUAL(device->starts.load(), 2); // exactly one new start
	BOOST_REQUIRE_EQUAL(session.captured_segment_count(), 1);
	BOOST_CHECK_EQUAL(session.segment_capture_outcome(0).toStdString(), "complete");
	BOOST_CHECK_EQUAL(session.capture_outcome().toStdString(), "stopped");
}

BOOST_AUTO_TEST_CASE(collection_roundtrip_partial_frames_triggers_and_selected_export)
{
	Fixture f; pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "collection");
	auto device = std::make_shared<SeriesDevice>(f);
	session.set_device(device); session.set_captures_per_run(3);
	device->timeout_run = 3;
	device->after_data = [&] { session.restore_segment_triggers({pv::util::Timestamp("0.000016")}); };
	session.start_capture([](QString) {}); wait_capture(session);
	QTemporaryDir dir;
	const auto path = dir.filePath("three.srcollection");
	{
		pv::StoreSession store(path.toStdString(), f.context->output_formats().at("srzip"), {}, {0,0}, session, 0, true);
		BOOST_REQUIRE(store.start()); store.wait();
		BOOST_REQUIRE_MESSAGE(store.error().isEmpty(), store.error().toStdString());
	}
	const auto manifest = pv::CaptureCollection::read(path);
	BOOST_REQUIRE_EQUAL(manifest.frames.size(), 3);
	BOOST_CHECK_EQUAL(manifest.status.toStdString(), "timeout");
	BOOST_CHECK_EQUAL(manifest.frames[0].triggers.size(), 1);
	pv::Session restored(manager, "restored collection");
	restored.set_device(std::make_shared<pv::devices::SessionFile>(f.context, path.toStdString()));
	// A setup can edit display names/enabled state before replay starts.
	restored.device()->device()->channels().at(0)->set_name("display name");
	restored.set_captures_per_run(7); // file playback is once, never seven repetitions
	QString error;
	restored.start_capture([&](QString text) { error = text; }); wait_capture(restored);
	BOOST_REQUIRE_MESSAGE(error.isEmpty(), error.toStdString());
	BOOST_REQUIRE_EQUAL(restored.captured_segment_count(), 3);
	for (unsigned i = 0; i < 3; ++i) {
		check_tag(restored, i, i + 1);
		BOOST_REQUIRE_EQUAL(restored.get_triggers(i).size(), 1);
		BOOST_CHECK_EQUAL(restored.get_triggers(i)[0], pv::util::Timestamp("0.000016"));
	}
	BOOST_CHECK_EQUAL(restored.segment_capture_outcome(0).toStdString(), "complete");
	BOOST_CHECK_EQUAL(restored.segment_capture_outcome(2).toStdString(), "timeout");
	BOOST_CHECK_EQUAL(restored.capture_error().toStdString(), "scripted timeout");
	// Saving over an open/replayed collection must work on Windows too.
	{
		pv::StoreSession store(path.toStdString(), f.context->output_formats().at("srzip"), {}, {0,0}, restored, 0, true);
		BOOST_REQUIRE(store.start()); store.wait(); BOOST_REQUIRE(store.error().isEmpty());
	}
	{
		const auto target = dir.filePath("selected.bin");
		pv::StoreSession store(target.toStdString(), f.context->output_formats().at("binary"), {}, {4,12}, restored, 1);
		BOOST_REQUIRE(store.start()); store.wait(); BOOST_REQUIRE(store.error().isEmpty());
		QFile file(target); BOOST_REQUIRE(file.open(QIODevice::ReadOnly));
		const auto bytes = file.readAll();
		BOOST_REQUIRE_EQUAL(bytes.size(), 8 * 4);
		for (int i = 0; i < bytes.size(); ++i) BOOST_CHECK_EQUAL(uint8_t(bytes[i]), i % 4 ? 0 : 2);
	}
	std::atomic_bool cancel{false};
	const auto updated_manifest = pv::CaptureCollection::read(path);
	auto altered = updated_manifest;
	altered.frames[0].sha256 = QString(64, '0');
	BOOST_CHECK_THROW(altered.extract(path, 0, dir.filePath("bad.sr"), cancel), std::runtime_error);
	BOOST_CHECK(!QFile::exists(dir.filePath("bad.sr")));
	const auto sentinel = dir.filePath("existing.srcollection");
	{ QFile file(sentinel); BOOST_REQUIRE(file.open(QIODevice::WriteOnly)); file.write("keep"); }
	QStringList members;
	for (unsigned i = 0; i < 3; ++i) {
		const auto member = dir.filePath(QString::number(i) + ".sr");
		updated_manifest.extract(path, i, member, cancel); members.append(member);
	}
	BOOST_CHECK_THROW(updated_manifest.write(sentinel, members, cancel, [&](int, int) { cancel = true; }), std::runtime_error);
	QFile file(sentinel); BOOST_REQUIRE(file.open(QIODevice::ReadOnly)); BOOST_CHECK(file.readAll() == "keep");
}

BOOST_AUTO_TEST_CASE(dla32_capture_series_hardware_opt_in)
{
	const QString directory = qEnvironmentVariable("PV_CAPTURE_SERIES_HARDWARE");
	if (directory.isEmpty()) return; // No physical discovery in normal tests.
	application();
	auto context = sigrok::Context::create();
	pv::DeviceManager manager(context, "", false);
	const auto found = context->drivers().at("fnirsi-dla32")->scan();
	BOOST_REQUIRE_EQUAL(found.size(), 1);
	pv::Session session(manager, "DLA32 hardware capture series");
	session.set_device(std::make_shared<pv::devices::HardwareDevice>(context, found[0]));
	const auto device = session.device()->device();
	const uint64_t samples = qEnvironmentVariableIsSet("PV_CAPTURE_SERIES_GUARD_PREFIX") ? 100000 : 20000;
	for (const auto& channel : device->channels()) channel->set_enabled(true);
	device->config_set(sigrok::ConfigKey::SAMPLERATE, Glib::Variant<guint64>::create(1000000));
	device->config_set(sigrok::ConfigKey::LIMIT_SAMPLES, Glib::Variant<guint64>::create(samples));
	device->config_set(sigrok::ConfigKey::CAPTURE_RATIO, Glib::Variant<guint64>::create(50));
	device->config_set(sigrok::ConfigKey::VOLTAGE_THRESHOLD,
		Glib::VariantBase(g_variant_new("(dd)", 1.6, 1.6)));
	auto trigger = context->create_trigger("numbered UART input");
	trigger->add_stage()->add_match(device->channels().at(22), sigrok::TriggerMatchType::FALLING);
	session.set_trigger(trigger);
	session.set_captures_per_run(3);
	QFile events(QDir(directory).filePath("capture-events.jsonl"));
	BOOST_REQUIRE(events.open(QIODevice::WriteOnly | QIODevice::NewOnly));
	QTextStream output(&events);
	QElapsedTimer elapsed; elapsed.start();
	QString mode;
	QObject::connect(&session, &pv::Session::capture_series_progress, &session,
		[&](unsigned current, unsigned total) {
			output << QJsonDocument(QJsonObject{{"mode",mode},{"capture",int(current)},
				{"total",int(total)},{"elapsed_ms",double(elapsed.elapsed())}}).toJson(QJsonDocument::Compact) << '\n';
			output.flush();
		}, Qt::DirectConnection);
	for (const auto& current_mode : {QString("Buffer"), QString("Stream")}) {
		mode = current_mode;
		device->config_set(sigrok::ConfigKey::DATA_SOURCE, Glib::Variant<Glib::ustring>::create(mode.toStdString()));
		QString error;
		session.start_capture([&](QString text) { error = text; });
		QElapsedTimer timer; timer.start();
		while (session.get_capture_state() != pv::Session::Stopped && timer.elapsed() < 45000) {
			QApplication::processEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		const bool stopped = session.get_capture_state() == pv::Session::Stopped;
		session.stop_capture();
		BOOST_REQUIRE(stopped);
		BOOST_REQUIRE_MESSAGE(error.isEmpty(), error.toStdString());
		BOOST_REQUIRE_EQUAL(session.captured_segment_count(), 3);
		BOOST_REQUIRE_EQUAL(session.capture_outcome().toStdString(), "complete");
		for (unsigned i = 0; i < 3; ++i) {
			BOOST_REQUIRE_EQUAL(session.get_segment_sample_count(i), samples);
			BOOST_CHECK_EQUAL(session.segment_capture_outcome(i).toStdString(), "complete");
			const auto path = QDir(directory).filePath(mode + QString("-%1.bin").arg(i+1));
			pv::StoreSession store(path.toStdString(), context->output_formats().at("binary"), {}, {0,0}, session, i);
			BOOST_REQUIRE(store.start()); store.wait(); BOOST_REQUIRE(store.error().isEmpty());
		}
		const auto path = QDir(directory).filePath(mode + ".srcollection");
		pv::StoreSession store(path.toStdString(), context->output_formats().at("srzip"), {}, {0,0}, session, 0, true);
		BOOST_REQUIRE(store.start()); store.wait(); BOOST_REQUIRE_MESSAGE(store.error().isEmpty(), store.error().toStdString());
		BOOST_CHECK_EQUAL(pv::CaptureCollection::read(path).frames.size(), 3);
		pv::Session restored(manager, "hardware collection replay");
		restored.set_device(std::make_shared<pv::devices::SessionFile>(context, path.toStdString()));
		QString replay_error;
		restored.start_capture([&](QString message) { replay_error = message; }); wait_capture(restored);
		BOOST_REQUIRE_MESSAGE(replay_error.isEmpty(), replay_error.toStdString());
		BOOST_REQUIRE_EQUAL(restored.captured_segment_count(), 3);
		for (unsigned i = 0; i < 3; ++i) {
			const auto segment = restored.signalbases().at(0)->logic_data()->logic_segments().at(i);
			QByteArray bytes(int(samples * 4), '\0');
			segment->get_samples(0, samples, reinterpret_cast<uint8_t*>(bytes.data()));
			QFile expected(QDir(directory).filePath(mode + QString("-%1.bin").arg(i+1)));
			BOOST_REQUIRE(expected.open(QIODevice::ReadOnly));
			BOOST_CHECK(expected.readAll() == bytes);
			BOOST_CHECK(restored.get_triggers(i) == session.get_triggers(i));
		}
	}
}

BOOST_AUTO_TEST_CASE(collection_rejects_bad_metadata_and_defers_packet_errors_safely)
{
	Fixture f; pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "corruption source");
	auto source = std::make_shared<SeriesDevice>(f);
	session.set_device(source); session.set_captures_per_run(3);
	session.start_capture([](QString) {}); wait_capture(session);
	QTemporaryDir dir;
	const auto original = dir.filePath("original.srcollection");
	{
		pv::StoreSession store(original.toStdString(), f.context->output_formats().at("srzip"), {}, {0,0}, session, 0, true);
		BOOST_REQUIRE(store.start()); store.wait(); BOOST_REQUIRE(store.error().isEmpty());
	}
	int error;
	auto zip = zip_open(original.toUtf8().constData(), ZIP_RDONLY, &error);
	BOOST_REQUIRE(zip);
	zip_stat_t stat;
	BOOST_REQUIRE_EQUAL(zip_stat(zip, "pulseview-frames.json", 0, &stat), 0);
	QByteArray bytes(int(stat.size), '\0');
	auto entry = zip_fopen(zip, "pulseview-frames.json", 0);
	BOOST_REQUIRE(entry);
	BOOST_REQUIRE_EQUAL(zip_fread(entry, bytes.data(), bytes.size()), bytes.size());
	zip_fclose(entry); zip_close(zip);
	const auto manifest = QJsonDocument::fromJson(bytes).object();
	const auto replace_manifest = [](const QString& path, const QJsonObject& manifest) {
		int error;
		auto zip = zip_open(path.toUtf8().constData(), 0, &error);
		BOOST_REQUIRE(zip);
		const auto bytes = QJsonDocument(manifest).toJson();
		auto source = zip_source_buffer(zip, bytes.constData(), bytes.size(), 0);
		BOOST_REQUIRE(source);
		BOOST_REQUIRE(zip_file_add(zip, "pulseview-frames.json", source, ZIP_FL_OVERWRITE) >= 0);
		BOOST_REQUIRE_EQUAL(zip_close(zip), 0);
	};
	for (unsigned scenario = 0; scenario < 7; ++scenario) {
		auto altered = manifest;
		auto frames = altered.value("frames").toArray();
		auto frame = frames[0].toObject();
		if (scenario == 0) altered["version"] = 2;
		if (scenario == 1) altered["inter_frame_timing"] = "known";
		if (scenario == 2) frame["member"] = "../escape.sr";
		if (scenario == 3) frame["samples"] = "0";
		if (scenario == 4) frame["triggers"] = QJsonArray{"NaN"};
		if (scenario == 5) frame["sha256"] = "invalid";
		if (scenario == 6) frame["bytes"] = "1";
		frames[0] = frame; altered["frames"] = frames;
		const auto path = dir.filePath(QString("corrupt-%1.srcollection").arg(scenario));
		BOOST_REQUIRE(QFile::copy(original, path)); replace_manifest(path, altered);
		BOOST_CHECK_THROW(pv::CaptureCollection::read(path), std::runtime_error);
	}
	// A valid checksum with an inconsistent declared sample count must produce
	// a load error, not throw through libsigrok's noexcept C callback trampoline.
	auto altered = manifest; auto frames = altered.value("frames").toArray();
	auto frame = frames[1].toObject(); frame["samples"] = "31";
	frames[1] = frame; altered["frames"] = frames;
	const auto path = dir.filePath("wrong-count.srcollection");
	BOOST_REQUIRE(QFile::copy(original, path)); replace_manifest(path, altered);
	pv::Session restored(manager, "corrupt collection");
	restored.set_device(std::make_shared<pv::devices::SessionFile>(f.context, path.toStdString()));
	QString problem;
	restored.start_capture([&](QString message) { problem = message; }); wait_capture(restored);
	BOOST_CHECK(problem.contains("sample count"));
	BOOST_REQUIRE_EQUAL(restored.captured_segment_count(), 1);
	check_tag(restored, 0, 1);
	BOOST_CHECK_EQUAL(restored.segment_capture_outcome(0).toStdString(), "complete");
	BOOST_CHECK_EQUAL(restored.capture_outcome().toStdString(), "error");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CrashRecovery)

BOOST_AUTO_TEST_CASE(retired_condition_widgets_ignore_pending_input)
{
	Fixture f;
	pv::dialogs::TriggerEditor editor(f.context, f.device,
		pv::build_trigger_sequence(f.context, f.device, f.sequence()));
	auto *table = widget<QTableWidget>(editor, "triggerConditions");
	QPointer<QComboBox> retired = qobject_cast<QComboBox *>(table->cellWidget(1, 1));
	BOOST_REQUIRE(retired);
	// The input was queued before removing the row. Qt defers deleting its
	// cell widgets, so it can still reach the previous condition control.
	QApplication::postEvent(retired, new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier));
	table->selectRow(1);
	click(editor, "removeTriggerCondition");
	BOOST_CHECK_NO_THROW(QApplication::processEvents());
	BOOST_CHECK_EQUAL(editor.trigger()->stages()[0]->matches().size(), 1);
	BOOST_CHECK_EQUAL(editor.trigger()->stages()[0]->matches()[0]->channel()->index(), 31);
}

BOOST_AUTO_TEST_CASE(retired_condition_widgets_cannot_edit_another_stage)
{
	Fixture f;
	pv::dialogs::TriggerEditor editor(f.context, f.device,
		pv::build_trigger_sequence(f.context, f.device, f.sequence()));
	auto *table = widget<QTableWidget>(editor, "triggerConditions");
	QPointer<QComboBox> retired = qobject_cast<QComboBox *>(table->cellWidget(0, 1));
	BOOST_REQUIRE(retired);
	widget<QListWidget>(editor, "triggerStages")->setCurrentRow(1);
	BOOST_REQUIRE(retired); // Cell widgets are waiting for deferred deletion.
	retired->setCurrentIndex(retired->findData(SR_TRIGGER_RISING));
	check_same(f.sequence(), pv::read_trigger_sequence(editor.trigger()));
}

BOOST_AUTO_TEST_CASE(setup_with_invalid_view_reference_is_ignored)
{
	Fixture f;
	pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "invalid view setup");
	QTemporaryDir directory;
	QSettings settings(directory.filePath("views.ini"), QSettings::IniFormat);
	settings.setValue("meta_objs", 1);
	settings.setValue("meta_obj0/type", "time_marker");
	for (const auto &index : {QVariant(-1), QVariant(1000), QVariant("invalid")}) {
		settings.setValue("meta_obj0/assoc_view", index);
		BOOST_CHECK_NO_THROW(session.restore_setup(settings));
		BOOST_CHECK(settings.group().isEmpty());
	}
}

BOOST_AUTO_TEST_CASE(setup_does_not_dereference_missing_views)
{
	Fixture f;
	pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "missing view setup");
	QTemporaryDir directory;
	QSettings settings(directory.filePath("views.ini"), QSettings::IniFormat);
	settings.setValue("views", 2);
	settings.setValue("view1/type", 0);
	// No main view or add_view receiver exists, e.g. a removed optional view.
	BOOST_CHECK_NO_THROW(session.restore_setup(settings));
	BOOST_CHECK(session.views().empty());
	BOOST_CHECK(settings.group().isEmpty());
}

BOOST_AUTO_TEST_CASE(setup_preserves_metadata_indexes_when_a_view_is_unavailable)
{
	Fixture f;
	pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "partial view setup");
	// The first view creates the session's shared range/hover metadata,
	// just as MainWindow does before constructing secondary views.
	auto first = std::make_shared<pv::views::trace::View>(session, true);
	session.register_view(first);
	QObject::connect(&session, &pv::Session::add_view, &session,
		[&](pv::views::ViewType, pv::Session *) {
			session.register_view(std::make_shared<pv::views::trace::View>(session));
		});
	QTemporaryDir directory;
	QSettings settings(directory.filePath("views.ini"), QSettings::IniFormat);
	settings.setValue("views", 3);
	settings.setValue("view1/type", 9999); // Optional view absent from this build.
	settings.setValue("view2/type", static_cast<int>(pv::views::ViewTypeTrace));
	settings.setValue("meta_objs", 3);
	for (int index = 0; index < 3; ++index) {
		const auto group = QString("meta_obj%1/").arg(index);
		settings.setValue(group + "type", "time_marker");
		settings.setValue(group + "assoc_view", index);
		settings.setValue(group + "text", QString("view%1").arg(index));
	}
	BOOST_CHECK_NO_THROW(session.restore_setup(settings));
	BOOST_REQUIRE_EQUAL(session.views().size(), 2);
	auto second = std::dynamic_pointer_cast<pv::views::trace::View>(session.views()[1]);
	BOOST_REQUIRE(second);
	BOOST_REQUIRE_EQUAL(first->flags().size(), 1);
	BOOST_REQUIRE_EQUAL(second->flags().size(), 1);
	BOOST_CHECK_EQUAL(first->flags()[0]->get_text().toStdString(), "view0");
	BOOST_CHECK_EQUAL(second->flags()[0]->get_text().toStdString(), "view2");
	BOOST_CHECK(settings.group().isEmpty());
}

namespace {
class FailingCaptureDevice : public pv::devices::Device {
public:
	enum Failure { Start, Run, Allocation, None } failure;
	std::atomic<unsigned int> stops{0};
	FailingCaptureDevice(Fixture &f, Failure value) : failure(value)
	{
		device_ = f.device;
		session_ = f.session;
	}
	std::string full_name() const override { return "test capture failure"; }
	std::string display_name(const pv::DeviceManager &) const override { return full_name(); }
	void open() override {}
	void close() override {}
	void start() override
	{
		if (failure == Start) throw std::runtime_error("start failure");
	}
	void run() override
	{
		if (failure == Run) throw std::runtime_error("run failure");
		if (failure == Allocation) throw std::bad_alloc();
	}
	void stop() override { ++stops; }
};
}

BOOST_AUTO_TEST_CASE(capture_worker_reports_standard_exceptions)
{
	Fixture f;
	pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "capture exception");
	auto device = std::make_shared<FailingCaptureDevice>(f, FailingCaptureDevice::Start);
	session.set_device(device);
	for (auto failure : {FailingCaptureDevice::Start, FailingCaptureDevice::Run,
			FailingCaptureDevice::Allocation}) {
		device->failure = failure;
		std::atomic<bool> reported{false};
		QString error;
		session.start_capture([&](QString text) { error = text; reported = true; });
		const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
		while (!reported && std::chrono::steady_clock::now() < deadline)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		BOOST_REQUIRE(reported.load());
		session.stop_capture();
		BOOST_CHECK(!error.isEmpty());
		if (failure == FailingCaptureDevice::Allocation)
			BOOST_CHECK(error.contains("Out of memory"));
		BOOST_CHECK_EQUAL(session.get_capture_state(), pv::Session::Stopped);
		BOOST_CHECK_EQUAL(session.capture_outcome().toStdString(), "error");
		BOOST_CHECK(!session.capture_error().isEmpty());
	}
	BOOST_CHECK_EQUAL(device->stops.load(), 3);
	// The same selected device can run again after the failure is reported.
	device->failure = FailingCaptureDevice::None;
	std::atomic<bool> unexpected_error{false};
	session.start_capture([&](QString) { unexpected_error = true; });
	session.stop_capture();
	BOOST_CHECK(!unexpected_error.load());
	BOOST_CHECK_EQUAL(session.get_capture_state(), pv::Session::Stopped);
}

BOOST_AUTO_TEST_CASE(trigger_notifications_after_popup_destruction)
{
	Fixture f;
	pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "popup notifications");
	session.set_device(std::make_shared<FailingCaptureDevice>(f, FailingCaptureDevice::None));
	auto base = std::make_shared<pv::data::SignalBase>(f.device->channels()[0], pv::data::SignalBase::LogicChannel);
	pv::views::trace::LogicSignal signal(session, base);
	for (int repeat = 0; repeat < 8; ++repeat) {
		{
			QWidget popup;
			auto *form = new QFormLayout(&popup);
			static_cast<pv::views::trace::Signal &>(signal).populate_popup_form(&popup, form);
		}
		// Session and channel notifications outlive the popup's controls.
		BOOST_CHECK_NO_THROW(session.set_trigger(pv::build_trigger_sequence(f.context, f.device, f.sequence())));
		BOOST_CHECK_NO_THROW(session.set_trigger(nullptr));
		BOOST_CHECK_NO_THROW(base->set_name(QString("channel%1").arg(repeat)));
	}
}

BOOST_AUTO_TEST_CASE(input_capture_and_failed_export_remain_usable)
{
	Fixture f;
	pv::DeviceManager manager(f.context, "", false);
	pv::Session session(manager, "file capture/export");
	QTemporaryDir directory;
	const QByteArray bytes(4096, '\x5a');
	const auto source = directory.filePath("input.bin");
	{
		QFile input(source);
		BOOST_REQUIRE(input.open(QIODevice::WriteOnly));
		BOOST_REQUIRE_EQUAL(input.write(bytes), bytes.size());
	}
	session.set_device(std::make_shared<pv::devices::InputFile>(f.context, source.toStdString(),
		f.context->input_formats().at("binary"), std::map<std::string, Glib::VariantBase>()));
	QString error;
	session.start_capture([&](QString message) { error = message; });
	wait_capture(session);
	BOOST_CHECK(error.isEmpty());
	BOOST_CHECK_EQUAL(session.get_capture_state(), pv::Session::Stopped);
	const auto format = f.context->output_formats().at("binary");
	{
		pv::StoreSession failed(directory.filePath("absent/output.bin").toStdString(), format, {}, {0, 0}, session);
		BOOST_CHECK(!failed.start());
		BOOST_CHECK(!failed.error().isEmpty());
	}
	const auto target = directory.filePath("output.bin");
	{
		pv::StoreSession saved(target.toStdString(), format, {}, {0, 0}, session);
		BOOST_REQUIRE(saved.start());
		saved.wait();
		BOOST_CHECK(saved.error().isEmpty());
	}
	QFile result(target);
	BOOST_REQUIRE(result.open(QIODevice::ReadOnly));
	BOOST_CHECK(result.readAll() == bytes);
}

BOOST_AUTO_TEST_SUITE_END()
