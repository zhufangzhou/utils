#ifndef __CMDLINE_H
#define __CMDLINE_H

#include <iostream>
#include <map>
#include <string>

class cmdLineParser {
private:
	std::map<std::string, std::string> option;
	std::map<std::string, std::string> info;
public:
	cmdLineParser (int argc, char** argv) {
		int i;
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
					throw "can not specified " + std::string(opt) + " twice";
				}

				i++; // move on to option value

				if (i+1 < argc) {
					opt_val = argv[i+1];
					if (strlen(opt_val) > 0 && opt_val[0] == '-') {
						option[std::string(opt)] = std::string("1"); // if not specified value, give 1 for default means open this option
						continue; // deal with this option name in next iteration
					} else {
						opt_val = "";
					}
				} else {
					opt_val = "";
				}
				// give value to corresponding option name
				option[std::string(opt)] = std::string(opt_val);

				i++; // move on to option name
			} else {
				throw "bad command line argument";
			}
		}
	}

	const std::string registerOption(std::string opt_name, std::string info_content) {
		info[opt_name] = info_content;
		return opt_name;
	}

	void checkOption() {
		for (auto it = option.begin(); it != option.end(); it++) {
			if (info.find(it->first) == info.end()) {
				throw "the option " + it->first + " do not register, please check again";
			}
		}
	}

	void displayOption() {
		for (auto it = info.begin(); it != info.end(); it++) {
			std::cout << "-" << it->first << ":\t" << it->second << std::endl;
		}
	}

	std::string getOptionValue(std::string opt_name) {
		if (option.find(opt_name) != option.end()) {
			return option[opt_name];
		} else {
			throw "no option named " + opt_name;
		}
	}

};


#endif
