#include "solver.h"
#include <iostream>
#include <cmath>

// Con,structeur de base
Solver::Solver() : T_old(nullptr), T_new(nullptr), bc(nullptr) {}

//Destructeur
Solver::~Solver() {
    delete T_old;
    delete T_new;
    delete bc;
}

void Solver::initialize(const SimulationParams& params) {
    // Create grids
    T_old = new Grid(params.Nx, params.Ny);
    std::cout << "DEBUG: STEP 1 - T_old created." << std::endl;
    T_new = new Grid(params.Nx, params.Ny);
    std::cout << "DEBUG: STEP 2 - T_new created." << std::endl;

    // Initialize grid with initial condition
    T_old->initialize(params.init_type, params.T_initial, params.Lx, params.Ly);
    std::cout << "DEBUG: STEP 3 - T_old initialized." << std::endl;

    // Create boundary conditions
    bc = new BoundaryConditions(params.boundary_type,
                                params.T_left, params.T_right,
                                params.T_bottom, params.T_top);
    std::cout << "DEBUG: STEP 4 - BoundaryConditions created." << std::endl;
    alpha = params.alpha;
    std::cout << "DEBUG: STEP 5 - alpha set." << std::endl;

    // Apply initial boundary conditions
    bc->apply(*T_old);
    std::cout << "DEBUG: STEP 6 - Boundary conditions applied to T_old." << std::endl;

    // Calculate grid spacing and time step
    dx = params.Lx / (params.Nx - 1);
    std::cout << "DEBUG: STEP 7 - dx calculated." << std::endl;
    dy = params.Ly / (params.Ny - 1);
    std::cout << "DEBUG: STEP 8 - dy calculated." << std::endl;

    // For stability, dt <= min(dx^2, dy^2) / (4 * alpha)
    double h2 = std::min(dx * dx, dy * dy);
    std::cout << "DEBUG: STEP 9 - h2 calculated." << std::endl;
    dt = 0.25 * h2 / params.alpha;  // Conservative time step
    std::cout << "DEBUG: STEP 10 - dt calculated." << std::endl;

    factor = params.alpha * dt / (dx * dx);  // assuming dx=dy for simplicity
    std::cout << "DEBUG: STEP 11 - factor calculated." << std::endl;

    std::cout << "Grid spacing: dx=" << dx << ", dy=" << dy << std::endl;
    std::cout << "Time step: dt=" << dt << ", factor=" << factor << std::endl;
    std::cout << "DEBUG: Grid Size: " << params.Nx << "x" << params.Ny << std::endl;
    std::cout << "DEBUG: Steps: " << params.num_steps << std::endl;
    std::cout << "DEBUG: T_top: " << params.T_top << std::endl;
    std::cout << "DEBUG: T_bottom: " << params.T_bottom << std::endl;
    std::cout << "DEBUG: T_left: " << params.T_left << std::endl;
    std::cout << "DEBUG: T_right: " << params.T_right << std::endl;
    std::cout << "DEBUG: STEP 12 - Debug messages printed." << std::endl;
}

void Solver::run(int num_steps) {
    total_timer.start();
    
    for (int step = 0; step < num_steps; step++) {
        computation_timer.start();
        time_step();
        
        computation_timer.stop();
        // Swap grids
        T_old->swap(*T_new);
    }
    
    total_timer.stop();
}

void Solver::report_timing() const {
    std::cout << "Timing:" << std::endl;
    std::cout << "  Total: ";
    total_timer.report();
    std::cout << "  Computation: ";
    computation_timer.report();
    std::cout << "  Communication: ";
    communication_timer.report();
}

bool Solver::validate(Grid* reference, double tolerance) const {
    if (!reference || !T_old) return false;
    return T_old->is_approx_equal(*reference, tolerance);
}
