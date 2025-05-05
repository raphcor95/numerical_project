#include <iostream>
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../Processes/BSEuler1D.h"
#include "../Processes/BSEulerND.h"
#include "../Processes/Underlyings/Basket.h"
#include "../Pricers/LongstaffSchwarz/LongstaffSchwarz.h"
#include "../Pricers/LongstaffSchwarz/LSLaguerrePoly.h"
#include "../Payoffs/EuropeanCall.h"
#include "../Utils/Input.h"
#include "../Utils/Output.h"

int main()
{

        // Variables
        double rate = 0.05;
        double vol = 0.1;
        double spot = 1;
        double startTime = 0;
        double endTime = 1;
        double nbSim = 1000;
        size_t nbSteps = 1000;

        // Generators
        UniformGenerator* Unif = new EcuyerCombined();
        Normal* Norm = new NormalBoxMuller(0.0, 1.0, Unif);

        // Asset Dynamics Parameters - Basket Case
        double N = 3.0;
        std::vector<double> vecSpots(3, 1.0);
        std::vector<double> vecRates(3, 0.05);
        std::vector<double> vecWeights = {1.0, 0.0, 0.0};
        Input* Inp = new Input();
        Matrix* matCov = Inp->CSV2Mat("Inputs/matCovIndep.csv");

        // MultiAsset Path Generator
        RandomProcess* BSEulerMulti = new BSEulerND(Norm, vecSpots, vecRates, matCov);
        Underlying* myBasket = new Basket(BSEulerMulti, vecSpots[0], vecWeights);

        // Longstaff Schwarz Pricer - Laguerre Polynomials
        std::vector<double> vecTimes = {0.2, 0.4, 0.6, 1.0};
        LongstaffSchwarz* PricerLS = new LSLaguerrePoly(myBasket, vecTimes, nbSim,
                                    startTime, endTime, nbSteps);
        PricerLS->Simulate();
        const std::vector<SinglePath*>& vecPaths = PricerLS->ReturnPaths();

        // Outputting the simulations
        std::vector< std::vector<double> > vecSim;
        for (size_t k = 0; k < nbSim; k++)
        {
            vecSim.push_back(vecPaths[k]->GetValues());
        }

        // Outputting the results for visualisation
        Output* Out = new Output();
        Out->Vec2CSV(vecSim, "Outputs/LSBasket_Simulations.csv");
        std::cout << "Outputting the results in: Outputs/LSBasket_Simulations.csv" << std::endl;
        
        // Payoff
        Payoff* EUCall = new EuropeanCall(1.0);
        double price = PricerLS->Price(EUCall);
        std::cout << "BS Price of the Bermudan Call Option: " << price << std::endl;



        std::cout << "Execution Successful!" << std::endl;



    return 0;
}