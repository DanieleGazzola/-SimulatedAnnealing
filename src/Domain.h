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
        std::vector<double> generateNeighborhood(std::vector<double>, double);

    private:
        int dimensions;
        std::vector<std::pair<double, double>> bounds;
        std::mt19937 engine;
};

#endif