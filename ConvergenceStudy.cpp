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
#include "../Utils/Output.h"
#include "../Utils/Tools.h"

int main() {
    // === Simulation Parameters ===
    double startTime = 0.0;
    double endTime = 1.0;

    double spot = 100.0;
    double strike = 120.0;
    double rate = 0.20;

    size_t nbSteps = 252;
    std::vector<size_t> vecSim;
    for (int i = 50; i <= 1000; i += 50) vecSim.push_back(i);
    for (int i = 2000; i <= 10000; i += 2000) vecSim.push_back(i);

    std::vector<double> vecSpots(3, spot);
    std::vector<double> vecRates(3, rate);
    std::vector<double> vecWeights = {1, 0, 0};

    Input* Inp = new Input();
    Matrix* matCov = Inp->CSV2Mat("Inputs/matCov.csv");

    // === Variable Definition ===
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
    
    Output* Out = new Output();
    
    std::vector<std::vector<double>> allPricesMCCall(vecSim.size());
    std::vector<std::vector<double>> allPricesMCPut(vecSim.size());


    // ==========================================================================================================
    // === Monte Carlo ===
    for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
        std::cout << vecSim[nbSim] << std::endl;
        RandomProcess* BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
        Underlying* myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);
        MonteCarlo* mc = new MonteCarlo(myBasket, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffCall, false);
        allPricesMCCall[nbSim] = mc->VecPrices;

        mc->Price(payoffPut, false);
        allPricesMCPut[nbSim] = mc->VecPrices;
        // delete BSEulerBasket;
        // delete myBasket;
        delete mc;
    };
    // === Output the simulations ===
    Out->Vec2CSV(allPricesMCCall, "Outputs/MonteCarlo_Simulations_Convergence_Call.csv");
    std::cout << "Outputting the results of Monte Carlo for a Call in: Outputs/MonteCarlo_Simulations_Convergence_Call.csv" << std::endl;

    Out->Vec2CSV(allPricesMCPut, "Outputs/MonteCarlo_Simulations_Convergence_Put.csv");
    std::cout << "Outputting the results of Monte Carlo for a Put in: Outputs/MonteCarlo_Simulations_Convergence_Put.csv" << std::endl;
    // ==========================================================================================================


    // ==========================================================================================================
    // === Monte Carlo + Control variate ===
    for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
        std::cout << vecSim[nbSim] << std::endl;
        RandomProcess* BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
        Underlying* myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);
        MonteCarlo* mc = new MonteCarlo(myBasket, vecSim[nbSim], startTime, endTime, nbSteps, rate);
        
        mc->Price(payoffCall, true);
        allPricesMCCall[nbSim] = mc->VecPrices;
        delete mc;

        BSEulerBasket = new BSEulerND(Norm, vecSpots, vecRates, matCov);
        myBasket = new Basket(BSEulerBasket, 100.0, vecWeights);
        mc = new MonteCarlo(myBasket, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffPut, true);
        allPricesMCPut[nbSim] = mc->VecPrices;
        // delete BSEulerBasket;
        // delete myBasket;
        delete mc;
    };
    // === Output the simulations ===
    Out->Vec2CSV(allPricesMCCall, "Outputs/MonteCarlo_ControlVariate_Simulations_Convergence_Call.csv");
    std::cout << "Outputting the results of Monte Carlo with Control Variate for a Call in: Outputs/MonteCarlo_ControlVariate_Simulations_Convergence_Call.csv" << std::endl;
    
    Out->Vec2CSV(allPricesMCPut, "Outputs/MonteCarlo_ControlVariate_Simulations_Convergence_Put.csv");
    std::cout << "Outputting the results of Monte Carlo with Control Variate for a Put in: Outputs/MonteCarlo_ControlVariate_Simulations_Convergence_Put.csv" << std::endl;
    // ==========================================================================================================

    
    // ==========================================================================================================
    // === Monte Carlo + Antithetic ===
    for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
        std::cout << vecSim[nbSim] << std::endl;
        RandomProcess* BSEulerBasketAnti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
        Underlying* myBasketAnti = new Basket(BSEulerBasketAnti, 100.0, vecWeights);
        MonteCarlo* mc = new MonteCarlo(myBasketAnti, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffCall, false);
        allPricesMCCall[nbSim] = mc->VecPrices;
        delete mc;

        BSEulerBasketAnti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
        myBasketAnti = new Basket(BSEulerBasketAnti, 100.0, vecWeights);
        mc = new MonteCarlo(myBasketAnti, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffPut, false);
        allPricesMCPut[nbSim] = mc->VecPrices;
        // delete BSEulerBasketAnti;
        // delete myBasketAnti;
        delete mc;
    };
    // === Output the simulations ===
    Out->Vec2CSV(allPricesMCCall, "Outputs/MonteCarlo_Antithetic_Simulations_Convergence_Call.csv");
    std::cout << "Outputting the results of Monte Carlo with Antithetic for a Call in: Outputs/MonteCarlo_Antithetic_Simulations_Convergence_Call.csv" << std::endl;
    
    Out->Vec2CSV(allPricesMCPut, "Outputs/MonteCarlo_Antithetic_Simulations_Convergence_Put.csv");
    std::cout << "Outputting the results of Monte Carlo with Antithetic for a Put in: Outputs/MonteCarlo_Antithetic_Simulations_Convergence_Put.csv" << std::endl;
    // ==========================================================================================================


    // ==========================================================================================================
    // === Monte Carlo + Antithetic + Control Variate ===
    for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
        std::cout << vecSim[nbSim] << std::endl;
        RandomProcess* BSEulerBasketAnti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
        Underlying* myBasketAnti = new Basket(BSEulerBasketAnti, 100.0, vecWeights);
        MonteCarlo* mc = new MonteCarlo(myBasketAnti, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffCall, true);
        allPricesMCCall[nbSim] = mc->VecPrices;
        // delete BSEulerBasketAnti;
        // delete myBasketAnti;
        delete mc;

        BSEulerBasketAnti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
        myBasketAnti = new Basket(BSEulerBasketAnti, 100.0, vecWeights);
        mc = new MonteCarlo(myBasketAnti, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffPut, true);
        allPricesMCPut[nbSim] = mc->VecPrices;
        // delete BSEulerBasketAnti;
        // delete myBasketAnti;
        delete mc;
    };
    // === Output the simulations ===
    Out->Vec2CSV(allPricesMCCall, "Outputs/MonteCarlo_Antithetic_ControlVariate_Simulations_Convergence_Call.csv");
    std::cout << "Outputting the results of Monte Carlo with Antithetic and Control Variate for a Call in: Outputs/MonteCarlo_Antithetic_ControlVariate_Simulations_Convergence_Call.csv" << std::endl;
    
    Out->Vec2CSV(allPricesMCPut, "Outputs/MonteCarlo_Antithetic_ControlVariate_Simulations_Convergence_Put.csv");
    std::cout << "Outputting the results of Monte Carlo with Antithetic and Control Variate for a Put in: Outputs/MonteCarlo_Antithetic_ControlVariate_Simulations_Convergence_Put.csv" << std::endl;
    // ==========================================================================================================


    // ==========================================================================================================
    // === Quasi Monte Carlo ===
    for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
        std::cout << vecSim[nbSim] << std::endl;
        RandomProcess* BSEulerBasketQuasi = new BSEulerND(NormQuasi, vecSpots, vecRates, matCov);
        Underlying* myBasketQuasi = new Basket(BSEulerBasketQuasi, 100.0, vecWeights);
        MonteCarlo* mc = new MonteCarlo(myBasketQuasi, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffCall, false);
        allPricesMCCall[nbSim] = mc->VecPrices;
        // delete BSEulerBasketQuasi;
        // delete myBasketQuasi;
        delete mc;

        BSEulerBasketQuasi = new BSEulerND(NormQuasi, vecSpots, vecRates, matCov);
        myBasketQuasi = new Basket(BSEulerBasketQuasi, 100.0, vecWeights);
        mc = new MonteCarlo(myBasketQuasi, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffPut, false);
        allPricesMCPut[nbSim] = mc->VecPrices;
        // delete BSEulerBasketQuasi;
        // delete myBasketQuasi;
        delete mc;
    };
    // === Output the simulations ===
    Out->Vec2CSV(allPricesMCCall, "Outputs/Quasi_MonteCarlo_Simulations_Convergence_Call.csv");
    std::cout << "Outputting the results of Quasi Monte Carlo for a Call in: Outputs/Quasi_MonteCarlo_Simulations_Convergence_Call.csv" << std::endl;

    Out->Vec2CSV(allPricesMCPut, "Outputs/Quasi_MonteCarlo_Simulations_Convergence_Put.csv");
    std::cout << "Outputting the results of Quasi Monte Carlo for a Put in: Outputs/Quasi_MonteCarlo_Simulations_Convergence_Put.csv" << std::endl;
    // ==========================================================================================================


    // ==========================================================================================================
    // === Quasi Monte Carlo + Control variate ===
    for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
        std::cout << vecSim[nbSim] << std::endl;
        RandomProcess* BSEulerBasketQuasi = new BSEulerND(NormQuasi, vecSpots, vecRates, matCov);
        Underlying* myBasketQuasi = new Basket(BSEulerBasketQuasi, 100.0, vecWeights);
        MonteCarlo* mc = new MonteCarlo(myBasketQuasi, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffCall, true);
        allPricesMCCall[nbSim] = mc->VecPrices;
        // delete BSEulerBasketQuasi;
        // delete myBasketQuasi;
        delete mc;

        BSEulerBasketQuasi = new BSEulerND(NormQuasi, vecSpots, vecRates, matCov);
        myBasketQuasi = new Basket(BSEulerBasketQuasi, 100.0, vecWeights);
        mc = new MonteCarlo(myBasketQuasi, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffPut, true);
        allPricesMCPut[nbSim] = mc->VecPrices;
        // delete BSEulerBasketQuasi;
        // delete myBasketQuasi;
        delete mc;
    };
    // === Output the simulations ===
    Out->Vec2CSV(allPricesMCCall, "Outputs/Quasi_MonteCarlo_ControlVariate_Simulations_Convergence_Call.csv");
    std::cout << "Outputting the results of Quasi Monte Carlo with Control Variate for a Call in: Outputs/Quasi_MonteCarlo_ControlVariate_Simulations_Convergence_Call.csv" << std::endl;

    Out->Vec2CSV(allPricesMCPut, "Outputs/Quasi_MonteCarlo_ControlVariate_Simulations_Convergence_Put.csv");
    std::cout << "Outputting the results of Quasi Monte Carlo with Control Variate for a Put in: Outputs/Quasi_MonteCarlo_ControlVariate_Simulations_Convergence_Put.csv" << std::endl;
    // ==========================================================================================================

    
    // ==========================================================================================================
    // === Quasi Monte Carlo + Antithetic ===
    for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
        std::cout << vecSim[nbSim] << std::endl;
        RandomProcess* BSEulerBasketQuasiAnti = new BSEulerNDAnti(NormQuasi, vecSpots, vecRates, matCov);
        Underlying* myBasketQuasiAnti = new Basket(BSEulerBasketQuasiAnti, 100.0, vecWeights);
        MonteCarlo* mc = new MonteCarlo(myBasketQuasiAnti, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffCall, false);
        allPricesMCCall[nbSim] = mc->VecPrices;
        // delete BSEulerBasketQuasiAnti;
        // delete myBasketQuasiAnti;
        delete mc;

        BSEulerBasketQuasiAnti = new BSEulerNDAnti(NormQuasi, vecSpots, vecRates, matCov);
        myBasketQuasiAnti = new Basket(BSEulerBasketQuasiAnti, 100.0, vecWeights);
        mc = new MonteCarlo(myBasketQuasiAnti, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffPut, false);
        allPricesMCPut[nbSim] = mc->VecPrices;
        // delete BSEulerBasketQuasiAnti;
        // delete myBasketQuasiAnti;
        delete mc;
    };
    // === Output the simulations ===
    Out->Vec2CSV(allPricesMCCall, "Outputs/Quasi_MonteCarlo_Antithetic_Simulations_Convergence_Call.csv");
    std::cout << "Outputting the results of Quasi Monte Carlo with Antithetic for a Call in: Outputs/Quasi_MonteCarlo_Antithetic_Simulations_Convergence_Call.csv" << std::endl;

    Out->Vec2CSV(allPricesMCPut, "Outputs/Quasi_MonteCarlo_Antithetic_Simulations_Convergence_Put.csv");
    std::cout << "Outputting the results of Quasi Monte Carlo with Antithetic for a Put in: Outputs/Quasi_MonteCarlo_Antithetic_Simulations_Convergence_Put.csv" << std::endl;
    // ==========================================================================================================


    // ==========================================================================================================
    // === Quasi Monte Carlo + Antithetic + Control Variate ===
    for (size_t nbSim = 0; nbSim < vecSim.size(); ++nbSim) {
        std::cout << vecSim[nbSim] << std::endl;
        RandomProcess* BSEulerBasketQuasiAnti = new BSEulerNDAnti(NormQuasi, vecSpots, vecRates, matCov);
        Underlying* myBasketQuasiAnti = new Basket(BSEulerBasketQuasiAnti, 100.0, vecWeights);
        MonteCarlo* mc = new MonteCarlo(myBasketQuasiAnti, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffCall, true);
        allPricesMCCall[nbSim] = mc->VecPrices;
        // delete BSEulerBasketQuasiAnti;
        // delete myBasketQuasiAnti;
        delete mc;

        BSEulerBasketQuasiAnti = new BSEulerNDAnti(NormQuasi, vecSpots, vecRates, matCov);
        myBasketQuasiAnti = new Basket(BSEulerBasketQuasiAnti, 100.0, vecWeights);
        mc = new MonteCarlo(myBasketQuasiAnti, vecSim[nbSim], startTime, endTime, nbSteps, rate);

        mc->Price(payoffPut, true);
        allPricesMCPut[nbSim] = mc->VecPrices;
        // delete BSEulerBasketQuasiAnti;
        // delete myBasketQuasiAnti;
        delete mc;
    };
    // === Output the simulations ===
    Out->Vec2CSV(allPricesMCCall, "Outputs/Quasi_MonteCarlo_Antithetic_ControlVariate_Simulations_Convergence_Call.csv");
    std::cout << "Outputting the results of Quasi Monte Carlo with Antithetic and Control Variate for a Call in: Outputs/Quasi_MonteCarlo_Antithetic_ControlVariate_Simulations_Convergence_Call.csv" << std::endl;

    Out->Vec2CSV(allPricesMCPut, "Outputs/Quasi_MonteCarlo_Antithetic_ControlVariate_Simulations_Convergence_Put.csv");
    std::cout << "Outputting the results of Quasi Monte Carlo with Antithetic and Control Variate for a Put in: Outputs/Quasi_MonteCarlo_Antithetic_ControlVariate_Simulations_Convergence_Put.csv" << std::endl;
    // ==========================================================================================================

    return 0;
}