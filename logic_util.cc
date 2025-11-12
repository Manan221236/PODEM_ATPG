#include "logic_util.h"

LogicValue logic_not(LogicValue val) {
    switch (val) {
        case LogicValue::ZERO: return LogicValue::ONE;
        case LogicValue::ONE:  return LogicValue::ZERO;
        case LogicValue::D:    return LogicValue::DBAR;
        case LogicValue::DBAR: return LogicValue::D;
        case LogicValue::X:    return LogicValue::X;
        default:               return LogicValue::UNSET;
    }
}

// Implements the 5-valued AND truth table
LogicValue logic_and(LogicValue v1, LogicValue v2) {
    if (v1 == LogicValue::ZERO || v2 == LogicValue::ZERO) return LogicValue::ZERO;
    if (v1 == LogicValue::ONE && v2 == LogicValue::ONE) return LogicValue::ONE;
    if (v1 == LogicValue::D && v2 == LogicValue::D) return LogicValue::D;
    if (v1 == LogicValue::DBAR && v2 == LogicValue::DBAR) return LogicValue::DBAR;
    if ((v1 == LogicValue::D && v2 == LogicValue::DBAR) || (v1 == LogicValue::DBAR && v2 == LogicValue::D)) return LogicValue::ZERO;
    if ((v1 == LogicValue::ONE && v2 == LogicValue::D) || (v1 == LogicValue::D && v2 == LogicValue::ONE)) return LogicValue::D;
    if ((v1 == LogicValue::ONE && v2 == LogicValue::DBAR) || (v1 == LogicValue::DBAR && v2 == LogicValue::ONE)) return LogicValue::DBAR;
    
    // All other combinations with X or UNSET result in X
    return LogicValue::X;
}

// Implements the 5-valued OR truth table
LogicValue logic_or(LogicValue v1, LogicValue v2) {
    if (v1 == LogicValue::ONE || v2 == LogicValue::ONE) return LogicValue::ONE;
    if (v1 == LogicValue::ZERO && v2 == LogicValue::ZERO) return LogicValue::ZERO;
    if (v1 == LogicValue::D && v2 == LogicValue::D) return LogicValue::D;
    if (v1 == LogicValue::DBAR && v2 == LogicValue::DBAR) return LogicValue::DBAR;
    if ((v1 == LogicValue::D && v2 == LogicValue::DBAR) || (v1 == LogicValue::DBAR && v2 == LogicValue::D)) return LogicValue::ONE;
    if ((v1 == LogicValue::ZERO && v2 == LogicValue::D) || (v1 == LogicValue::D && v2 == LogicValue::ZERO)) return LogicValue::D;
    if ((v1 == LogicValue::ZERO && v2 == LogicValue::DBAR) || (v1 == LogicValue::DBAR && v2 == LogicValue::ZERO)) return LogicValue::DBAR;

    return LogicValue::X;
}

LogicValue logic_nand(LogicValue v1, LogicValue v2) {
    return logic_not(logic_and(v1, v2));
}

LogicValue logic_nor(LogicValue v1, LogicValue v2) {
    return logic_not(logic_or(v1, v2));
}

LogicValue logic_xor(LogicValue v1, LogicValue v2) {
    return logic_or(logic_and(v1, logic_not(v2)), logic_and(logic_not(v1), v2));
}

LogicValue logic_xnor(LogicValue v1, LogicValue v2) {
    return logic_not(logic_xor(v1, v2));
}