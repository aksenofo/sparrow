#include "circlebuffer.h"
#include "gtest/gtest.h"

const uint32_t size = 1000;

TEST(circular_buffer, latecomer001)
{
    mchs::CircularBuffer buf(size + 1);

    uint8_t buffer[size];

    ASSERT_EQ(buf.Push(buffer, size), size);

    for(size_t ref = 0; ref < size; ref ++) {
        ASSERT_EQ(buf.PushLatecomer(ref, buffer, sizeof(buffer)), sizeof(buffer) - ref);
    }
    ASSERT_EQ(buf.Blocks(), 1);

    buf.Pop();
    buf.Pop();

    buf.Push("  ", 2);
    ASSERT_EQ(buf.Blocks(), 2);

    for(size_t ref = 0; ref < size; ref ++) {
        ASSERT_EQ(buf.PushLatecomer(ref, buffer, sizeof(buffer)), sizeof(buffer) - ref);
    }
}

