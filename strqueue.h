/* 
 * Try the following:
 * Use stream queue concept http://fgiesen.wordpress.com/2010/12/14/ring-buffers-and-queues/
 * Use C++ templates for the elem type and also the queue size.  Having queue size as
 * a template parameter should allow us the efficiencies of power-of-two sizes, without
 * restricting us to choosing a given power of two when implementing the queue.
 * The efficient implementation for power-of-two sizes is done by most compilers
 * (at least by gcc).  This file has an implicit dependency on the queue size being a power
 * of two, but unfortunately does not enforce that.
 *
 *
 * Threading:
 * TODO: Can we safely call enq in one task and deq in another?  writeCnt is modified only
 * in enq, and readCnt is modified only in deq.
 *
 * xxx Could we produce a compile-time warning or failure if size is not a power of two
 * (which causes this implementation to be defective)?
 *
 */

#ifndef STR_QUEUE_H
#define STR_QUEUE_H
#include <stdint.h>  // uint8_t, etc


// An advantage of the streams model is that it doesn't waste
// one buffer entry to distinguish between "full" and "empty".
// If size is a power of two, and known at compile-time, the compiler
// should be able to make efficient implementations of the modulus operation
// used in enq() and deq().
template <class T, unsigned size = 8>
class queue
{
public:
    // The type must be big enough to count up to the maximum number of elements in the queue,
    // given by the size parameter of this template class.
    typedef uint8_t counter_t;

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
        // Because the counters are unsigned, this is correct even
        // if counters have wrapped.
        return writeCnt == readCnt + (counter_t)size;
    }

    bool empty()
    {
        return (readCnt == writeCnt);
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

#endif /* STR_QUEUE_H */

