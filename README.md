# SimulatedAnnealing

## Description
Simulated annealing is one of the simplest and best-known metaheuristic methods for solving global optimization problems.
It aims to find the global optimum of a non-convex function.
Is used in problems where the optimum global research is constrained by a fixed amount of time to perform the algorithm. The name comes from annealing in metallurgy, a technique involving heating and controlled cooling of a material to alter its physical properties.

### <ins>Main idea</ins>
Given an initial random location ${x}$, the algorithm perform a sample of a nearby location.
If ${f(x_{sample}) <= f(x)}$, the sample is accepted. 
If ${f(x_{sample}) > f(x)}$, the sample is accepted with a probability that depends on a variable T (the temperature).
The formula used to calculate the acceptance probability is the following one: ${Pr(Accept) = \exp{(\frac{f(x_{sample}) - f(x)}{T})}}$.

The algorithm starts with a high value of temperature, so
that it will accept also moves that increase the objective function.
The optimum research is stopped when the variable T goes under a certain tolerance.
For each value assumed by the variable T, are going to be performed L sampling and after those sampling T will be decreased by multiplying it for ${\alpha ∈ (0.8, 1)}$.

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
### <ins>Parallel implementation</ins>
Usually, to obtain a more accurate solution, the sequential algorithm is performed starting from multiple points and choosing the results given by the best solution.

The parallel implementation is based on performing the execution of the algorithm on multiple initial points at the same time. The used approach may be described as follows:

1. The MPI program initially generates P processes that have ranks 0, ..., P-1;
2. Each process generates a random starting point and a random stepsize; 
3. Each process executes the sequential algorithm;
4. At the end of the sequential algorithm, each process sends the obtained results to the process with rank 0. The process with rank 0, collects all the results and keeps just the best one;
5. If the number of points on which has been performed the algorithm matches the one provided by the user, the results are displayed on the screen, else return to point 2 maintaining alive the necessary processes to cover the remaining points.

---
## Dependencies
In order to use the Simulated Annealing Algorithm, run the following commands after downloading the repository
```
cd muparser-2.3.4/
cmake . -DENABLE_SAMPLES=OFF -DENABLE_OPENMP=ON -DENABLE_WIDE_CHAR=OFF -DBUILD_SHARED_LIBS=ON
make
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD
```
---
## How to run the code
## Example
```
cd src/
mpic++ -I../muparser-2.3.4/include main.cpp Domain.cpp SimulatedAnnealing.cpp -o main -L../muparser-2.3.4 -lmuparser -lstdc++ -lm -fopenmp -Wall
mpiexec -n 2 ./main "../test/input1" 3000 100 0.95 0.0001 4
```

## Input arguments
To use the program it is necessary to provide the simulated annealing parameters via command line and all the information related to the function and the associated domain via input file.

### Command line arguments
```
First argument  : Input file path
Second argument : Number of iterations performed for each value assumed by the temperature
Third argument  : Temperature
Fourth argument : Alpha - Temperature reduction factor
Fifth argument  : Tolerance
Sixth argument  : Number of initial points from which perform the simulated annealing
```

### Input file arguments
```
First row  : Used function
Second row : Domain dimension
Other rows : Bounds for each dimension
```
---

## Benchmark
To analize the performance of the program on both the single-process and the multi-process case, run the following commands.
It will compile the program and run it 4 times but with different number of processes.

```
cd src/
make
```
### **Eggholder function**
- **Description:**

    The function used as benchmark is the eggholder function because of the large number of local minima that makes really difficult to obtain the correct result.

    The function is evaluated on the square $x_{i} ∈ [-512, 512]$ , for all i = 1, 2. 

-  **Formula:**
    
    ${f(x) = - (x_{2}+47)\sin\sqrt{\left\lvert{x_{2}+\frac{x_{1}}{2}+47}\right\rvert} - x_{1}\sin\sqrt{\left\lvert{x_{1}-(x_{2}+47)}\right\rvert}}$
    
<p align="center">
<img src="https://www.sfu.ca/~ssurjano/egg.png" alt="Eggholder function" title="Eggholder function">
</p>

-  **Global Minimum:**
    
    ${f(x*) = -959.6407}$, at ${x* = (512, 404.2319)}$
