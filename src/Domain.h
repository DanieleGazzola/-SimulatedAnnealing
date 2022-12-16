//
// Created by danie on 15/12/2022.
//

#ifndef DOMAIN_H
#define DOMAIN_H

#include <string>
#include <vector>
#include <random>

class Domain {
public:
    explicit Domain(const std::string& filename);
    ~Domain() = default;

    std::string getFunction() { return this->function; }
    int getDimensions() const{ return this->dimensions; }
    double lowerBound() const{ return this->bounds.first; }
    double upperBound() const{ return this->bounds.second; }
    double random(std::uniform_real_distribution<double> dis) { return dis(engine); }

    std::vector<double> generateNeighborhood(std::vector<double>);

private:
    std::string function;
    int dimensions{};
    std::pair<double, double> bounds;
    std::mt19937 engine;

};


#endif //DOMAIN_H
