/*****************************************************************************
 * This file is part of the Project MPISynchronizedBarrier
 * 
 * Copyright (c) 2018, Michael Axtmann <michael.axtmann@kit.edu>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
******************************************************************************/

#pragma once

#include <mpi.h>

class SynchronizedBarrier {
public:
    SynchronizedBarrier() = delete;

    SynchronizedBarrier(bool local_success);

    unsigned long Success(MPI_Comm comm);

protected:
    unsigned long local_success_;
};

class SynchronizedClock {
public:
    SynchronizedClock() = delete;
    
    SynchronizedClock(MPI_Comm comm,
                      int sync_tag = 01101,
                      double max_async_time = 0.000005,
                      double time_to_sync = 0.01);

    SynchronizedBarrier Waitall(MPI_Comm comm);

protected:
    double time_to_sync_;
    double time_diff_;
};
