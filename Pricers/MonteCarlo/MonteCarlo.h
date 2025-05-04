#pragma once
#include "../Pricer.h"
#include "../Utils/Matrix.h"

class MonteCarlo : public Pricer
{
    protected:
        std::vector<double> VecTimes;
        std::vector<SinglePath*> VecPaths;

        
        public:
        MonteCarlo(
            Underlying* myBasket,
            double nbSim,
            double startTime,
            double endTime,
            size_t nbSteps
        );
        
        ~MonteCarlo();
        // Function to do the dimulations
        std::vector<std::vector<double>> Simulate();
        // Function to price by simulating
        double Price(Payoff* payoff);

};