#ifndef PODEM_SOLVER_H
#define PODEM_SOLVER_H

#include "Circuit.h"
#include "logic.h"
#include <vector>

class PodemSolver {
public:
    // Constructor
    PodemSolver(Circuit& c, const std::string& faultGateName, FaultType fault);

    // Main function to run the algorithm
    bool solve(std::vector<std::pair<std::string, LogicValue>>& testVector);

private:
    // --- Core PODEM Functions ---
    bool podemRecursion();
    
    bool getObjective(Gate*& objectiveGate, LogicValue& objectiveValue);
    
    void backtrace(Gate* objectiveGate, LogicValue objectiveValue, 
                   Gate*& pi, LogicValue& piValue);
                   
    void updateDFrontier();

    // --- State Variables ---
    Circuit& circuit;
    Gate* faultGate;
    FaultType faultType;
    LogicValue faultActivationVal;
    
    std::vector<Gate*> dFrontier;

    // --- Helper Functions ---
    bool checkTest();
    Gate* findEasiestDFrontierGate();
    
    // Helper for backtrace
    LogicValue getControllingValue(GateType type);
    
    // NEW HELPER for getObjective
    LogicValue getNonControllingValue(GateType type);
    
    bool isInvertingGate(GateType type);
};

#endif // PODEM_SOLVER_H