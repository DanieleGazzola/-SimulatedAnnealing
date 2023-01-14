#ifndef DOMAIN_H
#define DOMAIN_H

#include <iostream>
#include <string>
#include <vector>
#include <random>

using domainBounds = std::vector<std::pair<double, double>>;

class Domain {
    public:
        explicit Domain(domainBounds passedBounds, int rank);
        ~Domain() = default;

        int getDimensions() const { return this->dimensions; }
        double lowerBound(int i) const { return this->bounds.at(i).first; }
        double upperBound(int i) const { return this->bounds.at(i).second; }
        double randomUnitary();

        std::vector<double> generateInitialSolution(int& rank, int& size);
        std::vector<double> generateStepsize(int& rank, int& size);
        //std::vector<double> generateNewPoint(std::vector<double> centre, double radius);
        std::vector<double> generateNewPoint(std::vector<double> currentPoint, std::vector<double>& stepsize);
        
    private:
        int dimensions;
        std::vector<std::pair<double, double>> bounds;
        std::mt19937 engine;
};

#endif