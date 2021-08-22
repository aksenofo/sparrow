/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"

#include "gtest/gtest.h"

TEST(circular_buffer, test10)
{
    sparrow::CircularBuffer buf(5);
    ASSERT_EQ(buf.Set("ABCD", 4), 4);
    ASSERT_EQ(memcmp(buf.Ptr(), "ABC", 3), 0);
    buf.Consume(2);
    ASSERT_EQ(memcmp(buf.Ptr(), "CD", 2), 0);
    buf.Consume(2);
    ASSERT_EQ(buf.Set("NH", 2), 2);

    ASSERT_EQ(memcmp(buf.Ptr(), "N", 1), 0);
    buf.Consume(1);
    ASSERT_EQ(memcmp(buf.Ptr(), "H", 1), 0);
    buf.Consume(1);
    ASSERT_TRUE(buf.IsEmpty());

    ASSERT_EQ(buf.Set("12345", 5), 5);
    uint32_t t = buf.ConsumeSize();
    buf.Consume(t);
    t = buf.ConsumeSize();
    buf.Consume(t);
    ASSERT_TRUE(buf.IsEmpty());
}