#include "PodemSolver.h"
#include "logic_util.h"
#include <stdexcept>
#include <iostream>

PodemSolver::PodemSolver(Circuit& c, const std::string& faultGateName, FaultType fault)
    : circuit(c) {
        
    faultGate = circuit.getGate(faultGateName);
    if (!faultGate) {
        throw std::runtime_error("Fault gate not found: " + faultGateName);
    }
    
    this->faultType = fault;
    
    circuit.setAllFaults(FaultType::NO_FAULT);
    faultGate->setFault(fault);
    
    faultActivationVal = (fault == FaultType::SA0) ? LogicValue::ONE : LogicValue::ZERO;
}

bool PodemSolver::solve(std::vector<std::pair<std::string, LogicValue>>& testVector) {
    // Run the main recursion
    bool success = podemRecursion();

    if (success) {
        for (Gate* pi : circuit.getPIs()) {
            testVector.push_back({pi->getName(), pi->getValue()});
        }
    }
    
    return success;
}

bool PodemSolver::podemRecursion() {
    
    // Run a full simulation first to get the current state
    circuit.runFullSimulation();

    // 1. Check if test is found (D or D-bar at a PO)
    if (checkTest()) {
        return true;
    }

    // 2. Get the next objective
    Gate* objectiveGate = nullptr;
    LogicValue objectiveValue = LogicValue::UNSET;
    if (!getObjective(objectiveGate, objectiveValue)) {
        return false; // Backtrack: Failed to find an objective
    }

    // 3. Backtrace to find a PI to set
    Gate* pi = nullptr;
    LogicValue piValue = LogicValue::UNSET;
    backtrace(objectiveGate, objectiveValue, pi, piValue);

    // 4. Try setting the PI
    pi->setValue(piValue);
    circuit.runFullSimulation();
    
    if (podemRecursion()) {
        return true; // Success!
    }

    // 5. Backtrack: Try the opposite value
    // We must reset the PI to X before trying the opposite
    pi->setValue(LogicValue::X);
    circuit.runFullSimulation();
    
    pi->setValue(logic_not(piValue));
    circuit.runFullSimulation();
    
    if (podemRecursion()) {
        return true; // Success!
    }

    // 6. Backtrack: Failed, reset PI to X
    pi->setValue(LogicValue::X);
    circuit.runFullSimulation();
    
    return false; // Backtrack
}

bool PodemSolver::checkTest() {
    for (Gate* po : circuit.getPOs()) {
        LogicValue val = po->getValue(); // Just read the value
        if (val == LogicValue::D || val == LogicValue::DBAR) {
            return true;
        }
    }
    return false;
}

bool PodemSolver::getObjective(Gate*& objectiveGate, LogicValue& objectiveValue) {
    // We read the value directly, since simulation was just run
    LogicValue faultGateVal = faultGate->getValue(); 
    
    // 1. Check if fault is activated
    if (faultGateVal == LogicValue::D || faultGateVal == LogicValue::DBAR) {
        // 2. Propagate: Find D-Frontier
        updateDFrontier();
        
        if (dFrontier.empty()) {
            return false; // Backtrack: Fault is activated but cannot be propagated
        }
        
        // 3. Find objective from D-frontier
        Gate* dGate = findEasiestDFrontierGate(); 
        
        for (Gate* input : dGate->getInputs()) {
            if (input->getValue() == LogicValue::X) {
                objectiveGate = input;
                // THIS IS THE FIX: We need the NON-controlling value
                objectiveValue = getNonControllingValue(dGate->getType());
                return true;
            }
        }
    }

    // 4. Check if fault is *not* active (value is X or the value needed for activation)
    if (faultGateVal == LogicValue::X || faultGateVal == faultActivationVal) {
        objectiveGate = faultGate;
        objectiveValue = faultActivationVal;
        return true;
    }
    
    // 5. If we are here, the fault gate has the *opposite* value (e.g., 0 for an SA0)
    // This is a conflict, so we must backtrack.
    return false;
}

void PodemSolver::updateDFrontier() {
    dFrontier.clear();
    for (auto& g_ptr : circuit.getAllGates()) {
        Gate* g = g_ptr.get();
        // A gate is on the D-frontier if its output is X
        if (g->getValue() == LogicValue::X) {
            // And at least one of its inputs is D or D-bar
            for (Gate* input : g->getInputs()) {
                if (input->getValue() == LogicValue::D || input->getValue() == LogicValue::DBAR) {
                    dFrontier.push_back(g);
                    break;
                }
            }
        }
    }
}

Gate* PodemSolver::findEasiestDFrontierGate() {
    if (!dFrontier.empty()) {
        return dFrontier[0];
    }
    return nullptr;
}

void PodemSolver::backtrace(Gate* objectiveGate, LogicValue objectiveValue, 
                           Gate*& pi, LogicValue& piValue) {
    
    Gate* currentGate = objectiveGate;
    LogicValue currentValue = objectiveValue;

    while (currentGate->getType() != GateType::PI) {
        
        if (currentGate->getType() == GateType::FANOUT) {
             currentGate = currentGate->getInputs()[0];
             continue;
        }

        Gate* nextGate = nullptr;
        for (Gate* input : currentGate->getInputs()) {
            if (input->getValue() == LogicValue::X) {
                nextGate = input;
                break;
            }
        }
        
        if (!nextGate && !currentGate->getInputs().empty()) {
            nextGate = currentGate->getInputs()[0];
        }

        if (!nextGate) {
             throw std::runtime_error("Backtrace hit a dead end");
        }

        if (isInvertingGate(currentGate->getType())) {
            currentValue = logic_not(currentValue);
        }
        
        currentGate = nextGate;
    }
    
    pi = currentGate;
    piValue = currentValue;
}

LogicValue PodemSolver::getControllingValue(GateType type) {
    switch(type) {
        case GateType::AND:
        case GateType::NAND:
            return LogicValue::ZERO;
        case GateType::OR:
        case GateType::NOR:
            return LogicValue::ONE;
        default:
            return LogicValue::X; 
    }
}

// NEW HELPER FUNCTION
LogicValue PodemSolver::getNonControllingValue(GateType type) {
    switch(type) {
        case GateType::AND:
        case GateType::NAND:
            return LogicValue::ONE;
        case GateType::OR:
        case GateType::NOR:
            return LogicValue::ZERO;
        case GateType::XOR:  // D ^ 0 = D
        case GateType::XNOR: // (D ^ 0)' = D'
            return LogicValue::ZERO;
        default:
            return LogicValue::X; // Buff/Not/Fanout
    }
}


bool PodemSolver::isInvertingGate(GateType type) {
    switch(type) {
        case GateType::NOT:
        case GateType::NAND:
        case GateType::NOR:
        case GateType::XNOR:
            return true;
        default:
            return false;
    }
}