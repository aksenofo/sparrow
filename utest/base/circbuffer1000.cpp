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

    buf.Put("123");
    ASSERT_EQ(*buf.Ptr(), '1');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '2');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '3');
    buf.Consume(1);

    ASSERT_EQ(buf.ConsumeSize(), 0);
    buf.Put("456");
    ASSERT_EQ(*buf.Ptr(), '4');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '5');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '6');
    buf.Consume(1);
}

TEST(circular_buffer, test1000_1)
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


TEST(circular_buffer, test1000_2)
{
    sparrow::CircularBuffer buf(10);
    buf.Put("0123456789");
    ASSERT_EQ(buf.PopulateSize(), 0);
    buf.Consume(8);
    ASSERT_EQ(buf.PopulateSize(), 8);
    buf.Put("QWERTYUI");
    ASSERT_EQ(buf.PopulateSize(), 0);
    buf.Consume(2);
    ASSERT_EQ(buf.PopulateSize(), 2);
    ASSERT_EQ(*buf.Ptr(), 'Q');

    buf.Consume(2);
    ASSERT_EQ(*buf.Ptr(), 'E');
    ASSERT_EQ(buf.PopulateSize(), 2);
}

TEST(circular_buffer, test1000_3)
{
    sparrow::CircularBuffer buf(10);
    buf.Populate(5);
    ASSERT_EQ(buf.ConsumeSize(), 5);
    buf.Populate(4);
    ASSERT_EQ(buf.ConsumeSize(), 9);
    buf.Consume(1);

    ASSERT_EQ(buf.Put("01xxxxxxxxxxxxxx"), 2);
    ASSERT_EQ(buf.ConsumeSize(), 9);
    buf.Consume(9);
    ASSERT_EQ(buf.ConsumeSize(), 1);
}

TEST(circular_buffer, test1000_4)
{
    sparrow::CircularBuffer buf(10);
    char v = 10;
    char tv = 0;
    for (size_t t = 0; t < 100; t++) {
        buf.Put(&v, 1);
        buf.Get(&tv, 1);
        ASSERT_EQ(v, tv);
        v++;
    }
}

TEST(circular_buffer, test1000_5)
{
    sparrow::CircularBuffer buf(10);
    char v[3] = { 1, 2, 3 };
    char tv[3];
    for (size_t t = 0; t < 100; t++) {
        buf.Put(v, 3);
        buf.Get(tv, 3);
        ASSERT_EQ(memcmp(v, tv, 3), 0);
        v[0] ++;
        v[1] ++;
        v[2] ++;
    }
}
