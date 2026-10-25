/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "triggersequence.hpp"
#include <algorithm>
#include <cmath>
#include <set>
#include <stdexcept>
#include <libsigrokcxx/libsigrokcxx.hpp>

namespace pv {

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
