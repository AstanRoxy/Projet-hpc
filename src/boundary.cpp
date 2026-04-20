#include "boundary.h"

BoundaryConditions::BoundaryConditions(int type, 
                                       double left, double right,
                                       double bottom, double top) :
    boundary_type(type), T_left(left), T_right(right),
    T_bottom(bottom), T_top(top) {}

void BoundaryConditions::apply(Grid& grid) const {
    int nx = grid.get_nx();
    int ny = grid.get_ny();
    
    if (boundary_type == 0) {  // Dirichlet (fixed temperature)
        // TODO: Apply Dirichlet boundary conditions
        // Left and right boundaries
        //forcer les valeurs sur les bords de la matrice grid
        for (int j = 0; j < ny; j++) {
            grid(0, j) = T_left;//Fixxe le bord gauche
            grid(nx-1, j) = T_right;
        }
        
        // Top and bottom boundaries
        for (int i = 0; i < nx; i++) {
            grid(i, 0) = T_bottom;
            grid(i, ny-1) = T_top;
        }
    } 
    else {  // Neumann (insulated)
        // Gradient nul : copie des valeurs voisines
        // Left and right boundaries
        for (int j = 0; j < ny; j++) {
            grid(0, j)     = grid(1, j);       // gauche
            grid(nx-1, j)  = grid(nx-2, j);    // droite
        }

        // Top and bottom boundaries
        for (int i = 0; i < nx; i++) {
            grid(i, 0)     = grid(i, 1);       // bas
            grid(i, ny-1)  = grid(i, ny-2);    // haut
        }
    }
}
