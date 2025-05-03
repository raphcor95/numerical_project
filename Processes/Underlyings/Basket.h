#include "Underlying.h"

class Basket : public Underlying
{
    public:
        Basket(RandomProcess* generator, double initVal, const std::vector<double>& vecWeights);
        ~Basket();
        void Simulate(double startTime, double endTime, size_t nbSteps);
    
};