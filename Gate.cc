#include "Gate.h"

Gate::Gate(const std::string& name, GateType type)
    : name(name), 
      type(type), 
      value(LogicValue::UNSET),
      fault(FaultType::NO_FAULT) {}

void Gate::clearValue() {
    // PIs hold their value, others are unknown
    if (type != GateType::PI) {
        value = LogicValue::UNSET;
    }
}