#!/bin/bash
#SBATCH --job-name=strong_scaling
#SBATCH --output=strong_scaling_%j.out
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --partition=debug
#SBATCH --time=00:20:00

module load gcc openmpi
cd ~/Projet-hpc/build_cluster

echo "DEBUT DU TEST DE STRONG SCALING"
echo "Taille de grille fixe : 4096 x 4096"

for n in 1 2 4
do
    echo "------------------------------------------"
    echo "Execution avec $n processus MPI"
    mpirun -np $n ./bin/heat_mpi --nx 4096 --ny 4096 --steps 500 --Ttop 100
done
