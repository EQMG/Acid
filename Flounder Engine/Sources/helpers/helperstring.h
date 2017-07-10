#pragma once

#include <string>
#include <vector>

namespace flounder {
	class helperstring {
	public:
		static std::vector<std::string> split(const std::string str, const std::string sep) {
			char* cstr = const_cast<char*>(str.c_str());
			char* current;
			std::vector<std::string> arr;
			current = strtok(cstr, sep.c_str());

			while (current != NULL) 
			{
				arr.push_back(current);
				current = strtok(NULL, sep.c_str());
			}

			return arr;
		}
	};
}