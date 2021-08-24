/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"

#include "gtest/gtest.h"

TEST(circular_buffer, test00)
{
    sparrow::CircularBuffer buf(3);
    ASSERT_EQ(buf.Head(), buf.Buffer());
    ASSERT_EQ(buf.Put("A", 1), 1);
    ASSERT_EQ(buf.Head() - buf.Buffer(), 1);
    ASSERT_EQ(buf.Put("B", 1), 1);
    ASSERT_EQ(buf.Head() - buf.Buffer(), 2);
    ASSERT_EQ(buf.Put("C", 1), 1);
    ASSERT_EQ(buf.Head(), buf.Buffer());
    ASSERT_TRUE(buf.IsFull());
    buf.Get();
    ASSERT_EQ(buf.Tail() - buf.Buffer(), 1);
    buf.Get();
    ASSERT_EQ(buf.Tail() - buf.Buffer(), 2);
    buf.Get();
    ASSERT_EQ(buf.Tail(), buf.Buffer());
    ASSERT_TRUE(buf.IsEmpty());
}