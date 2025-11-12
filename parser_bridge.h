#ifndef PARSER_BRIDGE_H
#define PARSER_BRIDGE_H

// This is a bridge to pass the circuit object
// to the C-style parser.
#include "Circuit.h"
extern Circuit* g_Circuit; // "g_" for global

#endif // PARSER_BRIDGE_H