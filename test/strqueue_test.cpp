//
#include <iostream>
#include "strqueue.h"
#include "TestHarness.h"

using namespace std;


int main()
{
	TestResult tr;
	TestRegistry::runAllTests(tr);
	return 0;
}


// Write one entry and read it
TEST(write1, strqueue)
{
    queue q;

    queue::t_element in = 43;
    q.enq(in);

    queue::t_element out;
    q.deq(out);
    CHECK(out == 43);
}


// Write 2 entries, then read them
TEST(write2, strqueue)
{
    queue q;

    queue::t_element in = 43;
    q.enq(in);
    in = 2;
    q.enq(in);

    queue::t_element out;
    q.deq(out);
    CHECK(out == 43);

    q.deq(out);
    CHECK(out == 2);
}


// Write an entry and read, then another
TEST(write1and1, strqueue)
{
    queue q;

    queue::t_element in = 43;
    q.enq(in);

    queue::t_element out;
    q.deq(out);
    CHECK(out == 43);

    in = 7;
    q.enq(in);

    q.deq(out);
    CHECK(out == 7);
}


