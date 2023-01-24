#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <cmath>
#include <iostream>
#include <mpi.h>
#include <random>
#include <vector>
#include "Domain.h"
#include "muParser.h"

class SimulatedAnnealing{
    public:
        explicit SimulatedAnnealing(int L, int numStartingPoints, double T, double alpha, double tol): 
            L{L}, 
            numStartingPoints{numStartingPoints},
            T{T},
            alpha{alpha},
            tol{tol}
        {}

        ~SimulatedAnnealing() = default;

        //Performs simulatedAnnealing in parallel (if number of MPI processes > 1)
        void simulatedAnnealing(Domain const & domain, std::string const & function);

        std::vector<double> getSolution() const {return this->solution;}
        double getFSolution() const {return this->fSolution;}

    private:
        int L;                          //Number of iterations performed for each value assumed by the temperature
        int numStartingPoints;          //Number of initial points from which perform the simulated annealing
        double T;                       //Temperature
        double alpha;                   //Temperature reduction factor
        double tol;                     //Tolerance
        double fSolution;               //Solution image
        mu::Parser parser;              //Parser
        std::vector<double> solution;   
        std::vector<double> stepsize;   //Maximum shift that can be made to calculate the neighbor of a given point
        
        //Perform the sequential annealing
        void findMinimum(Domain const & domain, std::vector<double> & currentSolution, double & currentFSolution);
        
        //Set the parser to evaluate the function in a new point
        void setNewPoint(int const & domainDimension, std::vector<double> & newPoint);

        //Manages data exchange between different MPI processes
        void exchangeData(int const & dimensions);
};

#endif