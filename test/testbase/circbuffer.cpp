
// Remove this file

#if 0


#include <iostream>
#include "circlebuffer.h"

extern void test00();
extern void test0();
extern void test1();
extern void test2();
extern void test3();
extern void test10();
extern void test20();

int main1() {
	test00();
	test0();
	test1();
	test2();
	test3();

	test10();
	test20();
}

void test20() {
	const size_t bufSize = 100000;
	std::unique_ptr<uint8_t[]> buf(new uint8_t [bufSize]);
	uint8_t* ptr = buf.get();

	uint8_t counter = 0;
	std::for_each(ptr, ptr + bufSize, [&](uint8_t& v){
		v = counter++;
	}); 

	mchs::CircularBuffer cbuf(bufSize / 11);

	size_t inside = 0;
	size_t step = bufSize / 11 / 7;
	uint8_t lastCounterValue = 0;
	uint32_t counterValue = 0;

	while(counterValue != bufSize) {
		size_t t = std::min(step, bufSize - inside);
		t = std::min(t, (size_t)cbuf.AvailableSize());
		if(t) {
			cbuf.Push(ptr + inside, t);
			inside += t;
		}

		size_t stepRead = step / 2 + 111 ;
		uint32_t av = std::min(cbuf.SizeToConsume(), (uint32_t)stepRead);
		if(av) {
			for(uint32_t t = 0; t < av; t++) {
				assert(lastCounterValue == cbuf.Ptr()[t]);

				lastCounterValue ++;
				counterValue++;
			}
			cbuf.Consume(av);
		}
	}
	assert(cbuf.IsEmpty());
}

void test10() {
	mchs::CircularBuffer buf(5);
	assert(buf.Push("ABCD", 4) == 4);
	assert(memcmp(buf.Ptr(), "ABC", 3) == 0);
	buf.Consume(2);
	assert(memcmp(buf.Ptr(), "CD", 2) == 0);
	buf.Consume(2);
	assert(buf.Push("NH", 2) == 2);

	assert(memcmp(buf.Ptr(), "N", 1) == 0);
	buf.Consume(1);
	assert(memcmp(buf.Ptr(), "H", 1) == 0);
	buf.Consume(1);
	assert(buf.IsEmpty());

	assert(buf.Push("12345", 5) == 5);
	uint32_t t = buf.SizeToConsume();
	buf.Consume(t);
	t = buf.SizeToConsume();
	buf.Consume(t);
	assert(buf.IsEmpty());

}

void test00() {
	mchs::CircularBuffer buf(3);
	assert(buf.Head() == buf.Buffer());
	assert(buf.Push("A", 1) == 1);
	assert(buf.Head() - buf.Buffer() == 1);
	assert(buf.Push("B", 1) == 1);
	assert(buf.Head() - buf.Buffer() == 2);
	assert(buf.Push("C", 1) == 1);
	assert(buf.Head() == buf.Buffer());
	assert(buf.IsFull());
	buf.Pop();
	assert(buf.Tail() - buf.Buffer() == 1);
	buf.Pop();
	assert(buf.Tail() - buf.Buffer() == 2);
	buf.Pop();
	assert(buf.Tail() == buf.Buffer());
	assert(buf.IsEmpty());

}

void test0() {
	mchs::CircularBuffer buf(3);
	assert(buf.Push("A", 1) == 1);
	assert(buf.FilledSize() == 1);
	assert(buf.SizeToConsume() == 1);
	assert(*buf.Ptr() == 'A');

	assert(buf.Push("B", 1) == 1);
	assert(buf.FilledSize() == 2);
	assert(buf.SizeToConsume() == 2);
	assert(buf.Ptr()[1] == 'B');


	assert(buf.Push("C", 1) == 1);
	assert(buf.FilledSize() == 3);
	assert(buf.SizeToConsume() == 3);
	assert(buf.Ptr()[2] == 'C');	

	assert(buf.IsFull());
	assert(buf.SizeToConsume() == 3);
	buf.Consume(1);
	assert(buf.SizeToConsume() == 2);
	assert(buf.FilledSize() == 2);
	
	assert(buf.Push("x", 1) == 1);
	assert(buf.SizeToConsume() == 2);
	assert(buf.FilledSize() == 3);

}


void test3() {
	mchs::CircularBuffer buf(3);
	assert(buf.Push("A", 1) == 1);
	assert(buf.FilledSize() == 1);
	assert(buf.Push("B", 1) == 1);
	assert(buf.FilledSize() == 2);
	assert(buf.Push("C", 1) == 1);
	assert(buf.FilledSize() == 3);

	buf.Consume(1);
	assert(buf.FilledSize() == 2);
	buf.Consume(1);
	assert(buf.FilledSize() == 1);
	buf.Consume(1);
	assert(buf.FilledSize() == 0);
	
	assert(buf.IsEmpty());

}

void test2() {
	mchs::CircularBuffer buf(3);
	assert(buf.Push("A", 1) == 1);
	assert(buf.FilledSize() == 1);
	assert(buf.Push("B", 1) == 1);
	assert(buf.FilledSize() == 2);

 	assert(memcmp(buf.Payload(), "AB", 2) == 0);
	uint32_t v = buf.SizeToConsume();
	buf.Consume(1);
	assert(buf.FilledSize() == 1);
	assert(!buf.IsEmpty());
	buf.Consume(1);
	assert(buf.FilledSize() == 0);
	assert(buf.IsEmpty());

	assert(buf.Push("x", 1) == 1);
	assert(buf.FilledSize() == 1);
	assert(!buf.IsEmpty());
	auto pl = buf.Payload();
	buf.Consume(1);
	assert(buf.FilledSize() == 0);
	assert(buf.IsEmpty());
	
	assert(buf.Tail() == buf.Head());

}

void test1() {

	mchs::CircularBuffer buf(3);

	assert(buf.Push("A", 1) == 1);
	assert(buf.Push("B", 1) == 1);
	assert(buf.Push("C", 1) == 1);
	assert(memcmp(buf.Payload(), "ABC", 3) == 0);

	assert(buf.Pop() == 'A');
	assert(buf.Push("N", 1) == 1);
	
	auto pl = buf.Payload();
	assert(memcmp(pl, "NBC", 3) == 0);

	assert(buf.Pop() == 'B');
	assert(buf.Push("X", 1) == 1);
	assert(memcmp(pl, "NXC", 3) == 0);
	assert(buf.Pop() == 'C');

	mchs::CircularBuffer buf6(6);
	auto pl6 = buf6.Payload();
	assert(buf6.Push("Aa", 2) == 2);
	assert(buf6.Push("Bb", 2) == 2);
	assert(buf6.Push("Cc", 2) == 2);
	assert(memcmp(buf6.Payload(), "AaBbCc", 6) == 0);
	buf6.Pop();
	assert(buf6.Push("X", 1) == 1);
	buf6.Pop();
	assert(buf6.Push("x", 1) == 1);
	assert(memcmp(buf6.Payload(), "XxBbCc", 6) == 0);

	mchs::CircularBuffer buf6_1(6);
	auto pl6_1 = buf6_1.Payload();
	assert(buf6_1.Push("Aa", 2) == 2);
	assert(buf6_1.Push("Bb", 2) == 2);
	buf6_1.Pop();
	assert(buf6_1.Push("Ccx", 3) == 3);
	assert(memcmp(buf6_1.Payload(), "xaBbCc", 6) == 0);

	mchs::CircularBuffer buf6_2(6);
	auto pl6_2 = buf6_2.Payload();
	assert(buf6_2.Push("Aa", 2) == 2);
	assert(buf6_2.Push("Bb", 2) == 2);
	buf6_2.Pop();
	assert(buf6_2.Push("Ccx", 3) == 3);
	buf6_2.Pop();
	buf6_2.Pop();
	assert(buf6_2.Push("12", 2) == 2);
	assert(memcmp(buf6_2.Payload(), "x12bCc", 6) == 0);
}


#endif 0
