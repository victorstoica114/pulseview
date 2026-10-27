/* SPDX-License-Identifier: GPL-3.0-or-later */
#ifndef PULSEVIEW_PV_TRIGGERSEQUENCE_HPP
#define PULSEVIEW_PV_TRIGGERSEQUENCE_HPP

#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <glibmm/variant.h>

namespace sigrok {
class Context;
class Device;
class Trigger;
class Session;
}

namespace pv {

struct TriggerQualifier {
	unsigned int count = 1;
	bool continuous = false;
};

struct HardwareTriggerOptions {
	std::string mode = "default";
	std::vector<TriggerQualifier> stages;
	unsigned int clock = 0, data = 1, bits = 8, value = 0;
	bool falling = false;
};

bool hardware_trigger_supported(const std::shared_ptr<sigrok::Device> &device);
HardwareTriggerOptions read_hardware_trigger(const std::shared_ptr<sigrok::Device> &device);
HardwareTriggerOptions parse_hardware_trigger(const Glib::VariantBase &value);
Glib::VariantBase encode_hardware_trigger(const HardwareTriggerOptions &options);
void validate_hardware_trigger(const std::shared_ptr<sigrok::Device> &device,
	const std::shared_ptr<sigrok::Trigger> &trigger, const HardwareTriggerOptions &options);
void apply_trigger_configuration(const std::shared_ptr<sigrok::Session> &session,
	const std::shared_ptr<sigrok::Device> &device,
	const std::shared_ptr<sigrok::Trigger> &trigger, const HardwareTriggerOptions &options);

struct TriggerCondition {
	TriggerCondition(unsigned int channel, int type,
		float value = std::numeric_limits<float>::quiet_NaN()) :
		channel(channel), type(type), value(value) {}
	unsigned int channel;
	int type;
	float value;
};

struct TriggerSequence {
	bool enabled = false;
	std::string name;
	std::vector<std::vector<TriggerCondition>> stages;
};

bool trigger_match_allowed(int channel_type, int match_type);
std::vector<int> trigger_match_types(const std::shared_ptr<sigrok::Device> &device);
TriggerSequence read_trigger_sequence(const std::shared_ptr<sigrok::Trigger> &trigger);
// Build a fully validated candidate; never change the live session.
std::shared_ptr<sigrok::Trigger> build_trigger_sequence(
	const std::shared_ptr<sigrok::Context> &context,
	const std::shared_ptr<sigrok::Device> &device, const TriggerSequence &sequence);

}
#endif
