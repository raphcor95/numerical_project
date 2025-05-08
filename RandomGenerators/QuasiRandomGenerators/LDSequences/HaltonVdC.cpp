#include "HaltonVdC.h"
#include "Tools.h"

HaltonVdC::HaltonVdC(int dimension) : LDSequence(dimension), Current(0), cnt(0)
{
    std::cout << "[HaltonVdC] Instanciation of the Halton Sequence..." << std::endl;
    // Construct the vector of bases
    for (int d = 0; d < Dimension; d++)
    {
        std::deque<int> init(1, 0);
        VecBases.push_back(primes[d]);
        VecDecompos.push_back(
            bAdic(0, VecBases[d])
        );
    }
    std::cout << "[HaltonVdC] Instanciation completed." << std::endl;
}

/* Function to generate another element of the Halton sequence 
    -> This really mean the next element, we can be at the same time step and
        simply generate the value of the next dimension
*/
double HaltonVdC::Generate()
{
    // Get BAdic Decomposition of the cnt-th (1 ...d) element of the vector for a given time
    VecDecompos[cnt] = RecursiveBAdic(VecDecompos[cnt], VecBases[cnt]);

    // Convert this vector into [0, 1] number
    double result = BAdicToDouble(VecDecompos[cnt], VecBases[cnt]);

    // Update cnt counter (for dimensions)
    cnt = (cnt + 1) % Dimension;

    return result;
}

// Conversion of BAdic decomposition to [0, 1] double
double HaltonVdC::BAdicToDouble(std::deque<int>& vecBAdic, int b)
{
    double result = 0.0;
    double q = 1.0 / b;
    double m = vecBAdic.size();

    for (size_t j = 0; j < m; j++)
    {
        result += q * vecBAdic[m - j - 1];
        q /= b;
    }
    return result;
}

HaltonVdC::~HaltonVdC()
{

}