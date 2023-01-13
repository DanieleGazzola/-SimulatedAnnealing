#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <vector>
#include <cmath>
#include <random>
#include <mpi.h>
#include <iostream>
#include <chrono>
#include "Domain.h"
#include "muParser.h"

class SimulatedAnnealing{
    public:
        SimulatedAnnealing(): L{1000}, 
            c{0.99},
            alpha{0.95},
            toll{0.001},
            fSolution{}
        {}

        SimulatedAnnealing(int & L, double & c, double & alpha, double & toll): 
            L{L}, 
            c{c},
            alpha{alpha},
            toll{toll}
        {}

        ~SimulatedAnnealing() = default;

        void simulatedAnnealing(Domain domain);

        std::vector<double> getSolution() {return this->solution;}
        double getFSolution() const{return this->fSolution;}

    private:
        int L;
        double c;
        double alpha;
        double toll;
        double fSolution;
        std::vector<double> solution;
};

#endif