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
void SimulatedAnnealing::findMinimum(Domain const & domain, std::vector<double> & currentSolution, double & currentFSolution){
    double fNew, tempFSolution{currentFSolution}, temp{T};
    std::vector<double> newPoint, tempSol = currentSolution;
    newPoint.resize(domain.getDimensions());

    while(temp > tol){
        for (int i = 0; i < L; ++i) {
            newPoint = domain.generateNewPoint(tempSol, stepsize);
            setNewPoint(domain.getDimensions(), newPoint);
            fNew = parser.Eval();

            if(fNew < tempFSolution || std::exp((tempFSolution - fNew) / temp) > domain.randomUnitary()){
                if(fNew < currentFSolution){
                    currentSolution = newPoint;
                    currentFSolution = fNew;
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
    tempSol.resize(dimensions);
    double tempFSol;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        for (int i = 1; i < size; ++i){
            MPI_Recv(tempSol.data(), dimensions, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&tempFSol, 1, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

            if(tempFSol < fSolution){
                fSolution = tempFSol;
                solution = tempSol;
            }
        }
    }
    else{
        MPI_Send(solution.data(), dimensions, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&fSolution, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    }   
}

//Performs simulatedAnnealing in parallel (if number of MPI processes > 1)
void SimulatedAnnealing::simulatedAnnealing(Domain const & domain, std::string const & function){
    int rank, size, dimensions = domain.getDimensions();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int numCurrentPoint = rank;

    std::vector<double> tempSolution;
    double tempFSolution;

    while(numCurrentPoint < numStartingPoints){
        tempSolution = domain.generateInitialSolution();
        stepsize = domain.generateStepsize();

        parser.SetExpr(function);
        setNewPoint(dimensions, tempSolution);
        tempFSolution = parser.Eval();
        findMinimum(domain, tempSolution, tempFSolution);

        if (numCurrentPoint == rank){
            fSolution = tempFSolution;
            solution = tempSolution;
        }
        else{
            if (fSolution > tempFSolution){
                fSolution = tempFSolution;
                solution = tempSolution;
            }
        }

        numCurrentPoint += size;
    }
    exchangeData(dimensions);
}