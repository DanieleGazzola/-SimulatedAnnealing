#include "SimulatedAnnealing.h"

//Set the parser to evaluate the function in a new point
void SimulatedAnnealing::setNewPoint(int const & domainDimension, std::vector<double> & newPoint){
    for (int j = 0; j < domainDimension; ++j) {
        std::string arg = "x";
        arg += std::to_string(j + 1);
        parser.DefineVar(arg, &newPoint[j]);
    }
}

//Perform the sequential annealing:
//External loop deals with the temperature
//Internal loop deals with the number of iterations at each temperature
void SimulatedAnnealing::findMinimum(Domain const & domain, std::vector<double> & solution, double & fSolution){
    double fNew, tempFSolution{fSolution}, temp{T};
    std::vector<double> newPoint, tempSol = solution;
    newPoint.resize(domain.getDimensions());
    setNewPoint(domain.getDimensions(), newPoint);

    while(temp > tol){
        for (int i = 0; i < L; ++i) {
            newPoint = domain.generateNewPoint(tempSol, stepsize);         
            fNew = parser.Eval();

            if(fNew < tempFSolution || std::exp((tempFSolution - fNew) / temp) > domain.randomUnitary()){
                if(fNew < fSolution){
                    solution = newPoint;
                    fSolution = fNew;
                }
                tempSol = newPoint;
                tempFSolution = fNew;
            }
        }
        temp = temp * alpha;
    }
}

//Manages data exchange between different MPI processes
void SimulatedAnnealing::exchangeData(int const & dimensions){ 
    std::vector<double> tempSol;
    tempSol.reserve(dimensions);
    double tempFSol;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        for (int i = 1; i < size; ++i){
            MPI_Recv(tempSol.data(), dimensions, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&tempFSol, 1, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

            if(tempFSol < bestFSolution){
                bestFSolution = tempFSol;
                bestSolution = tempSol;
            }
        }
    }
    else{
        MPI_Send(bestSolution.data(), dimensions, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&bestFSolution, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    }   
}

//Performs simulatedAnnealing in parallel (if number of MPI processes > 1)
void SimulatedAnnealing::simulatedAnnealing(Domain const & domain, std::string const & function){
    int rank, size, dimensions = domain.getDimensions();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int numCurrentPoint = rank;

    std::vector<double> solution;
    double fSolution;

    while(numCurrentPoint < numStartingPoints){
        solution = domain.generateInitialSolution();
        stepsize = domain.generateStepsize();

        parser.SetExpr(function);
        setNewPoint(dimensions, solution);
        fSolution = parser.Eval();
        findMinimum(domain, solution, fSolution);

        if (numCurrentPoint == rank){
            bestFSolution = fSolution;
            bestSolution = solution;
        }
        else{
            if (bestFSolution > fSolution){
                bestFSolution = fSolution;
                bestSolution = solution;
            }
        }

        numCurrentPoint += size;
    }
    exchangeData(dimensions);
}