#include "Circuit.h"
#include "LogicGates.h"
#include <stdexcept>
#include <queue>
#include <set>
#include <sstream> // For fanout naming

// Private helper to create gate objects
Gate* Circuit::createGate(const std::string& name, GateType type) {
    std::unique_ptr<Gate> newGate;
    if (type == GateType::PI) {
        newGate = std::make_unique<PiGate>(name);
    } else if (type == GateType::PO) {
        newGate = std::make_unique<PoGate>(name);
    } else if (type == GateType::AND) {
        newGate = std::make_unique<AndGate>(name);
    } else if (type == GateType::OR) {
        newGate = std::make_unique<OrGate>(name);
    } else if (type == GateType::NOT) {
        newGate = std::make_unique<NotGate>(name);
    } else if (type == GateType::NAND) {
        newGate = std::make_unique<NandGate>(name);
    } else if (type == GateType::NOR) {
        newGate = std::make_unique<NorGate>(name);
    } else if (type == GateType::XOR) {
        newGate = std::make_unique<XorGate>(name);
    } else if (type == GateType::XNOR) {
        newGate = std::make_unique<XnorGate>(name);
    } else if (type == GateType::BUFF) {
        newGate = std::make_unique<BuffGate>(name);
    } else if (type == GateType::FANOUT) { // <-- ADDED
        newGate = std::make_unique<FanoutGate>(name);
    } else {
        throw std::runtime_error("Unknown gate type");
    }
    return newGate.release(); // Release ownership to the caller
}


Gate* Circuit::addGate(const std::string& name, GateType type) {
    if (gateMap.count(name)) {
        return gateMap[name];
    }
    
    Gate* newGatePtr = createGate(name, type);
    
    allGates.emplace_back(newGatePtr); // Vector takes ownership
    gateMap[name] = newGatePtr;        // Map gets raw pointer
    
    if (type == GateType::PI) {
        PIs.push_back(newGatePtr);
    } else if (type == GateType::PO) {
        POs.push_back(newGatePtr);
    }
    
    return newGatePtr;
}

void Circuit::addOutput(const std::string& name) {
    poNames.push_back(name);
}

void Circuit::connect(const std::string& sourceName, const std::string& destName) {
    Gate* source = getGate(sourceName);
    Gate* dest = getGate(destName);
    
    if (!source) {
        // If source doesn't exist, create it (as a buffer)
        // This handles cases where .bench files are incomplete
        source = addGate(sourceName, GateType::BUFF);
    }
     if (!dest) {
        dest = addGate(destName, GateType::BUFF);
    }
    
    dest->addInput(source);
    source->addOutput(dest);
}

Gate* Circuit::getGate(const std::string& name) {
    if (gateMap.count(name)) {
        return gateMap[name];
    }
    return nullptr;
}

bool Circuit::isPO(Gate* g) {
    for (auto po : POs) {
        if (po == g) return true;
    }
    return false;
}

void Circuit::clearAllValues() {
    for (auto& gate : allGates) {
        if (gate->getType() != GateType::PI) {
            gate->setValue(LogicValue::UNSET);
        } else {
            gate->setValue(LogicValue::X);
        }
    }
}

void Circuit::setAllFaults(FaultType f) {
    for (auto& gate : allGates) {
        gate->setFault(f);
    }
}

// NEW: Runs a full simulation using the topologically sorted list
void Circuit::runFullSimulation() {
    for (Gate* g : simulationList) {
        LogicValue val = g->evaluate();
        g->setValue(val);
    }
}

// MODIFIED: This function now also creates fanout gates
void Circuit::buildSimulationList() {
    // --- FIX FOR PROBLEM 2: Add Fanout Gates ---
    // This logic is from the original project's ClassCircuit.cc
    std::vector<Gate*> originalGates;
    for(auto& g_ptr : allGates) {
        originalGates.push_back(g_ptr.get());
    }

    for (Gate* g : originalGates) {
        if (g->getOutputs().size() > 1) {
            // This is a fanout stem
            std::vector<Gate*> originalOutputs = g->getOutputs();
            g->clearOutputs(); // Disconnect stem from branches

            for (int i = 0; i < originalOutputs.size(); ++i) {
                Gate* branch = originalOutputs[i];
                
                // Create a new fanout gate name (e.g., "A_0", "A_1")
                std::stringstream fanoutName;
                fanoutName << g->getName() << "_" << i;
                
                // Create the new fanout gate
                Gate* fanoutGate = addGate(fanoutName.str(), GateType::FANOUT);
                
                // Connect g -> fanoutGate -> branch
                g->addOutput(fanoutGate);
                fanoutGate->addInput(g);
                fanoutGate->addOutput(branch);
                branch->replaceInput(g, fanoutGate); // Requires a helper
            }
        }
    }

    // --- Original Logic ---
    // First, hook up the POs
    for (const std::string& name : poNames) {
        Gate* gate = getGate(name);
        if (gate) {
            std::string poName = name + "_PO";
            Gate* poGate = addGate(poName, GateType::PO);
            connect(name, poName);
        }
    }

    // Now, do a topological sort (Kahn's algorithm)
    simulationList.clear();
    std::map<Gate*, int> inDegree;
    std::queue<Gate*> q;

    for (auto& gate : allGates) {
        inDegree[gate.get()] = gate->getInputs().size();
        if (inDegree[gate.get()] == 0) {
            q.push(gate.get()); 
        }
    }

    while (!q.empty()) {
        Gate* g = q.front();
        q.pop();
        simulationList.push_back(g);

        for (Gate* output : g->getOutputs()) {
            inDegree[output]--;
            if (inDegree[output] == 0) {
                q.push(output);
            }
        }
    }
}