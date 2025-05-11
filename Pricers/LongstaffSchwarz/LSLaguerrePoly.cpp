#include "LSLaguerrePoly.h"
#include "Matrix.h"
#include "Tools.h"
#include <iostream>
#include <cmath>
#include <numeric>


// Helper
constexpr double EPSILON = 1e-5;
auto is_close = [](double a, double b) {
    return std::fabs(a - b) < EPSILON;
};


LSLaguerrePoly::LSLaguerrePoly(
    Underlying* undl,
    std::vector<double> times,
    double nbSim,
    double startTime,
    double endTime,
    size_t nbSteps,
    double rate
) : 
    LongstaffSchwarz(undl, times, nbSim, startTime, endTime, nbSteps, rate)
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

double LSLaguerrePoly::ComputeExpectedValue(Matrix& MatColAlphas, double x)
{
    return MatColAlphas(0, 0) * Order0Poly(x) \
            + MatColAlphas(0, 1) * Order1Poly(x) \
            + MatColAlphas(0, 2) * Order2Poly(x);
}


/* Pricing Method Implementation */
double LSLaguerrePoly::Price(Payoff* payoff, bool ControlVariate)
{   
    std::cout << "[LSLaguerre] Initiating the pricing ..." << std::endl;
    VecPrices.clear();

    // General Variables
    std::vector<double> vecExercise(NbSim, 0.0);            // Vector of immediate exercise
    std::vector<double> vecContinue(NbSim, 0.0);            // Vector of Continuation value
    std::vector<double> vecOptiTime(NbSim, 0.0);            // Vector of optimal times exercise
    double price = 0.0;

    // Control Variate Variables
    std::vector<double> vecIdx;
    std::vector< std::vector<double> > vecControlVariate(NbSim);
    std::vector<double> vecW = Undl->GetWeights();
    std::vector<double> vecSpots = Undl->GetSpots();

    // Optimisation variables
    double Yj;
    double Xj;
    std::vector< std::vector<double> > vecUpToT(NbSim);
    std::vector<double> (vecAlpha)(3, 0.0);                                           // Vector for the alpha coefficients


    // Check whether simulations exist or if we need to simulate
    if (VecPaths.size() == 0)
    {
        Undl->ClearPaths();
        std::cout << "[LSLaguerre] Simulating trajectories ..." << std::endl;
        Simulate();
        std::cout << "[LSLaguerre] Simulation completed!" << std::endl;
    }

    // Compute Control Variate if necessary
    if(ControlVariate)
    {
        // Retrieve the simulations and simulate the control variates
        std::vector<double> vecFindIdx = VecPaths[0]->GetTimes();
        for (int t = 0; t < static_cast<int>(NbSteps); t++)
        {        
            for (double target : VecTimes)
            {
                if (is_close(vecFindIdx[t], target))
                {
                    vecIdx.push_back(t);
                    std::cout << vecIdx.back() << std::endl;
                    std::cout << "Pushed" << std::endl;
                    break;
                }
            }
        }
        vecIdx.push_back(NbSteps);

        // Reconstruct the control variable values at each time step
        std::vector< std::vector < std::vector<double> > > vecSimulations = Undl->ReturnSimulations();
        for (int t = 0; t < vecIdx.size(); t++)
        {
            double idx = vecIdx[t];
            std::cout << idx << std::endl;
            for (size_t j = 0; j < NbSim; j++)
            {
                std::vector<double> vecLogSpot(vecW.size(), 0.0);
                for (size_t k = 0; k < vecW.size(); k++)
                {
                    vecLogSpot[k] = log(vecSimulations[j][k][idx]);
                }
                double logspot = std::inner_product(vecW.begin(), vecW.end(), vecLogSpot.begin(), 0.0);
                vecControlVariate[j].push_back(exp(logspot));
            }
        }
    }
    std::cout << "Size " << vecControlVariate[0].size() << std::endl;

     
    // Initialisation of the Optimal Exercise @ Values
    std::cout << "[LSLaguerre] Initialising the optimal times/values." << std::endl;
    double minPrice = 0.0;
    if (ControlVariate)
    {
        // Compute Control Variate Expectation
        double cvExpectation = ComputeCVExpectation(
            vecSpots, vecW, payoff->GetStrike(), Rate, Undl->GetMatCov(), EndTime, payoff
        );


        for (size_t j = 0; j < NbSim; j++)
        {
            std::vector<double> vecControlVariable = {vecControlVariate[j][vecIdx.size()-1]};
            vecContinue[j] = (*payoff)(VecPaths[j]->GetValues()) - (*payoff)(vecControlVariable)
                             + cvExpectation * exp(Rate * EndTime);
            vecOptiTime[j] = EndTime;
            minPrice += exp(-Rate * EndTime) * vecContinue[j] / NbSim;
        }
    std::cout << "[LSLaguerre] Minimal price (European Payoff): " << minPrice << std::endl;
    }
    else
    {
        for (size_t j = 0; j < NbSim; j++)
        {
            vecContinue[j] = (*payoff)(VecPaths[j]->GetValues());
            vecOptiTime[j] = EndTime;
            minPrice += vecContinue[j] * exp(-Rate * EndTime) / NbSim;
        }
        std::cout << "[LSLaguerre] Minimal price (European Payoff): " << minPrice << std::endl;
    }
   


    // Loop on the exercise dates backwards
    std::cout << "[LSLaguerre] Looping on observation dates ..." << std::endl;
    for (int idx =  static_cast<int>(VecTimes.size()) - 2; idx >= 0; idx--)
    {
        // Retrieve the observation date
        double t = VecTimes[idx];
        std::cout << "[LSLaguerre] Running computations for t = " << t << std::endl;

        // Compute exercise value
        std::cout << "[LSLaguerre] Computing immediate exercise values ..." << std::endl;
        if (ControlVariate)
        {

            // Compute new Control Variate expected valued
            double cvExpectation = ComputeCVExpectation(
                vecSpots, vecW, payoff->GetStrike(), Rate, Undl->GetMatCov(), t, payoff
            );
            for (size_t j = 0; j < NbSim; j++)
            {
                std::vector<double> vecControlVariable = {vecControlVariate[j][idx]};
                vecUpToT[j] = VecPaths[j]->GetValuesUpToT(t);
                vecExercise[j] = (*payoff)(vecUpToT[j]) - (*payoff)(vecControlVariable)
                                + cvExpectation * exp(Rate * t);
            }
        }
        else
        {
            for (size_t j = 0; j < NbSim; j++)
            {
                vecUpToT[j] = VecPaths[j]->GetValuesUpToT(t);
                vecExercise[j] = (*payoff)(vecUpToT[j]);
            }
        }

        // Preliminary computation for the Conditional Expectation Approximation
        std::cout << "[LSLaguerre] Alpha preliminary computations ..." << std::endl;

        // OLS Variables
        std::vector< std::vector<double> > vecY(NbSim, std::vector<double>(1, 0.0));
        std::vector< std::vector<double> > vecX(NbSim, std::vector<double>(3, 0.0));
        
        // Filling the vectors
        // Update the values
        for (size_t j = 0; j < NbSim; j++)
        {
            // Fill the target values
            vecY[j][0] = (*payoff)(vecUpToT[j]) * exp(-Rate * (vecOptiTime[j] - t));
            Xj = VecPaths[j]->GetValue(t);

            // Fill the regressors
            for (int k = 0; k < vecAlpha.size(); k++)
            {
                switch(k)
                {
                    case 0:
                        vecX[j][k] = Order0Poly(Xj);
                        break;
                    case 1:
                        vecX[j][k] = Order1Poly(Xj);
                        break;
                    case 2:
                        vecX[j][k] = Order2Poly(Xj);
                        break;
                    default:
                        throw std::runtime_error("Polynomial of order " + std::to_string(k) + "not supported.");
                }
            }
        }

        // Converting to matrivces
        Matrix Y = Matrix(vecY);
        Matrix X = Matrix(vecX);

        std::cout << "[LSLaguerre] Preliminary computations completed!" << std::endl;

        /* Now that we have all the coefficients to solve the linear systemn we determine alphas */
        
        // Compute the optimal alphas
        Matrix MatColAlphas = (
            X.transpose().matrix_product(X)
        ).InverseLU().matrix_product(
            X.transpose().matrix_product(Y)
        );


        // Compute the optimal decision rule: (immediate exercise > conditional expectation)
        std::cout << "[LSLaguerre] Compute optimal decision value for date " << t << std::endl;
        for (size_t j = 0; j < NbSim; j++)
        {
            if (vecExercise[j] > ComputeExpectedValue(MatColAlphas, vecUpToT[j].back()))
            {
                // Update the optimal exercise rule to t
                vecOptiTime[j] = t;
                vecContinue[j] = vecExercise[j];
            }
        }
    }

    // Now that we know the optiaml decision rule for each path, we can compute the price
    std::cout << "[LSLaguerre] Averaging Discounted Optimal Exercise Values ..." << std::endl;
    for (size_t j = 0; j < NbSim; j++)
    {
        VecPrices.push_back(100 * vecContinue[j] * exp(-Rate * (vecOptiTime[j])));
        price += (1 / NbSim) * vecContinue[j] * exp(-Rate * (vecOptiTime[j]));
    }

    return price * 100;
}