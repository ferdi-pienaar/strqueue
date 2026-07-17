/*
 * Experiment with so-called "explicit specialization" of template strqueue.
 * What I learned:
 *  If the specialization does not implement the methods of the template
 *  that are used in the client code, compilation fails -- it doesn't
 *  inherit methods from the template.
 *  Methods that aren't used, don't need to be implemented.
 *  You can add methods that don't exist in the template, even public ones.
 *
 */

#include "strqueue.h"
#include <stdint.h>  // uint8_t, etc
#include <iostream>

using namespace std;
// A template specialization that binds both parameters: queue of 4 ints.
// The following line is required by gcc -- else it gives the following compiler error:
// "error: an explicit specialization must be preceded by 'template <>'"
template <>
class queue<int, 4>
{
public:
    bool enq(const int & entry)
    {
        cout << __func__ << " enq" << endl;
        return true;
    }

    int f()
    {
        cout << __func__ << " f" << endl;
        return 0;
    }

    int numItems()
    {
        return 31;
    }
};

// A queue with a peek method.
// A template that inherits from a template, adding a method.
// I'm not very clear on why I have to use 'this' to refer to base
// class members, but gcc apparently requires it.
template <typename T, unsigned size>
class peekQueue : public queue <T, size>
{
public:
    peekQueue() {}
    bool peek(T & entry)
    {
        // Implement using only the public/protected methods of the base class.
        T temp;
        if (!this->deq(temp))
        {
            return false;
        }
        entry = temp;
        this->enq(temp);
        return true;
    }
};


// A queue of ints.
// A template specialization that binds only one of the parameters.
class intQueue : public queue<int>
{
    // seems we don't need anything here.
};

// Another way to name a queue of ints: there's no need to derive
// a class from the template just to create the new name;
// just use typedef to assign a name.
typedef queue<int, 2> intQueueSmall;

static void use_explicit_specialization();
static void use_derived_template();
static void use_derived_class();
static void use_named_type();

int main(int argc, char** argv)
{
    use_explicit_specialization();
    use_derived_template();
    use_derived_class();
    use_named_type();
    return 0;
}


// Example use of explicit specialization
static void use_explicit_specialization()
{
    queue<int, 4> q;
    q.enq(1);
    int n = q.numItems();
    cout << __func__ << " numItems=" << n << endl;
    q.f();
    int i;
    //q.deq(i); // this method was not implemented in the above explicit specialization, so compilation fails.

#if 0 // This violates compile-time check size is power of 2, so compile error.
    queue<int, 5> q2;
    int i2;
    q2.enq(45);
    cout << __func__ << " numItems=" << (int)q2.numItems() << endl;
    q2.deq(i2);
    cout << __func__ << " out=" << i2 << endl;
#endif
}

// Example use of derived template class
static void use_derived_template()
{
    peekQueue<int, 8> pq;
    pq.enq(5);
    int preview;
    pq.peek(preview);
    cout << __func__ << " preview=" << preview << endl;
    pq.enq(6);
    pq.peek(preview);
    cout << __func__ << " preview=" << preview << endl;
    cout << __func__ << " numItems=" << (int)pq.numItems() << endl;
}

static void use_derived_class()
{
    intQueue iq;

    iq.enq(555);
    cout << __func__ << " numItems=" << (int)iq.numItems() << endl;

    int out;
    iq.deq(out);
    cout << __func__ << " out=" << out << endl;
}

static void use_named_type()
{
    intQueueSmall sq;

    sq.enq(999);
    cout << __func__ << " numItems=" << (int)sq.numItems() << endl;

    int out;
    sq.deq(out);
    cout << __func__ << " out=" << out << endl;
}
