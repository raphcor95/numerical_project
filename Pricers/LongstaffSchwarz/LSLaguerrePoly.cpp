#include "LSLaguerrePoly.h"
#include <iostream>

LSLaguerrePoly::LSLaguerrePoly(
    Underlying* undl,
    std::vector<double> times,
    double nbSim,
    double startTime,
    double endTime,
    size_t nbSteps
) : 
    LongstaffSchwarz(undl, times, nbSim, startTime, endTime, nbSteps)
{

}

LSLaguerrePoly::~LSLaguerrePoly()
{

}

double LSLaguerrePoly::Price()
{
    std::cout << "Not implemented yet" << std::endl;
}