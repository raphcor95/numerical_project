#include "Tools.h"
#include "../Processes/RandomProcess.h"
#include <vector>
#include <iostream>

/* Black Scholes Formulas */
double BSCall(double spot, double strike, double vol, double rate, double matu)
{
    double d1 = (log(spot / strike) + (rate + (vol * vol)/2) * matu) / (vol * std::sqrt(matu));
    double d2 = d1 - vol * std::sqrt(matu);

    return spot * norm_cdf(d1) - strike * exp(- rate * matu) * norm_cdf(d2);
}

/* Black Scholes Formulas */
double BSPut(double spot, double strike, double vol, double rate, double matu)
{
    double d1 = (log(spot / strike) + (rate + (vol * vol)/2) * matu) / (vol * std::sqrt(matu));
    double d2 = d1 - vol * std::sqrt(matu);

    return strike * exp(- rate * matu) * norm_cdf(-d2) - spot * norm_cdf(-d1);
}


/* Statistical Formulas */
double norm_cdf(double x)
{
    return 0.5 * std::erfc(- x / std::sqrt(2));
}




/* Control Variate Expected Values */
double ComputeCVExpectation(
    std::vector<double>& vecSpots,
    std::vector<double>& vecWeights,
    double strike,
    double rate,
    Matrix* MatCov,
    double matu,
    Payoff* payoff
)
{

    // Variables
    double prodSiWi = 1.0;
    double adjRate = 0.0;
    double adjVol = 0.0;

    // Compute the sigma sigma transpose matrix
    std::pair<Matrix, Matrix> cholesky = (*MatCov).choleskyDecomposition();
    Matrix MatSigma = cholesky.first;


    // Convert weight vectors into matrices
    std::vector< std::vector<double> > matWeights = {vecWeights};
    Matrix MatRowW = Matrix(matWeights);
    MatRowW.print(); 

    // Compute matrix products
    double matProd = MatRowW.matrix_product(
        MatSigma.matrix_product(
            MatSigma.transpose().matrix_product(MatRowW.transpose())
        )
    )(0, 0);


    // Compute product of spots and adjusted rate
    Matrix MatSigmaSigma = MatSigma.matrix_product(MatSigma.transpose());
    MatSigmaSigma.print();
    for (int d = 0; d < vecWeights.size(); d++)
    {
        // Compute variance
        double var = MatSigmaSigma(d, d);
        prodSiWi *= pow(vecSpots[d], vecWeights[d]);
        adjRate += vecWeights[d] * var;
    }
    adjRate = rate - 0.5 * adjRate + 0.5 * matProd;

    // Compute price
    if(payoff->Name == "Call")
    {
        return BSCall(prodSiWi, strike, std::sqrt(matProd), adjRate, matu);
    }
    if(payoff->Name == "Put")
    {
        return BSPut(prodSiWi, strike, std::sqrt(matProd), adjRate, matu);
    }
    else
    {
        throw std::runtime_error("Payoff " + payoff->Name + " not implented yet!");
    }

}

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


/* Tool function to compute b-adic expansion of a positive integer */
std::deque<int> bAdic(int k, int b)
{
    // Variables
    std::deque<int> vecDecompo(1, 0);                // Vector to store the decomposition
    int i_max = 0;                                    // Integerfor the maximum size of the vector
    int q = 0;
    

    // Initialise the decomposition
    if (k > 0)
    {
        i_max = static_cast<int>(log(k)/log(b));
        vecDecompo.resize(i_max + 1, 0);
        
        // Create a first power of the base to compute the decomposition
        q = pow(b, i_max);

        // Compute the p-adic decomposition
        for (size_t i = 0; i < i_max + 1; i++)
        {
            vecDecompo[i] = static_cast<int>(k / q);
            k -= q * vecDecompo[i];
            q /= b;
        }
        return vecDecompo;
    }
    else
    {
        return vecDecompo;
    }
}

/* Tool function to compute the b-adic expansion of a positive intger */
/* For a given basis, this function allows to compute the decomposition of the n-th
term knowing the decomposition vector of the previous one. */
std::deque<int>& RecursiveBAdic(std::deque<int>& vecDecompo, int b)
{
    /* This algorithm was proposed in Monte Carlo methods
    for Financial Engineering by Paul Glasserman */

    // Variables
    bool Carry = true;                  // Set to true = increment of 1 given previous decompo
    size_t N = vecDecompo.size();

    // Loop over the decomposition for adding 1 (=Carry on the right-most coefficient)
    for (int i = N-1; i >= 0; i--)
    {
        if (Carry)
        {
            // std::cout << "Increment first value" << std::endl;
            if(vecDecompo[i] == b - 1)
            {
                vecDecompo[i] = 0; // And keep the carry
            }
            else
            {
                vecDecompo[i] += 1;
                Carry = false;
                break;          // No further iterations will be made, we can break the loop

            }
        }
    }

    // If there remains a carry, we need to increment the basis expansion by 1 dimension
    if (Carry)
    {
        vecDecompo.push_front(1);
    }
    return vecDecompo;
}




/* Vector of prime numbers */
std::vector<int> primes = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 
    79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 
    167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 
    57, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 
    353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 
    449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 
    563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 
    653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 
    761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 
    877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 
    991, 997, 1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 
    1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 
    1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 
    1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 
    1373, 1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 
    1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553, 
    1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 
    1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741, 
    1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823, 1831, 1847, 1861, 
    1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 
    1973, 1979, 1987, 1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053, 
    2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129, 2131, 2137, 2141, 
    2143, 2153, 2161, 2179, 2203, 2207, 2213, 2221, 2237, 2239, 2243, 2251, 2267, 
    2269, 2273, 2281, 2287, 2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 
    2357, 2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423, 2437, 2441, 
    2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531, 2539, 2543, 2549, 2551, 2557, 
    2579, 2591, 2593, 2609, 2617, 2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 
    2683, 2687, 2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741, 2749, 
    2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833, 2837, 2843, 2851, 
    2857, 2861, 2879, 2887, 2897, 2903, 2909, 2917, 2927, 2939, 2953, 2957, 2963, 
    2969, 2971, 2999, 3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 
    3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181, 3187, 3191, 3203, 
    3209, 3217, 3221, 3229, 3251, 3253, 3257, 3259, 3271, 3299, 3301, 3307, 3313, 
    3319, 3323, 3329, 3331, 3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 
    3413, 3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511, 3517, 3527, 
    3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571
};
