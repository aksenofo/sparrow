/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"

#include "gtest/gtest.h"

TEST(circular_buffer, test0)
{
    sparrow::CircularBuffer buf(3);
    ASSERT_EQ(buf.Push("A", 1), 1);
    ASSERT_EQ(buf.FilledSize(), 1);
    ASSERT_EQ(buf.ConsumeSize(), 1);
    ASSERT_EQ(*buf.Ptr(), 'A');

    ASSERT_EQ(buf.Push("B", 1), 1);
    ASSERT_EQ(buf.FilledSize(), 2);
    ASSERT_EQ(buf.ConsumeSize(), 2);
    ASSERT_EQ(buf.Ptr()[1], 'B');

    ASSERT_EQ(buf.Blocks(), 1);

    ASSERT_EQ(buf.Push("C", 1), 1);
    ASSERT_EQ(buf.FilledSize(), 3);
    ASSERT_EQ(buf.ConsumeSize(), 3);
    ASSERT_EQ(buf.Ptr()[2], 'C');

    ASSERT_EQ(buf.Blocks(), 2);

    ASSERT_TRUE(buf.IsFull());
    ASSERT_EQ(buf.ConsumeSize(), 3);
    buf.Consume(1);
    ASSERT_EQ(buf.ConsumeSize(), 2);
    ASSERT_EQ(buf.FilledSize(), 2);

    ASSERT_EQ(buf.Push("x", 1), 1);
    ASSERT_EQ(buf.ConsumeSize(), 2);
    ASSERT_EQ(buf.FilledSize(), 3);
}