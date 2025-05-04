#include "MonteCarlo.h"
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../Processes/BSEuler1D.h"
#include "../Processes/BSEulerND.h"
#include "../Processes/Underlyings/Basket.h"

#include <iostream>

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
    std::vector<std::vector<double>> SimulatedPaths(NbSim);

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
        sumPayoff += (*payoff)(Path->GetValues());
    }
    return sumPayoff / NbSim;
}
