#ifndef DOMAIN_H
#define DOMAIN_H

#include <iostream>
#include <random>
#include <string>
#include <vector>

using domainBounds = std::vector<std::pair<double, double>>;

class Domain {
    public:
        explicit Domain(domainBounds passedBounds, int rank);
        ~Domain() = default;

        int getDimensions() const { return this->dimensions; }
        double lowerBound(int i) const { return this->bounds[i].first; }
        double upperBound(int i) const { return this->bounds[i].second; }
        double randomUnitary();

        std::vector<double> generateInitialSolution();
        std::vector<double> generateStepsize(); //int& rank, int& size);
        std::vector<double> generateNewPoint(std::vector<double> currentPoint, std::vector<double>& stepsize);
        
    private:
        int dimensions;
        std::vector<std::pair<double, double>> bounds;
        std::mt19937 engine;
};

#endif