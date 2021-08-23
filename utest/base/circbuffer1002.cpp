/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"

#include "gtest/gtest.h"

TEST(circular_buffer, test1002)
{
    sparrow::CircularBuffer buf(10);

    sparrow::Populator populator;
    ASSERT_EQ(populator(buf, [&](uint32_t size) { return 5; }), 5);
    ASSERT_EQ(buf.PopulateSize(), 5);
    ASSERT_EQ(buf.Blocks(), 1);

    ASSERT_EQ(populator(buf, [&](uint32_t size) { return 4; }), 4);
    ASSERT_EQ(buf.PopulateSize(), 1);
    ASSERT_EQ(buf.Blocks(), 1);

    sparrow::Consumer consumer;
    ASSERT_EQ(consumer(buf, [&](uint32_t size) { return 1; }), 1);
    ASSERT_EQ(buf.Blocks(), 1);

    ASSERT_EQ(buf.PopulateSize(), 1);

    ASSERT_EQ(populator(buf, [&](uint32_t size) { return 2; }), 2);
    ASSERT_EQ(buf.PopulateSize(), 0);
}
