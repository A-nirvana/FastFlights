# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -I./include

# Source files
SRC_CPP := $(wildcard src/*.cpp)
SRC_C := $(wildcard src/*.c)
MAIN := main.cpp
OBJS := $(SRC_CPP:.cpp=.o) $(SRC_C:.c=.o) main.o

# Output executable
TARGET = FastFlights

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compiling C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compiling C files
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build
clean:
	rm -f $(OBJS) $(TARGET)
