#pragma once
#include "Matrix.h"
#include "../Payoffs/Payoff.h"
#include "../Processes/RandomProcess.h"
#include <deque>
#include <iostream>


/* Black Scholes Formulas */
double BSCall(double spot, double strike, double vol, double rate, double matu);
double BSPut(double spot, double strike, double vol, double rate, double matu);

/* Statistical Formulas */
double norm_cdf(double x);

/* Control Variate Expected Values */
double ComputeCVExpectation(
    std::vector<double>& vecSpots,
    std::vector<double>& vecWeights,
    double strike, 
    double rate,
    Matrix* MatCov,
    double matu,
    Payoff* payoff
);

std::vector<std::vector<std::vector<double>>> generate_antithetic(Normal* Gen, size_t dim_sim, size_t dim_steps, size_t dim_undl);


/* Tool functions for the generation of the Quasi Random Numbers */
// The optimised algorithms (especially the recursive one were proposed by Paul Glasserman)
// B-Adic Decomposition of a positive integer
std::deque<int> bAdic(int k, int b);

// Recursive B-Adic Decomposition of a positive integer
std::deque<int>& RecursiveBAdic(std::deque<int>& vecDecompo, int b);



/* 500 first prime numbers for Quasi Random Numbers up to 500 dimensional vectors */
extern std::vector<int> primes;