# Introduction
Implement stream queue concept. This is an efficient queue implementation that doesn't waste
one buffer entry to distinguish between "full" and "empty" state of the queue.

We use template parameters for the elem type and the queue size. Having queue size as
a template parameter allows us to enforce use of power-of-two sizes, without
restricting us to choosing a given power of two when implementing the queue.
The efficient implementation for power-of-two sizes is done by most compilers
(at least by gcc). This implementation has an implicit dependency on the queue size being a power
of two, and in C++ 11, static_assert allows us to verify that easily.

# Building and installing the library
Installation is not supported yet, but since the project consists of a single header file,
using it is simple.

# Thread safety
 - To make it safe to call enq in one task and deq in another, writeCnt and readCnt should
   be atomic so accesses are not re-ordered or interrupted.
    - writeCnt is modified only in enq, and readCnt is modified only in deq.
    - enq updates writeCnt AFTER writing to the queue. This ensures that when
      enq is writing to an empty queue, deq's empty() returns true until the write is
      complete, avoiding deq reading partially-written data.
    - Similarly, deq updates readCnt AFTER reading from the queue.  This ensures that when
      deq is reading from a full queue, enq's full() returns true until the read is
      complete, avoiding enq overwriting data that's still being read.
 - It's not safe to enqueue to one queue in several tasks, or dequeue one queue in
   several tasks -- if you need to do this, add the necessary protection.
 - Making readCnt and writeCnt atomic will additionally ensure they are not accessed when
   partly updated.

**@TODO**: writeCnt and readCnt must be atomic.

# Compile-time checks
We produce a compile-time failure if size is not a power of two, since this is a requirement
of this model. Also fail if the counter type is not big enough to count up to size.

# Difference in executable size depends on counter type
Difference in executable size on laptop (presumably a 32-bit Intel processor) when
switching between counter_t uin8_t and uint32_t:

For uint8_t:
```
   text    data     bss     dec     hex filename
  18568    4848     408   23824    5d10 strqueue_test.exe
```
For uint32_t:
```
   text    data     bss     dec     hex filename
  18552    4848     408   23808    5d00 strqueue_test.exe
```
Conclusion: the executable is smaller when using the processor's natural integer size.
For the uint8_t version, various instructions are byte-instructions, but
there are also several additional instructions.

_See MIPS Run_, 2nd Edition, confirms that for MIPS, the compiler
generates additional instructions to cause smaller-than-word sized
variables to "overflow" -- this confirms that for MIPS, at least,
the most efficient implementation uses uint32_t (or maybe
uint64_t for MIPS 64).

# Reference
The concept is descibed here: http://fgiesen.wordpress.com/2010/12/14/ring-buffers-and-queues/.
