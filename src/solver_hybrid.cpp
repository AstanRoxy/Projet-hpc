#if defined(USE_MPI) && defined(USE_OPENMP)
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
    
    // 1. ÉCHANGE DES HALOS (MPI)
    communication_timer.start();
    // On lance l'échange des bords entre les différents processus
    exchange_halos_nonblocking();
    communication_timer.stop();


  //  2. CALCUL DU STENCIL (OpenMP) - Intra-nœud
    computation_timer.start();

    // On s'assure d'avoir les bonnes dimensions locales
     int local_nx = local_grid_old->get_nx();
    int local_ny = local_grid_old->get_ny();


    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 1; i < local_nx - 1; ++i) { // Correction : utiliser local_nx
        for (int j = 1; j < local_ny - 1; ++j) { // Correction : utiliser local_ny
            double center = (*local_grid_old)(i, j);
            (*local_grid_new)(i, j) = center + factor * (
                (*local_grid_old)(i+1, j) + (*local_grid_old)(i-1, j) +
                (*local_grid_old)(i, j+1) + (*local_grid_old)(i, j-1) - 
                4.0 * center
            );
        }
    }
    
    
    // 3. CONDITIONS AUX LIMITES PHYSIQUES
    // On applique les températures sur les bords réels (X=0, X=L, etc.)
    apply_physical_boundary();
   
    
    // TODO: Add OpenMP parallelization here
    // Be careful with collapse(2) and thread safety
    //#pragma omp parallel for collapse(2) schedule(static)
    //for (int i = 1; i < local_nx-1; i++) {
       // for (int j = 1; j < local_ny-1; j++) {
            // Update interior points
       // }
  //  }
    
    computation_timer.stop();
    
    // Apply boundary conditions (serial)
    // Need to apply only on physical boundaries
}

#endif // USE_MPI && USE_OPENMP
