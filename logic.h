#ifndef LOGIC_H
#define LOGIC_H

#include <iostream>
#include <string>

// Use enum class for type-safe enumerations
enum class LogicValue {
    ZERO,
    ONE,
    D,
    DBAR,
    X,
    UNSET
};

enum class FaultType {
    NO_FAULT,
    SA0,
    SA1
};

// Add PI (Primary Input) and PO (Primary Output) types
enum class GateType {
    PI,
    PO,
    AND,
    OR,
    NOT,
    NAND,
    NOR,
    XOR,
    XNOR,
    BUFF,
    FANOUT // <-- ADD THIS LINE
};

// Helper function to print logic values (for debugging)
inline std::string to_string(LogicValue v) {
    switch (v) {
        case LogicValue::ZERO: return "0";
        case LogicValue::ONE:  return "1";
        case LogicValue::D:    return "D";
        case LogicValue::DBAR: return "B";
        case LogicValue::X:    return "X";
        default:               return "U";
    }
}

// Overload the << operator for easy printing
inline std::ostream& operator<<(std::ostream& os, LogicValue v) {
    os << to_string(v);
    return os;
}

#endif // LOGIC_H