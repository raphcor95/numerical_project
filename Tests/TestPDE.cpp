#include <iostream>
#include <fstream>
#include "../PDE/PDEGrid2D.h"

int main()
{
    // Main Pricing Variables
    double spot = 100.0;
    double strike = 100.0;
    double maturity = 1.0;
    double rate = 0.05;
    double vol = 0.1;

    // PDE Variables
    double sMin = 0.0;
    double sMax = 1000.0;
    double spotStep = 1;
    double nTimeSteps = 252;
    R2R1Function* VarianceFunction = new BSVariance(vol);
    R2R1Function* TrendFunction = new BSTrend(rate);
    R2R1Function* ActualizationFunction = new BSActualization(rate);
    R2R1Function* SourceTermFunction = new NullFunction();

    /* Grid Setup */
    R1R1Function* TopBoundaryCondition = new CallTopBoundary(sMax, strike);
    R1R1Function* BottomBoundaryCondition = new CallBottomBoundary(sMin, strike);
    R1R1Function* RightBoundaryCondition = new CallTerminalCondition(strike);
    PDEGrid2DExplicit* BSCallPricingGrid = new PDEGrid2DExplicit(
        maturity,
        sMin,
        sMax,
        nTimeSteps,
        spotStep,
        VarianceFunction,
        TrendFunction,
        ActualizationFunction,
        SourceTermFunction,
        TopBoundaryCondition,
        BottomBoundaryCondition,
        RightBoundaryCondition
    );

    // /* Solving for Price */
    BSCallPricingGrid->FillNodes();
    double result = BSCallPricingGrid->GetTimeZeroNodeValue(spot);
    std::cout << "Call price: " << result << std::endl;
    BSCallPricingGrid->OutputNodes();

    return 0;
}