#ifndef LOGIC_UTIL_H
#define LOGIC_UTIL_H

#include "logic.h"
#include <vector>

// 5-valued logic NOT
LogicValue logic_not(LogicValue val);

// 5-valued logic AND
LogicValue logic_and(LogicValue v1, LogicValue v2);

// 5-valued logic OR
LogicValue logic_or(LogicValue v1, LogicValue v2);

// 5-valued logic XOR
LogicValue logic_xor(LogicValue v1, LogicValue v2);

// These can be derived from the ones above
LogicValue logic_nand(LogicValue v1, LogicValue v2);
LogicValue logic_nor(LogicValue v1, LogicValue v2);
LogicValue logic_xnor(LogicValue v1, LogicValue v2);

#endif // LOGIC_UTIL_H