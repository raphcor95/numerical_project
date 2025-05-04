#include "LSLaguerrePoly.h"
#include <iostream>
#include <cmath>

LSLaguerrePoly::LSLaguerrePoly(
    Underlying* undl,
    std::vector<double> times,
    double nbSim,
    double startTime,
    double endTime,
    size_t nbSteps
) : 
    LongstaffSchwarz(undl, times, nbSim, startTime, endTime, nbSteps)
{

}

LSLaguerrePoly::~LSLaguerrePoly()
{

}

/* Defining the polynomirla function for the conditional expectation regression */
double LSLaguerrePoly::Order0Poly(double x)
{
    return exp(-x / 2);
}

double LSLaguerrePoly::Order1Poly(double x)
{
    return exp(-x / 2) * (1 - x);
}

double LSLaguerrePoly::Order2Poly(double x)
{
    return exp(-x / 2) * (1 - 2 * x + (x * x) / 2);
}


/* Pricing Method Implementation */
double LSLaguerrePoly::Price(Payoff* payoff)
{   
    std::cout << "[LSLaguerre] Initiating the pricing ..." << std::endl;

    // General Variables
    std::vector<double> vecExercise(NbSim, 0.0);            // Vector of immediate exercise
    std::vector<double> vecContinue(NbSim, 0.0);            // Vector of Continuation value
    std::vector<double> vecOptiTime(NbSim, 0.0);            // Vector of optimal times exercise


    // Optimisation variables
    double Yj;
    double Xj;
    std::vector<double> vecUpToT;
    std::vector<double> vecAlpha(3, 0.0);                   // Vector for the alpha coefficients
    std::vector<double> vecSumPiYj(3, 0.0);                 // Vector containing the sum of Yj * Pi(xj)
    std::vector< std::vector<double> > vecHil(3, std::vector<double>(3, 0.0));       // Matrix of Hij coefficients



    // Check whether simulations exist or if we need to simulate
    if (VecPaths.size() > 0)
    {
        std::cout << "[LSLaguerre] Simulating trajectories ..." << std::endl;
        Simulate();
        std::cout << "[LSLaguerre] Simulation completed!" << std::endl;
    }

    // Initialisation of the Optimal Exercise @ Values
    std::cout << "[LSLaguerre] Initialising the optimal times/values." << std::endl;
    for (size_t j= 0; j < NbSim; j++)
    {
        vecContinue[j] = (*payoff)(VecPaths[j]->GetValues());
        vecOptiTime[j] = EndTime;
    }

    // Loop on the exercise dates backwards
    std::cout << "[LSLaguerre] Looping on observation dates ..." << std::endl;
    for (size_t idx = VecTimes.size(); idx >= 0; idx--)
    {
        // Retrieve the observation date
        double t = VecTimes[idx];

        // Compute exercise value
        std::cout << "[LSLaguerre] Computing immediate exercise values ..." << std::endl;
        for (size_t j = 0; j < NbSim; j++)
        {
            // vecExercise[i] = (*payoff)(VecPaths[i]->GetValue(t));
        }

        // Preliminary computation for the Conditional Expectation Approximation
        std::cout << "[LSLaguerre] Alpha preliminary computations ..." << std::endl;
        for (size_t i = 0; i < vecAlpha.size(); i++)
        {
            std::cout << "[LSLaguerre] Preliminary computations for "
                    + std::to_string(i) + "-th order polynomial ..." << std::endl;

            // Clean Previous Values
            vecSumPiYj[i] = 0.0;
            for (size_t k = 0; k < vecAlpha.size(); k++)
            {
                vecHil[i][k] = 0.0;
            }

            // Update the values
            /* For each order of the polynomial, we compute
                - The sum of Yj Pi(xj) involving the discounted value of optimal continuation value.
                - The coefficients Hil: Sum of the product polynomial of order i and each of the others 
                    from order 0 to L (here 3). */
            switch(i)
            {
                case 0:
                    for (size_t j = 0; j < NbSim; j++)
                    {
                        vecUpToT = VecPaths[j]->GetValuesUpToT(t);
                        Yj = (*payoff)(vecUpToT);
                        Xj = VecPaths[j]->GetValue(t);
                        vecSumPiYj[i] += Yj * Order0Poly(Xj);
                        for (size_t k = 0; k < vecAlpha.size(); k++)
                        {
                            switch(k)
                            {

                                case 0:
                                    vecHil[i][k] += Order0Poly(Xj) * Order0Poly(Xj);
                                case 1:
                                    vecHil[i][k] += Order0Poly(Xj) * Order1Poly(Xj);
                                case 2:
                                    vecHil[i][k] += Order0Poly(Xj) * Order2Poly(Xj);
                                default:
                                    throw std::runtime_error("Order not supported.");
                            }
                        }
                    }
                case 1:
                    for (size_t j = 0; j < NbSim; j++)
                    {
                        Yj = (*payoff)(vecUpToT);
                        vecSumPiYj[i] += Yj * Order1Poly(VecPaths[j]->GetValue(t));
                        for (size_t k = 0; k < vecAlpha.size(); k++)
                        {
                            switch(k)
                            {
                                case 0:
                                    vecHil[i][k] += Order1Poly(Xj) * Order0Poly(Xj);
                                case 1:
                                    vecHil[i][k] += Order1Poly(Xj) * Order1Poly(Xj);
                                case 2:
                                    vecHil[i][k] += Order1Poly(Xj) * Order2Poly(Xj);
                                default:
                                    throw std::runtime_error("Order not supported.");
                            }
                        }
                    }
                case 2:
                    for (size_t j = 0; j < NbSim; j++)
                    {
                        Yj = (*payoff)(VecPaths[j]->GetValues());
                        vecSumPiYj[i] += Yj * Order2Poly(VecPaths[j]->GetValue(t));
                        for (size_t k = 0; k < vecAlpha.size(); k++)
                        {
                            switch(k)
                            {

                                case 0:
                                    vecHil[i][k] += Order2Poly(Xj) * Order0Poly(Xj);
                                case 1:
                                    vecHil[i][k] += Order2Poly(Xj) * Order1Poly(Xj);
                                case 2:
                                    vecHil[i][k] += Order2Poly(Xj) * Order2Poly(Xj);
                                default:
                                    throw std::runtime_error("Order not supported.");
                            }
                        }
                    }
                default:
                    throw std::runtime_error("Polynomial of order " + std::to_string(i) + "not supported.");
            }   
        }
        std::cout << "Preliminary computations completed!" << std::endl;

        // Now that we have all the coefficients to solve the linear systemn we determine alphas

        std::cout << "Work In Progress ... " << std::endl;
    }
}