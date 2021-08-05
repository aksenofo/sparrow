/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */


#include "circlebuffer.h"
#include "gtest/gtest.h"

TEST(circular_buffer, test1)
{
	mchs::CircularBuffer buf(3);

	ASSERT_EQ(buf.Push("A", 1), 1);
	ASSERT_EQ(buf.Push("B", 1), 1);
	ASSERT_EQ(buf.Push("C", 1), 1);
	ASSERT_EQ(memcmp(buf.Payload(), "ABC", 3), 0);

	ASSERT_EQ(buf.Pop(), 'A');
	ASSERT_EQ(buf.Push("N", 1), 1);
	
	auto pl = buf.Payload();
	ASSERT_EQ(memcmp(pl, "NBC", 3), 0);

	ASSERT_EQ(buf.Pop(), 'B');
	ASSERT_EQ(buf.Push("X", 1), 1);
	ASSERT_EQ(memcmp(pl, "NXC", 3), 0);
	ASSERT_EQ(buf.Pop(), 'C');

	mchs::CircularBuffer buf6(6);
	auto pl6 = buf6.Payload();
	ASSERT_EQ(buf6.Push("Aa", 2), 2);
	ASSERT_EQ(buf6.Push("Bb", 2), 2);
	ASSERT_EQ(buf6.Push("Cc", 2), 2);
	ASSERT_EQ(memcmp(buf6.Payload(), "AaBbCc", 6), 0);
	buf6.Pop();
	ASSERT_EQ(buf6.Push("X", 1), 1);
	buf6.Pop();
	ASSERT_EQ(buf6.Push("x", 1), 1);
	ASSERT_EQ(memcmp(buf6.Payload(), "XxBbCc", 6), 0);

        ASSERT_EQ(buf.Blocks(), 1);

	mchs::CircularBuffer buf6_1(6);
	auto pl6_1 = buf6_1.Payload();
	ASSERT_EQ(buf6_1.Push("Aa", 2), 2);
	ASSERT_EQ(buf6_1.Push("Bb", 2), 2);
	buf6_1.Pop();
	ASSERT_EQ(buf6_1.Push("Ccx", 3), 3);
	ASSERT_EQ(memcmp(buf6_1.Payload(), "xaBbCc", 6), 0);

	mchs::CircularBuffer buf6_2(6);
	auto pl6_2 = buf6_2.Payload();
	ASSERT_EQ(buf6_2.Push("Aa", 2), 2);
	ASSERT_EQ(buf6_2.Push("Bb", 2), 2);
	buf6_2.Pop();
	ASSERT_EQ(buf6_2.Push("Ccx", 3), 3);
	buf6_2.Pop();
	buf6_2.Pop();
	ASSERT_EQ(buf6_2.Push("12", 2), 2);
	ASSERT_EQ(memcmp(buf6_2.Payload(), "x12bCc", 6), 0);
}