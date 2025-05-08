#pragma once
#include "../Processes/RandomProcess.h"

#include <iostream>

std::vector<std::vector<std::vector<double>>> generate_antithetic(Normal* Gen, size_t dim_sim, size_t dim_steps, size_t dim_undl);

/* Tool functions for the generation of the Quasi Random Numbers */

// B-Adic Decomposition of a positive integer
std::vector<int> bAdic(int k, int b);