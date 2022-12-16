//
// Created by danie on 15/12/2022.
//

#include <vector>
#include <cmath>
#include <random>
#include "Domain.h"
#include "muParser.h"

class SimulatedAnnealing{
public:
    SimulatedAnnealing() = default;
    ~SimulatedAnnealing() = default;

    void simulatedAnnealing(Domain domain){

        solution.reserve(domain.getDimensions());
        solution.resize(domain.getDimensions());
        for (int i = 0; i < domain.getDimensions(); ++i)
            solution.at(i) = 0.;

        mu::Parser parser;
        parser.SetExpr(domain.getFunction());

        for (int i = 0; i < domain.getDimensions(); ++i) {
            std::string arg = "x";
            arg += std::to_string(i + 1);
            parser.DefineVar(arg, &solution.at(i));
        }

        fSolution = parser.Eval();

        double c = 0.99;
        int L = 1000;
        double alpha = 0.9;

        std::vector<double> newPoint;
        double fNew;
        newPoint.reserve(domain.getDimensions());
        newPoint.resize(domain.getDimensions());

        std::uniform_real_distribution<double> unitary(0, 1);

        do{
            for (int i = 0; i < L; ++i) {

                newPoint = domain.generateNeighborhood(solution);

                for (int j = 0; j < domain.getDimensions(); ++j) {
                    std::string arg = "x";
                    arg += std::to_string(j + 1);
                    parser.DefineVar(arg, &newPoint.at(j));
                }

                fNew = parser.Eval();

                if(fNew < fSolution || std::exp((fSolution - fNew) / c) > domain.random(unitary)){
                    solution = newPoint;
                    fSolution = fNew;
                }

            }

            c = c * alpha;

        } while (c > 0.01);

    }

    std::vector<double> getSolution() { return this->solution; }

private:
    std::vector<double> solution;
    double fSolution{};
};