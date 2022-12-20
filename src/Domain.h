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
    double lowerBound(int i) const{ return this->bounds.at(i).first; }
    double upperBound(int i) const{ return this->bounds.at(i).second; }
    double randomUnitary();

    std::vector<double> generateNeighborhood(std::vector<double>, double);

private:
    std::string function;
    int dimensions{};
    std::vector<std::pair<double, double>> bounds;
    std::mt19937 engine;

};


#endif //DOMAIN_H
