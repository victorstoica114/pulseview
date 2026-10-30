/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef PULSEVIEW_PV_DATA_IDLESCAN_HPP
#define PULSEVIEW_PV_DATA_IDLESCAN_HPP

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <vector>

namespace pv { namespace data {

/* Incremental, exact comparison of all stored digital bits, including narrow
 * pulses. Call only on a stopped segment; no sampling or decimation is used. */
class IdleScan {
public:
	using Range = std::pair<uint64_t, uint64_t>;
	IdleScan(unsigned unit_size, uint64_t minimum_samples) :
		unit_size_(unit_size), minimum_(minimum_samples) {
		if (!unit_size || unit_size > 8 || minimum_samples < 2)
			throw std::invalid_argument("Invalid idle scan geometry");
	}
	void append(const uint8_t *bytes, uint64_t count) {
		for (uint64_t i = 0; i < count; ++i, ++position_) {
			uint64_t word = 0;
			std::memcpy(&word, bytes + i * unit_size_, unit_size_);
			if (position_ && word != previous_) {
				finish_run();
				run_start_ = position_;
			}
			previous_ = word;
		}
	}
	void finish() { finish_run(); run_start_ = position_; }
	uint64_t position() const { return position_; }
	const std::vector<Range>& ranges() const { return ranges_; }
private:
	void finish_run() {
		if (position_ - run_start_ < minimum_) return;
		const uint64_t guard = std::min<uint64_t>(8, minimum_ / 8);
		ranges_.emplace_back(run_start_ + guard, position_ - guard);
		if (ranges_.size() > 100000)
			throw std::runtime_error("Too many idle intervals; increase the idle threshold");
	}
	unsigned unit_size_;
	uint64_t minimum_, position_ = 0, run_start_ = 0, previous_ = 0;
	std::vector<Range> ranges_;
};

} }
#endif
