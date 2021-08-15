/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"

#include "gtest/gtest.h"

TEST(circular_buffer, test20)
{
    const size_t bufSize = 100000;
    std::unique_ptr<uint8_t[]> buf(new uint8_t[bufSize]);
    uint8_t* ptr = buf.get();

    uint8_t counter = 0;
    std::for_each(ptr, ptr + bufSize, [&](uint8_t& v) {
        v = counter++;
    });

    sparrow::CircularBuffer cbuf(bufSize / 11);

    size_t inside = 0;
    size_t step = bufSize / 11 / 7;
    uint8_t lastCounterValue = 0;
    uint32_t counterValue = 0;

    while (counterValue != bufSize) {
        size_t t = std::min(step, bufSize - inside);
        t = std::min(t, (size_t)cbuf.AvailableSize());
        if (t) {
            cbuf.Push(ptr + inside, t);
            inside += t;
        }

        size_t stepRead = step / 2 + 111; // Just same randomizing
        uint32_t av = std::min(cbuf.SizeToConsume(), (uint32_t)stepRead);
        if (av) {
            for (uint32_t t = 0; t < av; t++) {
                ASSERT_EQ(lastCounterValue, cbuf.Ptr()[t]);

                lastCounterValue++;
                counterValue++;
            }
            cbuf.Consume(av);
        }
    }
    ASSERT_TRUE(cbuf.IsEmpty());
}