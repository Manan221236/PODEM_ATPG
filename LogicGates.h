#ifndef LOGIC_GATES_H
#define LOGIC_GATES_H

#include "Gate.h"
#include "logic_util.h"
#include <vector>

// --- Primary Input (PI) ---
class PiGate : public Gate {
public:
    PiGate(const std::string& name);
    LogicValue evaluate() override;
};

// --- Primary Output (PO) ---
class PoGate : public Gate {
public:
    PoGate(const std::string& name);
    LogicValue evaluate() override;
};

// --- NOT Gate ---
class NotGate : public Gate {
public:
    NotGate(const std::string& name);
    LogicValue evaluate() override;
};

// --- Buffer Gate ---
class BuffGate : public Gate {
public:
    BuffGate(const std::string& name);
    LogicValue evaluate() override;
};

// --- Fanout Gate (NEW) ---
class FanoutGate : public Gate {
public:
    FanoutGate(const std::string& name);
    LogicValue evaluate() override;
};

// --- 2-Input Gates ---
class AndGate : public Gate {
public:
    AndGate(const std::string& name);
    LogicValue evaluate() override;
};

class OrGate : public Gate {
public:
    OrGate(const std::string& name);
    LogicValue evaluate() override;
};

// --- Multi-Input Gates (Handle > 2 inputs) ---
class NandGate : public Gate {
public:
    NandGate(const std::string& name);
    LogicValue evaluate() override;
};

class NorGate : public Gate {
public:
    NorGate(const std::string& name);
    LogicValue evaluate() override;
};

class XorGate : public Gate {
public:
    XorGate(const std::string& name);
    LogicValue evaluate() override;
};

class XnorGate : public Gate {
public:
    XnorGate(const std::string& name);
    LogicValue evaluate() override;
};

#endif // LOGIC_GATES_H