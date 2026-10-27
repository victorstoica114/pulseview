/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Real demo sessions and real Qt widgets; no physical device discovery. */
#include "test/test.hpp"
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QEvent>
#include <QFormLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QTableWidget>
#include <QTemporaryDir>
#include <libsigrokcxx/libsigrokcxx.hpp>
#include <pv/dialogs/triggereditor.hpp>
#include <pv/triggersettings.hpp>
#include <pv/triggersequence.hpp>
#include <pv/devicemanager.hpp>
#include <pv/session.hpp>
#include <pv/views/trace/logicsignal.hpp>
#include <pv/prop/int.hpp>

namespace {
void application()
{
	static int argc = 1;
	static char executable[] = "pulseview-test";
	static char *argv[] = {executable, nullptr};
	static QApplication app(argc, argv);
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

BOOST_AUTO_TEST_SUITE_END()
