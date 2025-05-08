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


/* Tool function to compute p-adic expansion of a positive integer */
std::vector<int> bAdic(int k, int b)
{
    // Variables
    std::vector<int> vecDecompo(1, 0);                // Vector to store the decomposition
    int i_max = 0;                                    // Integerfor the maximum size of the vector
    int q = 0;
    

    // Initialise the decomposition
    if (k > 0)
        {
            i_max = static_cast<int>(log(k)/log(b));
            vecDecompo.resize(i_max + 1, 0);
            
            // Create a first power of the base to compute the decomposition
            q = pow(b, i_max);
        }

        // Compute the p-adic decomposition
        for (size_t i = 0; i < i_max + 1; i++)
        {
            vecDecompo[i] = static_cast<int>(k / q);
            k -= q * vecDecompo[i];
            q /= b;
        }

    return vecDecompo;
}