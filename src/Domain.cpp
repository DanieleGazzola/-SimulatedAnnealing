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
    inFile >> bounds.first;
    inFile >> bounds.second;

    if(bounds.first == bounds.second)
        std::exit(-1);

    if(bounds.first > bounds.second)
        std::swap(bounds.first, bounds.second);

    inFile.close();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    engine.seed(rank);
}

std::vector<double> Domain::generateNeighborhood(std::vector<double> centre) {

    std::uniform_real_distribution<double> maxStep(-0.1, 0.1);
    double move;

    for (int i = 0; i < dimensions; ++i){
        move = maxStep(engine);
        if(centre.at(i) + move > upperBound() || centre.at(i) + move < lowerBound()){
            i--;
        } else {
            centre.at(i) += move;
        }
    }

    return centre;
}
