#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Circuit.h"
#include "PodemSolver.h"

// Bison/Flex C-style functions
extern "C" {
    extern FILE *yyin;
    extern int yyparse();
}

// The global bridge for the parser
Circuit* g_Circuit = nullptr;

// Helper to print test vectors
std::string printPIValue(LogicValue v) {
    switch(v) {
        case LogicValue::ZERO: return "0";
        case LogicValue::ONE:  return "1";
        case LogicValue::D:    return "1"; // D means 1 in good, 0 in bad
        case LogicValue::DBAR: return "0"; // DBAR means 0 in good, 1 in bad
        default:               return "X";
    }
}


int main(int argc, char* argv[]) {
    // 1. Check Arguments
    if (argc != 4) {
        std::cerr << "Usage: ./my_atpg [bench_file] [output_file] [fault_file]" << std::endl;
        return 1;
    }

    std::string benchFile = argv[1];
    std::string outputFile = argv[2];
    std::string faultFile = argv[3];

    // 2. Create the Circuit Object
    Circuit circuit;
    g_Circuit = &circuit; // Set the global bridge for the parser

    // 3. Parse the Bench File
    FILE* bench = fopen(benchFile.c_str(), "r");
    if (!bench) {
        std::cerr << "Error: Cannot open bench file " << benchFile << std::endl;
        return 1;
    }
    yyin = bench;
    yyparse(); // This populates the 'circuit' object via the global pointer
    fclose(bench);

    // 4. Build the simulation list
    circuit.buildSimulationList();
    std::cout << "Circuit parsed and ready." << std::endl;

    // 5. Open Fault and Output Files
    std::ifstream faultStream(faultFile);
    std::ofstream outputStream(outputFile);
    
    if (!faultStream.is_open()) {
        std::cerr << "Error: Cannot open fault file " << faultFile << std::endl;
        return 1;
    }
    if (!outputStream.is_open()) {
        std::cerr << "Error: Cannot open output file " << outputFile << std::endl;
        return 1;
    }

    // 6. Loop through faults and run solver
    std::string faultGateName;
    int faultVal;
    while (faultStream >> faultGateName >> faultVal) {
        
        // Reset all circuit values to UNSET/X
        circuit.clearAllValues();
        
        FaultType fault = (faultVal == 0) ? FaultType::SA0 : FaultType::SA1;
        
        try {
            // Create the solver
            PodemSolver solver(circuit, faultGateName, fault);
            
            // Run the solver
            std::vector<std::pair<std::string, LogicValue>> testVector;
            bool success = solver.solve(testVector);

            if (success) {
                // Print the test vector to the file
                for (const auto& pi : circuit.getPIs()) {
                    // Find the value in our testVector map
                    for (const auto& tv_pair : testVector) {
                        if (tv_pair.first == pi->getName()) {
                            outputStream << printPIValue(tv_pair.second);
                            break;
                        }
                    }
                }
                outputStream << std::endl;
                std::cout << "Fault: " << faultGateName << "/" << faultVal << " -> TEST FOUND" << std::endl;

            } else {
                outputStream << "none found" << std::endl;
                std::cout << "Fault: " << faultGateName << "/" << faultVal << " -> none found" << std::endl;
            }

        } catch (const std::exception& e) {
            std::cerr << "Error processing fault " << faultGateName << ": " << e.what() << std::endl;
            outputStream << "error" << std::endl;
        }
    }

    faultStream.close();
    outputStream.close();

    std::cout << "ATPG complete. Results in " << outputFile << std::endl;
    return 0;
}