# MPISynchronizedBarrier

This is the implementation of a C++ MPI barrier which guarantees that the processes exit the barrier simultaneously.

## Example (see main.cpp)
```C++
// Sync timers.
auto clock = SynchronizedClock(comm);

// The actual barrier.
auto barrier = clock.Waitall(comm);

// Do your stuff.

// Check if the barrier worked.
if (barrier.Success(comm)) {
   // ... 
} else {
  // ...
}
```

You can compile an example by executing the following commands in your shell:
```
mpic++ -c synchronized_barrier.cpp -std=c++11 && mpic++ main.cpp synchronized_barrier.o -std=c++11 && mpirun -np 4 a.out
```