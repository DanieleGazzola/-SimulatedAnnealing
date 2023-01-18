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
        void simulatedAnnealing(Domain domain, std::string function);

        std::vector<double> getSolution() {return this->solution;}
        double getFSolution() const{return this->fSolution;}

    private:
        int L;                          //Number of iterations performed for each value assumed by the temperature
        int numStartingPoints;          //Number of initial points from which perform the simulated annealing
        double T;                       //Temperature
        double alpha;                   //Temperature reduction factor
        double tol;                     //Tolerance
        double fSolution;               //Solution image
        std::vector<double> solution;   
        std::vector<double> stepsize;   //Maximum shift that can be made to calculate the neighbor of a given point

        //Returns an initialized parser
        mu::Parser getInitializedParser(int& domainDimension, std::string function, std::vector<double>& solution);
        
        //Perform the sequential annealing
        void findMinimum(Domain domain, mu::Parser parser, int size);
        
        //Set the parser to evaluate the function in a new point
        void setNewPoint(int domainDimension, mu::Parser& parser, std::vector<double>& newPoint);

        //Manages data exchange between different MPI processes
        void exchangeData(int& dimensions, int& numCurrentPoint);
};

#endif