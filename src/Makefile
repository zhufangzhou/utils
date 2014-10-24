BIN_DIR := ../bin/
BUILD_DIR := ../build/
INCLUDE_DIR := ../include/

OBJS := $(patsubst %.cpp,$(BUILD_DIR)%.o,$(wildcard *.cpp))
CXX = g++
CC = $(CXX)
CXXFLAGS = -g -Wno-write-strings -std=c++0x 

all: create_dir util

util: $(OBJS)
	g++ $^ -o $(BIN_DIR)$@

$(BUILD_DIR)%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

.PHONY: create_dir
create_dir:
	@mkdir -p $(BIN_DIR) $(BUILD_DIR)

.PHONY: clean
clean:
	-rm $(BIN_DIR)util $(OBJS)
.PHONY: clean_all
clean_all:
	-rm -rf $(BIN_DIR) $(BUILD_DIR)
