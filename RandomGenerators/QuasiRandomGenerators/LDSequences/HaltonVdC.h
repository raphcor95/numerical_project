#pragma once
#include "LDSequence.h"
#include <vector>

class HaltonVdC : public LDSequence
{
    protected:
        int Dimension;
        std::vector<double> VecD;
        double Current;

    public:
        HaltonVdC(int Dimension);
        ~HaltonVdC();
        double Generate();
}