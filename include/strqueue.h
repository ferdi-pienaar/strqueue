/* Implement stream queue concept http://fgiesen.wordpress.com/2010/12/14/ring-buffers-and-queues/
 */

#pragma once

#include <stdint.h>  // uint8_t, etc
#include <math.h> // pow

namespace StreamQueue
{

// Used in compile-time assert.
constexpr bool is_powerof2(unsigned v)
{
    return v && ((v & (v - 1)) == 0);
}

// An advantage of the streams model is that it doesn't waste
// one buffer entry to distinguish between "full" and "empty".
// @param T - the type of the objects held in the queue, often a pointer
// @param size - the maximum number of elements the queue can hold
// Size is known at compile-time, and since it is a power of two, the compiler
// should be able to make efficient implementations of the modulus operation
// used in enq() and deq().
template <typename T, unsigned size = 8>
class queue
{
    static_assert(is_powerof2(size), "Size is a power of 2.");
public:
    // This unsigned type must be big enough to count up to the maximum number of elements in the queue,
    // given by the size parameter of this template class.  The most efficient (fastest)
    // implementation probably results when this is the natural word size of the processor.
    typedef uint8_t counter_t;
    static_assert(size <= pow(2, 8 * sizeof(counter_t)), "Size fits in counter_t.");

    queue() : readCnt(0), writeCnt(0) {}
    bool enq(const T & entry);
    bool deq(T & entry);
    counter_t numItems()
    {
        // Because the counters are unsigned, this is correct even
        // if counters have wrapped.
        return writeCnt - readCnt;
    }

private:

    bool full()
    {
        return numItems() == size;
    }

    bool empty()
    {
        return numItems() == 0;
    }

    T buf[size];

    counter_t readCnt;  // Number of reads
    counter_t writeCnt; // Number of writes
};


// Add an element to the tail of the queue
template <class T, unsigned size>
bool queue<T, size>::enq(const T & entry)
{
    if (full())
    {
        return false;
    }

    buf[writeCnt % size] = entry;
    writeCnt++;
    return true;
}


// Remove an element from head of queue, copying it to input param
template <class T, unsigned size>
bool queue<T, size>::deq(T & entry)
{
    if (empty())
    {
        return false;
    }

    entry = buf[readCnt % size];
    readCnt++;
    return true;
}

}
