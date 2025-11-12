#ifndef GATE_H
#define GATE_H

#include "logic.h"
#include <string>
#include <vector>
#include <memory>
#include <algorithm> // For std::find

class Gate {
public:
    // Constructor
    Gate(const std::string& name, GateType type);
    
    virtual ~Gate() = default;

    // --- Pure Virtual Functions ---
    virtual LogicValue evaluate() = 0; 

    // --- Common Functions ---
    void clearValue();
    
    // Getters
    const std::string& getName() const { return name; }
    GateType getType() const { return type; }
    LogicValue getValue() const { return value; }
    const std::vector<Gate*>& getInputs() const { return inputs; }
    const std::vector<Gate*>& getOutputs() const { return outputs; }

    // Setters
    void addInput(Gate* gate) { inputs.push_back(gate); }
    void addOutput(Gate* gate) { outputs.push_back(gate); }
    void setValue(LogicValue val) { this->value = val; }
    void setFault(FaultType f) { this->fault = f; }
    FaultType getFault() const { return fault; }

    // --- NEW HELPERS FOR FANOUT ---
    void clearOutputs() { outputs.clear(); }
    void replaceInput(Gate* oldGate, Gate* newGate) {
        auto it = std::find(inputs.begin(), inputs.end(), oldGate);
        if (it != inputs.end()) {
            *it = newGate;
        }
    }

protected:
    std::string name;
    GateType type;
    LogicValue value;
    FaultType fault;
    
    std::vector<Gate*> inputs;
    std::vector<Gate*> outputs;
};

#endif // GATE_H