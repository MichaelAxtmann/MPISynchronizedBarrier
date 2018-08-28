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

    SynchronizedClock{comm};

    // Insert delay.
    // Note: this delay does not shift the clock.
    if (myrank == 1) {
        double time = MPI_Wtime();
        while (MPI_Wtime() - time < 2.) {}
    }
    
    auto clock = SynchronizedClock(comm);

    auto barrier = clock.Waitall(comm);

    if (barrier.Success(comm)) {
        std::cout << "PE " << myrank << ": " << " was synced." << std::endl;
    } else {
        std::cout << "PE " << myrank << ": " << " was not synced." << std::endl;
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
