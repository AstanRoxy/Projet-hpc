#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "config.h"
#include "solver.h"

int main(int argc, char** argv)  {
    // Initialize MPI if needed
    #ifdef USE_MPI
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    #else
    int rank = 0;
    #endif
    
    // Parse command line arguments
    SimulationParams params;
    if (!parse_arguments(argc, argv, params)) {
        if (rank == 0) {
            std::cerr << "Failed to parse arguments" << std::endl;
            std::cerr << "Usage: " << argv[0] 
                      << " --nx <value> --ny <value> --steps <value>" 
                      << std::endl;
        }
        #ifdef USE_MPI
        MPI_Finalize();
        #endif
        return 1;
    }

    #ifdef USE_MPI
    // Le Rang 0 envoie la structure params à tous les autres rangs
    MPI_Bcast(&params, sizeof(SimulationParams), MPI_BYTE, 0, MPI_COMM_WORLD);
    #endif
    
    // Create appropriate solver based on compilation flags
    Solver* solver = nullptr;
    
    #ifdef SEQUENTIAL
    if (rank == 0) std::cout << "Using Sequential solver" << std::endl;
    solver = new SequentialSolver();
    #endif
    
    #ifdef USE_OPENMP
    #ifndef USE_MPI
    if (rank == 0) std::cout << "Using OpenMP solver" << std::endl;
    solver = new OpenMPSolver();
    #endif
    #endif
    
    #ifdef USE_MPI
    #ifndef USE_OPENMP
    if (rank == 0) std::cout << "Using MPI solver" << std::endl;
    solver = new MPISolver();
    #endif
    #endif
    
    #if defined(USE_MPI) && defined(USE_OPENMP)
    if (rank == 0) std::cout << "Using Hybrid MPI+OpenMP solver" << std::endl;
    solver = new HybridSolver();
    #endif
    
    if (!solver) {
        if (rank == 0) {
            std::cerr << "No solver defined. Check compilation flags." << std::endl;
        }
        #ifdef USE_MPI
        MPI_Finalize();
        #endif
        return 1;
    }
    
    // Initialize solver
    solver->initialize(params);
    
    // Run simulation
    if (rank == 0) {
        std::cout << "Running simulation with grid " 
                  << params.Nx << "x" << params.Ny 
                  << ", " << params.num_steps << " time steps" 
                  << std::endl;
    }
    
    solver->run(params.num_steps);

    double mean_temp = solver->calculate_mean_temperature();
    if (rank == 0) {
        std::cout << std::fixed << std::setprecision(6); // Pour voir les décimales
        std::cout << "Final Mean Temperature: " << mean_temp << std::endl;
    
    // Report timing
    
        solver->report_timing();
    }
    
    // Clean up
    delete solver;
    
    #ifdef USE_MPI
    MPI_Finalize();
    #endif
    
    return 0;
}
