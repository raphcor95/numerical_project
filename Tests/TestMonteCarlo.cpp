#include <iostream>
#include <vector>
#include "../Pricers/MonteCarlo/MonteCarlo.h"
#include "../Payoffs/EuropeanCall.h"
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../RandomGenerators/QuasiRandomGenerators/LDSequences/HaltonVdC.h"
#include "../Processes/BSEuler1D.h"
#include "../Processes/BSEulerND.h"
#include "../Processes/BSEulerNDAnti.h"
#include "../Processes/Underlyings/Basket.h"
#include "../Utils/Input.h"
#include "../Utils/Output.h"
#include "../Utils/Tools.h"

int main() {
    // === Simulation Parameters ===
    double startTime = 0.0;
    double spot = 100.0;
    double strike = 85.0;
    double rate = 0.25;
    double endTime = 1.0;
    size_t nbSteps = 252;
    size_t nbSim = 1;

    std::vector<double> vecSpots(3, spot);
    std::vector<double> vecRates(3, rate);
    std::vector<double> vecWeights = {1.0, 0.0, 0.0};
    Input* Inp = new Input();
    Matrix* matCov = Inp->CSV2Mat("Inputs/matCov.csv");

    // === Class Instances ===
    UniformGenerator* Unif = new EcuyerCombined();
    // LDSequence* LDS = new HaltonVdC(3);
    // QuasiRandomNormal* Norm = new NormInvCDF(0.0, 1.0, LDS);
    Normal* Norm = new NormalBoxMuller(0.0, 1.0, Unif);
    RandomProcess* BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
    // RandomProcess* BSEulerBasket = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
    Underlying* myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);

    MonteCarlo* mc = new MonteCarlo(
        myBasket, nbSim, startTime, endTime, nbSteps, rate
    );

    // === Simulation ===
    // std::vector<std::vector<double>> SimulatedPaths = mc->Simulate();

    // === Output the simulations ===
    // Output* Out = new Output();
    // Out->Vec2CSV(SimulatedPaths, "Outputs/MonteCarlo_Simulations.csv");
    // std::cout << "Outputting the results in: Outputs/MonteCarlo_Simulations.csv" << std::endl;

    // === Compute Payoff option ===
    Payoff* myCall = new EuropeanCall(strike);
    double final_payoff = mc->Price(myCall, true);
    double BSPrice = BSCall(spot, strike, 0.1, rate, endTime);
    std::cout << "Option price: " << final_payoff << std::endl;
    std::cout << "Black Scholes Price: " << BSPrice << std::endl;
    // === Cleaning ===
    // delete Out;
    delete matCov;
    return 0; 
}
