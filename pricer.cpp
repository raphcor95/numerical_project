#include <iostream>

#include "../Pricers/MonteCarlo/MonteCarlo.h"
#include "../Payoffs/EuropeanCall.h"
#include "../Payoffs/EuropeanPut.h"
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../RandomGenerators/QuasiRandomGenerators/LDSequences/HaltonVdC.h"
#include "../Processes/BSEulerND.h"
#include "../Processes/BSEulerNDAnti.h"
#include "../Processes/Underlyings/Basket.h"
#include "../Pricers/LongstaffSchwarz/LongstaffSchwarz.h"
#include "../Pricers/LongstaffSchwarz/LSLaguerrePoly.h"
#include "../Pricers/LongstaffSchwarz/LSStandard.h"
#include "../Utils/Input.h"
#include "../Utils/Tools.h"

int main() {
    // === Simulation Parameters ===
    // Option parameters
    double startTime = 0.0;
    double endTime = 1.0;
    double strike = 100.0;
    std::string payoffType = "Call"; // Please use only "Call" or "Put"

    // Undelrying Parameters
    double spot = 100.0;
    double rate = 0.05;
    std::vector<double> vecSpots(3, spot);
    std::vector<double> vecRates(3, rate);
    std::vector<double> vecWeights = {1, 0, 0};
    Input* Inp = new Input();
    Matrix* matCov = Inp->CSV2Mat("Inputs/matCov.csv"); // To change the Covariance Matrix, go to the associated csv file in the input folder

    // Simulations Parameters
    size_t nbSteps = 252;
    double nbSim = 10000;
    bool useAntithetic = true;
    bool useControlVariate = true;
    bool useQuasi = true;
    
    bool useLS = false;
    bool useLSLaguerrePoly = false;
    
    bool useMC = true;
    
    bool useBS = false;

    std::vector<double> vecTimes = {0.2, 0.4, 0.6, 1.0}; // Longstaff Schwarz Pricer - Laguerre Polynomials
    int orderLS = 3;
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
    
    Underlying* basketToUse;
    Payoff* payoffToUse;
    std:: string method_to_display = "";
    // ===========================================
    
    // === Parameter Interpretation ===
    if (useQuasi) method_to_display += "Quasi ";

    if (useLS) method_to_display += "Longstaff Schwartz";
    else if (useLSLaguerrePoly) method_to_display += "Longstaff Schwartz Laguerre Poly";
    else if (useMC) method_to_display += "Monte Carlo";
    else if (useBS) method_to_display += "Black Scholes";
    else runtime_error("Select at least one methodology!");
    
    if (useAntithetic) {
        if (useQuasi) basketToUse = myBasketQuasiAnti;
        else basketToUse = myBasketAnti;
        method_to_display += " + Antithetic Variable";
    }
    else {
        if (useQuasi) basketToUse = myBasketQuasi;
        else basketToUse = myBasket;            
    }
    if (useControlVariate) method_to_display += " + Control Variate";
    
    if (payoffType == "Call") payoffToUse = payoffCall;
    else payoffToUse = payoffPut;
    if (useBS) method_to_display = "Black Scholes";
    // ===========================================
    
    // === Pricing ===
    double averagePrice = -1;
    if (useLS) {
        LongstaffSchwarz* PricerLS = new LSStandard(myBasket, vecTimes, nbSim, startTime, endTime, nbSteps, orderLS, rate);
        averagePrice = PricerLS->Price(payoffToUse, useControlVariate);
    } else if (useLSLaguerrePoly) {
        LongstaffSchwarz* PricerLSLaguerrePoly = new LSLaguerrePoly(myBasket, vecTimes, nbSim, startTime, endTime, nbSteps, rate);
        averagePrice = PricerLSLaguerrePoly->Price(payoffToUse, useControlVariate);
    } else if (useMC) {
        MonteCarlo* mc = new MonteCarlo(basketToUse, nbSim, startTime, endTime, nbSteps, rate);
        averagePrice = mc->Price(payoffToUse, useControlVariate);
    } else if (useBS) {
        if (payoffType == "Call") averagePrice = BSCall(spot, strike, (*matCov)(0, 0), rate, endTime);
        else averagePrice = BSPut(spot, strike, (*matCov)(0, 0), rate, endTime);
    }
    // ===========================================

    // === Display ===
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "You priced a " << payoffType << ", Strike: " << strike << ", Maturity: " << endTime << "Y." << std::endl;
    std::cout << "You choosed the " << method_to_display << " methodology." << std::endl;
    std::cout << "Option price: " << averagePrice << std::endl;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
    // ===========================================

    return 0;
}