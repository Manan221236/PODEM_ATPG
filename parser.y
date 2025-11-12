%{
#include <iostream>
#include <string>
#include <vector>
#include "logic.h"
#include "Circuit.h"
#include "parser_bridge.h" // For g_Circuit

// External C function prototypes
extern "C" int yylex();
extern "C" int yyparse(); // C-style function name for linking
extern "C" FILE *yyin;
void yyerror(const char *s);

// A global vector to hold the list of input names
std::vector<std::string> g_InputList;
%}

/* --- Bison Union --- */
/* This defines what types of data our tokens can hold */
%union {
    std::string* str;       // For identifiers (gate names)
    GateType     gateType;  // For GATETYPE tokens
}

/* --- Token Definitions --- */
%token LPAREN RPAREN EQUALS COMMA
%token INPUT OUTPUT
%token <gateType> GATETYPE
%token <str> IDENTIFIER

/* --- Grammar Rules --- */
%type <str> id_list_item



%%

/* The top-level rule: a file is one or more lines */
lines:
    line
    | lines line
    ;

line:
    input_line
    | output_line
    | assign_line
    | /* empty line */
    ;

/* Rule for INPUT(gateName) */
input_line:
    INPUT LPAREN IDENTIFIER RPAREN {
        g_Circuit->addGate(*$3, GateType::PI);
        delete $3; // Free the string memory
    };

/* Rule for OUTPUT(gateName) */
output_line:
    OUTPUT LPAREN IDENTIFIER RPAREN {
        g_Circuit->addOutput(*$3);
        delete $3; // Free the string memory
    };

/* Rule for outputName = GATE(input1, input2, ...) */
assign_line:
    IDENTIFIER EQUALS GATETYPE LPAREN id_list RPAREN {
        // 1. Create the destination gate
        g_Circuit->addGate(*$1, $3);
        
        // 2. Connect all inputs from the global list
        for (const std::string& inputName : g_InputList) {
            // Ensure the input gate exists (create if not)
            g_Circuit->addGate(inputName, GateType::BUFF); // Type doesn't matter here
            // Connect it
            g_Circuit->connect(inputName, *$1);
        }
        
        // 3. Clean up
        g_InputList.clear();
        delete $1; // Free the string memory
    };

/* Rule to build the list of inputs */
id_list:
    id_list_item {
        g_InputList.push_back(*$1);
        delete $1;
    }
    | id_list COMMA id_list_item {
        g_InputList.push_back(*$3);
        delete $3;
    }
    ;

/* An item in the list is just an identifier */
id_list_item:
    IDENTIFIER { $$ = $1; };

%%

/* --- C Code --- */
#include <iostream> // <--- THIS IS THE FIX

/* The error-reporting function */
void yyerror(const char *s) {
    std::cerr << "Parser Error: " << s << std::endl;
}