%{
#include <string>
#include "logic.h"        // Defines GateType
#include "parser.tab.h"   // Include the Bison-generated header
#define YY_DECL extern "C" int yylex() // C-style function name for linking
%}

%option noyywrap
/* %option nounistd -- This line is commented out to fix 'isatty' error */

%%

[ \t\n]+          { /* Skip whitespace */ }
"#".* { /* Skip comments */ }

"("               { return LPAREN; }
")"               { return RPAREN; }
"="               { return EQUALS; }
","               { return COMMA; }

"INPUT"           { return INPUT; }
"input"           { return INPUT; } /* ADDED */
"OUTPUT"          { return OUTPUT; }
"output"          { return OUTPUT; } /* ADDED */

"AND"             { yylval.gateType = GateType::AND; return GATETYPE; }
"and"             { yylval.gateType = GateType::AND; return GATETYPE; } /* ADDED */
"OR"              { yylval.gateType = GateType::OR; return GATETYPE; }
"or"              { yylval.gateType = GateType::OR; return GATETYPE; } /* ADDED */
"NOT"             { yylval.gateType = GateType::NOT; return GATETYPE; }
"not"             { yylval.gateType = GateType::NOT; return GATETYPE; } /* ADDED */
"NAND"            { yylval.gateType = GateType::NAND; return GATETYPE; }
"nand"            { yylval.gateType = GateType::NAND; return GATETYPE; } /* ADDED */
"NOR"             { yylval.gateType = GateType::NOR; return GATETYPE; }
"nor"             { yylval.gateType = GateType::NOR; return GATETYPE; } /* ADDED */
"XOR"             { yylval.gateType = GateType::XOR; return GATETYPE; }
"xor"             { yylval.gateType = GateType::XOR; return GATETYPE; } /* ADDED */
"XNOR"            { yylval.gateType = GateType::XNOR; return GATETYPE; }
"xnor"            { yylval.gateType = GateType::XNOR; return GATETYPE; } /* ADDED */
"BUFF"            { yylval.gateType = GateType::BUFF; return GATETYPE; }
"buff"            { yylval.gateType = GateType::BUFF; return GATETYPE; } /* ADDED */

[a-zA-Z0-9_\[\]]+  { 
                  yylval.str = new std::string(yytext); 
                  return IDENTIFIER; 
                  }

.                 { /* Ignore invalid characters */ }

%%