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
    size_t nbSteps
) : Pricer(myBasket, nbSim, startTime, endTime, nbSteps)

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

double MonteCarlo::Price(Payoff* payoff) {
    double sumPayoff = 0.0;
    for (size_t i = 0; i < static_cast<size_t>(NbSim); ++i) {
        Undl->Simulate(StartTime, EndTime, NbSteps);
        SinglePath* Path = Undl->ReturnPath();
        // TODO modifier pour recuperer la vrai valeur des taux
        sumPayoff += std::exp(-0.05 * this->EndTime) * (*payoff)(Path->GetValues());
    }
    return sumPayoff / NbSim;
}

double MonteCarlo::priceControlVariate(Payoff* payoff)
{
    double sumPayoff = 0.0;

    // TODO modifier pour donner la valeur de balck scholes
    double control_variate_payoff = 6.8; //compute_expected_value_control_variate(spots, weights, strike, rates[0], corrMatrix, maturity);

    for (int n = 0; n < static_cast<size_t>(NbSim); ++n)
    {
        Undl->Simulate(StartTime, EndTime, NbSteps);
        std::vector< std::vector<double> > spot_vectors = Undl->ReturnSimulations();
        std::vector<double> log_spots(spot_vectors[n].size());
        for (int j = 0; j < spot_vectors[n].size(); j++)
            log_spots[j] = log(spot_vectors[n][j]);
        SinglePath* Path = Undl->ReturnPath();
        // TODO modifier pour recuperer la vrai valeur des taux
        sumPayoff += std::exp(-0.05 * this->EndTime) * (*payoff)(Path->GetValues());
        double weighted_log_spots = std::inner_product(std::begin(weights), std::end(weights), std::begin(log_spots), 0.0);
        sumPayoff -= std::exp(-0.05 * this->EndTime) * std::max(exp(weighted_log_spots) - strike, 0.);
        sumPayoff += control_variate_payoff;
    }
    return sumPayoff / NbSim;
}