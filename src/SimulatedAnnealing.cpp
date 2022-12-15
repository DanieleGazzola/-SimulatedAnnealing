//
// Created by danie on 15/12/2022.
//

#include <vector>
#include "Domain.h"

class SimulatedAnnealing{
public:
    SimulatedAnnealing() = default;
    ~SimulatedAnnealing() = default;

    void simulatedAnnealing(const Domain& domain){
        solution.at(0) = -0.2;
        solution.at(1) = 0.5;
    }

    std::vector<double> getSolution() { return this->solution; }

private:
    std::vector<double> solution;

};