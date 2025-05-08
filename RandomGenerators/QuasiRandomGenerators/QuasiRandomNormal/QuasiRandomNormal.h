#pragma once
#include "../QuasiRandom.h"
#include "../LDSequences/LDSequence.h"

class QuasiRandomNormal : public QuasiRandom
{
    protected:
        double Mu;
        double Sigma;
        LDSequence* Sequence;

    public:
        QuasiRandomNormal(double inputMu, double inputSigma, LDSequence* sequence);
        ~QuasiRandomNormal();
};

class NormInvCDF : public QuasiRandomNormal
{
    public:
        NormInvCDF(double inputMu, double inputSigma, LDSequence* sequence);
        ~NormInvCDF();
        double Generate();
};