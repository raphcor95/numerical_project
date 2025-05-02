#include "../RandomProcess.h"

class Underlying
{
    protected:
        RandomProcess* Generator;
        SinglePath* Path;
        double InitVal;
        std::vector<double> VecWeights;

    public:
        Underlying(RandomProcess* generator, double initVal, const std::vector<double>& vecWeights);
        ~Underlying();
        virtual void Simulate(double startTime, double endTime, size_t nbSteps) = 0;
};