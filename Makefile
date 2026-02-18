CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC = src/order.cpp src/orderbook.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = orderbook_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) benchmark

benchmark: src/order.cpp src/orderbook.cpp src/benchmark.cpp
	$(CXX) $(CXXFLAGS) -O2 -o benchmark $^

.PHONY: all clean benchmark
