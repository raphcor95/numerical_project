#include "Pricer.h"
#include <iostream>

// Constructor
Pricer::Pricer(
    Underlying* undl,
    double nbSim,
    double startTime,
    double endTime,
    size_t nbSteps
) : 
    Undl(undl),
    NbSim(nbSim),
    StartTime(startTime),
    EndTime(endTime),
    NbSteps(nbSteps)
{

}

// Destructor
Pricer::~Pricer()
{

}