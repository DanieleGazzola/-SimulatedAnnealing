#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <mpi.h>
#include "omp.h"
#include "Domain.h"
#include "SimulatedAnnealing.h"

using domainBounds = std::vector<std::pair<double, double>>;

void readFile(const std::string& filename, domainBounds& bounds, std::string& function){
    int dimensions;
    std::ifstream inFile;
    inFile.open(filename, std::ios_base::in);

    if(!inFile) std::exit(-1);

    inFile >> function;
    inFile >> dimensions;

    if(dimensions < 1) std::exit(-1);
    
    if(dimensions > 0){
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
    }

    inFile.close();
}

int main(int argc, char** argv){

    if(argc != 6){
        std::cout << "Wrong number of command line arguments" << std::endl;
        return -1;
    }

    domainBounds bounds;
    std::string function;
    readFile(argv[1], bounds, function);

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Domain domain(bounds, rank);
    SimulatedAnnealing SA(std::stoi(argv[2]), std::stod(argv[3]), std::stod(argv[4]), std::stod(argv[5]));

    auto start = std::chrono::system_clock::now();
    SA.simulatedAnnealing(domain, function);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    if(rank == 0){
        std::cout << "Point found: ";
        for (int j = 0; j < domain.getDimensions(); ++j)
            std::cout << SA.getSolution().at(j) << " ";
        std::cout << std::endl;
        std::cout << "F(x) = " << SA.getFSolution() << std::endl;
        std::cout << "Elapsed time: " << elapsed_seconds.count() << " s" << std::endl;
    }

    MPI_Finalize();

    return 0;
}