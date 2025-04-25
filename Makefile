CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Iinclude/ibpp-2-5-3-1-src/core -DIBPP_LINUX
LDFLAGS = -Llib -libpp

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = repldb

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(BIN)
