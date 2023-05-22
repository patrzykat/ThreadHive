# specify compiler
CXX = g++

# Specify the C++ standard
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -I.

# Name of your output programs
OUTPUT1 = interactive_tests
OUTPUT2 = automated_tests

# default rule (builds both programs)
all: $(OUTPUT1) $(OUTPUT2)

# Rule for building the interactive test program
$(OUTPUT1): ThreadHive.cpp tests/interactive.cpp
	$(CXX) $(CXXFLAGS) ThreadHive.cpp tests/interactive.cpp -o $(OUTPUT1)

# Rule for building the automated test program
$(OUTPUT2): ThreadHive.cpp tests/automated.cpp
	$(CXX) $(CXXFLAGS) ThreadHive.cpp tests/automated.cpp -o $(OUTPUT2)

# Rule for cleaning the directory
clean:
	$(RM) $(OUTPUT1) $(OUTPUT2)
