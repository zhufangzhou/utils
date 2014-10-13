util: main.cpp Utils.cpp
	g++ main.cpp Utils.cpp -o util -lm -std=c++0x -Wno-write-strings

clean:
	rm util utils.o
