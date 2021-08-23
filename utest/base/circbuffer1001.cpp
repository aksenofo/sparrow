/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"

#include "gtest/gtest.h"

TEST(circular_buffer, test1001)
{
    
    sparrow::CircularBuffer buf(10);
    buf.Set("0123456789");

    sparrow::Consumer consumer;
    ASSERT_EQ(consumer(buf, [&](void* ptr, uint32_t size) { return 5; }), 5);
    ASSERT_EQ(buf.PopulateSize(), 5);

    ASSERT_EQ(consumer(buf, [&](void* ptr, uint32_t size) { return 1; }), 1);
    ASSERT_EQ(buf.PopulateSize(), 6);

    consumer(buf, [&](void* ptr, uint32_t size) { return 4; });
    ASSERT_EQ(buf.PopulateSize(), 10);
}

TEST(circular_buffer, test1001_1)
{

    sparrow::CircularBuffer buf(10);
    buf.Set("0123456789");

    sparrow::Consumer consumer;
    ASSERT_EQ(consumer(buf, [&](void* ptr, uint32_t size) { return 5; }), 5);
    ASSERT_EQ(buf.PopulateSize(), 5);

    ASSERT_EQ(consumer(buf, [&](void* ptr, uint32_t size) { return 1; }), 1);
    ASSERT_EQ(buf.PopulateSize(), 6);

    consumer(buf, [&](void* ptr, uint32_t size) { return 2; });
    ASSERT_EQ(buf.PopulateSize(), 8);

    buf.Set("01234567");
    ASSERT_EQ(consumer(buf, [&](void* ptr, uint32_t size) { return 2; }),
        4);
    ASSERT_EQ(buf.PopulateSize(), 2);

    consumer(buf, [&](void* ptr, uint32_t size) { return 4; });
    ASSERT_EQ(buf.PopulateSize(), 2);

    consumer(buf, [&](void* ptr, uint32_t size) { return 2; });
    ASSERT_EQ(buf.PopulateSize(), 2);

}
