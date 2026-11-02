/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "triggersettings.hpp"
#include "triggersequence.hpp"
#include "softwaretrigger.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

#include <QSettings>
#include <QVariant>
#include <libsigrokcxx/libsigrokcxx.hpp>

namespace pv {

void save_software_trigger(QSettings& settings, const SoftwareTrigger& config)
{
	settings.setValue("software_trigger/version", 1);
	settings.setValue("software_trigger/enabled", config.enabled);
	settings.setValue("software_trigger/any", config.any);
	QVariantList conditions;
	for (const auto& c : config.conditions) {
		QVariantMap row;
		row["channel"] = c.channel;
		row["kind"] = int(c.kind);
		row["comparison"] = int(c.comparison);
		row["lower_ns"] = QVariant::fromValue<qulonglong>(c.lower_ns);
		row["upper_ns"] = QVariant::fromValue<qulonglong>(c.upper_ns);
		conditions.push_back(row);
	}
	settings.setValue("software_trigger/conditions", conditions);
}

SoftwareTrigger restore_software_trigger(QSettings& settings)
{
	SoftwareTrigger result;
	if (!settings.contains("software_trigger/version")) return result;
	if (settings.value("software_trigger/version").toInt() != 1)
		throw std::runtime_error("Unsupported software trigger settings version.");
	result.enabled = settings.value("software_trigger/enabled").toBool();
	result.any = settings.value("software_trigger/any", true).toBool();
	const auto rows = settings.value("software_trigger/conditions").toList();
	if (rows.size() > 64 || (result.enabled && rows.empty()))
		throw std::runtime_error("Invalid software trigger condition count.");
	for (const auto& row : rows) {
		const auto values = row.toMap();
		auto integer = [&](const char* name) {
			bool ok = false;
			const auto value = values.value(name).toString().toULongLong(&ok);
			if (!ok) throw std::runtime_error("Invalid software trigger settings.");
			return uint64_t(value);
		};
		SoftwareTriggerCondition c;
		const auto channel = integer("channel"), kind = integer("kind"), comparison = integer("comparison");
		if (channel >= 64 || kind > unsigned(SoftwareTriggerKind::LowPulse) ||
			comparison > unsigned(PulseComparison::Between))
			throw std::runtime_error("Unknown software trigger channel or condition.");
		c.channel = channel; c.kind = SoftwareTriggerKind(kind); c.comparison = PulseComparison(comparison);
		c.lower_ns = integer("lower_ns"); c.upper_ns = integer("upper_ns");
		if ((c.kind == SoftwareTriggerKind::HighPulse || c.kind == SoftwareTriggerKind::LowPulse) &&
			(!c.lower_ns || (c.comparison == PulseComparison::Between && c.upper_ns < c.lower_ns)))
			throw std::runtime_error("Invalid saved pulse width bounds.");
		result.conditions.push_back(c);
	}
	return result;
}

void save_trigger_settings(QSettings &settings,
	const std::shared_ptr<sigrok::Session> &session,
	const std::shared_ptr<sigrok::Device> &device)
{
	if (!session)
		return;
	const auto trigger = session->trigger();
	settings.setValue("trigger/version", 1);
	settings.setValue("trigger/enabled", !!trigger);
	settings.setValue("trigger/name", trigger ?
		QString::fromStdString(trigger->name()) : QString());
	QVariantList stages;
	if (trigger) {
		for (const auto &stage : trigger->stages()) {
			QVariantList matches;
			for (const auto &match : stage->matches()) {
				QVariantMap item;
				item["channel"] = match->channel()->index();
				item["channel_type"] = match->channel()->type()->id();
				item["type"] = match->type()->id();
				if (!std::isnan(match->value()))
					item["value"] = match->value();
				matches.append(item);
			}
			QVariantMap item;
			item["matches"] = matches;
			stages.append(item);
		}
	}
	settings.setValue("trigger/stages", stages);
	if (hardware_trigger_supported(device)) {
		auto value = encode_hardware_trigger(read_hardware_trigger(device));
		char *text = g_variant_print(value.gobj(), TRUE);
		settings.setValue("trigger/hardware", QString::fromUtf8(text));
		g_free(text);
	} else settings.remove("trigger/hardware");
}

bool restore_trigger_settings(QSettings &settings,
	const std::shared_ptr<sigrok::Context> &context,
	const std::shared_ptr<sigrok::Device> &device,
	const std::shared_ptr<sigrok::Session> &session)
{
	if (!settings.contains("trigger/version"))
		return false;
	if (settings.value("trigger/version").toInt() != 1 ||
			!settings.contains("trigger/enabled") || !context || !device || !session)
		throw std::runtime_error("Invalid trigger setup or no capture device.");
	const QString enabled = settings.value("trigger/enabled").toString();
	if (enabled != "true" && enabled != "false")
		throw std::runtime_error("Invalid trigger enabled state.");
	HardwareTriggerOptions options;
	if (settings.contains("trigger/hardware")) {
		const auto text = settings.value("trigger/hardware").toString().toUtf8();
		if (text.size() > 65536) throw std::runtime_error("Hardware trigger setup is too large.");
		GError *error = nullptr;
		auto *raw = g_variant_parse(G_VARIANT_TYPE_VARDICT, text.constData(), nullptr, nullptr, &error);
		if (!raw) {
			g_clear_error(&error);
			throw std::runtime_error("Invalid hardware trigger setup.");
		}
		options = parse_hardware_trigger(Glib::VariantBase(raw));
	}
	if (enabled == "false") {
		apply_trigger_configuration(session, device, nullptr, options);
		return true;
	}
	const auto stages = settings.value("trigger/stages").toList();
	if (stages.empty())
		throw std::runtime_error("Trigger setup has no stages.");
	if (!device->config_check(sigrok::ConfigKey::TRIGGER_MATCH, sigrok::Capability::LIST))
		throw std::runtime_error("Device does not list supported trigger conditions.");
	auto supported = device->config_list(sigrok::ConfigKey::TRIGGER_MATCH);
	gsize size;
	const auto types = static_cast<const int32_t *>(
		g_variant_get_fixed_array(supported.gobj(), &size, sizeof(int32_t)));
	if (!size)
		throw std::runtime_error("Device lists no supported trigger conditions.");
	const auto channels = device->channels();
	const auto next = context->create_trigger(settings.value("trigger/name").toString().toStdString());
	for (const auto &stage : stages) {
		const auto matches = stage.toMap().value("matches").toList();
		if (matches.empty())
			throw std::runtime_error("Trigger setup has an empty stage.");
		const auto next_stage = next->add_stage();
		for (const auto &match : matches) {
			const auto item = match.toMap();
			bool channel_ok, type_ok, channel_type_ok;
			const auto index = item.value("channel").toUInt(&channel_ok);
			const int type = item.value("type").toInt(&type_ok);
			const int channel_type = item.value("channel_type").toInt(&channel_type_ok);
			const auto channel = std::find_if(channels.begin(), channels.end(),
				[index](const std::shared_ptr<sigrok::Channel> &c) { return c->index() == index; });
			if (!channel_ok || !type_ok || !channel_type_ok || channel == channels.end() ||
					(*channel)->type()->id() != channel_type ||
					std::find(types, types + size, type) == types + size)
				throw std::runtime_error("Unsupported trigger channel or condition.");
			float value = NAN;
			if (item.contains("value")) {
				bool value_ok;
				value = item.value("value").toFloat(&value_ok);
				if (!value_ok || !std::isfinite(value))
					throw std::runtime_error("Invalid trigger threshold value.");
			}
			next_stage->add_match(*channel, sigrok::TriggerMatchType::get(type), value);
		}
	}
	// All stages have been validated before changing the live session.
	apply_trigger_configuration(session, device, next, options);
	return true;
}

}
