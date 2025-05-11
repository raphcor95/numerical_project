#include <iostream>
#include <vector>

#include "RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "RandomGenerators/QuasiRandomGenerators/LDSequences/HaltonVdC.h"
#include "Processes/BSEuler1D.h"
#include "Processes/BSEulerND.h"
#include "Processes/BSEulerNDAnti.h"
#include "Processes/Underlyings/Basket.h"
#include "Pricers/LongstaffSchwarz/LongstaffSchwarz.h"
#include "Pricers/LongstaffSchwarz/LSLaguerrePoly.h"
#include "Pricers/LongstaffSchwarz/LSStandard.h"
#include "Payoffs/EuropeanCall.h"
#include "Payoffs/EuropeanPut.h"
#include "Utils/Input.h"
#include "Utils/Output.h"
#include "Utils/Tools.h"

int main()
{

    // === Simulation Parameters ===
    double startTime = 0.0;
    double endTime = 1.0;

    double spot = 100.0;
    double strike = 100.0;
    double rate = 0.20;

    size_t nbSteps = 252;
    std::vector<size_t> vecSim;
    for (int i = 50; i <= 1000; i += 50) vecSim.push_back(i);
    for (int i = 2000; i <= 10000; i += 2000) vecSim.push_back(i);

    std::vector<double> vecSpots(3, spot);
    std::vector<double> vecRates(3, rate);
    std::vector<double> vecWeights = {1, 0, 0};


    // === Input Helper ===
    Input* Inp = new Input();
    Matrix* matCov = Inp->CSV2Mat("Inputs/matCov.csv");

    // === Variable Definition ===
    UniformGenerator* Unif = new EcuyerCombined();
    Normal* Norm = new NormalBoxMuller(0.0, 1.0, Unif);
    
    RandomProcess* BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
    Underlying* myBasket = new Basket(BSEulerBasket, spot, vecWeights);
    
    RandomProcess* BSEulerBasketAnti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
    Underlying* myBasketAnti = new Basket(BSEulerBasketAnti, spot, vecWeights);

    LDSequence* LDS = new HaltonVdC(vecSpots.size());
    QuasiRandomNormal* NormQuasi = new NormInvCDF(0.0, 1.0, LDS);
    RandomProcess* BSEulerBasketQuasi = new BSEulerND(NormQuasi, vecSpots, vecRates, matCov);
    Underlying* myBasketQuasi = new Basket(BSEulerBasketQuasi, spot, vecWeights);
    RandomProcess* BSEulerBasketQuasiAnti = new BSEulerNDAnti(NormQuasi, vecSpots, vecRates, matCov);
    Underlying* myBasketQuasiAnti = new Basket(BSEulerBasketQuasiAnti, spot, vecWeights);
    
    Payoff* payoffPut = new EuropeanPut(strike);
    Payoff* payoffCall = new EuropeanCall(strike);
    

    // === Output Helper ===
    Output* Out = new Output();
    std::vector<std::vector<double>> allPricesLSCall(vecSim.size());
    std::vector<std::vector<double>> allPricesLSPut(vecSim.size());

    // === Longstaff Schwarz Specifics ===
    double order = 3;
    // std::vector<double> vecTimes = {0.2, 0.4, 0.6, 1.0};
    std::vector<double> vecTimes = {1.0};

    // ==========================================================================================================
    // === Longstaff Schwarz
    // ==========================================================================================================

    for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
        std::cout << "[Convergence] Running price. Number of simulations: " << vecSim[nbSim] << std::endl;
        RandomProcess* BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
        Underlying* myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);
        LongstaffSchwarz* LS = new LSStandard(myBasket, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
        // LongstaffSchwarz* LS = new LSLaguerrePoly(myBasket, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        LS->Price(payoffCall, false);
        allPricesLSCall[nbSim] = LS->VecPrices;

        LS->Price(payoffPut, false);
        allPricesLSPut[nbSim] = LS->VecPrices;
        // delete BSEulerBasket;
        // delete myBasket;
        delete LS;
    };
    // === Output the simulations ===
    Out->Vec2CSV(allPricesLSCall, "Outputs/LS_Simulations_Convergence_Call.csv");
    std::cout << "Outputting the results of Monte Carlo for a Call in: Outputs/LS_Simulations_Convergence_Call.csv" << std::endl;

    Out->Vec2CSV(allPricesLSPut, "Outputs/LS_Simulations_Convergence_Put.csv");
    std::cout << "Outputting the results of Monte Carlo for a Put in: Outputs/LS_Simulations_Convergence_Put.csv" << std::endl;
    // ==========================================================================================================
    


//     // ==========================================================================================================


//     // ==========================================================================================================
//     // === Longstaff Schwarz + Control variate ===
//     for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
//         std::cout << vecSim[nbSim] << std::endl;
//         RandomProcess* BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
//         Underlying* myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);
//         LongstaffSchwarz* LS = new LSStandard(myBasket, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LongstaffSchwarz* LS = new LSLaguerrePoly(myBasket, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);
        
//         LS->Price(payoffCall, true);
//         allPricesLSCall[nbSim] = LS->VecPrices;
//         delete LS;

//         BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
//         myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);
//         LS = new LSStandard(myBasket, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LS = new LSLaguerrePoly(myBasket, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffPut, true);
//         allPricesLSPut[nbSim] = LS->VecPrices;
//         // delete BSEulerBasket;
//         // delete myBasket;
//         delete LS;
//     };
//     // === Output the simulations ===
//     Out->Vec2CSV(allPricesLSCall, "Outputs/LS_ControlVariate_Simulations_Convergence_Call.csv");
//     std::cout << "Outputting the results of Monte Carlo with Control Variate for a Call in: Outputs/LS_ControlVariate_Simulations_Convergence_Call.csv" << std::endl;
    
//     Out->Vec2CSV(allPricesLSPut, "Outputs/LS_ControlVariate_Simulations_Convergence_Put.csv");
//     std::cout << "Outputting the results of Monte Carlo with Control Variate for a Put in: Outputs/LS_ControlVariate_Simulations_Convergence_Put.csv" << std::endl;
//     // ==========================================================================================================

    
//     // ==========================================================================================================
//     // === Longstaff Schwarz + Antithetic ===
//     for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
//         std::cout << vecSim[nbSim] << std::endl;
//         RandomProcess* BSEulerBasketAnti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
//         Underlying* myBasketAnti = new Basket(BSEulerBasketAnti, 100.0, vecWeights);
//         LongstaffSchwarz* LS = new LSStandard(myBasketAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LongstaffSchwarz* LS = new LSLaguerrePoly(myBasketAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffCall, false);
//         allPricesLSCall[nbSim] = LS->VecPrices;
//         delete LS;

//         BSEulerBasketAnti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
//         myBasketAnti = new Basket(BSEulerBasketAnti, 100.0, vecWeights);
//         LS = new LSStandard(myBasketAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LS = new LSLaguerrePoly(myBasketAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffPut, false);
//         allPricesLSPut[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketAnti;
//         // delete myBasketAnti;
//         delete LS;
//     };
//     // === Output the simulations ===
//     Out->Vec2CSV(allPricesLSCall, "Outputs/LS_Antithetic_Simulations_Convergence_Call.csv");
//     std::cout << "Outputting the results of Monte Carlo with Antithetic for a Call in: Outputs/LS_Antithetic_Simulations_Convergence_Call.csv" << std::endl;
    
//     Out->Vec2CSV(allPricesLSPut, "Outputs/LS_Antithetic_Simulations_Convergence_Put.csv");
//     std::cout << "Outputting the results of Monte Carlo with Antithetic for a Put in: Outputs/LS_Antithetic_Simulations_Convergence_Put.csv" << std::endl;
//     // ==========================================================================================================


//     // ==========================================================================================================
//     // === Longstaff Schwarz + Antithetic + Control Variate ===
//     for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
//         std::cout << vecSim[nbSim] << std::endl;
//         RandomProcess* BSEulerBasketAnti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
//         Underlying* myBasketAnti = new Basket(BSEulerBasketAnti, 100.0, vecWeights);
//         LongstaffSchwarz* LS = new LSStandard(myBasketAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LongstaffSchwarz* LS = new LSLaguerrePoly(myBasketAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffCall, true);
//         allPricesLSCall[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketAnti;
//         // delete myBasketAnti;
//         delete LS;

//         BSEulerBasketAnti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
//         myBasketAnti = new Basket(BSEulerBasketAnti, 100.0, vecWeights);
//         LS = new LSStandard(myBasketAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LS = new LSLaguerrePoly(myBasketAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffPut, true);
//         allPricesLSPut[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketAnti;
//         // delete myBasketAnti;
//         delete LS;
//     };
//     // === Output the simulations ===
//     Out->Vec2CSV(allPricesLSCall, "Outputs/LS_Antithetic_ControlVariate_Simulations_Convergence_Call.csv");
//     std::cout << "Outputting the results of Monte Carlo with Antithetic and Control Variate for a Call in: Outputs/LS_Antithetic_ControlVariate_Simulations_Convergence_Call.csv" << std::endl;
    
//     Out->Vec2CSV(allPricesLSPut, "Outputs/LS_Antithetic_ControlVariate_Simulations_Convergence_Put.csv");
//     std::cout << "Outputting the results of Monte Carlo with Antithetic and Control Variate for a Put in: Outputs/LS_Antithetic_ControlVariate_Simulations_Convergence_Put.csv" << std::endl;
//     // ==========================================================================================================


//     // ==========================================================================================================
//     // === Quasi Longstaff Schwarz ===
//     for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
//         std::cout << vecSim[nbSim] << std::endl;
//         RandomProcess* BSEulerBasketQuasi = new BSEulerND(NormQuasi, vecSpots, vecRates, matCov);
//         Underlying* myBasketQuasi = new Basket(BSEulerBasketQuasi, 100.0, vecWeights);
//         LongstaffSchwarz* LS = new LSStandard(myBasketQuasi, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LongstaffSchwarz* LS = new LSLaguerrePoly(myBasketQuasi, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffCall, false);
//         allPricesLSCall[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketQuasi;
//         // delete myBasketQuasi;
//         delete LS;

//         BSEulerBasketQuasi = new BSEulerND(NormQuasi, vecSpots, vecRates, matCov);
//         myBasketQuasi = new Basket(BSEulerBasketQuasi, 100.0, vecWeights);
//         LS = new LSStandard(myBasketQuasi, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LS = new LSLaguerrePoly(myBasketQuasi, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffPut, false);
//         allPricesLSPut[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketQuasi;
//         // delete myBasketQuasi;
//         delete LS;
//     };
//     // === Output the simulations ===
//     Out->Vec2CSV(allPricesLSCall, "Outputs/QuasiLS_Convergence_Call.csv");
//     std::cout << "Outputting the results of Quasi Monte Carlo for a Call in: Outputs/QuasiLS_Simulations_Convergence_Call.csv" << std::endl;

//     Out->Vec2CSV(allPricesLSPut, "Outputs/QuasiLS_Convergence_Put.csv");
//     std::cout << "Outputting the results of Quasi Monte Carlo for a Put in: Outputs/QuasiLS_Simulations_Convergence_Put.csv" << std::endl;
//     // ==========================================================================================================


//     // ==========================================================================================================
//     // === Quasi Longstaff Schwarz + Control variate ===
//     for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
//         std::cout << vecSim[nbSim] << std::endl;
//         RandomProcess* BSEulerBasketQuasi = new BSEulerND(NormQuasi, vecSpots, vecRates, matCov);
//         Underlying* myBasketQuasi = new Basket(BSEulerBasketQuasi, 100.0, vecWeights);
//         LongstaffSchwarz* LS = new LSStandard(myBasketQuasi, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LongstaffSchwarz* LS = new LSLaguerrePoly(myBasketQuasi, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffCall, true);
//         allPricesLSCall[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketQuasi;
//         // delete myBasketQuasi;
//         delete LS;

//         BSEulerBasketQuasi = new BSEulerND(NormQuasi, vecSpots, vecRates, matCov);
//         myBasketQuasi = new Basket(BSEulerBasketQuasi, 100.0, vecWeights);
//         LS = new LSStandard(myBasketQuasi, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LS = new LSLaguerrePoly(myBasketQuasi, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffPut, true);
//         allPricesLSPut[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketQuasi;
//         // delete myBasketQuasi;
//         delete LS;
//     };
//     // === Output the simulations ===
//     Out->Vec2CSV(allPricesLSCall, "Outputs/QuasiLS_ControlVariate_Simulations_Convergence_Call.csv");
//     std::cout << "Outputting the results of Quasi Monte Carlo with Control Variate for a Call in: Outputs/Quasi_LS_ControlVariate_Simulations_Convergence_Call.csv" << std::endl;

//     Out->Vec2CSV(allPricesLSPut, "Outputs/Quasi_LS_ControlVariate_Simulations_Convergence_Put.csv");
//     std::cout << "Outputting the results of Quasi Monte Carlo with Control Variate for a Put in: Outputs/Quasi_LS_ControlVariate_Simulations_Convergence_Put.csv" << std::endl;
//     // ==========================================================================================================

    
//     // ==========================================================================================================
//     // === Quasi Longstaff Schwarz + Antithetic ===
//     for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
//         std::cout << vecSim[nbSim] << std::endl;
//         RandomProcess* BSEulerBasketQuasiAnti = new BSEulerNDAnti(NormQuasi, vecSpots, vecRates, matCov);
//         Underlying* myBasketQuasiAnti = new Basket(BSEulerBasketQuasiAnti, 100.0, vecWeights);
//         LongstaffSchwarz* LS = new LSStandard(myBasketQuasiAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LongstaffSchwarz* LS = new LSLaguerrePoly(myBasketQuasiAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffCall, false);
//         allPricesLSCall[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketQuasiAnti;
//         // delete myBasketQuasiAnti;
//         delete LS;

//         BSEulerBasketQuasiAnti = new BSEulerNDAnti(NormQuasi, vecSpots, vecRates, matCov);
//         myBasketQuasiAnti = new Basket(BSEulerBasketQuasiAnti, 100.0, vecWeights);
//         LS = new LSStandard(myBasketQuasiAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LS = new LSLaguerrePoly(myBasketQuasiAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffPut, false);
//         allPricesLSPut[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketQuasiAnti;
//         // delete myBasketQuasiAnti;
//         delete LS;
//     };
//     // === Output the simulations ===
//     Out->Vec2CSV(allPricesLSCall, "Outputs/QuasiLS_Antithetic_Simulations_Convergence_Call.csv");
//     std::cout << "Outputting the results of Quasi Monte Carlo with Antithetic for a Call in: Outputs/QuasiLS_Antithetic_Simulations_Convergence_Call.csv" << std::endl;

//     Out->Vec2CSV(allPricesLSPut, "Outputs/QuasiLS_Antithetic_Simulations_Convergence_Put.csv");
//     std::cout << "Outputting the results of Quasi Monte Carlo with Antithetic for a Put in: Outputs/QuasiLS_Antithetic_Simulations_Convergence_Put.csv" << std::endl;
//     // ==========================================================================================================


//     // ==========================================================================================================
//     // === Quasi Longstaff Schwarz + Antithetic + Control Variate ===
//     for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
//         std::cout << vecSim[nbSim] << std::endl;
//         RandomProcess* BSEulerBasketQuasiAnti = new BSEulerNDAnti(NormQuasi, vecSpots, vecRates, matCov);
//         Underlying* myBasketQuasiAnti = new Basket(BSEulerBasketQuasiAnti, 100.0, vecWeights);
//         LongstaffSchwarz* LS = new LSStandard(myBasketQuasiAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LongstaffSchwarz* LS = new LSLaguerrePoly(myBasketQuasiAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffCall, true);
//         allPricesLSCall[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketQuasiAnti;
//         // delete myBasketQuasiAnti;
//         delete LS;

//         BSEulerBasketQuasiAnti = new BSEulerNDAnti(NormQuasi, vecSpots, vecRates, matCov);
//         myBasketQuasiAnti = new Basket(BSEulerBasketQuasiAnti, 100.0, vecWeights);
//         LS = new LSStandard(myBasketQuasiAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, order, rate);
//         // LS = new LSLaguerrePoly(myBasketQuasiAnti, vecTimes, vecSim[nbSim], startTime, endTime, nbSteps, rate);

//         LS->Price(payoffPut, true);
//         allPricesLSPut[nbSim] = LS->VecPrices;
//         // delete BSEulerBasketQuasiAnti;
//         // delete myBasketQuasiAnti;
//         delete LS;
//     };
//     // === Output the simulations ===
//     Out->Vec2CSV(allPricesLSCall, "Outputs/QuasiLS_Antithetic_ControlVariate_Simulations_Convergence_Call.csv");
//     std::cout << "Outputting the results of Quasi Monte Carlo with Antithetic and Control Variate for a Call in: Outputs/QuasiLS_Antithetic_ControlVariate_Simulations_Convergence_Call.csv" << std::endl;

//     Out->Vec2CSV(allPricesLSPut, "Outputs/QuasiLS_Antithetic_ControlVariate_Simulations_Convergence_Put.csv");
//     std::cout << "Outputting the results of Quasi Monte Carlo with Antithetic and Control Variate for a Put in: Outputs/QuasiLS_Antithetic_ControlVariate_Simulations_Convergence_Put.csv" << std::endl;
//     // ==========================================================================================================

//     return 0;
// }