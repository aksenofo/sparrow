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
    ASSERT_EQ(buf.SizeToConsume(), 0);
    
    buf.Push("123");
    ASSERT_EQ(*buf.Ptr(), '1');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '2');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '3');
    buf.Consume(1);

    ASSERT_EQ(buf.SizeToConsume(), 0);
    buf.Push("456");
    ASSERT_EQ(*buf.Ptr(), '4');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '5');
    buf.Consume(1);
    ASSERT_EQ(*buf.Ptr(), '6');
    buf.Consume(1);
}