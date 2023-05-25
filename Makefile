
# c++ compiler
CXX = g++

# flags
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -I.

# output library name
LIB_OUTPUT = libThreadHive.a

# output program names
OUTPUT1 = interactive_tests
OUTPUT2 = automated_tests

# default rule
all: $(LIB_OUTPUT)

# rule for building library
$(LIB_OUTPUT): ThreadHive.o
	ar rcs $(LIB_OUTPUT) ThreadHive.o

ThreadHive.o: ThreadHive.cpp ThreadHive.h
	$(CXX) $(CXXFLAGS) -c ThreadHive.cpp -o ThreadHive.o

# rule for building interactive test program
$(OUTPUT1): ThreadHive.o tests/interactive.cpp
	$(CXX) $(CXXFLAGS) ThreadHive.o tests/interactive.cpp -o $(OUTPUT1)

# rule for building automated test program
$(OUTPUT2): ThreadHive.o tests/automated.cpp
	$(CXX) $(CXXFLAGS) ThreadHive.o tests/automated.cpp -o $(OUTPUT2)

# clean rule
clean:
	$(RM) $(LIB_OUTPUT) $(OUTPUT1) $(OUTPUT2) ThreadHive.o
