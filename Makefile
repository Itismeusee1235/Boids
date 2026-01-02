CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -O2 -std=c++17 -lSDL2

# find all source files
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = boids-sim 

all: $(TARGET)

# link step
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# compile step
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o src/*.o $(TARGET)
