/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"
#include "gtest/gtest.h"

static void fill4(mchs::CircularBuffer& buf) {
    buf = mchs::CircularBuffer(buf.Size());
    buf.Push("A", 1);
    buf.Push("B", 1);
    buf.Push("C", 1);
    buf.Push("D", 1);
    buf.Consume(2);
    buf.Push("E", 1);
    buf.Push("F", 1);
}            

static void fill3(mchs::CircularBuffer& buf) {
    buf = mchs::CircularBuffer(buf.Size());
    buf.Push("A", 1);
    buf.Push("B", 1);
    buf.Push("C", 1);
    buf.Push("D", 1);
    buf.Consume(2);
    buf.Push("E", 1);
}

TEST(circular_buffer, latecomer_simple_2)
{
        mchs::CircularBuffer buf(4);
        fill4(buf);
        uint8_t ptr[10];
        uint32_t rc;

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 4);

        fill4(buf);
        rc = buf.PushLatecomer(0, (uint8_t*)"X", 1);
        ASSERT_EQ(rc, 1);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 4);
        ASSERT_EQ(ptr[0], 'X');

        fill4(buf);
        rc = buf.PushLatecomer(1, (uint8_t*)"Y", 1);
        ASSERT_EQ(rc, 1);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 4);
        ASSERT_EQ(ptr[1], 'Y');

        fill4(buf);
        rc = buf.PushLatecomer(2, (uint8_t*)"Y", 1);
        ASSERT_EQ(rc, 1);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 4);
        ASSERT_EQ(ptr[2], 'Y');

        fill4(buf);
        rc = buf.PushLatecomer(4, (uint8_t*)"Y", 1);
        ASSERT_EQ(rc, 0);


        fill4(buf);
        rc = buf.PushLatecomer(0, (uint8_t*)"XY", 2);
        ASSERT_EQ(rc, 2);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 4);
        ASSERT_EQ(ptr[0], 'X');
        ASSERT_EQ(ptr[1], 'Y');


        fill4(buf);
        rc = buf.PushLatecomer(1, (uint8_t*)"XY", 2);
        ASSERT_EQ(rc, 2);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 4);
        ASSERT_EQ(ptr[1], 'X');
        ASSERT_EQ(ptr[2], 'Y');

        fill4(buf);
        rc = buf.PushLatecomer(3, (uint8_t*)"XY", 2);
        ASSERT_EQ(rc, 1);
        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(ptr[3], 'X');

        fill4(buf);
        rc = buf.PushLatecomer(0, (uint8_t*)"XYZ", 3);
        ASSERT_EQ(rc, 3);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 4);
        ASSERT_EQ(ptr[0], 'X');
        ASSERT_EQ(ptr[1], 'Y');
        ASSERT_EQ(ptr[2], 'Z');

        fill3(buf);
        rc = buf.PushLatecomer(1, (uint8_t*)"XYZ", 3);
        ASSERT_EQ(rc, 2);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);
        ASSERT_EQ(ptr[1], 'X');
        ASSERT_EQ(ptr[2], 'Y');

        fill3(buf);
        rc = buf.PushLatecomer(2, (uint8_t*)"XYZ", 3);
        ASSERT_EQ(rc, 1);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);
        ASSERT_EQ(ptr[2], 'X');

        fill3(buf);
        rc = buf.PushLatecomer(3, (uint8_t*)"XYZ", 3);
        ASSERT_EQ(rc, 0);


}

TEST(circular_buffer, latecomer_simple_1)
{
        mchs::CircularBuffer buf(4);
        fill3(buf);
        uint8_t ptr[10];
        uint32_t rc;

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);

        fill3(buf);
        rc = buf.PushLatecomer(0, (uint8_t*)"X", 1);
        ASSERT_EQ(rc, 1);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);
        ASSERT_EQ(ptr[0], 'X');

        fill3(buf);
        rc = buf.PushLatecomer(1, (uint8_t*)"Y", 1);
        ASSERT_EQ(rc, 1);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);
        ASSERT_EQ(ptr[1], 'Y');

        fill3(buf);
        rc = buf.PushLatecomer(2, (uint8_t*)"Y", 1);
        ASSERT_EQ(rc, 1);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);
        ASSERT_EQ(ptr[2], 'Y');

        fill3(buf);
        rc = buf.PushLatecomer(3, (uint8_t*)"Y", 1);
        ASSERT_EQ(rc, 0);


        fill3(buf);
        rc = buf.PushLatecomer(0, (uint8_t*)"XY", 2);
        ASSERT_EQ(rc, 2);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);
        ASSERT_EQ(ptr[0], 'X');
        ASSERT_EQ(ptr[1], 'Y');


        fill3(buf);
        rc = buf.PushLatecomer(1, (uint8_t*)"XY", 2);
        ASSERT_EQ(rc, 2);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);
        ASSERT_EQ(ptr[1], 'X');
        ASSERT_EQ(ptr[2], 'Y');

        fill3(buf);
        rc = buf.PushLatecomer(2, (uint8_t*)"XY", 2);
        ASSERT_EQ(rc, 1);
        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(ptr[2], 'X');

        fill3(buf);
        rc = buf.PushLatecomer(0, (uint8_t*)"XYZ", 3);
        ASSERT_EQ(rc, 3);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);
        ASSERT_EQ(ptr[0], 'X');
        ASSERT_EQ(ptr[1], 'Y');
        ASSERT_EQ(ptr[2], 'Z');

        fill3(buf);
        rc = buf.PushLatecomer(1, (uint8_t*)"XYZ", 3);
        ASSERT_EQ(rc, 2);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);
        ASSERT_EQ(ptr[1], 'X');
        ASSERT_EQ(ptr[2], 'Y');

        fill3(buf);
        rc = buf.PushLatecomer(2, (uint8_t*)"XYZ", 3);
        ASSERT_EQ(rc, 1);

        rc = buf.Pop(ptr, sizeof(ptr));
        ASSERT_EQ(rc, 3);
        ASSERT_EQ(ptr[2], 'X');

        fill3(buf);
        rc = buf.PushLatecomer(3, (uint8_t*)"XYZ", 3);
        ASSERT_EQ(rc, 0);

}

TEST(circular_buffer, latecomer_sinple)
{
	mchs::CircularBuffer buf(4);
        ASSERT_TRUE(buf.Head() == buf.Tail() && buf.IsEmpty());

	ASSERT_EQ(buf.Push("A", 1), 1);
        ASSERT_TRUE(buf.Head() > buf.Tail() && !buf.IsFull() && !buf.IsEmpty());
	ASSERT_EQ(buf.FilledSize(), 1);

	ASSERT_EQ(buf.Push("B", 1), 1);
        ASSERT_TRUE(buf.Head() > buf.Tail() && !buf.IsFull() && !buf.IsEmpty());
	ASSERT_EQ(buf.FilledSize(), 2);

	ASSERT_EQ(buf.Push("C", 1), 1);
	ASSERT_EQ(buf.FilledSize(), 3);
        ASSERT_TRUE(buf.Head() > buf.Tail() && !buf.IsFull() && !buf.IsEmpty());

        ASSERT_EQ('A', *(buf.Tail() + 0));
	ASSERT_EQ('B', *(buf.Tail() + 1));
	ASSERT_EQ('C', *(buf.Tail() + 2));

        uint32_t rc;

        rc = buf.PushLatecomer(0, (uint8_t*)"X", 1);
        ASSERT_EQ('X', *(buf.Tail() + 2));
        ASSERT_EQ(rc, 1);

        rc = buf.PushLatecomer(1, (uint8_t*)"Y", 1);
        ASSERT_EQ('Y', *(buf.Tail() + 1));
	ASSERT_EQ(rc, 1);

        rc = buf.PushLatecomer(2, (uint8_t*)"Z", 1);
        ASSERT_EQ('Z', *(buf.Tail() + 0));
	ASSERT_EQ(rc, 1);

        rc = buf.PushLatecomer(3, (uint8_t*)"0", 1);
        ASSERT_EQ(rc, 0);

        rc = buf.PushLatecomer(0, (uint8_t*)"12", 2);
        ASSERT_EQ('1', *(buf.Tail() + 1));
        ASSERT_EQ('2', *(buf.Tail() + 2));
        ASSERT_EQ(rc, 2);

        rc = buf.PushLatecomer(1, (uint8_t*)"34", 2);
        ASSERT_EQ('3', *(buf.Tail() + 0));
        ASSERT_EQ('4', *(buf.Tail() + 1));
        ASSERT_EQ(rc, 2);

        rc = buf.PushLatecomer(2, (uint8_t*)"56", 2);
        ASSERT_EQ('5', *(buf.Tail() + 0));
        ASSERT_EQ(rc, 1);

        rc = buf.PushLatecomer(0, (uint8_t*)"asdf", 4);
        ASSERT_EQ('a', *(buf.Tail() + 0));
        ASSERT_EQ('s', *(buf.Tail() + 1));
	ASSERT_EQ('d', *(buf.Tail() + 2));

        ASSERT_EQ(rc, 3);

        rc = buf.PushLatecomer(1, (uint8_t*)"asdf", 4);
        ASSERT_EQ('a', *(buf.Tail() + 0));
        ASSERT_EQ('s', *(buf.Tail() + 1));

        ASSERT_EQ(rc, 2);

}

TEST(circular_buffer, latecomer_simple_error_params)
{
        mchs::CircularBuffer buf(4);
        ASSERT_EQ(buf.Push("A", 1), 1);
        ASSERT_EQ(buf.Push("B", 1), 1);
        ASSERT_EQ(buf.Push("C", 1), 1);
        ASSERT_EQ(buf.PushLatecomer((uint32_t)-1, (uint8_t*)"asdf", 4), 0);

}
