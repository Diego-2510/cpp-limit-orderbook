CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

all: order.o

order.o: src/order.cpp
	$(CXX) $(CXXFLAGS) -c src/order.cpp -o order.o

clean:
	rm -f *.o
