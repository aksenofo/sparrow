/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"

#include "gtest/gtest.h"

TEST(circular_buffer, test1002)
{

    sparrow::CircularBuffer buf(10);

    sparrow::Populator populator;
    ASSERT_EQ(populator(buf, [&](void* ptr, uint32_t size) { return 5; }), 5);
    ASSERT_EQ(buf.PopulateSize(), 5);
    ASSERT_EQ(buf.Blocks(), 1);

    ASSERT_EQ(populator(buf, [&](void* ptr, uint32_t size) { return 4; }), 4);
    ASSERT_EQ(buf.PopulateSize(), 1);
    ASSERT_EQ(buf.Blocks(), 1);

    sparrow::Consumer consumer;
    ASSERT_EQ(consumer(buf, [&](void* ptr, uint32_t size) { return 1; }), 1);
    ASSERT_EQ(buf.Blocks(), 1);

    ASSERT_EQ(buf.PopulateSize(), 1);
    ASSERT_EQ(buf.AvailableSize(), 2);
    ASSERT_EQ(populator(buf, [&](void* ptr, uint32_t size) {
        return 1;
    }),
        2);
    ASSERT_EQ(buf.PopulateSize(), 0);
}

struct Distributor {
    Distributor(const char* str, size_t size)
    : m_str(str, size)
    , m_last(0)
    {
    }

    std::pair<uint8_t*, size_t> Get(size_t size)
    {
        auto p = std::make_pair((uint8_t*)m_str.c_str() + m_last, std::min(size, m_str.size() - m_last));
        m_last += p.second;
        return p;
    }

    void Reset()
    {
        m_last = 0;
    }


    uint32_t m_last;
    const std::string m_str;
};

struct Collector {
    Collector(size_t size)
    : m_size(size)
    {
    }

    uint32_t Set(const void* ptr, size_t size)
    {
        uint32_t len = std::min(size, m_size - m_str.size());
        m_str += std::string((const char*)ptr, len);
        return len;
    }

    void Reset()
    {
        m_str.clear();
    }

    const char* Ptr() const { return m_str.c_str(); }
    const uint32_t m_size;
    std::string m_str;
};

TEST(circular_buffer, test1002_1)
{
    sparrow::CircularBuffer buf(10);
    sparrow::Consumer consumer;
    const char* tmpbuff = "0123456789";

    Collector collector(10);
    for (size_t simulSize : { 1, 3, 5, 9 }) {
        for (size_t t = 0; t < 100; t++) {
            collector.Reset();
            buf.Put(tmpbuff, simulSize);

            consumer(buf, [&](uint8_t* ptr, size_t size) {
                size = std::min(size, simulSize);
                return collector.Set(ptr, size);
            });
            ASSERT_TRUE(strncmp(tmpbuff, collector.Ptr(), simulSize) == 0);
        }
    }
}

TEST(circular_buffer, test1002_2)
{
    sparrow::CircularBuffer buf(10);
    sparrow::Populator populator;
    const char* tmpbuff = "0123456789";
    char outBuff[10];

    for (size_t simulSize : { 3, 5, 9 }) {
        Distributor distributor(tmpbuff, simulSize);
        for (size_t t = 0; t < 100; t++) {
            distributor.Reset();
            buf.Put(tmpbuff, simulSize);

//            populator(buf, [&](uint8_t* ptr, size_t size) {
//                size = std::min(size, simulSize);
//                auto p = distributor.Get(size);
//                strncpy((char*)ptr, (const char*)p.first, p.second);
//                return p.second;
//            });

            buf.Get(outBuff, simulSize);
            if (strncmp(tmpbuff, outBuff, simulSize) != 0)
                ASSERT_TRUE(strncmp(tmpbuff, outBuff, simulSize) == 0);
        }
    }
}
