#include "LongstaffSchwarz.h"

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
    if (endTime < times.back())
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
    if (VecPaths.size() > 0)
    {
        while (VecPaths.size() > 0)
        {
            VecPaths.pop_back();
        }
    }

    for (size_t n = 0; n < NbSim; n++)
    {   
        // Simulating the process
        Undl->Simulate(StartTime, EndTime, NbSteps);

        // Storing the Pointer in the Paths vector
        VecPaths.push_back(new SinglePath(*Undl->ReturnPath()));
    }
}