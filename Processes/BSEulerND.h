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
        void Simulate(double startTime, double endTime, size_t nbSteps) override;
        void Simulate(double startTime, double endTime, size_t nbSteps, std::vector<std::vector<double>>* vecW_anti=nullptr);

        // Destructor
        ~BSEulerND();
            
};