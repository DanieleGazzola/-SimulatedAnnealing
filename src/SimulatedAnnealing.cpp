//
// Created by danie on 15/12/2022.
//

#include <vector>
#include <cmath>
#include <random>
#include <mpi.h>
#include <iostream>
#include <chrono>
#include "SimulatedAnnealing.h"
#include "muParser.h"

mu::Parser getInitializedParser(int domainDimension, std::string function, std::vector<double> &solution){
    double PI = M_PI;
    mu::Parser parser;

    parser.SetExpr(function);
    parser.DefineVar("pi", &PI);

    setNewPoint(domainDimension, parser, solution);
    return parser;
}

void setNewPoint(int domainDimension, mu::Parser &parser, std::vector<double> &newPoint){
    for (int j = 0; j < domainDimension; ++j) {
        std::string arg = "x";
        arg += std::to_string(j + 1);
        parser.DefineVar(arg, &newPoint.at(j));
    }
}

void findMinimum(const Domain domain, const mu::Parser parser){
    double radius = (domain.upperBound(0) - domain.lowerBound(0)) / 5.;  //????????
    double fNew;

    std::vector<double> newPoint;
    newPoint.reserve(domain.getDimensions());
    newPoint.resize(domain.getDimensions());

    do{
        for (int i = 0; i < L; ++i) {
            newPoint = domain.generateNeighborhood(solution, radius);
            setNewPoint(domain.getDimensions(), parser, newPoint);

            fNew = parser.Eval();

            if(fNew < fSolution || std::exp((fSolution - fNew) / c) > domain.randomUnitary()){
                solution = newPoint;
                fSolution = fNew;
            }
        }
        c = c * alpha;
    } while (c > toll);
}

SimulatedAnnealing::SimulatedAnnealing(){
        void simulatedAnnealing(Domain domain, std::string function){
            int rank, size;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            MPI_Comm_size(MPI_COMM_WORLD, &size);

            solution = domain.generateInitialSolution(rank, size);
            mu::Parser parser = getInitializedParser(domain.getDimensions(), domain, solution);
            fSolution = parser.Eval();

            findMinimum(domain, parser);

            if(rank == 0){
                std::vector<double> tempSol;
                tempSol.reserve(domain.getDimensions());
                tempSol.resize(domain.getDimensions());
                double tempFSol;

                for (int i = 1; i < size; ++i) {
                    MPI_Recv(&tempSol[0], domain.getDimensions(), MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Recv(&tempFSol, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    if(tempFSol < fSolution){
                        solution = tempSol;
                        fSolution = tempFSol;
                    }
                }
            } else {
                MPI_Send(&solution[0], domain.getDimensions(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&fSolution, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
            }

        }
}