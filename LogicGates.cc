#include "LogicGates.h"
#include <stdexcept>

// --- Helper function to apply faults ---
// This is the correct logic from the original project's setValueCheckFault
LogicValue applyFault(LogicValue goodValue, FaultType fault) {
    if (fault == FaultType::SA0) {
        if (goodValue == LogicValue::ONE) return LogicValue::D;
        if (goodValue == LogicValue::DBAR) return LogicValue::ZERO;
        // else, 0, D, X remain 0, D, X
    }
    if (fault == FaultType::SA1) {
        if (goodValue == LogicValue::ZERO) return LogicValue::DBAR;
        if (goodValue == LogicValue::D) return LogicValue::ONE;
        // else, 1, DBAR, X remain 1, DBAR, X
    }
    return goodValue;
}

// --- PI ---
PiGate::PiGate(const std::string& name) : Gate(name, GateType::PI) {
    value = LogicValue::X;
}
LogicValue PiGate::evaluate() {
    // A PI's "good value" is its own value.
    LogicValue good_value = this->value; 
    return applyFault(good_value, this->fault);
}

// --- PO ---
PoGate::PoGate(const std::string& name) : Gate(name, GateType::PO) {}
LogicValue PoGate::evaluate() {
    if (inputs.empty()) {
        value = LogicValue::X;
    } else {
        value = inputs[0]->getValue();
    }
    // POs are just observers and don't have faults in this model
    return value;
}

// --- NOT ---
NotGate::NotGate(const std::string& name) : Gate(name, GateType::NOT) {}
LogicValue NotGate::evaluate() {
    LogicValue good_value;
    if (inputs.empty()) {
        good_value = LogicValue::X;
    } else {
        LogicValue in_val = inputs[0]->getValue();
        good_value = logic_not(in_val);
    }
    return applyFault(good_value, this->fault);
}

// --- BUFF ---
BuffGate::BuffGate(const std::string& name) : Gate(name, GateType::BUFF) {}
LogicValue BuffGate::evaluate() {
    LogicValue good_value;
    if (inputs.empty()) {
        good_value = LogicValue::X;
    } else {
        good_value = inputs[0]->getValue();
    }
    return applyFault(good_value, this->fault);
}

// --- FANOUT ---
FanoutGate::FanoutGate(const std::string& name) : Gate(name, GateType::FANOUT) {}
LogicValue FanoutGate::evaluate() {
    LogicValue good_value;
    if (inputs.empty()) {
        good_value = LogicValue::X;
    } else {
        good_value = inputs[0]->getValue();
    }
    return applyFault(good_value, this->fault);
}


// --- 2-Input AND ---
AndGate::AndGate(const std::string& name) : Gate(name, GateType::AND) {}
LogicValue AndGate::evaluate() {
    LogicValue good_value = LogicValue::ONE; // Identity for AND
    for (Gate* in_gate : inputs) {
        good_value = logic_and(good_value, in_gate->getValue());
    }
    return applyFault(good_value, this->fault);
}

// --- 2-Input OR ---
OrGate::OrGate(const std::string& name) : Gate(name, GateType::OR) {}
LogicValue OrGate::evaluate() {
    LogicValue good_value = LogicValue::ZERO; // Identity for OR
    for (Gate* in_gate : inputs) {
        good_value = logic_or(good_value, in_gate->getValue());
    }
    return applyFault(good_value, this->fault);
}

// --- Multi-Input NAND ---
NandGate::NandGate(const std::string& name) : Gate(name, GateType::NAND) {}
LogicValue NandGate::evaluate() {
    LogicValue and_result = LogicValue::ONE;
    for (Gate* in_gate : inputs) {
        and_result = logic_and(and_result, in_gate->getValue());
    }
    LogicValue good_value = logic_not(and_result);
    return applyFault(good_value, this->fault);
}

// --- Multi-Input NOR ---
NorGate::NorGate(const std::string& name) : Gate(name, GateType::NOR) {}
LogicValue NorGate::evaluate() {
    LogicValue or_result = LogicValue::ZERO;
    for (Gate* in_gate : inputs) {
        or_result = logic_or(or_result, in_gate->getValue());
    }
    LogicValue good_value = logic_not(or_result);
    return applyFault(good_value, this->fault);
}

// --- Multi-Input XOR ---
XorGate::XorGate(const std::string& name) : Gate(name, GateType::XOR) {}
LogicValue XorGate::evaluate() {
    LogicValue good_value = LogicValue::ZERO; // Identity for XOR
    for (Gate* in_gate : inputs) {
        good_value = logic_xor(good_value, in_gate->getValue());
    }
    return applyFault(good_value, this->fault);
}

// --- Multi-Input XNOR ---
XnorGate::XnorGate(const std::string& name) : Gate(name, GateType::XNOR) {}
LogicValue XnorGate::evaluate() {
    LogicValue xor_result = LogicValue::ZERO;
    for (Gate* in_gate : inputs) {
        xor_result = logic_xor(xor_result, in_gate->getValue());
    }
    LogicValue good_value = logic_not(xor_result);
    return applyFault(good_value, this->fault);
}