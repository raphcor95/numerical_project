#pragma once
#include "../Processes/RandomProcess.h"

#include <iostream>

std::vector<std::vector<std::vector<double>>> generate_antithetic(Normal* Gen, size_t dim_sim, size_t dim_steps, size_t dim_undl);