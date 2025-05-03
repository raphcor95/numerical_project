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
    Times(times)
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