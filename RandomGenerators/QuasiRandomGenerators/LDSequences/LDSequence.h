#pragma once
#include "../QuasiRandom.h"

class LDSequence : public QuasiRandom
{
    protected:
        int Dimension;

    public:
        LDSequence(int dimension);
        ~LDSequence();
};