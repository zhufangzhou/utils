#ifndef __CMDLINE_H
#define __CMDLINE_H

#include <iostream>
#include <map>
#include <string>

class cmdLineParser {
private:
	std::map<std::string, std::string> param;
	std::map<std::string, std::string> info;
public:
	cmdLineParser(int argc, char** argv) {
			
	}


};


#endif
