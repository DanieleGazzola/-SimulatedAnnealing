.PHONY: all
all: main

main: main.cpp Domain.cpp SimulatedAnnealing.cpp Domain.h SimulatedAnnealing.h
	mpicc -I../muparser-2.3.4/include main.cpp Domain.cpp SimulatedAnnealing.cpp -o main -L../muparser-2.3.4 -lmuparser -lstdc++ -lm -fopenmp -Wall
	mpiexec -n 1 ./main "../test/input5" 1000 200 0.95 0.001 7
	mpiexec -n 2 ./main "../test/input5" 1000 200 0.95 0.001 7
	mpiexec -n 3 ./main "../test/input5" 1000 200 0.95 0.001 7
	mpiexec -n 4 ./main "../test/input5" 1000 200 0.95 0.001 7
	rm -f main

.PHONY: clear
clear:
	rm -f main