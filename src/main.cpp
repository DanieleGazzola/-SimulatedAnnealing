#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <mpi.h>
#include "Domain.h"
#include "SimulatedAnnealing.h"

using domainBounds = std::vector<std::pair<double, double>>;

//Reads the function and the values ​​contained in the input file
void readFile(const std::string& filename, domainBounds& bounds, std::string& function){
    int dimensions;
    std::ifstream inFile;
    inFile.open(filename, std::ios_base::in);

    if(!inFile) std::exit(-1);

    inFile >> function;
    inFile >> dimensions;

    if(dimensions < 1) std::exit(-1);
    
    if(dimensions > 0){
        bounds.resize(dimensions);  

        for (int i = 0; i < dimensions; ++i) {
            inFile >> bounds[i].first;
            inFile >> bounds[i].second;

            if(bounds[i].first == bounds[i].second)
                std::exit(-1);

            if(bounds[i].first > bounds[i].second)
                std::swap(bounds[i].first, bounds[i].second);
        }
    }
    inFile.close();
}

int main(int argc, char** argv){

    if(argc != 7){
        std::cout << "Wrong number of command line arguments" << std::endl;
        return -1;
    }

    domainBounds bounds;
    std::string function;
    readFile(argv[1], bounds, function);

    int rank, size;
    int const L = std::stoi(argv[2]);
    int const numStartingPoints = std::stoi(argv[6]);
    double const T = std::stod(argv[3]);
    double alpha = std::stod(argv[4]);
    double const tol = std::stod(argv[5]);
    
    if(alpha < 0.8 || alpha > 0.99){ alpha = 0.95; }
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Domain domain(bounds, rank);
    SimulatedAnnealing SA(L, numStartingPoints, T, alpha, tol);

    auto start = std::chrono::system_clock::now();
    SA.simulatedAnnealing(domain, function);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    if(rank == 0){
        std::cout << "Point found: (";
        for (int j = 0; j < domain.getDimensions(); ++j){
            std::cout << SA.getBestSolution().at(j);
            if(j < domain.getDimensions() -1)
                std::cout << ", ";
        }
        std::cout << ")"<<std::endl;
        std::cout << "F(x) = " << SA.getBestFSolution() << std::endl;
        std::cout << "Elapsed time: " << elapsed_seconds.count() << " s" << std::endl;
    }

    MPI_Finalize();

    return 0;
}