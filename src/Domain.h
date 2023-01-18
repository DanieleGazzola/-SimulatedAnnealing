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

        //Returns a random value between 0 and 1
        double randomUnitary();

        //Returns the coordinates of an initial solution contained in the corresponding domain
        std::vector<double> generateInitialSolution();

        //Returns the maximum shift that can be made to calculate the neighbor of a given point
        std::vector<double> generateStepsize();

        //Returns a new point, generated starting from the one provided as a parameter by moving it by a maximum length contained in the stepsize
        std::vector<double> generateNewPoint(std::vector<double> currentPoint, std::vector<double>& stepsize);
        
    private:
        int dimensions;
        std::vector<std::pair<double, double>> bounds;
        std::mt19937 engine;
};

#endif