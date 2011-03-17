#ifndef STR_QUEUE_H
#define STR_QUEUE_H

#ifndef Q_SIZE // Allows you to define it in Makefile or on command line
#define Q_SIZE 8
#endif

#if ((Q_SIZE & (Q_SIZE - 1)) != 0)
#error Q_SIZE must be a power of two
#endif

#define MASK ((Q_SIZE) - 1)


// An advantage of the streams model is it means we don't have to waste
// one buffer entry to distinguish between "full" and "empty".
// If Q_SIZE is a power of two, and known at compile-time, the compiler
// should be able to make efficient implementations of the modulus operation
// used in enq() and deq().
class queue
{
public:
    typedef unsigned t_element;

    queue() : readCnt(0), writeCnt(0) {}
    bool enq(const t_element & e);
    bool deq(t_element & e);

private:

    bool full()
    {
        // Because the counters are unsigned, this is correct even
        // if the counters wrap.
        return (writeCnt == readCnt + Q_SIZE);
    }

    bool empty()
    {
        return (readCnt == writeCnt);
    }

    t_element buf[Q_SIZE];

    unsigned readCnt;  // Number of reads
    unsigned writeCnt; // Number of writes
};


#endif /* STR_QUEUE_H */
