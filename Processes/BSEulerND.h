#include "BlackScholesND.h"
#include <vector>

class BSEulerND : public BlackScholesND
{
    public:

        // Constructor
        BSEulerND(Normal* Gen, 
                    const std::vector<double>& vecSpots,
                    const std::vector<double>& vecRates,
                    Matrix* matCorr
        );
        
        // Simulating Method
        void Simulate(double startTime, double endTime, size_t nbSteps);

        // Destructor
        ~BSEulerND();
            
};