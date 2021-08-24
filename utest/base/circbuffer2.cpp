/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <circlebuffer.h>
#include <gtest/gtest.h>

TEST(circular_buffer, test2)
{
    sparrow::CircularBuffer buf(3);
    ASSERT_EQ(buf.Put("A", 1), 1);
    ASSERT_EQ(buf.FilledSize(), 1);
    ASSERT_EQ(buf.Put("B", 1), 1);
    ASSERT_EQ(buf.FilledSize(), 2);

    ASSERT_EQ(memcmp(buf.Buffer(), "AB", 2), 0);
    uint32_t v = buf.ConsumeSize();
    buf.Consume(1);
    ASSERT_EQ(buf.FilledSize(), 1);
    ASSERT_FALSE(buf.IsEmpty());
    buf.Consume(1);
    ASSERT_EQ(buf.FilledSize(), 0);
    ASSERT_TRUE(buf.IsEmpty());

    ASSERT_EQ(buf.Put("x", 1), 1);
    ASSERT_EQ(buf.FilledSize(), 1);
    ASSERT_FALSE(buf.IsEmpty());
    auto pl = buf.Buffer();
    buf.Consume(1);
    ASSERT_EQ(buf.FilledSize(), 0);
    ASSERT_TRUE(buf.IsEmpty());

    ASSERT_EQ(buf.Tail(), buf.Head());
}