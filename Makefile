BIN_DIR := bin/
INCLUDE_DIR := include/
LIB_DIR := lib/
ALL_OBJ := $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
CC = g++
CXXFLAGS = -g -Wno-write-strings -std=c++0x 

util: $(ALL_OBJ)
	mkdir -p $(BIN_DIR)
	g++ $(ALL_OBJ) -o $(BIN_DIR)util

%.o: %.cpp
	g++ -g -std=c++0x -c $< -o $@ -Iinclude/

clean:
	rm $(BIN_DIR)util $(ALL_OBJ)
