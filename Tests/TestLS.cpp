#include <iostream>
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../Processes/BSEuler1D.h"
#include "../Processes/BSEulerND.h"
#include "../Processes/BSEulerNDAnti.h"
#include "../Processes/Underlyings/Basket.h"
#include "../Pricers/LongstaffSchwarz/LongstaffSchwarz.h"
#include "../Pricers/LongstaffSchwarz/LSLaguerrePoly.h"
#include "../Pricers/LongstaffSchwarz/LSStandard.h"
#include "../Payoffs/EuropeanCall.h"
#include "../Payoffs/EuropeanPut.h"
#include "../Utils/Input.h"
#include "../Utils/Output.h"
#include "../Utils/Tools.h"

int main()
{

        // Variables
        double rate = 0.05;
        double vol = 0.1;
        double spot = 100;
        double strike = 100;
        double startTime = 0;
        double endTime = 1;
        double nbSim = 10;
        size_t nbSteps = 100;

        // Generators
        UniformGenerator* Unif = new EcuyerCombined();
        Normal* Norm = new NormalBoxMuller(0.0, 1.0, Unif);

        // Asset Dynamics Parameters - Basket Case
        double N = 3.0;
        std::vector<double> vecSpots(3, spot);
        std::vector<double> vecRates(3, rate);
        // std::vector<double> vecWeights = {1.0, 0.0, 0.0};
        std::vector<double> vecWeights = {0.2, 0.3, 0.5};
        Input* Inp = new Input();
        Matrix* matCov = Inp->CSV2Mat("Inputs/matCov.csv");

        // Control Variate
        bool ControlVariate = false;

        // MultiAsset Path Generator
        RandomProcess* BSEulerMulti = new BSEulerND(Norm, vecSpots, vecRates, matCov);
        // RandomProcess* BSEulerMulti = new BSEulerNDAnti(Norm, vecSpots, vecRates, matCov);
        Underlying* myBasket = new Basket(BSEulerMulti, vecSpots[0], vecWeights);

        // Longstaff Schwarz Pricer - Laguerre Polynomials
        std::vector<double> vecTimes = {0.2, 0.4, 0.6, 1.0};
        // std::vector<double> vecTimes = {1.0};
        int order = 3;
        LongstaffSchwarz* PricerLS = new LSLaguerrePoly(myBasket, vecTimes, nbSim,
                                    startTime, endTime, nbSteps, rate);
        LongstaffSchwarz* PricerLS = new LSStandard(myBasket, vecTimes, nbSim,
                                        startTime, endTime, nbSteps, order, rate);


        // PricerLS->Simulate();
        // const std::vector<SinglePath*>& vecPaths = PricerLS->ReturnPaths();

        // // Outputting the simulations
        // std::vector< std::vector<double> > vecSim;
        // for (size_t k = 0; k < nbSim; k++)
        // {
        //     vecSim.push_back(vecPaths[k]->GetValues());
        // }

        // // Outputting the results for visualisation
        // Output* Out = new Output();
        // Out->Vec2CSV(vecSim, "Outputs/LSBasket_Simulations.csv");
        // std::cout << "Outputting the results in: Outputs/LSBasket_Simulations.csv" << std::endl;
        
        // Payoff
        Payoff* payoff = new EuropeanCall(strike);
        // Payoff* payoff = new EuropeanPut(strike);
        double BSPrice = BSCall(spot, strike, vol, rate, endTime);
        // double BSPrice = BSPut(spot, strike, vol, rate, endTime);
        double price = PricerLS->Price(payoff, ControlVariate);
        std::cout << "BS Price of the Bermudan Option: " << price << std::endl;
        std::cout << "BS Price of the European Option: " << BSPrice << std::endl;

        std::cout << "Execution Successful!" << std::endl;



    return 0;
}