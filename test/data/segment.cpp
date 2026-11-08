/*
 * This file is part of the PulseView project.
 *
 * Copyright (C) 2016 Soeren Apel <soeren@apelpie.net>
 * Copyright (C) 2013 Joel Holdsworth <joel@airwebreathe.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <extdef.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <pv/data/segment.hpp>

using pv::data::Segment;

BOOST_AUTO_TEST_SUITE(SegmentTest)

class CompletionSegment : public Segment
{
public:
	explicit CompletionSegment(unsigned int unit_size) :
		Segment(7, 1000000, unit_size) {}

	using Segment::append_samples;
	using Segment::get_raw_samples;
	using Segment::begin_sample_iteration;
	using Segment::continue_sample_iteration;
	using Segment::end_sample_iteration;
	using Segment::get_iterator_value;
};

BOOST_AUTO_TEST_CASE(CompletionAcrossChunks)
{
	// A 24-bit payload crossing the production chunk boundary exercises both
	// the full chunks and the last, partially filled chunk.
	const uint64_t count = 4 * 1024 * 1024;
	std::vector<uint8_t> input(count * 3), output(input.size());
	for (size_t i = 0; i < input.size(); ++i)
		input[i] = uint8_t(i * 37 + 11);

	CompletionSegment segment(3);
	segment.append_samples(input.data(), count);
	unsigned int notifications = 0;
	QObject::connect(&segment, &Segment::completed, [&] {
		++notifications;
		BOOST_CHECK(segment.is_complete());
		BOOST_CHECK_EQUAL(segment.get_sample_count(), count);
		segment.get_raw_samples(0, count, output.data());
		BOOST_CHECK(output == input);
	});
	segment.set_complete();
	BOOST_CHECK_EQUAL(notifications, 1);
	BOOST_CHECK_EQUAL(segment.unit_size(), 3);
	BOOST_CHECK_EQUAL(segment.samplerate(), 1000000);
	BOOST_CHECK_EQUAL(segment.segment_id(), 7);

	segment.free_unused_memory();
	segment.free_unused_memory();
	std::fill(output.begin(), output.end(), 0);
	segment.get_raw_samples(0, count, output.data());
	BOOST_CHECK(output == input);
}

BOOST_AUTO_TEST_CASE(CompletionWithHeldIterator)
{
	std::vector<uint32_t> input(513), output(input.size());
	for (size_t i = 0; i < input.size(); ++i)
		input[i] = uint32_t(i * 131) ^ UINT32_C(0xa5a55a5a);

	CompletionSegment segment(sizeof(uint32_t));
	segment.append_samples(input.data(), input.size());
	auto iterator = segment.begin_sample_iteration(0);
	segment.set_complete();
	segment.free_unused_memory();
	for (size_t i = 0; i < input.size(); ++i) {
		uint32_t value;
		std::memcpy(&value, segment.get_iterator_value(iterator), sizeof(value));
		BOOST_CHECK_EQUAL(value, input[i]);
		segment.continue_sample_iteration(iterator, 1);
	}
	segment.end_sample_iteration(iterator);
	segment.free_unused_memory();
	segment.get_raw_samples(0, input.size(),
		reinterpret_cast<uint8_t*>(output.data()));
	BOOST_CHECK(output == input);
	BOOST_CHECK(segment.is_complete());
}

BOOST_AUTO_TEST_CASE(CompletionEmpty)
{
	CompletionSegment segment(4);
	BOOST_CHECK_NO_THROW(segment.set_complete());
	BOOST_CHECK_NO_THROW(segment.free_unused_memory());
	BOOST_CHECK_NO_THROW(segment.free_unused_memory());
	BOOST_CHECK(segment.is_complete());
	BOOST_CHECK_EQUAL(segment.get_sample_count(), 0);
}


/* --- For debugging only
BOOST_AUTO_TEST_CASE(SmallSize8Single)
{
	Segment s(0, 1, sizeof(uint8_t));
	uint32_t num_samples = 10;

	//----- Chunk size << pv::data::Segment::MaxChunkSize @ 8bit, added in 1 call ----//
	uint8_t* const data = new uint8_t[num_samples];
	for (uint32_t i = 0; i < num_samples; i++)
		data[i] = i;

	s.append_samples(data, num_samples);
	delete[] data;

	BOOST_CHECK(s.get_sample_count() == num_samples);

	uint8_t *sample_data = new uint8_t[1];
	for (uint32_t i = 0; i < num_samples; i++) {
		s.get_raw_samples(i, 1, sample_data);
		BOOST_CHECK_EQUAL(*sample_data, i);
	}
	delete[] sample_data;
} */

/* --- For debugging only
BOOST_AUTO_TEST_CASE(MediumSize8Single)
{
	Segment s(0, 1, sizeof(uint8_t));
	uint32_t num_samples = pv::data::Segment::MaxChunkSize;

	//----- Chunk size == pv::data::Segment::MaxChunkSize @ 8bit, added in 1 call ----//
	uint8_t* const data = new uint8_t[num_samples];
	for (uint32_t i = 0; i < num_samples; i++)
		data[i] = i;

	s.append_samples(data, num_samples);
	delete[] data;

	BOOST_CHECK(s.get_sample_count() == num_samples);

	uint8_t *sample_data = new uint8_t[1];
	for (uint32_t i = 0; i < num_samples; i++) {
		s.get_raw_samples(i, 1, sample_data);
		BOOST_CHECK_EQUAL(*sample_data, i % 256);
	}
	delete[] sample_data;
} */

/* --- For debugging only
BOOST_AUTO_TEST_CASE(MaxSize8Single)
{
	Segment s(0, 1, sizeof(uint8_t));

	// We want to see proper behavior across chunk boundaries
	uint32_t num_samples = 2*pv::data::Segment::MaxChunkSize;

	//----- Chunk size >> pv::data::Segment::MaxChunkSize @ 8bit, added in 1 call ----//
	uint8_t* const data = new uint8_t[num_samples];
	for (uint32_t i = 0; i < num_samples; i++)
		data[i] = i;

	s.append_samples(data, num_samples);
	delete[] data;

	BOOST_CHECK(s.get_sample_count() == num_samples);

	uint8_t *sample_data = new uint8_t[1];
	for (uint32_t i = 0; i < num_samples; i++) {
		s.get_raw_samples(i, 1, sample_data);
		BOOST_CHECK_EQUAL(*sample_data, i % 256);
	}
	delete[] sample_data;
} */

/* --- For debugging only
BOOST_AUTO_TEST_CASE(MediumSize24Single)
{
	Segment s(0, 1, 3);

	// Chunk size is num*unit_size, so with pv::data::Segment::MaxChunkSize/unit_size, we reach the maximum size
	uint32_t num_samples = pv::data::Segment::MaxChunkSize / 3;

	//----- Chunk size == pv::data::Segment::MaxChunkSize @ 24bit, added in 1 call ----//
	uint8_t* const data = new uint8_t[num_samples * 3];
	for (uint32_t i = 0; i < num_samples * 3; i++)
		data[i] = i % 256;

	s.append_samples(data, num_samples);
	delete[] data;

	BOOST_CHECK(s.get_sample_count() == num_samples);

	uint8_t *sample_data = new uint8_t[3];
	for (uint32_t i = 0; i < num_samples; i++) {
		s.get_raw_samples(i, 1, sample_data);
		BOOST_CHECK_EQUAL(*((uint8_t*)sample_data),      3*i    % 256);
		BOOST_CHECK_EQUAL(*((uint8_t*)(sample_data+1)), (3*i+1) % 256);
		BOOST_CHECK_EQUAL(*((uint8_t*)(sample_data+2)), (3*i+2) % 256);
	}
	delete[] sample_data;
} */

/* --- For debugging only
BOOST_AUTO_TEST_CASE(MediumSize32Single)
{
	Segment s(0, 1, sizeof(uint32_t));

	// Chunk size is num*unit_size, so with pv::data::Segment::MaxChunkSize/unit_size, we reach the maximum size
	uint32_t num_samples = pv::data::Segment::MaxChunkSize / sizeof(uint32_t);

	//----- Chunk size == pv::data::Segment::MaxChunkSize @ 32bit, added in 1 call ----//
	uint32_t* const data = new uint32_t[num_samples];
	for (uint32_t i = 0; i < num_samples; i++)
		data[i] = i;

	s.append_samples(data, num_samples);
	delete[] data;

	BOOST_CHECK(s.get_sample_count() == num_samples);

	uint8_t *sample_data = new uint8_t[sizeof(uint32_t)];
	for (uint32_t i = 0; i < num_samples; i++) {
		s.get_raw_samples(i, 1, sample_data);
		BOOST_CHECK_EQUAL(*((uint32_t*)sample_data), i);
	}
	delete[] sample_data;
} */

/* --- For debugging only
BOOST_AUTO_TEST_CASE(MaxSize32Single)
{
	Segment s(0, 1, sizeof(uint32_t));

	// Chunk size is num*unit_size, so with pv::data::Segment::MaxChunkSize/unit_size, we reach the maximum size
	// Also, we want to see proper behavior across chunk boundaries
	uint32_t num_samples = 2*(pv::data::Segment::MaxChunkSize / sizeof(uint32_t));

	//----- Chunk size >> pv::data::Segment::MaxChunkSize @ 32bit, added in 1 call ----//
	uint32_t* const data = new uint32_t[num_samples];
	for (uint32_t i = 0; i < num_samples; i++)
		data[i] = i;

	s.append_samples(data, num_samples);
	delete[] data;

	BOOST_CHECK(s.get_sample_count() == num_samples);

	uint8_t *sample_data = new uint8_t[sizeof(uint32_t)];
	for (uint32_t i = 0; i < num_samples; i++) {
		s.get_raw_samples(i, 1, sample_data);
		BOOST_CHECK_EQUAL(*((uint32_t*)sample_data), i);
	}
	delete[] sample_data;
} */

/* --- For debugging only
BOOST_AUTO_TEST_CASE(MediumSize32Multi)
{
	Segment s(0, 1, sizeof(uint32_t));

	// Chunk size is num*unit_size, so with pv::data::Segment::MaxChunkSize/unit_size, we reach the maximum size
	uint32_t num_samples = pv::data::Segment::MaxChunkSize / sizeof(uint32_t);

	//----- Chunk size == pv::data::Segment::MaxChunkSize @ 32bit, added in num_samples calls ----//
	uint32_t data;
	for (uint32_t i = 0; i < num_samples; i++) {
		data = i;
		s.append_samples(&data, 1);
	}

	BOOST_CHECK(s.get_sample_count() == num_samples);

	uint8_t *sample_data = new uint8_t[sizeof(uint32_t)];
	for (uint32_t i = 0; i < num_samples; i++) {
		s.get_raw_samples(i, 1, sample_data);
		BOOST_CHECK_EQUAL(*((uint32_t*)sample_data), i);
	}
	delete[] sample_data;
} */

BOOST_AUTO_TEST_CASE(MaxSize32Multi)
{
	Segment s(0, 1, sizeof(uint32_t));

	// Chunk size is num*unit_size, so with pv::data::Segment::MaxChunkSize/unit_size, we reach the maximum size
	uint32_t num_samples = 2*(pv::data::Segment::MaxChunkSize / sizeof(uint32_t));

	//----- Chunk size == pv::data::Segment::MaxChunkSize @ 32bit, added in num_samples calls ----//
	uint32_t data;
	for (uint32_t i = 0; i < num_samples; i++) {
		data = i;
		s.append_samples(&data, 1);
	}

	BOOST_CHECK(s.get_sample_count() == num_samples);

	uint8_t *sample_data = new uint8_t[sizeof(uint32_t) * num_samples];
	for (uint32_t i = 0; i < num_samples; i++) {
		s.get_raw_samples(i, 1, sample_data);
		BOOST_CHECK_EQUAL(*((uint32_t*)sample_data), i);
	}

	s.get_raw_samples(0, num_samples, sample_data);
	for (uint32_t i = 0; i < num_samples; i++) {
		BOOST_CHECK_EQUAL(*((uint32_t*)(sample_data + i * sizeof(uint32_t))), i);
	}
	delete[] sample_data;
}

BOOST_AUTO_TEST_CASE(MaxSize32MultiAtOnce)
{
	Segment s(0, 1, sizeof(uint32_t));

	// Chunk size is num*unit_size, so with pv::data::Segment::MaxChunkSize/unit_size, we reach the maximum size
	uint32_t num_samples = 3*(pv::data::Segment::MaxChunkSize / sizeof(uint32_t));

	//----- Add all samples, requiring multiple chunks, in one call ----//
	uint32_t *data = new uint32_t[num_samples];
	for (uint32_t i = 0; i < num_samples; i++)
		data[i] = i;

	s.append_samples(data, num_samples);
	delete[] data;

	BOOST_CHECK(s.get_sample_count() == num_samples);

	uint8_t *sample_data = new uint8_t[sizeof(uint32_t) * num_samples];
	for (uint32_t i = 0; i < num_samples; i++) {
		s.get_raw_samples(i, 1, sample_data);
		BOOST_CHECK_EQUAL(*((uint32_t*)sample_data), i);
	}

	s.get_raw_samples(0, num_samples, sample_data);
	for (uint32_t i = 0; i < num_samples; i++) {
		BOOST_CHECK_EQUAL(*((uint32_t*)(sample_data + i * sizeof(uint32_t))), i);
	}
	delete[] sample_data;
}

BOOST_AUTO_TEST_CASE(MaxSize32MultiIterated)
{
	Segment s(0, 1, sizeof(uint32_t));

	// Chunk size is num*unit_size, so with pv::data::Segment::MaxChunkSize/unit_size, we reach the maximum size
	uint32_t num_samples = 2*(pv::data::Segment::MaxChunkSize / sizeof(uint32_t));

	//----- Chunk size == pv::data::Segment::MaxChunkSize @ 32bit, added in num_samples calls ----//
	uint32_t data;
	for (uint32_t i = 0; i < num_samples; i++) {
		data = i;
		s.append_samples(&data, 1);
	}

	BOOST_CHECK(s.get_sample_count() == num_samples);

	pv::data::SegmentDataIterator* it = s.begin_sample_iteration(0);

	for (uint32_t i = 0; i < num_samples; i++) {
		BOOST_CHECK_EQUAL(*((uint32_t*)s.get_iterator_value(it)), i);
		s.continue_sample_iteration(it, 1);
	}

	s.end_sample_iteration(it);
}

BOOST_AUTO_TEST_SUITE_END()
