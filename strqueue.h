#ifndef STR_QUEUE_H
#define STR_QUEUE_H

#ifndef Q_SIZE // Allows you to define it in Makefile or on command line
#define Q_SIZE 8
#endif

#if ((Q_SIZE & (Q_SIZE - 1)) != 0)
#error Q_SIZE must be a power of two
#endif

#define MASK ((Q_SIZE) - 1)


class queue
{
public:
    typedef unsigned t_element;

    queue() : head(0), tail(0) {}
    bool enq(const t_element & e);
    bool deq(t_element & e);

private:
    unsigned int nextIndex(unsigned int idx)
    {
        return (idx + 1) & MASK;
    }

    bool full()
    {
        return (head == nextIndex(tail));
    }

    bool empty()
    {
        return (head == tail);
    }

    t_element buf[Q_SIZE];

    unsigned head; // index
    unsigned tail; // index
};


#endif /* STR_QUEUE_H */
