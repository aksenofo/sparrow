/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"

#include "gtest/gtest.h"

TEST(circular_buffer, test1000)
{
    sparrow::CircularBuffer buf(3);
    ASSERT_EQ(buf.ConsumeSize(), 0);

    buf.Set("123");
    ASSERT_EQ(*buf.Ptr(), '1');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '2');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '3');
    buf.Consume(1);

    ASSERT_EQ(buf.ConsumeSize(), 0);
    buf.Set("456");
    ASSERT_EQ(*buf.Ptr(), '4');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '5');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '6');
    buf.Consume(1);
}

TEST(circular_buffer, test1001)
{
    sparrow::CircularBuffer buf(3);
    ASSERT_EQ(buf.PopulateSize(), 3);
    buf.Populate(1);
    ASSERT_EQ(buf.PopulateSize(), 2);
    buf.Populate(1);
    ASSERT_EQ(buf.PopulateSize(), 1);
    buf.Populate(1);
    ASSERT_EQ(buf.PopulateSize(), 0);
    ASSERT_TRUE(buf.IsFull());
    buf.Consume(1);
    ASSERT_FALSE(buf.IsFull());
    ASSERT_EQ(buf.PopulateSize(), 1);
    buf.Consume(1);
    ASSERT_EQ(buf.PopulateSize(), 2);
    buf.Consume(1);
    ASSERT_TRUE(buf.IsEmpty());
    ASSERT_EQ(buf.PopulateSize(), 3);
    buf.Populate(2);
    ASSERT_FALSE(buf.IsFull());
    ASSERT_FALSE(buf.IsEmpty());
    ASSERT_EQ(buf.PopulateSize(), 1);
    buf.Populate(1);
    ASSERT_TRUE(buf.IsFull());
    ASSERT_FALSE(buf.IsEmpty());
    ASSERT_EQ(buf.PopulateSize(), 0);
}