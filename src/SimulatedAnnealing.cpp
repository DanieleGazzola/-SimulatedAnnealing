#include <cmath>
#include <iostream>
#include <mpi.h>
#include <random>
#include <vector>
#include "Domain.h"
#include "muParser.h"
#include "SimulatedAnnealing.h"

//Returns an initialized parser
mu::Parser SimulatedAnnealing::getInitializedParser(int& domainDimension, std::string function, std::vector<double>& solution){
    mu::Parser parser;
    parser.SetExpr(function);
    setNewPoint(domainDimension, parser, solution);
    return parser;
}

//Set the parser to evaluate the function in a new point
void SimulatedAnnealing::setNewPoint(int domainDimension, mu::Parser& parser, std::vector<double>& newPoint){
    for (int j = 0; j < domainDimension; ++j) {
        std::string arg = "x";
        arg += std::to_string(j + 1);
        parser.DefineVar(arg, &newPoint[j]);
    }
}

//Perform the sequential annealing:
//External loop deals with the temperature
//Internal loop deals with the number of iterations at each temperature
void SimulatedAnnealing::findMinimum(Domain domain, mu::Parser parser){
    double fNew, tempFSolution{fSolution}, temp{T};
    std::vector<double> newPoint, tempSol = solution;
    newPoint.reserve(domain.getDimensions());
    newPoint.resize(domain.getDimensions());

    while(temp > tol){
        for (int i = 0; i < L; ++i) {
            newPoint = domain.generateNewPoint(tempSol, stepsize);
            setNewPoint(domain.getDimensions(), parser, newPoint);
            fNew = parser.Eval();

            if(fNew < tempFSolution || std::exp((tempFSolution - fNew) / temp) > domain.randomUnitary()){
                if(fNew < fSolution){
                    solution = newPoint;
                    fSolution = fNew;
                }
                tempSol = newPoint;
                tempFSolution = fNew;
            }
        }
        temp = temp * alpha;
    }
}

//Manages data exchange between different MPI processes
void SimulatedAnnealing::exchangeData(int& dimensions){ 
    std::vector<double> tempSol;
    tempSol.reserve(dimensions);
    tempSol.resize(dimensions);
    double tempFSol;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        for (int i = 1; i < size; ++i){
            MPI_Recv(&tempSol[0], dimensions, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&tempFSol, 1, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

            if(tempFSol < fSolution){
                fSolution = tempFSol;
                solution = tempSol;
            }
        }
    }
    else{
        MPI_Send(&solution[0], dimensions, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&fSolution, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    }   
}

//Performs simulatedAnnealing in parallel (if number of MPI processes > 1)
void SimulatedAnnealing::simulatedAnnealing(Domain domain, std::string function){
    int rank, size, dimensions = domain.getDimensions();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int numCurrentPoint = rank;

    std::vector<double> bestSolution;
    double bestFSolution;
    solution.reserve(dimensions);
    solution.resize(dimensions);
    stepsize.reserve(dimensions);
    stepsize.resize(dimensions);
    bestSolution.reserve(dimensions);
    bestSolution.resize(dimensions);

    while(numCurrentPoint < numStartingPoints){
        solution = domain.generateInitialSolution();
        stepsize = domain.generateStepsize();

        mu::Parser parser = getInitializedParser(dimensions, function, solution);
        fSolution = parser.Eval();
        findMinimum(domain, parser);

        if (numCurrentPoint == rank){
            bestFSolution = fSolution;
            bestSolution = solution;
        }
        else{
            if (bestFSolution > fSolution){
                bestFSolution = fSolution;
                bestSolution = solution;
            }
        }

        numCurrentPoint += size;
    }
    fSolution = bestFSolution;
    solution = bestSolution;
    exchangeData(dimensions);
}