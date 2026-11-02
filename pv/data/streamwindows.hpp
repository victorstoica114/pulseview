/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef PULSEVIEW_DATA_STREAMWINDOWS_HPP
#define PULSEVIEW_DATA_STREAMWINDOWS_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <functional>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace pv { namespace data {

struct WindowPulseWidth {
	unsigned channel = 0;
	bool high = true;
	// Inclusive sample bounds, derived from the requested time comparison.
	uint64_t minimum = 1, maximum = UINT64_MAX;
};

struct WindowTriggerStage {
	uint64_t high = 0, low = 0, rising = 0, falling = 0, edge = 0;
	bool any = false;
	std::vector<WindowPulseWidth> pulses;
	bool matches(uint64_t previous, uint64_t current, bool have_previous,
		const std::array<uint64_t, 64>& widths, uint64_t completed) const {
		const auto changed = previous ^ current;
		bool result = any ? ((current & high) || (~current & low) || (have_previous &&
			((changed & current & rising) || (changed & previous & falling) || (changed & edge)))) :
			(current & high) == high && !(current & low) &&
			(!(rising | falling | edge) || (have_previous &&
			 ((changed & current & rising) == rising) &&
			 ((changed & previous & falling) == falling) &&
			 ((changed & edge) == edge)));
		for (const auto& pulse : pulses) {
			const auto bit = UINT64_C(1) << pulse.channel;
			const bool matched = (completed & bit) && bool(previous & bit) == pulse.high &&
				widths[pulse.channel] >= pulse.minimum && widths[pulse.channel] <= pulse.maximum;
			result = any ? result || matched : result && matched;
		}
		return result;
	}
};

// One uninterrupted sample clock. Other trigger matches inside an active
// post-trigger interval are already included in that window; no device rearm.
class StreamWindows {
public:
	struct Window {
		std::vector<uint8_t> bytes;
		uint64_t origin = 0, trigger_offset = 0;
		bool complete = false;
	};
	using Callback = std::function<void(Window&&)>;
	StreamWindows(unsigned unit, uint64_t depth, uint64_t pre, unsigned target,
		std::vector<WindowTriggerStage> stages, Callback callback,
		std::function<bool()> cancelled = {}) :
		unit_(unit), target_(target), depth_(depth), pre_(pre),
		stages_(std::move(stages)), callback_(std::move(callback)), cancelled_(std::move(cancelled))
	{
		if (!unit || unit > 8 || !depth || pre >= depth || !target || target > 10000 ||
			stages_.empty() || stages_.size() > 64 || !callback_ ||
			depth > std::numeric_limits<size_t>::max() / unit)
			throw std::invalid_argument("Invalid continuous capture window configuration.");
		const uint64_t mask = unit == 8 ? UINT64_MAX : (UINT64_C(1) << (unit * 8)) - 1;
		for (const auto& stage : stages_) {
			auto used = stage.high | stage.low | stage.rising | stage.falling | stage.edge;
			for (const auto& pulse : stage.pulses) {
				if (pulse.channel >= unit * 8 || !pulse.minimum || pulse.minimum > pulse.maximum)
					throw std::invalid_argument("Invalid pulse width bounds or channel.");
				used |= UINT64_C(1) << pulse.channel;
				pulse_mask_ |= UINT64_C(1) << pulse.channel;
			}
			if (!used || (used & ~mask) || (!stage.any && ((stage.high & stage.low) ||
				(stage.rising & (stage.falling | stage.low)) || (stage.falling & stage.high))))
				throw std::invalid_argument("Invalid digital software trigger stage.");
		}
		for (unsigned channel = 0; channel < unit * 8; ++channel)
			if (pulse_mask_ & (UINT64_C(1) << channel)) pulse_channels_.push_back(channel);
		history_.resize(size_t(pre * unit));
	}
	uint64_t samples_seen() const { return next_sample_; }
	unsigned unit_size() const { return unit_; }
	unsigned completed() const { return completed_; }
	bool target_reached() const { return completed_ >= target_; }
	bool pending() const { return active_; }

	void feed(const void* data, size_t bytes) {
		if (bytes % unit_ || (bytes && !data))
			throw std::invalid_argument("Misaligned continuous logic packet.");
		if (finished_) return;
		const auto input = static_cast<const uint8_t*>(data);
		size_t position = 0;
		while (position < bytes && !target_reached()) {
			if (!(position & 4095) && cancelled_ && cancelled_()) break;
			if (active_) {
				const size_t count = std::min(bytes - position, size_t(depth_ * unit_) - current_.bytes.size());
				append_active(input + position, count);
				position += count;
				continue;
			}
			const auto sample = load(input + position);
			// Consecutive standard trigger stages, including overlapping prefixes
			// and stage transitions across USB packet boundaries.
			scan(sample, next_sample_);
			if ((prefix_ & (UINT64_C(1) << (stages_.size() - 1))) && filled_ == history_.size()) {
				active_ = true;
				current_ = Window();
				current_.origin = next_sample_ - pre_;
				current_.trigger_offset = pre_;
				current_.bytes.reserve(size_t(depth_ * unit_));
				current_.bytes.insert(current_.bytes.end(), history_.begin() + head_, history_.end());
				current_.bytes.insert(current_.bytes.end(), history_.begin(), history_.begin() + head_);
				append_active(input + position, unit_, false);
			} else {
				append_history(input + position, unit_);
				advance(input + position, unit_);
			}
			position += unit_;
		}
	}
	void finish() {
		if (finished_) return;
		finished_ = true;
		if (active_ && !current_.bytes.empty()) publish(false);
	}

private:
	void scan(uint64_t sample, uint64_t position) {
		const auto changed = have_previous_ ? (previous_ ^ sample) & pulse_mask_ : 0;
		const auto completed = changed & known_pulse_start_;
		for (const auto channel : pulse_channels_) {
			const auto bit = UINT64_C(1) << channel;
			if (completed & bit) widths_[channel] = position - pulse_start_[channel];
		}
		uint64_t matches = 0;
		for (size_t stage = 0; stage < stages_.size(); ++stage)
			if (stages_[stage].matches(previous_, sample, have_previous_, widths_, completed))
				matches |= UINT64_C(1) << stage;
		prefix_ = ((prefix_ << 1) | 1) & matches;
		for (const auto channel : pulse_channels_)
			if (changed & (UINT64_C(1) << channel)) pulse_start_[channel] = position;
		known_pulse_start_ |= changed;
		previous_ = sample;
		have_previous_ = true;
	}
	uint64_t load(const uint8_t* data) const {
		// libsigrok logic samples use least-significant byte first.
		uint64_t value = 0;
		for (unsigned byte = 0; byte < unit_; ++byte) value |= uint64_t(data[byte]) << (8 * byte);
		return value;
	}
	void advance(const uint8_t* data, size_t bytes) {
		const auto count = bytes / unit_;
		if (count > UINT64_MAX - next_sample_)
			throw std::overflow_error("Continuous source sample index overflow.");
		next_sample_ += count;
		previous_ = load(data + bytes - unit_);
		have_previous_ = true;
	}
	void append_history(const uint8_t* data, size_t bytes) {
		if (history_.empty()) return;
		if (bytes >= history_.size()) {
			std::memcpy(history_.data(), data + bytes - history_.size(), history_.size());
			head_ = 0;
			filled_ = history_.size();
			return;
		}
		const auto first = std::min(bytes, history_.size() - head_);
		std::memcpy(history_.data() + head_, data, first);
		if (bytes > first) std::memcpy(history_.data(), data + first, bytes - first);
		head_ = (head_ + bytes) % history_.size();
		filled_ = std::min(history_.size(), filled_ + bytes);
	}
	void append_active(const uint8_t* data, size_t bytes, bool scan = true) {
		current_.bytes.insert(current_.bytes.end(), data, data + bytes);
		// A sequence may start inside this recorded window and finish just
		// outside it. Carry matching prefixes across that boundary as well.
		if (scan && (stages_.size() > 1 || pulse_mask_))
			for (size_t offset = 0; offset < bytes; offset += unit_) {
				const auto sample = load(data + offset);
				this->scan(sample, next_sample_ + offset / unit_);
			}
		append_history(data, bytes);
		advance(data, bytes);
		if (current_.bytes.size() == depth_ * unit_) publish(true);
	}
	void publish(bool complete) {
		current_.complete = complete;
		active_ = false;
		if (complete) ++completed_;
		callback_(std::move(current_));
		current_ = Window();
	}
	uint64_t pulse_mask_ = 0, known_pulse_start_ = 0;
	std::vector<unsigned> pulse_channels_;
	std::array<uint64_t, 64> pulse_start_{}, widths_{};
	unsigned unit_, target_, completed_ = 0;
	uint64_t depth_, pre_, next_sample_ = 0, previous_ = 0, prefix_ = 0;
	size_t head_ = 0, filled_ = 0;
	bool have_previous_ = false, active_ = false, finished_ = false;
	std::vector<uint8_t> history_;
	Window current_;
	std::vector<WindowTriggerStage> stages_;
	Callback callback_;
	std::function<bool()> cancelled_;
};

}} // namespace pv::data
#endif
