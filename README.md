Modern C++ PODEM ATPG ToolThis is an Automatic Test Pattern Generation (ATPG) tool written from scratch in modern C++17. It implements the PODEM (Path-Oriented Decision Making) algorithm to generate test vectors for single stuck-at (SSA) faults in digital circuits.About This ProjectThis program parses standard ISCAS .bench circuit files and .fault lists. For each specified fault, it uses the PODEM algorithm to find a primary input (PI) vector that will propagate the fault (as a D or D-bar) to a primary output (PO).This implementation was built using an object-oriented design, smart pointers for memory management, and a CMake build system. It was developed and tested on macOS using the Clang compiler and tools from a Conda environment.FeaturesCore PODEM Algorithm: Implements the complete PODEM flow:Objective: Select an objective gate and value.Backtrace: Trace the objective back to a primary input.Simulate: Propagate the input value and check for success.Recurse: Backtrack if a path fails.5-Valued Logic: Uses a 5-valued logic system (0, 1, X, D, D-bar) for fault simulation [cite: logic.h].Circuit Parser: A custom parser built with Flex and Bison reads and constructs circuits from standard .bench files (both uppercase and lowercase) [cite: parser.l, parser.y].Fault List Parsing: Reads standard .fault files.Fanout Handling: Automatically detects fanout stems and creates dedicated branch gates (e.g., gate_0, gate_1) to allow for branch-level fault testing [cite: Circuit.cc].Modern C++: Written in C++17, using enum class, std::unique_ptr for memory management, and a clean, encapsulated class structure [cite: Gate.h, Circuit.h].Project Structure.
├── CMakeLists.txt      # The main build file
├── parser.l            # Flex (lex) file for parsing
├── parser.y            # Bison (yacc) file for parsing
│
├── logic.h             # Defines LogicValue, GateType, etc.
├── logic_util.h/.cc    # 5-valued logic helper functions
│
├── Gate.h/.cc          # Abstract base class for all gates
├── LogicGates.h/.cc    # Concrete implementations (AndGate, NotGate, etc.)
│
├── Circuit.h/.cc       # Manages the circuit (gate network)
├── PodemSolver.h/.cc   # The core PODEM algorithm implementation
│
├── main.cc             # Main driver: reads files, calls solver
├── parser_bridge.h     # Helper to connect C parser to C++ circuit
│
├── build/              # (Created by cmake) Build files
└── test/               # Benchmark circuits (c17, c432, etc.)
Technologies UsedC++17CMakeFlex & BisonConda (for managing build tools on macOS)Build Instructions (macOS with Conda)This project relies on flex and bison versions that are newer than the ones included with macOS. The easiest way to build is by using a Conda environment.1. PrerequisitesXcode Command Line Tools: (Provides the clang++ compiler)xcode-select --install
CMake:brew install cmake
Conda: (Miniconda or Anaconda)2. Set Up EnvironmentCreate a Conda environment and install the necessary build tools:conda create -n atpg
conda activate atpg
conda install flex bison
3. Build the ProjectClone the repository:git clone [https://github.com/your-username/your-repo-name.git](https://github.com/your-username/your-repo-name.git)
cd your-repo-name
Find your Conda tool paths:which flex
# (e.g., /opt/anaconda3/envs/atpg/bin/flex)
which bison
# (e.g., /opt/anaconda3/envs/atpg/bin/bison)
Create the build directory and run CMake:mkdir build
cd build

# Run cmake, pointing it to your Conda tools
# (Replace paths with your output from step 2)
cmake .. -DFLEX_EXECUTABLE=/opt/anaconda3/envs/atpg/bin/flex \
         -DBISON_EXECUTABLE=/opt/anaconda3/envs/atpg/bin/bison
Compile the code:make
An executable named PODEM_ATPG will be created in the build/ directory.UsageThe program is run from the build directory with three arguments: the circuit file, the output file, and the fault file.Syntax:./PODEM_ATPG <path-to-bench-file> <output-file-name> <path-to-fault-file>
Example (running the ex1 test):./PODEM_ATPG ../test/ex1.bench my_ex1.out ../test/ex1.fault
This will run the program, find test vectors for all faults in ex1.fault, and save the results to my_ex1.out.TestingYou can verify your output against the provided reference files (.refout) using diff.Example:# Run the test
./PODEM_ATPG ../test/ex1.bench my_ex1.out ../test/ex1.fault

# Check the output
diff my_ex1.out ../test/ex1.refout
If diff prints nothing, your output is a perfect match to the reference solution.This project includes the following test benches:ex1ex2c17targettarget2c432 (small, medium, and big fault lists)
