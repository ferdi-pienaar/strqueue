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
    queue<int> q;

    int in = 43;
    q.enq(in);

    int out;
    q.deq(out);
    CHECK(out == 43);
}

// Write 2 entries, then read them
TEST(write2, strqueue)
{
    queue<int> q;

    int in = 43;
    q.enq(in);
    in = 2;
    q.enq(in);

    int out;
    q.deq(out);
    CHECK(out == 43);

    q.deq(out);
    CHECK(out == 2);
}


// Write an entry and read, then another
TEST(write1and1, strqueue)
{
    queue<int> q;

    int in = 43;
    q.enq(in);

    int out;
    q.deq(out);
    CHECK(out == 43);

    in = 7;
    q.enq(in);

    q.deq(out);
    CHECK(out == 7);
}


// Write until full, then read until empty.
// As many elements are read as were written.
TEST(writeFull, strqueue)
{
    queue<int> q;
    int in;
    unsigned nIn, nOut;

    for (nIn = 0; ; nIn++)
    {
        in = 5 + nIn;
        if (!q.enq(in))
        {
            break;
        }
    }

    int out;

    for (nOut = 0; q.deq(out) == true; nOut++)
    {
        CHECK(out == 5 + nOut);
    }

    CHECK(nOut == nIn);
    CHECK(nIn == 8);
}


// Write and read for a few cycles
TEST(chase, strqueue)
{
    static const unsigned qSize = 8;
    queue<int, qSize> q;
    unsigned n;

    for (n = 0; n < qSize * 3; n++)
    {
        int in = n;
        q.enq(in);

        int out;
        q.deq(out);

        CHECK(out == in);
    }
}

