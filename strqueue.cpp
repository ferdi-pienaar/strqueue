/* 
 * Try the following:
 * Use stream queue concept http://fgiesen.wordpress.com/2010/12/14/ring-buffers-and-queues/
 * Use C++ templates for the elem type and also the queue size.  Having queue size as
 * a template parameter should allow us the efficiencies of power-of-two sizes, without
 * restricting us to choosing a given power of two when writing the code.
 *
 */

#include "strqueue.h"


// Add an element to the tail of the queue
bool queue::enq(const t_element & e)
{
    if (full())
    {
        return false;
    }

    buf[writeCnt % Q_SIZE] = e;
    writeCnt++;
    return true;
}


// Remove an element from head of queue, copying it to input param
bool queue::deq(t_element & e)
{
    if (empty())
    {
        return false;
    }

    e = buf[readCnt % Q_SIZE];
    readCnt++;
    return true;
}

