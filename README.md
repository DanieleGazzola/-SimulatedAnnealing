# **SimulatedAnnealing**

# Problem description



# Dependencies
In order to use the Simulated Annealing Algorithm, you should download the repo and then perform the following commands from your terminal
```
cd muparser-2.3.4/
cmake . -DENABLE_SAMPLES=OFF -DENABLE_OPENMP=ON -DENABLE_WIDE_CHAR=OFF -DBUILD_SHARED_LIBS=ON
make
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jellyfish/shared-folder/-SimulatedAnnealing/muparser-2.3.4
```

## Command line arguments
```
First argument  : Input file path
Second argument : Number of iterations
Third argument  : T value
Fourth argument : alpha
Fifth argument  : tollerance
Sixth argument  : Number of points
```

## Input file arguments
```
First row  : Function used
Second row : Domain dimension
Other rows : bounds for each dimension
```

## Compiling Example
```
cd src/
make
mpiexec -n 2 ./main "../test/input15" 3000 100 0.95 0.0001 4
```

# Benchmark
To analize the performances associated to the program on both the single process case and the multiprocess one, go to the src folder and launch the 'make benchmark' command.
It will compile the program and launch it 4 times setting a different number of process each time.
```
cd src/
make benchmark
```
## **Eggholder function**
---
- ### **Description:**
    The chosen function on which has been performed the simulated annealing is the eggholder funtion because of the large number of local minima that makes it really difficult to obtain the correct result.

    The function is evaluated on the square $x_{i} ∈ [-512, 512]$ , for all i = 1, 2. 

- ### **Formula:**
    ${f(x) = - (x_{2}+47)\sin\sqrt{\left\lvert{x_{2}+\frac{x_{1}}{2}+47}\right\rvert} - x_{1}\sin\sqrt{\left\lvert{x_{1}-(x_{2}+47)}\right\rvert}}$

    ![Function](https://www.sfu.ca/~ssurjano/egg.png)

- ### **Global Minimum:**
    ${f(x^*) = -959.6407}$, at ${x^* = (512, 404.2319)}$
