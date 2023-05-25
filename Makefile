# c++ compiler
CXX = g++

# flags
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -I.

# output program names
OUTPUT1 = interactive_tests
OUTPUT2 = automated_tests

# default rule (builds both programs)
all: $(OUTPUT1) $(OUTPUT2)

# rule for building interactive test program
$(OUTPUT1): ThreadHive.cpp tests/interactive.cpp
	$(CXX) $(CXXFLAGS) ThreadHive.cpp tests/interactive.cpp -o $(OUTPUT1)

# rule for building automated test program
$(OUTPUT2): ThreadHive.cpp tests/automated.cpp
	$(CXX) $(CXXFLAGS) ThreadHive.cpp tests/automated.cpp -o $(OUTPUT2)

# clean rule
clean:
	$(RM) $(OUTPUT1) $(OUTPUT2)
