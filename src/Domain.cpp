//
// Created by danie on 15/12/2022.
//

#include "Domain.h"
#include <string>
#include <fstream>
#include <mpi.h>
#include <iostream>

Domain::Domain(const std::string& filename) {

    std::ifstream inFile;
    inFile.open(filename, std::ios_base::in);
    if(!inFile)
        std::exit(-1);

    inFile >> function;
    inFile >> dimensions;

    bounds.reserve(dimensions);
    bounds.resize(dimensions);

    for (int i = 0; i < dimensions; ++i) {
        inFile >> bounds.at(i).first;
        inFile >> bounds.at(i).second;

        if(bounds.at(i).first == bounds.at(i).second)
            std::exit(-1);

        if(bounds.at(i).first > bounds.at(i).second)
            std::swap(bounds.at(i).first, bounds.at(i).second);
    }

    inFile.close();

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    engine.seed(rank);

    double partialSize0 = (bounds.at(0).second - bounds.at(0).first) / (size / 2.);
    double partialSize1 = (bounds.at(1).second - bounds.at(1).first) / 2.;

    bounds.at(0).first = bounds.at(0).first + (rank % (size / 2)) * partialSize0;
    bounds.at(0).second = bounds.at(0).first + partialSize0;

    if(dimensions == 2){
        bounds.at(1).first = bounds.at(1).first + ((rank < (size / 2)) ? 0 : 1) * partialSize1;
        bounds.at(1).second = bounds.at(1).first + partialSize1;
    }

}

std::vector<double> Domain::generateNeighborhood(std::vector<double> centre, double radius) {

    std::uniform_real_distribution<double> maxStep(-radius, radius);
    std::vector<double> moves;
    double dist;
    bool check = true;

    moves.reserve(dimensions);
    moves.resize(dimensions);

    do{
        dist = 0.;

        for (int i = 0; i < dimensions; ++i) {
            moves.at(i) = maxStep(engine);

            if(centre.at(i) + moves.at(i) > upperBound(i)){
                moves.at(i) = upperBound(i) - centre.at(i);
            } else if(centre.at(i) + moves.at(i) < lowerBound(i)){
                moves.at(i) = lowerBound(i) - centre.at(i);
            }

            dist += moves.at(i) * moves.at(i);
        }

        if(dist <= radius * radius)
            check = false;

    } while(check);

    for (int i = 0; i < dimensions; ++i)
        centre.at(i) += moves.at(i);

    return centre;
}

double Domain::randomUnitary() {
    std::uniform_real_distribution<double> unitary(0., 1.);

    return unitary(engine);
}
