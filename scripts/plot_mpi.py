import matplotlib.pyplot as plt

# 1. Données réelles MPI (1000 steps) issues de ton terminal
processus = [1, 2, 4]
temps_total = [1.248739, 0.689287, 0.305589]
temps_calcul = [1.242789, 0.663409, 0.266822]
temps_comm = [0.005680, 0.025678, 0.038610]

# 2. Calcul du Speedup et de l'Efficacité
t1 = temps_total[0]
speedup = [t1 / tn for tn in temps_total]
efficacite = [(s / n) * 100 for s, n in zip(speedup, processus)]

# 3. Affichage du tableau dans le terminal
print(f"{'Procs':<6} | {'Total (s)':<10} | {'Calcul (s)':<10} | {'Comm (s)':<10} | {'Speedup':<8} | {'Eff (%)':<8}")
print("-" * 75)
for i in range(len(processus)):
    print(f"{processus[i]:<6} | {temps_total[i]:<10.4f} | {temps_calcul[i]:<10.4f} | {temps_comm[i]:<10.4f} | {speedup[i]:<8.2f} | {efficacite[i]:<8.1f}%")

# 4. Création du graphique de Speedup
plt.figure(figsize=(10, 6))
plt.plot(processus, speedup, marker='o', color='blue', label='Speedup MPI mesuré')
plt.plot(processus, processus, linestyle='--', color='red', label='Speedup idéal')

plt.xlabel('Nombre de Processus MPI')
plt.ylabel('Speedup')
plt.title('Analyse de Strong Scaling - MPI Solver')
plt.legend()
plt.grid(True)

# 5. SAUVEGARDE de l'image (important pour le cluster !)
plt.savefig('scaling_mpi.png')
print("\nImage sauvegardée : scaling_mpi.png")