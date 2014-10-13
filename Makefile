util: main.cpp 
	g++ -g main.cpp -o util -lm -std=c++0x -Wno-write-strings

clean:
	rm util 
