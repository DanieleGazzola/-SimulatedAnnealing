# -SimulatedAnnealing

In order to use the Simulated Annealing Algorithm, you should download the repo and then perform the following commands from your terminal
```
cd muparser-2.3.4/
cmake . -DENABLE_SAMPLES=OFF -DENABLE_OPENMP=ON -DENABLE_WIDE_CHAR=OFF -DBUILD_SHARED_LIBS=ON
make
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jellyfish/shared-folder/-SimulatedAnnealing/muparser-2.3.4
```

## Command line arguments
---
```
Input file path
```

## Input file arguments
---
```
First row: Function used
Second row: Domain dimension
Other rows: bounds for each dimension
```

## Compiling Example
---
```
cd src/
make
mpiexec -n 1 ./main "../test/input1" 2000 10 0.99 0.001 5
```