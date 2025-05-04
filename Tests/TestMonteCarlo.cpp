#include <iostream>
#include <vector>
#include "../Pricers/MonteCarlo/MonteCarlo.h"
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../Processes/BSEuler1D.h"
#include "../Processes/BSEulerND.h"
#include "../Processes/Underlyings/Basket.h"
#include "../Utils/Input.h"
#include "../Utils/Output.h"

int main() {
    // === Simulation Parameters ===
    double startTime = 0.0;
    double endTime = 1.0;
    size_t nbSteps = 365;
    size_t nbSim = 10;

    std::vector<double> vecSpots(3, 100.0);
    std::vector<double> vecRates(3, 0.05);
    std::vector<double> vecWeights = {0.0, 1.0, 0.0};
    Input* Inp = new Input();
    Matrix* matCov = Inp->CSV2Mat("Inputs/matCov.csv");

    // === Class Instances ===
    UniformGenerator* Unif = new EcuyerCombined();
    Normal* Norm = new NormalBoxMuller(0.0, 1.0, Unif);
    RandomProcess* BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
    Underlying* myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);

    MonteCarlo* mc = new MonteCarlo(
        myBasket, nbSim, startTime, endTime, nbSteps
    );

    // === Simulation ===
    std::vector<std::vector<double>> SimulatedPaths = mc->Simulate();

    // === Output the simulations ===
    Output* Out = new Output();
    Out->Vec2CSV(SimulatedPaths, "Outputs/MonteCarlo_Simulations.csv");
    std::cout << "Outputting the results in: Outputs/MonteCarlo_Simulations.csv" << std::endl;
    
    // === Cleaning ===
    delete Out;
    delete matCov;
    return 0; 
}
