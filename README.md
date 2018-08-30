# MPISynchronizedBarrier

This is the implementation of a C++ MPI barrier which guarantees that the processes exit the barrier simultaneously.

## Example (see main.cpp)
```C++
// Create clock.
auto clock = SynchronizedClock{};

// Sync timers.
bool is_clock_synced = clock.Init(comm);

// Check whether clock is synced.
if (is_clock_synced) {
    std::cout << "PE " << myrank << ": " << " clock is synced." << std::endl;
} else {
    std::cout << "PE " << myrank << ": " << " clock is not synced." << std::endl;
}

// The actual barrier.
auto barrier = clock.Barrier(comm);

// Do your stuff.

// Check if barrier worked.
if (barrier.Success(comm)) {
    std::cout << "PE " << myrank << ": " << " barrier did sync." << std::endl;
} else {
    std::cout << "PE " << myrank << ": " << " barrier did not sync." << std::endl;
}
```

You can compile an example by executing the following commands in your shell:
```
make all
mpirun -np 4 ./build/example
```