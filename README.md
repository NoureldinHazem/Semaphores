# Semaphores
You are required to write a C program to solve the following synchronization problem
using POSIX and “semaphore.h” libraries. </br>
N mCounter threads count independent incoming messages in a system and another
thread.</br>
mMonitor gets the count of threads at time intervals of size t1, and then resets the
counter to
0.</br>
The mMonitor then places this value in a buffer of size b, and a mCollector thread
reads the values from the buffer.</br>
Any thread will have to wait if the counter is being locked by any other thread. Also, the
mMonitor and mCollector threads will not be able to access the buffer at the same time
or to add another entry if the buffer is full.</br>
Assume that the messages come randomly to the system, this can be realized if the
mCounter threads sleep for random times, and their activation (sleep time ends) corresponds to an
email arrival.</br>
Similarly, the mMonitor and mCollector will be activated at random time intervals.</br></br>
This was assignment 3 in OS course
