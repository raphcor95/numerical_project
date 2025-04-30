#include <iostream>
#include <vector>
#include "../Utils/Types.h"
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

    Multiplier = 40014;
    Increment = 0;
    Modulus = 2147483563;
    Seed = 1;

    int N = 10000;

    // Uniform Generator
    UniformGenerator* Unif = new LinearCongruential();

    // Bernoulli Distribution
    double proba = 0.5;
    Bernoulli* Bern = new Bernoulli(proba, Unif);
    std::vector<int> vec_bern(N);
    for (size_t i = 0; i < N; i++)
    {
        vec_bern[i] = Bern->Generate();
        // std::cout << vec_bern[i] << std::endl;
    }

    // Binomial Distribution
    int n = 20;
    Binomial* Bino = new Binomial(20, proba, Unif);
    std::vector<int> vec_bino(N);
    for (size_t i = 0; i < N; i++)
    {
        vec_bino[i] = Bino->Generate();
        // std::cout << vec_bino[i] << std::endl;
    }

    // Normal Distribution
    double mu = 0.0;
    double sigma = 1.0;
    NormalBoxMuller* Norm = new NormalBoxMuller(mu, sigma, Unif);
    std::vector<double> vec_norm(N);
    for (size_t i = 0; i < N; i++)
    {
        vec_norm[i] = Norm->Generate();
        std::cout << vec_norm[i] << std::endl;
    }

    // Poisson Distribution
    // double lambda = 0.2;
    // Poisson* Poiss = new Poisson(lambda, Unif);



    delete Bern;
    delete Bino;

    return 0;
}