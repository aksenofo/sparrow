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
    ASSERT_EQ(buf.Push("A", 1), 1);
    ASSERT_EQ(buf.Head() - buf.Buffer(), 1);
    ASSERT_EQ(buf.Push("B", 1), 1);
    ASSERT_EQ(buf.Head() - buf.Buffer(), 2);
    ASSERT_EQ(buf.Push("C", 1), 1);
    ASSERT_EQ(buf.Head(), buf.Buffer());
    ASSERT_TRUE(buf.IsFull());
    buf.Pop();
    ASSERT_EQ(buf.Tail() - buf.Buffer(), 1);
    buf.Pop();
    ASSERT_EQ(buf.Tail() - buf.Buffer(), 2);
    buf.Pop();
    ASSERT_EQ(buf.Tail(), buf.Buffer());
    ASSERT_TRUE(buf.IsEmpty());
}