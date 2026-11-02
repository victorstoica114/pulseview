/* SPDX-License-Identifier: GPL-3.0-or-later */
#ifndef PULSEVIEW_PV_SOFTWARETRIGGER_HPP
#define PULSEVIEW_PV_SOFTWARETRIGGER_HPP

#include <pv/data/streamwindows.hpp>
class QSettings;

namespace pv {
enum class SoftwareTriggerKind { Low, High, Rising, Falling, Edge, HighPulse, LowPulse };
enum class PulseComparison { Less, Greater, Between };
struct SoftwareTriggerCondition {
	unsigned channel = 0;
	SoftwareTriggerKind kind = SoftwareTriggerKind::Rising;
	PulseComparison comparison = PulseComparison::Less;
	uint64_t lower_ns = 1000, upper_ns = 2000;
};
struct SoftwareTrigger {
	bool enabled = false, any = true;
	std::vector<SoftwareTriggerCondition> conditions;
};

// Exact integer conversion, including fractional sample periods. Avoid
// floating point boundary changes and ns * rate overflow.
inline uint64_t pulse_time_samples(uint64_t ns, uint64_t rate, bool ceil)
{
	constexpr uint64_t billion = 1000000000;
	const auto seconds = ns / billion, remainder = ns % billion;
	const auto fraction = remainder * (rate % billion);
	const auto partial = remainder * (rate / billion) + fraction / billion;
	const auto rounded = partial + (ceil && fraction % billion ? 1 : 0);
	if (!rate || seconds > (UINT64_MAX - rounded) / rate)
		throw std::invalid_argument("Pulse width exceeds the sample clock range.");
	return seconds * rate + rounded;
}

inline data::WindowTriggerStage compile_software_trigger(const SoftwareTrigger& config,
	uint64_t rate, uint64_t enabled_channels)
{
	if (config.conditions.empty() || config.conditions.size() > 64 || !rate)
		throw std::invalid_argument("Use one to 64 software trigger conditions and a known sample rate.");
	data::WindowTriggerStage stage;
	stage.any = config.any;
	for (const auto& c : config.conditions) {
		if (c.channel >= 64 || !(enabled_channels & (UINT64_C(1) << c.channel)))
			throw std::invalid_argument("Enable every software trigger channel before capture.");
		const auto bit = UINT64_C(1) << c.channel;
		switch (c.kind) {
		case SoftwareTriggerKind::Low: stage.low |= bit; break;
		case SoftwareTriggerKind::High: stage.high |= bit; break;
		case SoftwareTriggerKind::Rising: stage.rising |= bit; break;
		case SoftwareTriggerKind::Falling: stage.falling |= bit; break;
		case SoftwareTriggerKind::Edge: stage.edge |= bit; break;
		case SoftwareTriggerKind::HighPulse:
		case SoftwareTriggerKind::LowPulse: {
			if (!c.lower_ns || (c.comparison == PulseComparison::Between && c.upper_ns < c.lower_ns))
				throw std::invalid_argument("Pulse width bounds must be positive and ordered.");
			data::WindowPulseWidth pulse;
			pulse.channel = c.channel;
			pulse.high = c.kind == SoftwareTriggerKind::HighPulse;
			switch (c.comparison) {
			case PulseComparison::Less:
				pulse.maximum = pulse_time_samples(c.lower_ns, rate, true);
				if (pulse.maximum) --pulse.maximum;
				break;
			case PulseComparison::Greater:
				pulse.minimum = pulse_time_samples(c.lower_ns, rate, false);
				if (pulse.minimum == UINT64_MAX) throw std::invalid_argument("Pulse width is too large.");
				++pulse.minimum;
				break;
			case PulseComparison::Between:
				pulse.minimum = std::max<uint64_t>(1, pulse_time_samples(c.lower_ns, rate, true));
				pulse.maximum = pulse_time_samples(c.upper_ns, rate, false);
				break;
			default: throw std::invalid_argument("Unknown pulse width comparison.");
			}
			if (pulse.minimum > pulse.maximum)
				throw std::invalid_argument("No measurable pulse fits these bounds at the selected sample rate.");
			stage.pulses.push_back(pulse);
			break;
		}
		default: throw std::invalid_argument("Unknown software trigger condition.");
		}
	}
	// Validate the same masks and pulse bounds used by the live extractor.
	data::StreamWindows check(8, 2, 0, 1, {stage}, [](data::StreamWindows::Window&&) {});
	return stage;
}

void save_software_trigger(QSettings& settings, const SoftwareTrigger& config);
SoftwareTrigger restore_software_trigger(QSettings& settings);
}
#endif
