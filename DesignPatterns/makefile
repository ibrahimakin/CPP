# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Find all .cpp files in the current directory and subdirectories
SOURCES = $(shell find . -name '*.cpp')

# Executable names (each source file with .out extension, placed in the same directory as the source)
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