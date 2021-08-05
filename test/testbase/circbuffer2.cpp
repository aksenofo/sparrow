/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <circlebuffer.h>
#include <gtest/gtest.h>

TEST(circular_buffer, test2)
{
    mchs::CircularBuffer buf(3);
	ASSERT_EQ(buf.Push("A", 1), 1);
	ASSERT_EQ(buf.FilledSize(), 1);
	ASSERT_EQ(buf.Push("B", 1), 1);
	ASSERT_EQ(buf.FilledSize(), 2);

 	ASSERT_EQ(memcmp(buf.Payload(), "AB", 2), 0);
	uint32_t v = buf.SizeToConsume();
	buf.Consume(1);
	ASSERT_EQ(buf.FilledSize(), 1);
	ASSERT_FALSE(buf.IsEmpty());
	buf.Consume(1);
	ASSERT_EQ(buf.FilledSize(), 0);
	ASSERT_TRUE(buf.IsEmpty());

	ASSERT_EQ(buf.Push("x", 1), 1);
	ASSERT_EQ(buf.FilledSize(), 1);
	ASSERT_FALSE(buf.IsEmpty());
	auto pl = buf.Payload();
	buf.Consume(1);
	ASSERT_EQ(buf.FilledSize(), 0);
	ASSERT_TRUE(buf.IsEmpty());
	
	ASSERT_EQ(buf.Tail(), buf.Head());

}