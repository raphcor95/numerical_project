#include "Tools.h"
#include "../Processes/RandomProcess.h"
#include <vector>
#include <iostream>

std::vector<std::vector<std::vector<double>>> generate_antithetic(Normal* Gen, size_t dim_sim, size_t dim_steps, size_t dim_undl)
{
    std::vector<std::vector<std::vector<double>>> results(dim_sim, std::vector<std::vector<double>>(dim_steps, std::vector<double>(dim_undl, 0.0)));
    for (size_t i = 0; i < dim_sim; i++)
    {
        for (size_t j = 0; j < dim_steps; j++)
        {
            for (size_t k = 0; k < dim_undl; k++)
            results[i][j][k] = Gen->Generate(); 
        }
    }
    return results;
}
