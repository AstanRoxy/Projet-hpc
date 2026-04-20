#ifdef USE_MPI && USE_OPENMP
#include "solver.h"
#include <omp.h>
#include <iostream>

HybridSolver::HybridSolver(int threads_per_process) : 
    MPISolver(), num_threads_per_process(threads_per_process) {
    
    if (num_threads_per_process > 0) {
        omp_set_num_threads(num_threads_per_process);
    }
}

void HybridSolver::time_step() {
    // TODO: Implement hybrid MPI+OpenMP time step
    // 1. Exchange halos using MPI (same as MPI version)
    // 2. Parallelize interior update with OpenMP
    // 3. Apply boundary conditions
    
    // Start communication timer
    communication_timer.start();
    
    // Exchange halos
    exchange_halos_nonblocking();
    
    communication_timer.stop();
    
    // Start computation timer
    computation_timer.start();
    
    int local_nx = local_grid_old->get_nx();
    int local_ny = local_grid_old->get_ny();
    
    // TODO: Add OpenMP parallelization here
    // Be careful with collapse(2) and thread safety
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 1; i < local_nx-1; i++) {
        for (int j = 1; j < local_ny-1; j++) {
            // Update interior points
        }
    }
    
    computation_timer.stop();
    
    // Apply boundary conditions (serial)
    // Need to apply only on physical boundaries
}

#endif // USE_MPI && USE_OPENMP
