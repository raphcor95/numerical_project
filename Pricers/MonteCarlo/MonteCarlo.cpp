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

void MonteCarlo::Simulate() {
    SimulatedPaths.clear();

    for (size_t i = 0; i < static_cast<size_t>(NbSim); ++i) {
        Undl->Simulate(StartTime, EndTime, NbSteps);
        SinglePath* Path = Undl->ReturnPath();
        SimulatedPaths.push_back(Path->GetValues());
    }
}

// Marche pas. Juste pour pas planter.
double MonteCarlo::Price() {
    Simulate();
    double sum = 0.0;
    for (const auto& path : SimulatedPaths) {
        sum += path.back();
    }
    return sum / SimulatedPaths.size();
}
