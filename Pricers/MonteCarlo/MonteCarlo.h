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
        // Function to do the simulations
        std::vector<std::vector<double>> Simulate();
        // Function to price by simulating
        double Price(Payoff* payoff);

        // Function to do the simulations with antithetic
        // std::vector<std::vector<double>> Simulate_antithetic();
        // Function to price by simulating with antithetic
        // double Price_antithetic(Payoff* payoff);

};