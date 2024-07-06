# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Find all .cpp files in the current directory
SOURCES = $(wildcard *.cpp)

# Executable names (each source file with .out extension)
EXECUTABLES = $(SOURCES:.cpp=.out)

# Default target
all: $(EXECUTABLES)

# Rule to create each executable from the corresponding source file
%.out: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean target to remove generated executables
clean:
	rm -f $(EXECUTABLES)

# Phony targets
.PHONY: all clean
