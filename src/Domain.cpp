#include "Domain.h"
#include <string>
#include <vector>
#include <mpi.h>
#include <iostream>
#include <random>

using domainBounds = std::vector<std::pair<double, double>>;

Domain::Domain(domainBounds passedBounds, int rank){
    bounds = passedBounds;
    dimensions = bounds.size();
    std::srand(time(nullptr));
    int rand = std::rand();
    engine.seed(rand * rank);
}

/*std::vector<double> Domain::generateNewPoint(std::vector<double> centre, double radius) {

    std::uniform_real_distribution<double> maxStep(-radius, radius);
    std::vector<double> moves;
    double dist;
    bool check = true;

    moves.reserve(dimensions);
    moves.resize(dimensions);

    do{
        dist = 0.;

        for (int i = 0; i < dimensions; ++i) {
            moves.at(i) = maxStep(engine);

            if(centre.at(i) + moves.at(i) > upperBound(i)){
                moves.at(i) = upperBound(i) - centre.at(i);
            } else if(centre.at(i) + moves.at(i) < lowerBound(i)){
                moves.at(i) = lowerBound(i) - centre.at(i);
            }

            dist += moves.at(i) * moves.at(i);
        }

        if(dist <= radius * radius)
            check = false;

    } while(check);

    for (int i = 0; i < dimensions; ++i)
        centre.at(i) += moves.at(i);

    return centre;
}*/

std::vector<double> Domain::generateNewPoint(std::vector<double> currentPoint, std::vector<double>& stepsize){
    std::vector<double> newPoint;
    newPoint.reserve(dimensions);
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

double Domain::randomUnitary() {
    std::uniform_real_distribution<double> unitary(0., 1.);

    return unitary(engine);
}

std::vector<double> Domain::generateInitialSolution(int& rank, int& size){
    std::vector<double> initialSolution;
    initialSolution.reserve(dimensions);
    initialSolution.resize(dimensions);
    
    for(int i = 0; i < dimensions; ++i){
        initialSolution[i] = bounds[i].first + (bounds[i].second - bounds[i].first) * randomUnitary(); 
    }

    return initialSolution;
}

std::vector<double> Domain::generateStepsize(int& rank, int& size){
    std::vector<double> stepsize;
    stepsize.reserve(dimensions);
    stepsize.resize(dimensions);

    for(int i = 0; i < dimensions; ++i){
        do {
            stepsize[i] = (bounds[i].second - bounds[i].first) * randomUnitary() / 50.0;// + (bounds[i].second - bounds[i].first) * ((static_cast<double>(rank))/(2.0*static_cast<double>(size))); 
        }
        while (stepsize[i] >= (bounds[i].second - bounds[i].first));
    }   

    return stepsize;
}
