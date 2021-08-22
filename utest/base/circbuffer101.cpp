/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"

#include "gtest/gtest.h"

static uint32_t size = 1000;

void Erase1(sparrow::CircularBuffer& buf)
{
    buf = sparrow::CircularBuffer(size);

    uint8_t buffer[size / 100];
    memset(buffer, 0, sizeof(buffer));

    for (uint32_t t = 0; t < size / sizeof(buffer); t++) {
        uint32_t g = buf.Set(buffer, sizeof(buffer));
        ASSERT_EQ(g, sizeof(buffer));
    }
}

void Pop(sparrow::CircularBuffer& buf, uint32_t size)
{
    while (size--) {
        buf.Get();
    }
}

void Erase2(sparrow::CircularBuffer& buf)
{
    buf = sparrow::CircularBuffer(size);

    uint8_t buffer[size / 100];
    memset(buffer, 0, sizeof(buffer));

    for (uint32_t t = 0; t < size / sizeof(buffer); t++) {
        uint32_t g = buf.Set(buffer, sizeof(buffer));
        ASSERT_EQ(g, sizeof(buffer));
    }

    Pop(buf, size / 100);
    Pop(buf, size / 100);
    ASSERT_EQ(buf.Set(buffer, sizeof(buffer)), sizeof(buffer));
    ASSERT_EQ(buf.Set(buffer, sizeof(buffer)), sizeof(buffer));
}

TEST(circular_buffer, latecomer01)
{
    sparrow::CircularBuffer buf(size);
    Erase1(buf);

    uint8_t buffer[size / 100];
    uint8_t getBuffer[size / 100];

    for (size_t ref = 0; ref < size - size / 100; ref++) {

        Erase1(buf);
        for (uint32_t t = 0; t < sizeof(buffer); t++) {
            buffer[t] = (uint32_t)t;
        }

        ASSERT_EQ(buf.SetLatecomer(ref, buffer, sizeof(buffer)), sizeof(buffer));
        Pop(buf, ref);

        ASSERT_EQ(buf.Get(getBuffer, sizeof(getBuffer)), sizeof(getBuffer));
        for (uint32_t t = 0; t < sizeof(buffer); t++) {
            ASSERT_EQ(getBuffer[t], buffer[t]);
        }
    }
}

TEST(circular_buffer, latecomer02)
{
    sparrow::CircularBuffer buf(size);
    Erase2(buf);

    uint8_t buffer[size / 100];
    uint8_t getBuffer[size / 100];

    for (size_t ref = 0; ref < size - size / 100; ref++) {

        Erase2(buf);
        for (uint32_t t = 0; t < sizeof(buffer); t++) {
            buffer[t] = (uint32_t)t;
        }

        ASSERT_EQ(buf.SetLatecomer(ref, buffer, sizeof(buffer)), sizeof(buffer));
        Pop(buf, ref);

        ASSERT_EQ(buf.Get(getBuffer, sizeof(getBuffer)), sizeof(getBuffer));
        for (uint32_t t = 0; t < sizeof(buffer); t++) {
            ASSERT_EQ(getBuffer[t], buffer[t]);
        }
    }
}
