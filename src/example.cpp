/*****************************************************************************
 * This file is part of the Project MPISynchronizedBarrier
 * 
 * Copyright (c) 2018, Michael Axtmann <michael.axtmann@kit.edu>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
******************************************************************************/

#include "synchronized_barrier.hpp"

#include <mpi.h>
#include <iomanip>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    MPI_Comm comm = MPI::COMM_WORLD;
    int nprocs, myrank;
    MPI_Comm_size(comm, &nprocs);
    MPI_Comm_rank(comm, &myrank);

    std::cout << std::setprecision (15);

    std::cout << "PE " << myrank << ": " << " Wtime at startup... " << MPI_Wtime() << std::endl;

    SynchronizedClock{};

    // Insert delay.
    // Note: this delay does not shift the clock.
    if (myrank == 1) {
        double time = MPI_Wtime();
        while (MPI_Wtime() - time < 2.) {}
    }
    
    auto clock = SynchronizedClock{};
    bool is_clock_synced = clock.Init(comm);
    if (is_clock_synced) {
        std::cout << "PE " << myrank << ": " << " clock is synced." << std::endl;
    } else {
        std::cout << "PE " << myrank << ": " << " clock is not synced." << std::endl;
    }

    auto barrier = clock.Barrier(comm);

    if (barrier.Success(comm)) {
        std::cout << "PE " << myrank << ": " << " barrier did sync." << std::endl;
    } else {
        std::cout << "PE " << myrank << ": " << " barrier did not sync." << std::endl;
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
