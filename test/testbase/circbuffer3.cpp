/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"
#include "gtest/gtest.h"

TEST(circular_buffer, test3)
{
	sparrow::CircularBuffer buf(3);
	ASSERT_EQ(buf.Push("A", 1), 1);
	ASSERT_EQ(buf.FilledSize(), 1);
	ASSERT_EQ(buf.Push("B", 1), 1);
	ASSERT_EQ(buf.FilledSize(), 2);
	ASSERT_EQ(buf.Push("C", 1), 1);
	ASSERT_EQ(buf.FilledSize(), 3);

	buf.Consume(1);
	ASSERT_EQ(buf.FilledSize(), 2);
	buf.Consume(1);
	ASSERT_EQ(buf.FilledSize(), 1);
	buf.Consume(1);
	ASSERT_EQ(buf.FilledSize(), 0);
	
	ASSERT_TRUE(buf.IsEmpty());

}