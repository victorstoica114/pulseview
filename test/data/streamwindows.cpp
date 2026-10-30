/* SPDX-License-Identifier: GPL-2.0-or-later */
#include <boost/test/unit_test.hpp>
#include <pv/data/streamwindows.hpp>
#include <array>

using pv::data::StreamWindows;
using pv::data::WindowTriggerStage;

BOOST_AUTO_TEST_SUITE(StreamWindowTest)

BOOST_AUTO_TEST_CASE(real_pretrigger_history_and_absolute_origins)
{
	std::vector<uint8_t> input(100, 1);
	input[20] = input[60] = 0;
	std::vector<StreamWindows::Window> windows;
	WindowTriggerStage trigger; trigger.falling = 1;
	StreamWindows stream(1, 10, 4, 2, {trigger}, [&](StreamWindows::Window&& w) { windows.push_back(std::move(w)); });
	stream.feed(input.data(), input.size());
	BOOST_REQUIRE_EQUAL(windows.size(), 2);
	BOOST_CHECK(stream.target_reached());
	for (unsigned i = 0; i < 2; ++i) {
		const auto start = 16 + i * 40;
		BOOST_CHECK_EQUAL(windows[i].origin, start);
		BOOST_CHECK_EQUAL(windows[i].trigger_offset, 4);
		BOOST_CHECK(windows[i].complete);
		BOOST_CHECK(windows[i].bytes == std::vector<uint8_t>(input.begin()+start, input.begin()+start+10));
	}
	BOOST_CHECK_EQUAL(windows[1].origin - windows[0].origin, 40);
}

BOOST_AUTO_TEST_CASE(chunk_boundaries_and_initial_unknown_level)
{
	const std::array<uint8_t, 19> input{{0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1}};
	for (size_t block = 1; block <= input.size(); ++block) {
		std::vector<StreamWindows::Window> windows;
		WindowTriggerStage t; t.falling = 1;
		StreamWindows stream(1, 6, 3, 2, {t}, [&](StreamWindows::Window&& w) { windows.push_back(std::move(w)); });
		for (size_t i = 0; i < input.size(); i += block)
			stream.feed(input.data()+i, std::min(block, input.size()-i));
		BOOST_REQUIRE_EQUAL(windows.size(), 2);
		BOOST_CHECK_EQUAL(windows[0].origin, 4);
		BOOST_CHECK_EQUAL(windows[1].origin, 10);
		BOOST_CHECK_EQUAL(windows[0].bytes[3], 0);
	}
}

BOOST_AUTO_TEST_CASE(pending_window_keeps_additional_events_and_partial_stop)
{
	std::vector<uint8_t> input(24, 1);
	input[8] = input[10] = input[19] = 0;
	std::vector<StreamWindows::Window> windows;
	WindowTriggerStage trigger; trigger.falling = 1;
	StreamWindows stream(1, 12, 4, 3, {trigger}, [&](StreamWindows::Window&& w) { windows.push_back(std::move(w)); });
	stream.feed(input.data(), input.size());
	stream.finish(); stream.finish();
	BOOST_REQUIRE_EQUAL(windows.size(), 2);
	BOOST_CHECK(windows[0].complete);
	BOOST_CHECK_EQUAL(windows[0].bytes[6], 0); // second edge is still recorded
	BOOST_CHECK(!windows[1].complete);
	BOOST_CHECK_EQUAL(windows[1].origin, 15);
	BOOST_CHECK_EQUAL(windows[1].bytes.size(), 9);
	BOOST_CHECK_EQUAL(stream.completed(), 1);
}

BOOST_AUTO_TEST_CASE(sparse_64bit_masks_and_multistage_overlapping_prefixes)
{
	std::array<uint64_t, 30> input{};
	input[13] = input[14] = UINT64_C(1) << 63;
	WindowTriggerStage low, high;
	low.low = UINT64_C(1) << 63; high.high = UINT64_C(1) << 63;
	std::vector<StreamWindows::Window> windows;
	StreamWindows stream(8, 8, 3, 1, {low,low,high,high}, [&](StreamWindows::Window&& w) { windows.push_back(std::move(w)); });
	stream.feed(input.data(), 12*8);
	stream.feed(input.data()+12, (input.size()-12)*8);
	BOOST_REQUIRE_EQUAL(windows.size(), 1);
	BOOST_CHECK_EQUAL(windows[0].origin, 11);
	BOOST_CHECK_EQUAL(windows[0].bytes[3*8+7], 0x80);
}

BOOST_AUTO_TEST_CASE(sequence_prefix_crosses_a_completed_window_boundary)
{
	std::vector<uint8_t> input(20,1);input[10]=input[12]=0;
	WindowTriggerStage high,low;high.high=1;low.low=1;
	std::vector<StreamWindows::Window> windows;
	StreamWindows stream(1,4,2,2,{high,low},[&](StreamWindows::Window&& w){windows.push_back(std::move(w));});
	stream.feed(input.data(),input.size());
	BOOST_REQUIRE_EQUAL(windows.size(),2);
	BOOST_CHECK_EQUAL(windows[0].origin,8);
	BOOST_CHECK_EQUAL(windows[1].origin,10);
}

BOOST_AUTO_TEST_CASE(level_and_multichannel_edge_stages)
{
	WindowTriggerStage stage;
	stage.rising=1; stage.falling=2; stage.high=4; stage.low=8;
	std::vector<uint8_t> input{2,2,2,2,5,5,5,5,5};
	std::vector<StreamWindows::Window> windows;
	StreamWindows stream(1, 5, 2, 1, {stage}, [&](StreamWindows::Window&& w) { windows.push_back(std::move(w)); });
	stream.feed(input.data(),input.size());
	BOOST_REQUIRE_EQUAL(windows.size(),1);
	BOOST_CHECK_EQUAL(windows[0].origin,2);
	BOOST_CHECK_EQUAL(windows[0].bytes[2],5);
}

BOOST_AUTO_TEST_CASE(warmup_is_not_fabricated_and_invalid_packets_fail)
{
	WindowTriggerStage t; t.falling=1;
	std::vector<StreamWindows::Window> windows;
	StreamWindows stream(1,8,5,2,{t},[&](StreamWindows::Window&& w){windows.push_back(std::move(w));});
	std::array<uint8_t,9> input{{1,0,1,1,1,1,1,1,1}};
	stream.feed(input.data(),input.size());stream.finish();
	BOOST_CHECK(windows.empty());
	WindowTriggerStage bad;bad.high=bad.low=1;
	BOOST_CHECK_THROW(StreamWindows(1,4,2,1,{bad},[](StreamWindows::Window&&){}),std::invalid_argument);
	BOOST_CHECK_THROW(StreamWindows(1,4,4,1,{t},[](StreamWindows::Window&&){}),std::invalid_argument);
	StreamWindows wide(4,8,3,1,{t},[](StreamWindows::Window&&){});
	BOOST_CHECK_THROW(wide.feed(input.data(),3),std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
