#include "LongstaffSchwarz.h"
#include <iostream>

// Constructor
LongstaffSchwarz::LongstaffSchwarz(
    Underlying* undl,
    std::vector<double> times,
    double nbSim,
    double startTime,
    double endTime,
    size_t nbSteps
) : 
    Pricer(undl, nbSim, startTime, endTime, nbSteps),
    VecTimes(times)
{
    // Verification of the inputs
    if (startTime > times.front())
    {
        throw std::runtime_error("First observation cannot be before startTime.");
    }
    if (endTime != times.back())
    {
        throw std::runtime_error("Last observation must equal endTime = Maturity.");
    }
}

// Destructor
LongstaffSchwarz::~LongstaffSchwarz()
{

}

// Simulating the paths
void LongstaffSchwarz::Simulate()
{
    // Variables

    // Removing the previously simulated paths if any
    std::cout << "[LS] Checking whether it already exists path in the vecPaths vector." << std::endl;
    if (VecPaths.size() > 0)
    {
        for (auto path: VecPaths)
        {
            delete path;
        }
        VecPaths.clear();
    }

    for (size_t n = 0; n < NbSim; n++)
    {   
        // Simulating the process
        Undl->Simulate(StartTime, EndTime, NbSteps);

        std::cout << Undl->ReturnPath()->GetValue(NbSteps) << std::endl;

        // Storing the Pointer in the Paths vector
        std::cout << "[LS] Path address from Undl: " << Undl->ReturnPath() << std::endl;
        VecPaths.push_back(new SinglePath(*Undl->ReturnPath()));
        std::cout << "[LS] Copied path stored at: " << VecPaths.back() << std::endl;

    }
}

// Method to return the reference to the VecPaths without altering its values
const std::vector<SinglePath*>& LongstaffSchwarz::ReturnPaths() const
{
    // Check if there are realised paths
    if (VecPaths.size() == 0)
    {
        throw std::runtime_error("No Paths to be returned.");
    }

    return VecPaths;
}