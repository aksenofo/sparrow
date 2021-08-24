/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */


#include "circlebuffer.h"

#include "gtest/gtest.h"

TEST(circular_buffer, test1)
{
    sparrow::CircularBuffer buf(3);

    ASSERT_EQ(buf.Put("A", 1), 1);
    ASSERT_EQ(buf.Put("B", 1), 1);
    ASSERT_EQ(buf.Put("C", 1), 1);
    ASSERT_EQ(memcmp(buf.Buffer(), "ABC", 3), 0);

    ASSERT_EQ(buf.Get(), 'A');
    ASSERT_EQ(buf.Put("N", 1), 1);

    auto pl = buf.Buffer();
    ASSERT_EQ(memcmp(pl, "NBC", 3), 0);

    ASSERT_EQ(buf.Get(), 'B');
    ASSERT_EQ(buf.Put("X", 1), 1);
    ASSERT_EQ(memcmp(pl, "NXC", 3), 0);
    ASSERT_EQ(buf.Get(), 'C');

    sparrow::CircularBuffer buf6(6);
    auto pl6 = buf6.Buffer();
    ASSERT_EQ(buf6.Put("Aa", 2), 2);
    ASSERT_EQ(buf6.Put("Bb", 2), 2);
    ASSERT_EQ(buf6.Put("Cc", 2), 2);
    ASSERT_EQ(memcmp(buf6.Buffer(), "AaBbCc", 6), 0);
    buf6.Get();
    ASSERT_EQ(buf6.Put("X", 1), 1);
    buf6.Get();
    ASSERT_EQ(buf6.Put("x", 1), 1);
    ASSERT_EQ(memcmp(buf6.Buffer(), "XxBbCc", 6), 0);

    ASSERT_EQ(buf.Blocks(), 1);

    sparrow::CircularBuffer buf6_1(6);
    auto pl6_1 = buf6_1.Buffer();
    ASSERT_EQ(buf6_1.Put("Aa", 2), 2);
    ASSERT_EQ(buf6_1.Put("Bb", 2), 2);
    buf6_1.Get();
    ASSERT_EQ(buf6_1.Put("Ccx", 3), 3);
    ASSERT_EQ(memcmp(buf6_1.Buffer(), "xaBbCc", 6), 0);

    sparrow::CircularBuffer buf6_2(6);
    auto pl6_2 = buf6_2.Buffer();
    ASSERT_EQ(buf6_2.Put("Aa", 2), 2);
    ASSERT_EQ(buf6_2.Put("Bb", 2), 2);
    buf6_2.Get();
    ASSERT_EQ(buf6_2.Put("Ccx", 3), 3);
    buf6_2.Get();
    buf6_2.Get();
    ASSERT_EQ(buf6_2.Put("12", 2), 2);
    ASSERT_EQ(memcmp(buf6_2.Buffer(), "x12bCc", 6), 0);
}