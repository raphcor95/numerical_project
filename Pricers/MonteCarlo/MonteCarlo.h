#pragma once
#include "../Pricer.h"
#include "../../Utils/Matrix.h"

class MonteCarlo : public Pricer
{
    protected:
        std::vector<double> VecTimes;
        std::vector<SinglePath*> VecPaths;

        
        public:
        MonteCarlo(
            Underlying* myBasket,
            double nbSim,
            double startTime,
            double endTime,
            size_t nbSteps
        );
        std::vector<std::vector<double>> SimulatedPaths;
        
        ~MonteCarlo();
        void Simulate();
        //Juste pour pas planter 
        virtual double Price() override;

};