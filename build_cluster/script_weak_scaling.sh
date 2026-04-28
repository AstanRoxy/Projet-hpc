#!/bin/bash
#SBATCH --job-name=weak_scaling
#SBATCH --output=weak_scaling_%j.out
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --partition=debug
#SBATCH --time=00:15:00

cd ~/Projet-hpc/build_cluster

echo "--- TEST 1: 1 COEUR (CHARGE NORMALE) ---"
mpirun -np 1 ./bin/heat_mpi --nx 2048 --ny 2048 --steps 500

echo "--- TEST 2: 4 COEURS (CHARGE QUADRUPLEE) ---"
mpirun -np 4 ./bin/heat_mpi --nx 4096 --ny 4096 --steps 500
