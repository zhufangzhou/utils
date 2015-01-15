#pragma once 

#include <iostream>
#include <map>
#include <string>

class cmdLineParser {
private:
	std::map<std::string, std::string> option;
	std::map<std::string, std::string> info;
public:
	cmdLineParser (int argc, char** argv) {
		int i = 1;
		char *opt, *opt_val;
		while (i < argc) {
			opt = argv[i];
			if (strlen(opt) > 0 && opt[0] == '-') {
				// supoort `--opt_name' or `-opt_name'
				if (strlen(opt) > 1 && opt[1] == '-')
					opt = opt + 2;
				else
					opt = opt + 1;
				
				// check whether this option has appeared
				if (option.find(opt) != option.end()) {
					std::cerr << "can not specified " << std::string(opt) << " twice" << std::endl;
					exit(EXIT_FAILURE);
				}

				i++; // move on to option value

				if (i < argc) {
					opt_val = argv[i];
					if (strlen(opt_val) > 0 && opt_val[0] == '-') {
						option[std::string(opt)] = std::string("1"); // if not specified value, give 1 for default means open this option
						continue; // deal with this option name in next iteration
					} 
				} else {
					opt_val = new char[1];
					opt_val[0] = '1';
				}
				// give value to corresponding option name
				option[std::string(opt)] = std::string(opt_val);

				i++; // move on to option name
			} else {
				std::cerr << "bad command line argument" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}

	const std::string registerOption(std::string opt_name, std::string info_content) {
		info[opt_name] = info_content;
		return opt_name;
	}

	bool hasOption(const std::string& opt_name) {
		if (option.find(opt_name) != option.end()) {
			return true;
		} else {
			return false;
		}
	}

	void checkOption() {
		for (auto it = option.begin(); it != option.end(); it++) {
			if (info.find(it->first) == info.end()) {
				std::cerr << "the option `" << it->first << "` do not register, please check again" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}

	void displayOption() {
		for (auto it = info.begin(); it != info.end(); it++) {
			std::cout << "-" << it->first << ":\t" << it->second << std::endl;
		}
	}

	std::string getOptionValue(const std::string& opt_name) {
		if (option.find(opt_name) != option.end()) {
			return option[opt_name];
		} else {
			throw "no option named " + opt_name;
		}
	}

};


