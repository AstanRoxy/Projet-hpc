#!/bin/bash
#SBATCH --job-name=heat_mpi
#SBATCH --output=heat_mpi_%j.out
#SBATCH --error=heat_mpi_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --time=00:10:00
#SBATCH --partition=debug

module load gcc
module load openmpi

cd ~/Projet-hpc/build_cluster
mpirun -np 4 ./bin/heat_mpi --nx 512 --ny 512 --steps 1000
