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
mpicc -I../muparser-2.3.4/include main.cpp -o main -L../muparser-2.3.4 -lmuparser -lstdc++ -lm -fopenmp -Wall
mpiexec -n 4 ./main "../input"
```