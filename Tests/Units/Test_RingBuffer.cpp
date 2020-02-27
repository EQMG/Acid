#include <gtest/gtest.h>

#include <Helpers/RingBuffer.hpp>

TEST(RingBuffer, pushPopResize) {
	acid::RingBuffer<int32_t> buffer(4);

	EXPECT_TRUE(buffer.push(1, 2, 3, 4));

	buffer[1] = 11;
	EXPECT_FALSE(buffer.front() != 1);

	buffer.pop();
	EXPECT_FALSE(buffer.front() != 11);

	buffer.pop();
	EXPECT_FALSE(buffer.front() != 3);

	buffer.pop();
	
	EXPECT_TRUE(buffer.push(5));

	buffer.resize(2);
	EXPECT_FALSE(buffer.front() != 4);

	buffer.pop();
	EXPECT_FALSE(buffer.front() != 5);

	buffer.pop();
	EXPECT_FALSE(buffer.size() != 0);
}
