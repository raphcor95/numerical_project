#include <iostream>
#include <vector>
#include <string>
#include "../Utils/Types.h"
#include "../Utils/Output.h"
#include "../RandomGenerators/UniformGenerators/EcuyerCombined.h"
#include "../RandomGenerators/UniformGenerators/LinearCongruential.h"
#include "../RandomGenerators/DiscreteGenerators/Bernoulli.h"
#include "../RandomGenerators/DiscreteGenerators/Binomial.h"
#include "../RandomGenerators/DiscreteGenerators/Poisson.h"
#include "../RandomGenerators/ContinuousGenerators/Normal.h"
#include "../RandomGenerators/ContinuousGenerators/Exponential.h"

int main()
{
    myLong Multiplier, Increment, Modulus, Seed;
    std::string outpath = "Outputs/";
    Output* Out = new Output();

    Multiplier = 40014;
    Increment = 0;
    Modulus = 2147483563;
    Seed = 1;

    int N = 10000;

    // Uniform Generator
    UniformGenerator* Unif = new EcuyerCombined();

    // Bernoulli Distribution
    double proba = 0.5;
    Bernoulli* Bern = new Bernoulli(proba, Unif);
    std::vector<double> vec_bern(N);
    for (size_t i = 0; i < N; i++)
    {
        vec_bern[i] = Bern->Generate();
        // std::cout << vec_bern[i] << std::endl;
    }
    std::vector<std::vector<double>> vec_out_ber;
    vec_out_ber.push_back(vec_bern) ;
    std::cout << "Outputing to the following folder: " << outpath + "bernoulli_simulation.csv" << std::endl;
    Out->Vec2CSV(vec_out_ber, outpath + "bernoulli_simulation.csv");

    // Binomial Distribution
    int n = 20;
    Binomial* Bino = new Binomial(20, proba, Unif);
    std::vector<double> vec_bino(N);
    for (size_t i = 0; i < N; i++)
    {
        vec_bino[i] = Bino->Generate();
        // std::cout << vec_bino[i] << std::endl;
    }
    std::vector<std::vector<double>> vec_out_bino;
    vec_out_bino.push_back(vec_bino);
    Out->Vec2CSV(vec_out_bino, outpath + "binomial_simulation.csv");

    // Normal Distribution
    double mu = 0.0;
    double sigma = 1.0;
    NormalBoxMuller* Norm = new NormalBoxMuller(mu, sigma, Unif);
    std::vector<double> vec_norm(N);
    for (size_t i = 0; i < N; i++)
    {
        vec_norm[i] = Norm->Generate();
        // std::cout << vec_norm[i] << std::endl;
    }
    std::vector<std::vector<double>> vec_out_norm;
    vec_out_norm.push_back(vec_norm);
    Out->Vec2CSV(vec_out_norm, outpath + "normal_simulations.csv");

    // Poisson Distribution
    // double lambda = 0.2;
    // Poisson* Poiss = new Poisson(lambda, Unif);



    delete Bern;
    delete Bino;
    delete Out;

    return 0;
}