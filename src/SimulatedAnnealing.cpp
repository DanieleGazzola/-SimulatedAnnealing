#include <vector>
#include <cmath>
#include <random>
#include <mpi.h>
#include <iostream>
#include <chrono>
#include "SimulatedAnnealing.h"
#include "muParser.h"

mu::Parser SimulatedAnnealing::getInitializedParser(int domainDimension, std::string function, std::vector<double>& solution){
    mu::Parser parser;
    parser.SetExpr(function);
    setNewPoint(domainDimension, parser, solution);
    return parser;
}

void SimulatedAnnealing::setNewPoint(int domainDimension, mu::Parser& parser, std::vector<double>& newPoint){
    for (int j = 0; j < domainDimension; ++j) {
        std::string arg = "x";
        arg += std::to_string(j + 1);
        parser.DefineVar(arg, &newPoint.at(j));
    }
}

void SimulatedAnnealing::findMinimum(Domain domain, mu::Parser parser, int size){
    double fNew, tempFSolution = fSolution;
    std::vector<double> newPoint, tempSol = solution;
    newPoint.reserve(domain.getDimensions());
    newPoint.resize(domain.getDimensions());

    do{
        for (int i = 0; i < L; ++i) {
            newPoint = domain.generateNewPoint(tempSol, stepsize);
            setNewPoint(domain.getDimensions(), parser, newPoint);
            fNew = parser.Eval();

            if(fNew < tempFSolution || std::exp((tempFSolution - fNew) / c) > domain.randomUnitary()){
                if(fNew < fSolution){
                    solution = newPoint;
                    fSolution = fNew;
                }
                tempSol = newPoint;
                tempFSolution = fNew;
            }
        }
        c = c * alpha;
    } while (c > toll);
}


void SimulatedAnnealing::simulatedAnnealing(Domain domain, std::string function){
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    solution.reserve(domain.getDimensions());
    solution.resize(domain.getDimensions());
    stepsize.reserve(domain.getDimensions());
    stepsize.resize(domain.getDimensions());

    if (rank == 0){
        for (int i = 0; i < size; ++i){
            std::vector<double> startingPoint = domain.generateInitialSolution(i, size);
            std::vector<double> tempStepsize = domain.generateStepsize(i, size);
            if(i != 0){ 
                MPI_Send(&startingPoint[0], domain.getDimensions(), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                MPI_Send(&tempStepsize[0], domain.getDimensions(), MPI_DOUBLE, i, 1, MPI_COMM_WORLD); 
            }
            else{
                solution = startingPoint;
                stepsize = tempStepsize;
            }
        }
    }
    else{
        MPI_Recv(&solution[0], domain.getDimensions(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&stepsize[0], domain.getDimensions(), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    //std::cout << solution.at(0) << " " << solution.at(1) << " " << stepsize.at(0) << " " << stepsize.at(1) << std::endl;

    mu::Parser parser = getInitializedParser(domain.getDimensions(), function, solution);
    fSolution = parser.Eval();

    findMinimum(domain, parser, size);
    //std::cout << solution.at(0) << " " << solution.at(1) << " " << fSolution << std::endl;

    std::vector<double> tempSol;
    tempSol.reserve(domain.getDimensions());
    tempSol.resize(domain.getDimensions());
    double tempFSol;

    if (rank == 0){
        for (int i = 1; i < size; ++i){
            MPI_Recv(&tempSol[0], domain.getDimensions(), MPI_DOUBLE, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&tempFSol, 1, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

            if(tempFSol < fSolution){
                fSolution = tempFSol;
                solution = tempSol;
            }
        }

        for (int i = 1; i < size; ++i){
            MPI_Send(&solution[0], domain.getDimensions(), MPI_DOUBLE, i, 4, MPI_COMM_WORLD);
            MPI_Send(&fSolution, 1, MPI_DOUBLE, i, 5, MPI_COMM_WORLD); 
        }
    }
    else{
        MPI_Send(&solution[0], domain.getDimensions(), MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&fSolution, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
        MPI_Recv(&solution[0], domain.getDimensions(), MPI_DOUBLE, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&fSolution, 1, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }   
}