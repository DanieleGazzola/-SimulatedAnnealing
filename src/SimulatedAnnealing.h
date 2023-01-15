#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <cmath>
#include <iostream>
#include <mpi.h>
#include <random>
#include <vector>
#include "Domain.h"
#include "muParser.h"

class SimulatedAnnealing{
    public:
        explicit SimulatedAnnealing(int L, int numStartingPoints, double T, double alpha, double tol): 
            L{L}, 
            numStartingPoints{numStartingPoints},
            T{T},
            alpha{alpha},
            tol{tol}
        {}

        ~SimulatedAnnealing() = default;

        void simulatedAnnealing(Domain domain, std::string function);

        std::vector<double> getSolution() {return this->solution;}
        double getFSolution() const{return this->fSolution;}

    private:
        int L;
        int numStartingPoints;
        double T;
        double alpha;
        double tol;
        double fSolution;
        std::vector<double> solution;
        std::vector<double> stepsize;

        mu::Parser getInitializedParser(int& domainDimension, std::string function, std::vector<double>& solution);
        void findMinimum(Domain domain, mu::Parser parser, int size);
        void setNewPoint(int domainDimension, mu::Parser& parser, std::vector<double>& newPoint);
        void exchangeData(int& dimensions, int& numCurrentPoint);
};

#endif