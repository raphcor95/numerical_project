#include "MonteCarlo.h"
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../Processes/BSEuler1D.h"
#include "../Processes/BSEulerND.h"
#include "../Processes/Underlyings/Basket.h"
#include "../Utils/Tools.h"

#include <iostream>
#include <cmath>
#include <numeric>
#include <iterator>

MonteCarlo::MonteCarlo(
    Underlying* myBasket,
    double nbSim,
    double startTime,
    double endTime,
    size_t nbSteps,
    double rate
) : Pricer(myBasket, nbSim, startTime, endTime, nbSteps, rate)

{}

MonteCarlo::~MonteCarlo() {}

std::vector<std::vector<double>> MonteCarlo::Simulate() {
    std::vector<std::vector<double>> SimulatedPaths;

    for (size_t i = 0; i < static_cast<size_t>(NbSim); ++i) {
        Undl->Simulate(StartTime, EndTime, NbSteps);
        SinglePath* Path = Undl->ReturnPath();
        SimulatedPaths.push_back(Path->GetValues());
    }

    return SimulatedPaths;
}

double MonteCarlo::Price(Payoff* payoff, bool ControlVariate) {

    // Variables
    double sumPayoff = 0.0;

    // Pricing depending on wether there is control variate or not
    if (!ControlVariate)
    {
        std::cout << "[MC] Monte Carlo Pricing without Control Variate ..." << std::endl;
        // Loop on the simulations
        for (size_t i = 0; i < static_cast<size_t>(NbSim); ++i) {
            Undl->Simulate(StartTime, EndTime, NbSteps);
            SinglePath* Path = Undl->ReturnPath();
            sumPayoff += (*payoff)(Path->GetValues());
        }

        // Return the discounted price
        std::cout << "[MC] Monte Carlo Pricing terminated." << std::endl;
        return exp(-Rate * EndTime) * sumPayoff / NbSim;
    }
    else
    {   
        std::cout << "[MC] Monte Carlo Pricing with Control Variate ..." << std::endl;

        // Compute the expected value of the Control Variate variable
        std:vector<double> vecSpots = Undl->GetSpots();
        std::vector<double> vecW = Undl->GetWeights();
        double cvExpectation = ComputeCVExpectation(vecSpots,Undl->GetWeights(),
            payoff->GetStrike(), Rate, Undl->GetMatCov(), EndTime, payoff);

        std::cout << "[MC] Control Variate Expectation: " << cvExpectation << std::endl;

        // Loop on the simulations
        for (size_t i = 0; i < static_cast<size_t>(NbSim); ++i) 
        {

            // Simulate standard trajectory
            Undl->Simulate(StartTime, EndTime, NbSteps);
            SinglePath* Path = Undl->ReturnPath();

            // Retrieve simulation for the Control Variate variable
            std::vector< std::vector<double> > vecSim = Undl->ReturnSimulations();
            std::vector<double> vecSpotsFinal = Undl->ReturnSimulations()[i];
            for (size_t d = 0; d < vecSpotsFinal.size(); d++)
            {
                vecSpotsFinal[d] = log(vecSpotsFinal[d]);
            }

            // Compute the control variate variable
            double ControlVariable =  std::inner_product(std::begin(vecW), std::end(vecW), std::begin(vecSpotsFinal), 0.0);
            std::vector<double> VecControlVariable = {exp(ControlVariable)};

            std::cout << "[MC] Terminal value " << Path->GetValue(EndTime) << std::endl;
            std::cout << "[MC] Terminal terminal value " << exp(ControlVariable) << std::endl;
            
            sumPayoff +=  exp(-Rate * EndTime) *(
                (*payoff)(Path->GetValues()) - (*payoff)(VecControlVariable)
                ) + cvExpectation;
        }

        // Return the discounted price
        std::cout << "[MC] Control Variate Monte Carlo Pricing terminated." << std::endl;
        return sumPayoff / NbSim;
    }
}