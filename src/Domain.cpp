#include "Domain.h"

using domainBounds = std::vector<std::pair<double, double>>;

//Constructor
Domain::Domain(domainBounds const & passedBounds, int rank){
    bounds = passedBounds;
    dimensions = bounds.size();
    int rand = std::rand();
    engine.seed(rand * rank);
}

//Returns a random value between 0 and 1
double Domain::randomUnitary() const{
    std::uniform_real_distribution<double> unitary(0., 1.);
    return unitary(engine);
}

//Returns the coordinates of an initial solution contained in the corresponding domain
std::vector<double> Domain::generateInitialSolution() const{
    std::vector<double> initialSolution;
    initialSolution.resize(dimensions);
    
    for(int i = 0; i < dimensions; ++i){
        initialSolution[i] = bounds[i].first + (bounds[i].second - bounds[i].first) * randomUnitary(); 
    }

    return initialSolution;
}

//Returns the maximum shift that can be made to calculate the neighbor of a given point
std::vector<double> Domain::generateStepsize() const{
    std::vector<double> stepsize;
    stepsize.resize(dimensions);

    for(int i = 0; i < dimensions; ++i){
        stepsize[i] = (bounds[i].second - bounds[i].first) * randomUnitary();
    }   

    return stepsize;
}

//Returns a new point, generated starting from the one provided as a parameter by moving it by a maximum length contained in the stepsize
std::vector<double> Domain::generateNewPoint(std::vector<double>& currentPoint, std::vector<double>& stepsize) const{
    std::vector<double> newPoint;
    newPoint.resize(dimensions);
    double shrinkingFactor = randomUnitary() * 2 - 1;

    for (int i = 0; i < dimensions; ++i){
        double newCoordinate = currentPoint[i] + stepsize[i]*shrinkingFactor;

        if(newCoordinate < bounds[i].first){
            newPoint[i] = bounds[i].first;
        }       
        else if(newCoordinate > bounds[i].second){
            newPoint[i] = bounds[i].second;
        }
        else {
            newPoint[i] = (currentPoint[i] + stepsize[i]*shrinkingFactor);
        }
    }

    return newPoint;
}