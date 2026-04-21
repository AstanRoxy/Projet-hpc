import matplotlib.pyplot as plt

# 1. Tes données mesurées (Nombre de threads et Temps en secondes)
threads = [1, 2, 4]
temps = [1.889633, 1.392505, 1.260651]

# 2. Calcul du Speedup (T1 / Tn)
t1 = temps[0]
speedup = [t1 / tn for tn in temps]

# 3. Calcul de l'Efficacité (Speedup / n)
efficacite = [(s / n) * 100 for s, n in zip(speedup, threads)]

# Affichage des résultats pour ton tableau
print(f"{'Threads':<10} | {'Temps (s)':<10} | {'Speedup':<10} | {'Efficacité (%)':<15}")
print("-" * 55)
for i in range(len(threads)):
    print(f"{threads[i]:<10} | {temps[i]:<10.4f} | {speedup[i]:<10.2f} | {efficacite[i]:<15.1f}%")

# Optionnel : Créer un graphique (si tu as matplotlib installé)
plt.plot(threads, speedup, marker='o', label='Speedup mesuré')
plt.plot(threads, threads, linestyle='--', label='Speedup idéal (Linéaire)')
plt.xlabel('Nombre de Threads')
plt.ylabel('Speedup')
plt.title('Strong Scaling - OpenMP Heat Solver')
plt.legend()
plt.grid(True)
plt.show()