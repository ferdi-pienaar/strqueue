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

#include "strqueue.h"

#if 0
// Add an element to the tail of the queue
template <class T>
bool queue<T>::enq(const T & entry)
{
    if (full())
    {
        return false;
    }

    buf[writeCnt % Q_SIZE] = entry;
    writeCnt++;
    return true;
}


// Remove an element from head of queue, copying it to input param
template <class T>
bool queue<T>::deq(T & entry)
{
    if (empty())
    {
        return false;
    }

    entry = buf[readCnt % Q_SIZE];
    readCnt++;
    return true;
}
#endif

