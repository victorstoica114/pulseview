/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "triggersequence.hpp"
#include <algorithm>
#include <cmath>
#include <set>
#include <stdexcept>
#include <libsigrokcxx/libsigrokcxx.hpp>

namespace pv {

bool hardware_trigger_supported(const std::shared_ptr<sigrok::Device> &device)
{
	return device && device->config_check(sigrok::ConfigKey::TRIGGER_OPTIONS, sigrok::Capability::GET) &&
		device->config_check(sigrok::ConfigKey::TRIGGER_OPTIONS, sigrok::Capability::SET);
}

HardwareTriggerOptions parse_hardware_trigger(const Glib::VariantBase &value)
{
	HardwareTriggerOptions result;
	auto owned = value;
	auto *dict = owned.gobj();
	const char *mode = nullptr, *edge = nullptr;
	if (!dict || !g_variant_is_of_type(dict, G_VARIANT_TYPE_VARDICT) ||
			!g_variant_lookup(dict, "mode", "&s", &mode))
		throw std::runtime_error("Invalid hardware trigger options.");
	result.mode = mode;
	if (result.mode == "default" && g_variant_n_children(dict) == 1) return result;
	if (result.mode == "parallel" && g_variant_n_children(dict) == 2) {
		auto *stages = g_variant_lookup_value(dict, "stages", G_VARIANT_TYPE("a(ub)"));
		if (!stages) throw std::runtime_error("Invalid trigger qualifiers.");
		for (gsize i = 0; i < g_variant_n_children(stages); ++i) {
			guint32 count; gboolean continuous;
			g_variant_get_child(stages, i, "(ub)", &count, &continuous);
			result.stages.push_back({count, !!continuous});
		}
		g_variant_unref(stages);
		if (result.stages.empty() || result.stages.size() > 16)
			throw std::runtime_error("Use one to sixteen trigger stages.");
		for (const auto &stage : result.stages)
			if (!stage.count || stage.count > 255)
				throw std::runtime_error("Trigger Count must be between 1 and 255.");
		return result;
	}
	if (result.mode == "serial" && g_variant_n_children(dict) == 6 &&
			g_variant_lookup(dict, "clock", "u", &result.clock) &&
			g_variant_lookup(dict, "data", "u", &result.data) &&
			g_variant_lookup(dict, "edge", "&s", &edge) &&
			g_variant_lookup(dict, "bits", "u", &result.bits) &&
			g_variant_lookup(dict, "value", "u", &result.value) &&
			(std::string(edge) == "rising" || std::string(edge) == "falling") &&
			result.clock < 32 && result.data < 32 && result.clock != result.data &&
			result.bits >= 1 && result.bits <= 16 && result.value < (1U << result.bits)) {
		result.falling = std::string(edge) == "falling";
		return result;
	}
	throw std::runtime_error("Invalid or unsupported hardware trigger options.");
}

Glib::VariantBase encode_hardware_trigger(const HardwareTriggerOptions &options)
{
	GVariantBuilder dict, stages;
	g_variant_builder_init(&dict, G_VARIANT_TYPE_VARDICT);
	g_variant_builder_add(&dict, "{sv}", "mode", g_variant_new_string(options.mode.c_str()));
	if (options.mode == "parallel") {
		g_variant_builder_init(&stages, G_VARIANT_TYPE("a(ub)"));
		for (const auto &stage : options.stages)
			g_variant_builder_add(&stages, "(ub)", stage.count, stage.continuous);
		g_variant_builder_add(&dict, "{sv}", "stages", g_variant_builder_end(&stages));
	} else if (options.mode == "serial") {
		g_variant_builder_add(&dict, "{sv}", "clock", g_variant_new_uint32(options.clock));
		g_variant_builder_add(&dict, "{sv}", "data", g_variant_new_uint32(options.data));
		g_variant_builder_add(&dict, "{sv}", "edge", g_variant_new_string(options.falling ? "falling" : "rising"));
		g_variant_builder_add(&dict, "{sv}", "bits", g_variant_new_uint32(options.bits));
		g_variant_builder_add(&dict, "{sv}", "value", g_variant_new_uint32(options.value));
	}
	return Glib::VariantBase(g_variant_builder_end(&dict));
}

HardwareTriggerOptions read_hardware_trigger(const std::shared_ptr<sigrok::Device> &device)
{
	return hardware_trigger_supported(device) ?
		parse_hardware_trigger(device->config_get(sigrok::ConfigKey::TRIGGER_OPTIONS)) : HardwareTriggerOptions();
}

void validate_hardware_trigger(const std::shared_ptr<sigrok::Device> &device,
	const std::shared_ptr<sigrok::Trigger> &trigger, const HardwareTriggerOptions &options)
{
	parse_hardware_trigger(encode_hardware_trigger(options));
	if (options.mode == "default") return;
	if (!hardware_trigger_supported(device))
		throw std::runtime_error("This device does not support hardware trigger options.");
	if (options.mode == "parallel") {
		if (!trigger || options.stages.size() != trigger->stages().size())
			throw std::runtime_error("Each parallel trigger stage needs one Count setting.");
	} else {
		if (trigger) throw std::runtime_error("Serial and parallel triggers cannot be combined.");
		for (const auto index : {options.clock, options.data}) {
			bool found = false;
			for (const auto &channel : device->channels())
				if (channel->index() == index && channel->enabled() && channel->type()->id() == SR_CHANNEL_LOGIC)
					found = true;
			if (!found) throw std::runtime_error("Enable both serial trigger channels.");
		}
	}
}

void apply_trigger_configuration(const std::shared_ptr<sigrok::Session> &session,
	const std::shared_ptr<sigrok::Device> &device,
	const std::shared_ptr<sigrok::Trigger> &trigger, const HardwareTriggerOptions &options)
{
	if (!session) throw std::runtime_error("No capture session.");
	validate_hardware_trigger(device, trigger, options);
	const bool extended = hardware_trigger_supported(device);
	Glib::VariantBase previous;
	if (extended) {
		previous = device->config_get(sigrok::ConfigKey::TRIGGER_OPTIONS);
		device->config_set(sigrok::ConfigKey::TRIGGER_OPTIONS, encode_hardware_trigger(options));
	}
	try { session->set_trigger(trigger); }
	catch (...) {
		if (extended) device->config_set(sigrok::ConfigKey::TRIGGER_OPTIONS, previous);
		throw;
	}
}

bool trigger_match_allowed(int channel_type, int match_type)
{
	if (channel_type == SR_CHANNEL_LOGIC)
		return match_type == SR_TRIGGER_ZERO || match_type == SR_TRIGGER_ONE ||
			match_type == SR_TRIGGER_RISING || match_type == SR_TRIGGER_FALLING ||
			match_type == SR_TRIGGER_EDGE;
	if (channel_type == SR_CHANNEL_ANALOG)
		return match_type == SR_TRIGGER_RISING || match_type == SR_TRIGGER_FALLING ||
			match_type == SR_TRIGGER_EDGE || match_type == SR_TRIGGER_OVER ||
			match_type == SR_TRIGGER_UNDER;
	return false;
}

std::vector<int> trigger_match_types(const std::shared_ptr<sigrok::Device> &device)
{
	if (!device || !device->config_check(sigrok::ConfigKey::TRIGGER_MATCH,
			sigrok::Capability::LIST))
		return {};
	auto listed = device->config_list(sigrok::ConfigKey::TRIGGER_MATCH);
	if (!listed.gobj() || !g_variant_is_of_type(listed.gobj(), G_VARIANT_TYPE("ai")))
		throw std::runtime_error("Invalid list of trigger conditions.");
	gsize count = 0;
	const auto *values = static_cast<const int32_t *>(
		g_variant_get_fixed_array(listed.gobj(), &count, sizeof(int32_t)));
	return count ? std::vector<int>(values, values + count) : std::vector<int>();
}

TriggerSequence read_trigger_sequence(const std::shared_ptr<sigrok::Trigger> &trigger)
{
	TriggerSequence result;
	if (!trigger)
		return result;
	result.enabled = true;
	result.name = trigger->name();
	for (const auto &stage : trigger->stages()) {
		result.stages.emplace_back();
		for (const auto &match : stage->matches())
			result.stages.back().push_back({match->channel()->index(),
				match->type()->id(), match->value()});
	}
	return result;
}

std::shared_ptr<sigrok::Trigger> build_trigger_sequence(
	const std::shared_ptr<sigrok::Context> &context,
	const std::shared_ptr<sigrok::Device> &device, const TriggerSequence &sequence)
{
	if (!sequence.enabled)
		return nullptr;
	if (!context || !device || sequence.stages.empty())
		throw std::runtime_error("An enabled trigger needs a device and at least one stage.");
	const auto supported = trigger_match_types(device);
	const auto channels = device->channels();
	auto candidate = context->create_trigger(sequence.name);
	for (const auto &stage : sequence.stages) {
		if (stage.empty())
			throw std::runtime_error("Each trigger stage needs at least one condition.");
		std::set<unsigned int> used;
		auto next = candidate->add_stage();
		for (const auto &condition : stage) {
			const auto channel = std::find_if(channels.begin(), channels.end(),
				[&](const std::shared_ptr<sigrok::Channel> &ch) {
					return ch->index() == condition.channel;
				});
			if (channel == channels.end() || !(*channel)->enabled())
				throw std::runtime_error("Every trigger channel must exist and be enabled.");
			if (!used.insert(condition.channel).second)
				throw std::runtime_error("Use a channel only once within each stage.");
			if (!trigger_match_allowed((*channel)->type()->id(), condition.type) ||
					std::find(supported.begin(), supported.end(), condition.type) == supported.end())
				throw std::runtime_error("A trigger condition is not supported by this device or channel.");
			if (std::isinf(condition.value) ||
					((condition.type == SR_TRIGGER_OVER || condition.type == SR_TRIGGER_UNDER) &&
					 !std::isfinite(condition.value)))
				throw std::runtime_error("This trigger threshold must be a finite number.");
			next->add_match(*channel, sigrok::TriggerMatchType::get(condition.type), condition.value);
		}
	}
	return candidate;
}

}
