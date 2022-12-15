//
// Created by danie on 15/12/2022.
//

#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <mpi.h>
#include "Domain.h"

#define ARGC 2

int main(int argc, char** argv){

    if(argc != ARGC){
        std::cout << "Wrong number of command line arguments: " << argc << ". Should be: " << ARGC << std::endl;
        return -1;
    }

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Domain domain(argv[1]);

    auto start = std::chrono::system_clock::now();
    //funzione che fa tutto
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;

    if(rank == 0){
        std::cout << "Point found: " << "" << std::endl
                  << "Exact point: " << domain.exactSolutionToString() << std::endl
                  << "Error: " << "" << std::endl;
        std::cout << "Elapsed time: " << elapsed_seconds.count() << " s" << std::endl;
    }

    MPI_Finalize();

    return 0;
}