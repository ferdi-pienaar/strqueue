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
 * Thread safety:
 *  - It's safe to call enq in one task and deq in another. 
 *     o writeCnt is modified only in enq, and readCnt is modified only in deq.
 *     o enq updates writeCnt AFTER writing to the queue.  This ensures that when
 *       enq is writing to an empty queue, deq's empty() returns true until the write is
 *       complete, avoiding deq reading partially-written data.
 *     o Similarly, deq updates readCnt AFTER reading from the queue.  This ensures that when
 *       deq is reading from a full queue, enq's full() returns true until the read is
 *       complete, avoiding enq overwriting data that's still being read.
 *  - It's not safe to enqueue to one queue in several tasks, or dequeue one queue in
 *    several tasks -- if you need to do this, add the necessary protection.
 *    
 *  - We assume that writing to readCnt and writeCnt is atomic.
 *    If writing is not atomic, an context switch could happen when they're
 *    partly written, and thus meaningless.  This means that the type used for readCnt
 *    and writeCnt should not be larger than the register size of the processor, e.g. on
 *    an 8-bit processor, don't use 16- or 32-bit integers.  If you need larger readCnt/writeCnt,
 *    modify the code to disable interrupts while writing readCnt and writeCnt.
 *
 * xxx Could we produce a compile-time warning or failure if size is not a power of two
 * (which causes this implementation to be defective)?  Same for if the counter type
 * is not big enough to count up to size.
 *
 * Difference in executable size on laptop (presumably a 32-bit Intel processor) when
 * switching between counter_t uin8_t and uint32_t:
 * For uint8_t:
 *    text    data     bss     dec     hex filename
 *   18568    4848     408   23824    5d10 strqueue_test.exe
 * For uint32_t:
 *    text    data     bss     dec     hex filename
 *   18552    4848     408   23808    5d00 strqueue_test.exe
 *
 * Conclusion: the executable is smaller when using the processor's natural integer size.
 * For the uint8_t version, various instructions are byte-instructions, but
 * there are also several additional instructions.
 *
 */

#ifndef STR_QUEUE_H
#define STR_QUEUE_H
#include <stdint.h>  // uint8_t, etc


// An advantage of the streams model is that it doesn't waste
// one buffer entry to distinguish between "full" and "empty".
// @param T - the type of the objects held in the queue, often a pointer
// @param size - the maximum number of elements the queue can hold
// Size is known at compile-time, so if it is a power of two, the compiler
// should be able to make efficient implementations of the modulus operation
// used in enq() and deq().
template <class T, unsigned size = 8>
class queue
{
public:
    // This unsigned type must be big enough to count up to the maximum number of elements in the queue,
    // given by the size parameter of this template class.  The most efficient (fastest)
    // implementation probably results when this is the natural word size of the processor.
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

#endif /* STR_QUEUE_H */

