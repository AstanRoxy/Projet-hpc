#!/bin/bash
# Comparaison rapide Seq vs MPI
echo "--- SÉQUENTIEL ---"
./bin/heat_seq --nx 256 --ny 256 --steps 500
echo "--- MPI ---"
mpirun -np 4 ./bin/heat_mpi --nx 256 --ny 256 --steps 500