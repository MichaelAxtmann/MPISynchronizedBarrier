/*****************************************************************************
 * This file is part of the Project MPISynchronizedBarrier
 * 
 * Copyright (c) 2018, Michael Axtmann <michael.axtmann@kit.edu>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
******************************************************************************/

#include "synchronized_barrier.hpp"

SynchronizedBarrier:: SynchronizedBarrier(bool local_success) : local_success_(local_success) {}

unsigned long SynchronizedBarrier::Success(MPI_Comm comm) {

    unsigned long global_success = 0;
    MPI_Allreduce(&local_success_, &global_success, 1, MPI_UNSIGNED_LONG, MPI_LAND, comm);
    return global_success;
    
}

SynchronizedClock:: SynchronizedClock(MPI_Comm comm,
                                      int sync_tag,
                                      double max_async_time,
                                      double time_to_sync)
    : time_to_sync_(time_to_sync) {

    int nprocs, myrank;
    MPI_Comm_size(comm, &nprocs);
    MPI_Comm_rank(comm, &myrank);

    if (!myrank) {
        
        for (int target = 0; target != nprocs; ++target) {
            // Sync PE 'target'.
            double start_time = 0;
            double end_time = 0;
            
            do {
                
                start_time = MPI_Wtime();
                MPI_Send(&start_time, 1, MPI_DOUBLE, target, sync_tag, comm);
                double dummy = 0;
                MPI_Recv(&dummy, 1, MPI_DOUBLE, target, sync_tag, comm, MPI_STATUS_IGNORE);
                end_time = MPI_Wtime();
                
            } while (end_time - start_time > max_async_time);
            
            double succ = -1;
            MPI_Send(&succ, 1, MPI_DOUBLE, target, sync_tag, comm);
            
        }
        
        time_diff_ = 0;
        
    } else {
        
        double time_diff = 0;
        double time = 0;
        MPI_Recv(&time, 1, MPI_DOUBLE, 0, sync_tag, comm, MPI_STATUS_IGNORE);
        time_diff = time - MPI_Wtime();
        
        while (time != (double)-1) {
            
            MPI_Send(&time, 1, MPI_DOUBLE, 0, sync_tag, comm);
            MPI_Recv(&time, 1, MPI_DOUBLE, 0, sync_tag, comm, MPI_STATUS_IGNORE);
            
            if (time != (double)-1) time_diff = time - MPI_Wtime();
        }
        time_diff_ = time_diff;
        
    }
    
    std::cout << "PE " << myrank << ": " << " clock diff... " << time_diff_ << std::endl;
}

SynchronizedBarrier SynchronizedClock::Waitall(MPI_Comm comm) {
    
    MPI_Barrier(comm);
    
    double start_time = MPI_Wtime() + time_to_sync_;
    
    MPI_Bcast( &start_time, 1, MPI_DOUBLE, 0, comm );

    bool sync_valid = false;
    while (time_diff_ + MPI_Wtime() < start_time) {
        sync_valid = true;
    }
    
    return SynchronizedBarrier(sync_valid);
}
