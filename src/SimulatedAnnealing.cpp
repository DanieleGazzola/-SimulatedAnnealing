//
// Created by danie on 15/12/2022.
//

#include <vector>
#include <cmath>
#include <random>
#include <mpi.h>
#include <iostream>
#include <chrono>
#include "Domain.h"
#include "muParser.h"

mu::Parser getInitializedParser(Domain &domain, std::vector<double> &solution){
    double PI = M_PI;
    mu::Parser parser;

    parser.SetExpr(domain.getFunction());
    parser.DefineVar("pi", &PI);

    for (int i = 0; i < domain.getDimensions(); ++i) {
        std::string arg = "x";
        arg += std::to_string(i + 1);
        parser.DefineVar(arg, &solution.at(i));
    }

    return parser;
}

class SimulatedAnnealing{
    public:
        SimulatedAnnealing() = default;
        ~SimulatedAnnealing() = default;

        void simulatedAnnealing(Domain domain){
            int rank, size;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            MPI_Comm_size(MPI_COMM_WORLD, &size);

            solution = domain.generateInitialSolution(rank, size);
            mu::Parser parser = getInitializedParser(domain, solution);

            fSolution = parser.Eval();

            //da far diventare parametri
            double c = 0.99;
            int L = 1000;
            double alpha = 0.95;
            double toll = 0.001;

            double radius = (domain.upperBound(0) - domain.lowerBound(0)) / 5.;

            std::vector<double> newPoint;
            double fNew;
            newPoint.reserve(domain.getDimensions());
            newPoint.resize(domain.getDimensions());

            do{
                for (int i = 0; i < L; ++i) {

                    newPoint = domain.generateNeighborhood(solution, radius);

                    for (int j = 0; j < domain.getDimensions(); ++j) {
                        std::string arg = "x";
                        arg += std::to_string(j + 1);
                        parser.DefineVar(arg, &newPoint.at(j));
                    }

                    fNew = parser.Eval();

                    if(fNew < fSolution || std::exp((fSolution - fNew) / c) > domain.randomUnitary()){
                        solution = newPoint;
                        fSolution = fNew;
                    }

                }

                c = c * alpha;

            } while (c > toll);

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

        std::vector<double> getSolution() { return this->solution; }
        double getFSolution() const{return this->fSolution; }

    private:
        std::vector<double> solution;
        double fSolution{};
};