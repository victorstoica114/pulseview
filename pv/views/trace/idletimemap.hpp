/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef PULSEVIEW_PV_VIEWS_TRACE_IDLETIMEMAP_HPP
#define PULSEVIEW_PV_VIEWS_TRACE_IDLETIMEMAP_HPP

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <pv/util.hpp>

namespace pv { namespace views { namespace trace {

/* Strictly monotonic and reversible. Only display coordinates are shortened;
 * samples, annotations, markers and all stored timestamps remain in real time.
 * Multiprecision avoids subtracting nearly equal doubles after a long idle. */
class IdleTimeMap {
public:
	using Time = pv::util::Timestamp;
	struct Gap { Time begin, end, display_begin, display_end; };
	void clear() { gaps_.clear(); }
	bool empty() const { return gaps_.empty(); }
	const std::vector<Gap>& gaps() const { return gaps_; }
	void append(const Time& begin, const Time& end, const Time& display_duration) {
		if (end <= begin || display_duration <= 0 || display_duration >= end - begin)
			return;
		if (!gaps_.empty() && begin < gaps_.back().end)
			throw std::invalid_argument("Overlapping idle intervals");
		const Time removed = gaps_.empty() ? Time(0) :
			gaps_.back().end - gaps_.back().display_end;
		gaps_.push_back({begin, end, begin - removed, begin - removed + display_duration});
	}
	Time to_display(const Time& real) const {
		const auto next = std::upper_bound(gaps_.begin(), gaps_.end(), real,
			[](const Time& t, const Gap& gap) { return t < gap.begin; });
		if (next == gaps_.begin()) return real;
		const Gap& gap = *std::prev(next);
		if (real < gap.end)
			return gap.display_begin + (real - gap.begin) *
				(gap.display_end - gap.display_begin) / (gap.end - gap.begin);
		return real - (gap.end - gap.display_end);
	}
	Time to_real(const Time& display) const {
		const auto next = std::upper_bound(gaps_.begin(), gaps_.end(), display,
			[](const Time& t, const Gap& gap) { return t < gap.display_begin; });
		if (next == gaps_.begin()) return display;
		const Gap& gap = *std::prev(next);
		if (display < gap.display_end)
			return gap.begin + (display - gap.display_begin) *
				(gap.end - gap.begin) / (gap.display_end - gap.display_begin);
		return display + (gap.end - gap.display_end);
	}
private:
	std::vector<Gap> gaps_;
};

} } }
#endif
