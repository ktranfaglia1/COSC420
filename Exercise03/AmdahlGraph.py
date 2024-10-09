# Kyle Tranfaglia
# COSC420 - Exercise03
# Last updated 10/08/24
# This program uses Amdahl's Law to plot a line graph for Speedup Corresponding to Number of Processors

import matplotlib.pyplot as plt


# Define a function to calculate speedup based on Amdahl's law
def amdahls_law(fp, p):
    return 1 / ((1 - fp) + (fp / p))


# Define values of p (number of processors) and the fraction of the program that can be parallelized (fp)
p_values = [5, 10, 100, 1000, 10000]
fp_values = [0.95, 0.9, 0.75, 0.5]

plt.figure(figsize=(10, 6))  # Create a plot object to prepare for point plotting

# Print and Plot for each value of fp
for fp in fp_values:
    speedup_values = []
    print(f"\nSpeedup values for fp = {fp}:")
    for p in p_values:
        speedup = amdahls_law(fp, p)
        speedup_values.append(speedup)
        print(f"S({p}) = {speedup:.4f}")  # Print the speedup value for each p and fp value

    plt.plot(p_values, speedup_values, label=f'fp = {fp}', marker='o')  # Plot the speedup values

# Format and display the line graph
plt.xscale('log')  # Logarithmic scale for the x-axis (p, number of processors)
plt.xlabel('Number of Processors (p)')
plt.ylabel('Speedup (S(p))')
plt.title("Speedup Corresponding to Number of Processors (Amdahl's Law)")
plt.legend()
plt.grid(True)
plt.show()
