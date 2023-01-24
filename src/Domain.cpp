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
    initialSolution.reserve(dimensions);
    
    for(int i = 0; i < dimensions; ++i){
        initialSolution.emplace_back(bounds[i].first + (bounds[i].second - bounds[i].first) 
        * randomUnitary()); 
    }
    return initialSolution;
}

//Returns the maximum shift that can be made to calculate the neighbor of a given point
std::vector<double> Domain::generateStepsize() const{
    std::vector<double> stepsize;
    stepsize.reserve(dimensions);

    for(int i = 0; i < dimensions; ++i){
        stepsize.emplace_back((bounds[i].second - bounds[i].first) * randomUnitary());
    }   

    return stepsize;
}

//Returns a new point, generated starting from the one provided as a parameter by moving it by a maximum length contained in the stepsize
std::vector<double> Domain::generateNewPoint(std::vector<double> const & currentPoint, std::vector<double> const & stepsize) const{
    std::vector<double> newPoint;
    newPoint.resize(dimensions);
    double shrinkingFactor = randomUnitary();

    for (int i = 0; i < dimensions; ++i){
        if(randomUnitary() < 0.5){
            if((currentPoint[i] + stepsize[i]*shrinkingFactor) <= bounds[i].second){
                newPoint[i] = (currentPoint[i] + stepsize[i]*shrinkingFactor);
            }
            else{
                newPoint[i] = bounds[i].second;
            }
        }
        else{
            if((currentPoint[i] - stepsize[i]*shrinkingFactor) >= bounds[i].first){
                newPoint[i] = (currentPoint[i] - stepsize[i]*shrinkingFactor );
            }
            else{
                newPoint[i] = bounds[i].first;
            }
        }
    }

    return newPoint;
}