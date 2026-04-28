#!/bin/bash
#SBATCH --job-name=heat_hybrid
#SBATCH --output=heat_hybrid_%j.out
#SBATCH --error=heat_hybrid_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=2
#SBATCH --time=00:10:00
#SBATCH --partition=debug

module load gcc
module load openmpi

export OMP_NUM_THREADS=2
cd ~/Projet-hpc/build_cluster
mpirun -np 2 ./bin/heat_hybrid --nx 512 --ny 512 --steps 1000
