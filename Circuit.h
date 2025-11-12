#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "Gate.h"
#include <string>
#include <vector>
#include <map>
#include <memory> 

class Circuit {
public:
    Circuit() = default;

    // --- Building Functions ---
    Gate* addGate(const std::string& name, GateType type);
    void addOutput(const std::string& name);
    void connect(const std::string& sourceName, const std::string& destName);

    // --- Accessors ---
    Gate* getGate(const std::string& name);
    std::vector<Gate*>& getPIs() { return PIs; }
    std::vector<Gate*>& getPOs() { return POs; }
    std::vector<std::unique_ptr<Gate>>& getAllGates() { return allGates; }
    bool isPO(Gate* g);

    // --- Simulation & State ---
    void clearAllValues();
    void setAllFaults(FaultType f);

    // Performs a topological sort and adds fanout gates
    void buildSimulationList();
    
    // Runs a full simulation
    void runFullSimulation(); // <-- REPLACES simulateEvent

private:
    // Helper for addGate
    Gate* createGate(const std::string& name, GateType type);

    // This vector OWNS all the gates
    std::vector<std::unique_ptr<Gate>> allGates;
    
    // This map provides fast lookups by name
    std::map<std::string, Gate*> gateMap;
    
    // Pointers for quick access
    std::vector<Gate*> PIs;
    std::vector<std::string> poNames; 
    std::vector<Gate*> POs;
    
    // For ordered simulation
    std::vector<Gate*> simulationList;
};

#endif // CIRCUIT_H