/* 
 * Try the following:
 * Use stream queue concept http://fgiesen.wordpress.com/2010/12/14/ring-buffers-and-queues/
 * Use C++ templates for the elem type and also the queue size.  Having queue size as
 * a template parameter should allow us the efficiencies of power-of-two sizes, without
 * restricting us to choosing a given power of two when writing the code.
 *
 * Note that we assume the increment operations on writeCnt and readCnt are atomic.
 * Choose them accordingly, e.g. in n-bit processors, they should not be larger than n bits.
 *
 */

#ifndef STR_QUEUE_H
#define STR_QUEUE_H


// An advantage of the streams model is it means we don't have to waste
// one buffer entry to distinguish between "full" and "empty".
// If Q_SIZE is a power of two, and known at compile-time, the compiler
// should be able to make efficient implementations of the modulus operation
// used in enq() and deq().
template <class T, unsigned size = 8>
class queue
{
public:
    queue() : readCnt(0), writeCnt(0) {}
    bool enq(const T & entry);
    bool deq(T & entry);
    
private:

    bool full()
    {
        // Because the counters are unsigned, this is correct even
        // if the counters wrap.
        return (writeCnt == readCnt + size);
    }

    bool empty()
    {
        return (readCnt == writeCnt);
    }

    T buf[size];

    unsigned readCnt;  // Number of reads
    unsigned writeCnt; // Number of writes
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

