#pragma once

#include <String>
#include <fstream>

namespace flounder {
	class helperfile {
	public:
		static std::string readFile(const std::string &filepath)
		{
			std::ifstream ifs = std::ifstream(filepath.c_str());
			
			if (!ifs.is_open())
			{
				std::cout << "Could not file file: " << filepath << std::endl;
			}

			std::string content(
				std::istreambuf_iterator<char>(ifs.rdbuf()),
				std::istreambuf_iterator<char>()
			);
			return content;
		}
	};
}