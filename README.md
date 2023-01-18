# SimulatedAnnealing

## Description
Simulated annealing is one of the simplest and best-known metaheuristic methods for solving global optimization problems.
It aims to find the global optimum of a non-convex function.
Is used in problem where the global optimum research is constrained by a fixed amount of time to perform the algorithm. The name comes from annealing in metallurgy, a technique involving heating and controlled cooling of a material to alter its physical properties.

### Main idea
Given an initial random location ${x}$, the algorithm perform a sample of a nearby location.
If ${f(x_{sample}) <= f(x)}$, the sample is accepted. 
If ${f(x_{sample}) > f(x)}$, the sample is accepted with a probability that depends on a variable T (the temperature).
The formula used to calculate the acceptance probability is the following one: ${Pr(Accept) = \exp{(\frac{f(x_{sample}) - f(x)}{T})}}$.

The algorithm starts with an high value of temperature, so
that it will accept also moves that increase the objective function, while eventually it only accepts
samples giving lower values.
The optimum research is stopped when the variable T goes under a certain tollerance.
For each value assumed by the variable T, are going to be performed L sampling and after those sampling T will be decreased by multipling it for ${\alpha ∈ (0.8, 1)}$.

**PSEUDOCODE**
```
Initialization x, L, T, tol, alpha
while T > tol
    for i = 0, ..., L:
        Sample y, neighbor of x;
        if f(y) < f(x) then x := y (y becomes the current solution);
        else, y becomes the current solution with probability exp((f(y)-f(x))/T);
    T = T * alpha;
```
### Parallel implementation
Usually, to obtain a more accurate solution, the previous steps are performed starting from multiple points and choosing the results given by the best solution.

The parallel implementation is based on performing the execution of the algorithm on multiple initial points at the same time. The used approach may be described as follows:

1. The MPI program initially generates P processes that have ranks 0, ..., P-1;
2. Each process generates a random starting point and a random stepsize; 
3. Each process executes the sequential algorithm;
4. At the end of the sequential algorithm, each process sends the obtained results to the process with rank 0. Process with rank 0, collect all the results and keep just the best one;
5. If the number of points on which has been performed the algorithm matches the one provided by the user, the results are displayed on the screen, else return to the point 2 mantaining alive the necessary processes to cover the remaining points.

### Code structure
Scrivere Code structure

---
## Dependencies
In order to use the Simulated Annealing Algorithm, you should download the repo and then perform the following commands from your terminal
```
cd muparser-2.3.4/
cmake . -DENABLE_SAMPLES=OFF -DENABLE_OPENMP=ON -DENABLE_WIDE_CHAR=OFF -DBUILD_SHARED_LIBS=ON
make
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jellyfish/shared-folder/-SimulatedAnnealing/muparser-2.3.4
```

### Command line arguments
```
First argument  : Input file path
Second argument : Number of iterations
Third argument  : T value
Fourth argument : alpha
Fifth argument  : tollerance
Sixth argument  : Number of points
```

### Input file arguments
```
First row  : Function used
Second row : Domain dimension
Other rows : bounds for each dimension
```

### Compiling Example
```
cd src/
make
mpiexec -n 2 ./main "../test/input15" 3000 100 0.95 0.0001 4
```
---

## Benchmark
To analize the performances associated to the program on both the single process case and the multiprocess one, go to the src folder and launch the 'make benchmark' command.
It will compile the program and launch it 4 times setting a different number of process each time.
```
cd src/
make benchmark
```
### **Eggholder function**
- **Description:**

    The chosen function on which has been performed the simulated annealing is the eggholder function because of the large number of local minima that makes really difficult to obtain the correct result.

    The function is evaluated on the square $x_{i} ∈ [-512, 512]$ , for all i = 1, 2. 

-  **Formula:**
    
    ${f(x) = - (x_{2}+47)\sin\sqrt{\left\lvert{x_{2}+\frac{x_{1}}{2}+47}\right\rvert} - x_{1}\sin\sqrt{\left\lvert{x_{1}-(x_{2}+47)}\right\rvert}}$
    
<p align="center">
<img src="https://www.sfu.ca/~ssurjano/egg.png" alt="Eggholder function" title="Eggholder function">
</p>

-  **Global Minimum:**
    
    ${f(x*) = -959.6407}$, at ${x* = (512, 404.2319)}$
