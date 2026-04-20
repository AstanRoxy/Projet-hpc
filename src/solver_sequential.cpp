#include "solver.h"
#include <iostream>

//SequentialSolver::SequentialSolver() : Solver() {}

void SequentialSolver::time_step() {
    //1. Récupération des dimensions
    int nx = T_old->get_nx(); 
    int ny = T_old->get_ny();

    //2. Calcul du facteur de diffusion

    double facteur= alpha * dt / (dx * dx);//Le calcul du facteur

    // TODO: Implement one sequential time step
    
    
    
    // 3. Apply stencil to all interior points
    //Hint:
     for (int i = 1; i < nx-1; i++) {
        for (int j = 1; j < ny-1; j++) {
            // 4. Update T_new from T_old using the 5-point stencil
           (*T_new)(i,j) = (*T_old)(i,j) + facteur * (
               (*T_old)(i-1,j) + (*T_old)(i+1,j) + 
              (*T_old)(i,j-1) + (*T_old)(i,j+1) - 4.0 * (*T_old)(i,j)//différence entre le centre et ces voisins
            );
        }
     }
     // 5. Apply boundary conditions to T_new
     bc->apply(*T_new);
        // 6. Le double Buffering (Le swap des étiquettes)
        //std::swap(T_old, T_new); // déjà dan ssolver.cpp
}
