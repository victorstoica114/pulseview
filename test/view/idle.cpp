/* SPDX-License-Identifier: GPL-2.0-or-later */
#include <boost/test/unit_test.hpp>
#include <pv/data/idlescan.hpp>
#include <pv/views/trace/idletimemap.hpp>
#include <array>
#include <cmath>

using pv::data::IdleScan;
using pv::views::trace::IdleTimeMap;
using pv::util::Timestamp;

BOOST_AUTO_TEST_SUITE(IdleDisplay)

BOOST_AUTO_TEST_CASE(real_time_roundtrip_and_monotonicity)
{
	IdleTimeMap map;
	map.append(Timestamp("0.001"), Timestamp("2.001"), Timestamp("0.00001"));
	map.append(Timestamp("2.002"), Timestamp("5.002"), Timestamp("0.00001"));
	Timestamp previous = map.to_display(-1);
	for (int i = 0; i <= 8000; ++i) {
		const Timestamp real = Timestamp(i) / 1000 - 1;
		const auto display = map.to_display(real);
		BOOST_CHECK(display >= previous);
		BOOST_CHECK(abs(map.to_real(display) - real) < Timestamp("1e-22"));
		previous = display;
	}
	BOOST_CHECK_EQUAL(map.to_real(map.to_display(Timestamp("2.001"))) -
		map.to_real(map.to_display(Timestamp("0.001"))), Timestamp(2));
}

BOOST_AUTO_TEST_CASE(long_capture_and_nanosecond_activity_keep_precision)
{
	IdleTimeMap map;
	const Timestamp epoch("1234567890123.000000001");
	map.append(epoch, epoch + 2000, Timestamp("1e-6"));
	const Timestamp edge = epoch + Timestamp("2000.000000003");
	BOOST_CHECK(abs(map.to_real(map.to_display(edge)) - edge) < Timestamp("1e-15"));
	BOOST_CHECK(abs(map.to_display(edge + Timestamp("1e-9")) -
		map.to_display(edge) - Timestamp("1e-9")) < Timestamp("1e-15"));
}

BOOST_AUTO_TEST_CASE(clear_restores_uniform_time_and_rejects_overlap)
{
	IdleTimeMap map;
	map.append(0, 2, Timestamp("1e-6"));
	BOOST_CHECK_THROW(map.append(1, 3, Timestamp("1e-6")), std::invalid_argument);
	map.clear();
	BOOST_CHECK(map.empty());
	for (int i = -10; i < 10; ++i) {
		BOOST_CHECK_EQUAL(map.to_real(i), Timestamp(i));
		BOOST_CHECK_EQUAL(map.to_display(i), Timestamp(i));
	}
}

BOOST_AUTO_TEST_CASE(single_sample_high_channel_pulse_is_not_idle)
{
	std::array<uint32_t, 41> words{};
	words[20] = UINT32_C(0x80000000);
	IdleScan scan(4, 16);
	scan.append(reinterpret_cast<const uint8_t*>(words.data()), 41);
	scan.finish();
	BOOST_REQUIRE_EQUAL(scan.ranges().size(), 2);
	BOOST_CHECK_EQUAL(scan.ranges()[0].first, 2);
	BOOST_CHECK_EQUAL(scan.ranges()[0].second, 18);
	BOOST_CHECK_EQUAL(scan.ranges()[1].first, 23);
	BOOST_CHECK_EQUAL(scan.ranges()[1].second, 39);
	BOOST_CHECK_EQUAL(words[20], UINT32_C(0x80000000));
}

BOOST_AUTO_TEST_CASE(chunk_boundaries_and_64bit_inputs)
{
	std::array<uint64_t, 500> words{};
	words[128] = UINT64_C(1) << 63;
	words[255] = UINT64_C(1) << 33;
	IdleScan whole(8, 32), pieces(8, 32);
	whole.append(reinterpret_cast<const uint8_t*>(words.data()), words.size());
	whole.finish();
	for (size_t i = 0; i < words.size();) {
		const auto count = std::min<size_t>((i % 13) + 1, words.size() - i);
		pieces.append(reinterpret_cast<const uint8_t*>(words.data() + i), count);
		i += count;
	}
	pieces.finish();
	BOOST_CHECK(whole.ranges() == pieces.ranges());
	BOOST_REQUIRE_EQUAL(whole.ranges().size(), 3);
	for (const auto& range : whole.ranges()) {
		BOOST_CHECK(!(range.first <= 128 && range.second > 128));
		BOOST_CHECK(!(range.first <= 255 && range.second > 255));
	}
}

BOOST_AUTO_TEST_CASE(short_constant_spans_and_invalid_geometry)
{
	std::array<uint8_t, 20> words{};
	for (size_t i = 0; i < words.size(); ++i) words[i] = i % 2;
	IdleScan scan(1, 8);
	scan.append(words.data(), words.size());
	scan.finish();
	BOOST_CHECK(scan.ranges().empty());
	BOOST_CHECK_THROW(IdleScan(0, 8), std::invalid_argument);
	BOOST_CHECK_THROW(IdleScan(9, 8), std::invalid_argument);
	BOOST_CHECK_THROW(IdleScan(1, 1), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
