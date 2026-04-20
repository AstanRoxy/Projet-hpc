#include "grid.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <malloc.h> // Pour Windows


Grid::Grid(int nx, int ny) : nx(nx), ny(ny) {
    // 1. Allocate data array of size nx*ny using aligned_alloc
    //data = (double*)_aligned_malloc(64, (size_t)nx * ny * sizeof(double));
    data = new double[nx*ny];
    if (data == nullptr) {
        std::cerr << "ERROR: Failed to allocate memory for Grid data." << std::endl;
        // Optionally throw an exception or handle error more robustly
        exit(1);
    }

    // 2. Allocate rows array of size nx
    rows = new double*[nx];
    if (rows == nullptr) {
        std::cerr << "ERROR: Failed to allocate memory for Grid rows." << std::endl;
        free(data); // Clean up previously allocated data
        exit(1);
    }

    // 3. Set each rows[i] to point to &data[i*ny]
    for (int i = 0; i < nx; i++) {
        rows[i] = &data[i * ny];
    }
}

Grid::~Grid() {
    // 1. Delete rows array
    //if (rows) {
       // delete[] rows;
       // rows = nullptr;
   // }
    // 2. Free data array
   // if (data) {
       // _aligned_free(data); // Use free because aligned_alloc was used
       // data = nullptr;
    //}
    delete[] rows; 
    delete[] data;
}

void Grid::fill(double value) {
    // TODO: Fill entire grid with constant value
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            (*this)(i, j) = value;
        }
    }
}

void Grid::initialize(int init_type, double T_initial, double Lx, double Ly) {
    double dx = Lx / (nx - 1);
    double dy = Ly / (ny - 1);

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            double x = i * dx;
            double y = j * dy;

            switch (init_type) {
                case 0: // Uniform
                    (*this)(i, j) = T_initial;
                    break;

                case 1: { // Gaussian hotspot in center
                    double cx = Lx / 2.0;
                    double cy = Ly / 2.0;
                    double sigma = Lx / 8.0;
                    double r2 = (x-cx)*(x-cx) + (y-cy)*(y-cy);
                    (*this)(i, j) = T_initial + 100.0 * exp(-r2/(2*sigma*sigma));
                    break;
                }

                case 2: // Random
                    (*this)(i, j) = T_initial + (rand() % 100) / 100.0 * 50.0;
                    break;

                default:
                    (*this)(i, j) = T_initial;
            }
        }
    }
}

void Grid::swap(Grid& other) {
    std::swap(data, other.data);
    std::swap(rows, other.rows);
    std::swap(nx, other.nx);
    std::swap(ny, other.ny);
}

bool Grid::is_approx_equal(const Grid& other, double tolerance) const {
    if (nx != other.nx || ny != other.ny) return false;

    // TODO: Check if all elements are within tolerance
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            if (std::abs((*this)(i, j) - other(i, j)) > tolerance) {
                return false;
            }
        }
    }
    return true;
}
