#include "QuasiRandomNormal.h"
#include <cmath>

/* Tool functions for John D. Cook algo */
double RationalApproximation(double t)
{
    // Abramowitz and Stegun formula 26.2.23.
    // The absolute value of the error should be less than 4.5 e-4.
    double c[] = {2.515517, 0.802853, 0.010328};
    double d[] = {1.432788, 0.189269, 0.001308};
    return t - ((c[2]*t + c[1])*t + c[0]) / 
               (((d[2]*t + d[1])*t + d[0])*t + 1.0);
}


/* Quasi Random Normal Class */
// Constructor
QuasiRandomNormal::QuasiRandomNormal(double inputMu, double inputSigma, 
                    LDSequence* sequence) : 
    Mu(inputMu),
    Sigma(inputSigma),
    Sequence(sequence)
{

}

// Destructor
QuasiRandomNormal::~QuasiRandomNormal()
{

}

/* Normal CDF Inversion -- John D. Cook algo */
// Constructor
NormInvCDF::NormInvCDF(double inputMu, double inputSigma, LDSequence* sequence) : 
    QuasiRandomNormal(inputMu, inputSigma, sequence)
{

}

// Generate function
double NormInvCDF::Generate()
{
    double p = 0.0;     // Dummy value waiting for LD Sequence implementation
    if (p < 0.5)
    {
        return -RationalApproximation(sqrt(-2.0 * log(p)));
    } 
    else
    {
        return RationalApproximation(sqrt(-2.0 * log(1-p)));
    }

}

// Destructor
NormInvCDF::~NormInvCDF()
{

}
