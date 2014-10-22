BIN_DIR = bin/
INCLUDE_DIR = include/
ALL_LIB = src/main.o src/utils.o src/random.o src/container.o src/distance.o
CC = g++
CXXFLAGS = -g -Wno-write-strings -std=c++0x 

util: $(ALL_LIB)
	mkdir -p $(BIN_DIR)
	g++ $(ALL_LIB) -o $(BIN_DIR)util

%.o: %.cpp
	g++ -g -std=c++0x -c $< -o $@ -Iinclude/

clean:
	rm $(BIN_DIR)util $(ALL_LIB)
