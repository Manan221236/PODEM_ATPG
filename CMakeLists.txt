# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Name your project
project(MyATPG CXX)
# set(CMAKE_MACOSX_RPATH True) # <-- We are replacing this

# Set the C++ standard to C++17 (for smart pointers, etc.)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the Flex (lexer) and Bison (parser) tools
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# --- Parser Generation ---
# Define the files that Bison and Flex will create
set(PARSER_SOURCES)
set(PARSER_HEADERS)

# Add the Bison parser rule
BISON_TARGET(BenchParser 
             ${CMAKE_CURRENT_SOURCE_DIR}/parser.y 
             ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp
             
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.h)

# Add the Flex lexer rule
FLEX_TARGET(BenchLexer
            ${CMAKE_CURRENT_SOURCE_DIR}/parser.l
            ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cpp
            
            DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.h)

# Add the generated files to our list of source files
list(APPEND PARSER_SOURCES ${BISON_BenchParser_OUTPUTS})
list(APPEND PARSER_SOURCES ${FLEX_BenchLexer_OUTPUTS})

# Add the generated header to our include path
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# --- Main Executable ---
# Tell the linker where to find conda libraries
link_directories($ENV{CONDA_PREFIX}/lib)
# List all of your C++ implementation (.cc) files
set(SOURCES
    main.cc
    Gate.cc
    LogicGates.cc
    Circuit.cc
    PodemSolver.cc
    logic_util.cc
    ${PARSER_SOURCES}
)

# Define your executable
add_executable(PODEM_ATPG ${SOURCES})

# --- THE FIX ---
# Manually add the RPATH for your Conda 'lib' directory to the executable
set_target_properties(PODEM_ATPG PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ENV{CONDA_PREFIX}/lib")

# We don't need to link fl or ll because of %option noyywrap