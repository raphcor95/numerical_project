#include <iostream>
#include <vector>

#include "../Pricers/MonteCarlo/MonteCarlo.h"
#include "../Payoffs/EuropeanCall.h"
#include "../Payoffs/EuropeanPut.h"
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../RandomGenerators/QuasiRandomGenerators/LDSequences/HaltonVdC.h"
#include "../Processes/BSEulerND.h"
#include "../Processes/BSEulerNDAnti.h"
#include "../Processes/Underlyings/Basket.h"
#include "../Utils/Input.h"
// #include "../Utils/Output.h"
#include "../Utils/Tools.h"

int main() {
    // === Simulation Parameters ===
    double startTime = 0.0;
    double endTime = 1.0;

    double spot = 100.0;
    double strike = 100.0;
    double rate = 0.05;

    size_t nbSteps = 252;
    double nbSim = 10000;

    std::vector<double> vecSpots(3, spot);
    std::vector<double> vecRates(3, rate);
    std::vector<double> vecWeights = {1, 0, 0};

    Input* Inp = new Input();
    Matrix* matCov = Inp->CSV2Mat("Inputs/matCov.csv"); // To change the Covariance Matrix, go to the associated csv file in the input folder

    bool useAntithetic = true;
    bool useControlVariate = true;
    bool useQuasiMC = true;
    std::string payoffType = "Call"; // Please use only "Call" or "Put"
    // ===========================================

    // === Start Variable Definition ===
    UniformGenerator* Unif = new EcuyerCombined();
    Normal* Norm = new NormalBoxMuller(0.0, 1.0, Unif);
    
    RandomProcess* BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
    Underlying* myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);
    
    RandomProcess* BSEulerBasketAnti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
    Underlying* myBasketAnti = new Basket(BSEulerBasketAnti, 100.0, vecWeights);

    LDSequence* LDS = new HaltonVdC(3);
    QuasiRandomNormal* NormQuasi = new NormInvCDF(0.0, 1.0, LDS);
    RandomProcess* BSEulerBasketQuasi = new BSEulerND(NormQuasi, vecSpots, vecRates, matCov);
    Underlying* myBasketQuasi = new Basket(BSEulerBasketQuasi, 100.0, vecWeights);
    RandomProcess* BSEulerBasketQuasiAnti = new BSEulerNDAnti(NormQuasi, vecSpots, vecRates, matCov);
    Underlying* myBasketQuasiAnti = new Basket(BSEulerBasketQuasiAnti, 100.0, vecWeights);
    
    Payoff* payoffPut = new EuropeanPut(strike);
    Payoff* payoffCall = new EuropeanCall(strike);
    
    // Output* Out = new Output();
    Underlying* basketToUse;
    Payoff* payoffToUse;
    std:: string method_to_display = "";
    // ===========================================

    // === Parameter Interpretation ===
    if (useAntithetic) {
        if (useQuasiMC) {
            basketToUse = myBasketQuasiAnti;
            method_to_display += "Quasi Monte Carlo + Antithetic Variable";
        }
        else {
            basketToUse = myBasketAnti;
            method_to_display += "Monte Carlo + Antithetic Variable";
        }
    }
    else {
        if (useQuasiMC) {
            basketToUse = myBasketQuasi;
            method_to_display += "Quasi Monte Carlo";
        }
        else {
            basketToUse = myBasket;
            method_to_display += "Monte Carlo";

        }
    }
    if (useControlVariate) {
        method_to_display += " + Control Variate";
    }

    if (payoffType == "Call") payoffToUse = payoffCall;
    else payoffToUse = payoffPut;
    // ===========================================

    // === Pricing ===
    MonteCarlo* mc = new MonteCarlo(basketToUse, nbSim, startTime, endTime, nbSteps, rate);
    double averagePrice = mc->Price(payoffToUse, useControlVariate);
    // ===========================================

    // === Display ===
    std::cout << "You priced a " << payoffType << ", Strike: " << strike << ", Maturity: " << endTime << "Y." << std::endl;
    std::cout << "You choosed the " << method_to_display << " methodology." << std::endl;
    std::cout << "Option price: " << averagePrice << std::endl;
    // ===========================================

    return 0;
}