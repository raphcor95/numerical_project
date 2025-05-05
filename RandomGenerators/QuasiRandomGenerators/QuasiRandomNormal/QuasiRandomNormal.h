#include "../QuasiRandom.h"
#include "../LDSequences/LDSequence.h"

class QuasiRandomNormal : public QuasiRandom
{
    protected:
        LDSequence* Sequence;

    public:
        QuasiRandomNormal(LDSequence* sequence);
        ~QuasiRandomNormal();
};