# Modern C++ PODEM ATPG Tool

This is an Automatic Test Pattern Generation (ATPG) tool written from scratch in modern **C++17**.  
It implements the **PODEM (Path-Oriented Decision Making)** algorithm to generate test vectors for single stuck-at (SSA) faults in digital circuits.

---

## 🧠 About This Project

This program parses standard ISCAS `.bench` circuit files and `.fault` lists.  
For each specified fault, it uses the **PODEM algorithm** to find a primary input (PI) vector that will propagate the fault (as a `D` or `D-bar`) to a primary output (PO).

It was designed using modern, object-oriented **C++17**, smart pointers for memory safety, and a modular build system using **CMake**.  
Developed and tested on macOS with the Clang compiler and Conda toolchain.

---

## 🚀 Features

- **Core PODEM Algorithm:**
  1. **Objective:** Select an objective gate and value.
  2. **Backtrace:** Trace the objective back to a primary input.
  3. **Simulate:** Propagate the input value and check for success.
  4. **Recurse:** Backtrack if a path fails.

- **5-Valued Logic System:**  
  (`0`, `1`, `X`, `D`, `D-bar`) for fault simulation.

- **Circuit Parser:**  
  Built with **Flex** and **Bison**, supports standard ISCAS `.bench` files.

- **Fault List Parsing:**  
  Reads `.fault` files for SSA fault specification.

- **Fanout Handling:**  
  Automatically detects fanout stems and creates branch gates (e.g., `gate_0`, `gate_1`).

- **Modern C++ Design:**  
  Uses `enum class`, `std::unique_ptr`, and encapsulated class structures.

---

## 📂 Project Structure

```
.
├── CMakeLists.txt          # Main build file
├── parser.l                # Flex (lex) file for parsing
├── parser.y                # Bison (yacc) file for parsing
│
├── logic.h                 # Defines LogicValue, GateType, etc.
├── logic_util.h/.cc        # 5-valued logic helper functions
│
├── Gate.h/.cc              # Abstract base class for all gates
├── LogicGates.h/.cc        # Concrete implementations (AndGate, NotGate, etc.)
│
├── Circuit.h/.cc           # Manages the circuit (gate network)
├── PodemSolver.h/.cc       # The core PODEM algorithm implementation
│
├── main.cc                 # Main driver: reads files, calls solver
├── parser_bridge.h         # Connects C parser to C++ circuit
│
├── build/                  # (Created by cmake) Build files
└── test/                   # Benchmark circuits (c17, c432, etc.)
```

---

## 🛠️ Technologies Used

- **C++17**
- **CMake**
- **Flex & Bison**
- **Conda** (for build tool management on macOS)

---

## 🧩 Build Instructions (macOS + Conda)

> macOS includes outdated versions of Flex and Bison — Conda provides newer versions.

### 1️⃣ Install Prerequisites

**Xcode Command Line Tools:**
```bash
xcode-select --install
```

**CMake:**
```bash
brew install cmake
```

**Conda (Miniconda or Anaconda)** — required for toolchain.

---

### 2️⃣ Set Up Environment

```bash
conda create -n atpg
conda activate atpg
conda install flex bison
```

---

### 3️⃣ Build the Project

**Clone the Repository:**
```bash
git clone https://github.com/your-username/your-repo-name.git
cd your-repo-name
```

**Locate Flex & Bison Paths:**
```bash
which flex
# Example: /opt/anaconda3/envs/atpg/bin/flex
which bison
# Example: /opt/anaconda3/envs/atpg/bin/bison
```

**Create Build Directory and Configure with CMake:**
```bash
mkdir build
cd build

cmake .. -DFLEX_EXECUTABLE=/opt/anaconda3/envs/atpg/bin/flex          -DBISON_EXECUTABLE=/opt/anaconda3/envs/atpg/bin/bison
```

**Compile:**
```bash
make
```

Executable `PODEM_ATPG` will be created inside the `build/` directory.

---

## ⚙️ Usage

Run from the `build` directory with:
```bash
./PODEM_ATPG <path-to-bench-file> <output-file-name> <path-to-fault-file>
```

**Example:**
```bash
./PODEM_ATPG ../test/ex1.bench my_ex1.out ../test/ex1.fault
```

This will:
- Load the circuit and fault list.
- Generate test vectors using PODEM.
- Write results to `my_ex1.out`.

---

## 🧪 Testing

Compare output with provided `.refout` reference files.

```bash
# Run test
./PODEM_ATPG ../test/ex1.bench my_ex1.out ../test/ex1.fault

# Verify output
diff my_ex1.out ../test/ex1.refout
```

If no differences are shown, the output matches perfectly.

---

## 🧭 Available Test Circuits

- `ex1`
- `ex2`
- `c17`
- `target`
- `target2`
- `c432` (small, medium, and large fault lists)

---

**Developed with ❤️ using Modern C++17 and clean design principles.**
