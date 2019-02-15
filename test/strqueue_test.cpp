/*
 * Tests using CppUnitLite unit test framework.
 *
 *
 When size is only 8:
   text    data     bss     dec     hex filename
  14532    4848     408   19788    4d4c strqueue_test.exe

 When a size is modified to 15 (but the rest are still 8):
  14836    4848     408   20092    4e7c strqueue_test.exe

 Note that the text is incremented.

 When a size is modified to 16 (but the rest are still 8):
  14772    4848     408   20028    4e3c strqueue_test.exe

 When size is a power of 2, the text size is smaller than when it's not.

 When a size is modified to 17 (but the rest are still 8):
  14836    4848     408   20092    4e7c strqueue_test.exe

 The result is the same as for size=15.

 When a size is modified to 32 (but the rest are still 8):
  14804    4848     408   20060    4e5c strqueue_test.exe

 It's smaller than the 15 and 17 cases, but larger than 16 case.

 Comparing the assembler generated for the 15 and 16 cases, we can see
 the 16 case is smaller because it has this for the "rem size" operation:

 andl	$15, %edx

 where the 15 case has more code.
 *
 */
#include <iostream>
#include "strqueue.h"
#include "gtest/gtest.h"

using namespace std;

class strqueue : public testing::Test
{
};

// Write one entry and read it
TEST_F(strqueue, write1)
{
    queue<int> q;

    int in = 43;
    q.enq(in);
    EXPECT_EQ(1, q.numItems());

    int out;
    q.deq(out);
    EXPECT_TRUE(out == 43);
    EXPECT_EQ(0, q.numItems());
}

// Write 2 entries, then read them
TEST_F(strqueue, write2)
{
    queue<int> q;

    int in = 43;
    q.enq(in);
    in = 2;
    q.enq(in);
    EXPECT_EQ(2, q.numItems());

    int out;
    q.deq(out);
    EXPECT_TRUE(out == 43);

    q.deq(out);
    EXPECT_TRUE(out == 2);
}


// Write an entry and read, then another
TEST_F(strqueue, write1and1)
{
    queue<int> q;

    int in = 43;
    q.enq(in);

    int out;
    q.deq(out);
    EXPECT_TRUE(out == 43);

    in = 7;
    q.enq(in);

    q.deq(out);
    EXPECT_TRUE(out == 7);
}


// Write until full, then read until empty.
// As many elements are read as were written.
TEST_F(strqueue, writeFull)
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
    EXPECT_EQ(8, q.numItems());

    int out;

    for (nOut = 0; q.deq(out) == true; nOut++)
    {
        EXPECT_EQ(5 + nOut, out);
    }

    EXPECT_EQ(0, q.numItems());
    EXPECT_TRUE(nOut == nIn);
    EXPECT_TRUE(nIn == 8);
}


// Write and read for a few cycles
TEST_F(strqueue, chase)
{
    static const unsigned qSize = 8;
    queue<int, qSize> q;
    unsigned n;

    for (n = 0; n < qSize * 3; n++)
    {
        int in = n;
        q.enq(in);
        EXPECT_EQ(1, q.numItems());

        int out;
        q.deq(out);
        EXPECT_EQ(0, q.numItems());
        EXPECT_TRUE(out == in);
    }
}


// Write until counter wraps.
// This test fails if qSize is not a power of 2, and counter_t is uint8_t (with 256
// being the number of writes to be done until the write index wraps).
// It demonstrates why qSize must be a power of 2 for this to work.
// Also qSize must be <= the maximum value that can be held in counter_t.
TEST_F(strqueue, wrap)
{
    static const unsigned qSize = 8;
    queue<int, qSize> q;
    unsigned n;
    int out;
    int in;

    // Write to queue, then read, leaving it empty each time
    // Do this until uint8_t is about to wrap.
    for (n = 0; n <= 256 - qSize; n++)
    {
        in = n;
        q.enq(in);

        q.deq(out);
        EXPECT_TRUE(out == in);
    }

    // Fill the queue
    for ( ; ; n++)
    {
        in = n;
        if (!q.enq(in))
            break;
    }

    // Read current contents
    unsigned nRead;
    int expected = 256 - qSize + 1; // the last write that's still in the queue

    for (nRead = 0; nRead<qSize; nRead++)
    {
        if (!q.deq(out))
        {
            break;
        }

        cout << "read from queue: " << out << endl;

        EXPECT_TRUE(out == expected);
        expected += 1;
    }

    cout << "nRead: " << nRead << "====================================================" << endl;

    EXPECT_TRUE(nRead == qSize);
}

