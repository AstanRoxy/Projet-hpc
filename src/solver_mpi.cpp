#ifdef USE_MPI
#include "solver.h"
#include <mpi.h>
#include <iostream>
#include <cmath>

MPISolver::MPISolver() : Solver(),
    rank(-1), size(-1), cart_comm(MPI_COMM_NULL),
    local_grid_old(nullptr), local_grid_new(nullptr),
    send_buffer_left(nullptr), send_buffer_right(nullptr),
    send_buffer_bottom(nullptr), send_buffer_top(nullptr),
    recv_buffer_left(nullptr), recv_buffer_right(nullptr),
    recv_buffer_bottom(nullptr), recv_buffer_top(nullptr) {
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
}

MPISolver::~MPISolver() {
    // TODO: Clean up allocated memory and MPI communicator
    delete local_grid_old;
    delete local_grid_new;
    
    delete[] send_buffer_left;
    delete[] send_buffer_right;
    delete[] send_buffer_bottom;
    delete[] send_buffer_top;
    delete[] recv_buffer_left;
    delete[] recv_buffer_right;
    delete[] recv_buffer_bottom;
    delete[] recv_buffer_top;
    
    if (cart_comm != MPI_COMM_NULL) {
        MPI_Comm_free(&cart_comm);
    }
}

void MPISolver::setup_cartesian_communicator() {
    // TODO: Create a 2D Cartesian communicator
    // 1. Determine grid dimensions for processes (try to make it square)
    // 2. Use MPI_Dims_create to create a balanced decomposition
    // 3. Use MPI_Cart_create to create the communicator
    // 4. Use MPI_Cart_coords to get coordinates for this rank
    // 5. Use MPI_Cart_shift to find neighbors in each direction
}

void MPISolver::initialize(const SimulationParams& params) {
    // TODO: Initialize MPI solver
    // 1. Store global dimensions
    // 2. Set up Cartesian communicator
    // 3. Calculate local grid dimensions (including halos)
    // 4. Allocate local grids
    // 5. Initialize local grid with appropriate part of global initial condition
    // 6. Allocate send/receive buffers
    // 7. Initialize base solver parameters (dx, dy, dt, factor)
    
    global_nx = params.Nx;
    global_ny = params.Ny;
    
    setup_cartesian_communicator();
    
    // Calculate local dimensions including halos
    // local_nx = (global_nx / dims[0]) + 2;  // +2 for halos
    // local_ny = (global_ny / dims[1]) + 2;
    
    // Allocate local grids
    local_grid_old = new Grid(local_nx, local_ny);
    local_grid_new = new Grid(local_nx, local_ny);
    
    // Initialize local grid from global initial condition
    // This requires mapping global indices to local indices
    
    // Allocate communication buffers
    // send_buffer_left = new double[local_ny-2];
    // etc.
}

void MPISolver::exchange_halos() {
    // TODO: Implement blocking halo exchange
    // Use MPI_Sendrecv for each neighbor direction
    // This is simpler but may have more waiting
}

void MPISolver::exchange_halos_nonblocking() {
    // TODO: Implement non-blocking halo exchange
    // Use MPI_Isend and MPI_Irecv with MPI_Waitall
    // This allows overlap of communication and computation
    
    MPI_Request requests[8];
    int req_count = 0;
    
    // Exchange with left and right neighbors
    // Exchange with bottom and top neighbors
    // Wait for all communications to complete
}

void MPISolver::time_step() {
    // TODO: Implement one MPI time step
    // 1. Exchange halo cells with neighbors
    // 2. Update interior points (excluding halos)
    // 3. Apply boundary conditions (only on physical boundaries)
    // 4. Swap local grids
    
    // Start communication timer
    communication_timer.start();
    
    // Exchange halos
    exchange_halos_nonblocking();
    
    communication_timer.stop();
    
    // Start computation timer
    computation_timer.start();
    
    // TODO: Update interior points (similar to sequential but on local grid)
    // Be careful with indices: halos are at i=0, i=local_nx-1, j=0, j=local_ny-1
    
    computation_timer.stop();
    
    // Apply boundary conditions to physical boundaries only
    // This requires knowing if this rank is on a global boundary
    // if (coords[0] == 0) // left boundary
    // if (coords[0] == dims[0]-1) // right boundary
    // etc.
}

void MPISolver::run(int num_steps) {
    total_timer.start();
    
    for (int step = 0; step < num_steps; step++) {
        time_step();
        
        // Swap grids
        std::swap(local_grid_old, local_grid_new);
    }
    
    total_timer.stop();
    
    // Gather results to rank 0 for output/validation
    gather_results();
}

void MPISolver::gather_results() {
    // TODO: Gather all local grids to rank 0
    // This is for final validation and output only
    // Not needed for performance measurements
    
    if (rank == 0) {
        // Allocate global grid
        // Receive from all ranks and assemble
    } else {
        // Send local grid to rank 0
    }
}

void MPISolver::report_timing() const {
    if (rank == 0) {
        std::cout << "MPI Solver Timing:" << std::endl;
        std::cout << "  Total: ";
        total_timer.report();
        std::cout << "  Computation: ";
        computation_timer.report();
        std::cout << "  Communication: ";
        communication_timer.report();
        std::cout << "  Communication ratio: " 
                  << (communication_timer.elapsed() / total_timer.elapsed() * 100.0)
                  << "%" << std::endl;
    }
}

#endif // USE_MPI
