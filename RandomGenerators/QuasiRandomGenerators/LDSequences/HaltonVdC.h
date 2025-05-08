#pragma once
#include "LDSequence.h"
#include <vector>
#include <deque>

class HaltonVdC : public LDSequence
{
    protected:
        double Current;
        std::vector<int> VecBases;
        std::vector< std::deque<int> > VecDecompos;

    private:
        int cnt;
        double BAdicToDouble(std::deque<int>& vecBAdic, int b);


    public:
        HaltonVdC(int dimension);
        double Generate();
        ~HaltonVdC();
};