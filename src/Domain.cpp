//
// Created by danie on 15/12/2022.
//

#include "Domain.h"
#include <string>
#include <fstream>
#include <mpi.h>

Domain::Domain(const std::string& filename) {

    std::ifstream inFile;
    inFile.open(filename, std::ios_base::in);
    if(!inFile) exit(-1);

    inFile >> function;
    inFile >> dimensions;
    inFile >> bounds.first;
    inFile >> bounds.second;
    for (int i = 0; i < dimensions; ++i)
        inFile >> exactSolution.at(i);

    if(bounds.first == bounds.second)
        exit(-1);

    if(bounds.first > bounds.second)
        std::swap(bounds.first, bounds.second);

    inFile.close();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    engine.seed(rank);
}

std::string Domain::exactSolutionToString() {
    std::string s = std::to_string(exactSolution.at(0));
    for (int i = 1; i < dimensions; ++i)
        s.append(std::to_string(exactSolution.at(i)));
    return s;
}
