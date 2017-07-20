#pragma once

#include <string>
#include <vector>
#include <algorithm> 
#include <cctype>
#include <locale>

namespace flounder 
{
	class helperstring 
	{
	public:
		static std::vector<std::string> split(const std::string &str, const std::string &sep) 
		{
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

		static bool startsWith(const std::string &text, const std::string &token)
		{
			if (text.length() < token.length())
			{
				return false;
			}

			return (text.compare(0, token.length(), token) == 0);
		}

		static bool contains(const std::string &text, const std::string &token)
		{
			return text.find(token) != std::string::npos;
		}

		static inline bool isInteger(const std::string &text)
		{
			if (text.empty() || ((!isdigit(text[0])) && (text[0] != '-') && (text[0] != '+'))) 
			{
				return false;
			}

			char *p;
			strtol(text.c_str(), &p, 10);

			return (*p == 0);
		}

		static int findCharPos(const std::string &line, const char &c) {
			for (unsigned int i = 0; i < line.length(); i++)
			{
				if (line.at(i) == c) 
				{
					return i;
				}
			}

			return 0;
		}

		static std::string trim(const std::string &str, const std::string &whitespace = " \t") 
		{
			const auto strBegin = str.find_first_not_of(whitespace);

			if (strBegin == std::string::npos)
			{
				return ""; // no content
			}

			const auto strEnd = str.find_last_not_of(whitespace);
			const auto strRange = strEnd - strBegin + 1;

			std::string result = str;
			result = result.substr(strBegin, strRange);
			return result;
		}

		static std::string substring(const std::string &str, const int &start, const int &end)
		{
			std::string result = str;
			result = result.substr(start, end - start);
			return result;
		}

		static std::string removeAll(const std::string &str, const char &token)
		{
			std::string result = str;
			result.erase(std::remove(result.begin(), result.end(), token), result.end());
			return result;
		}

		static std::string replace(const std::string& str, const std::string& from, const std::string& to)
		{
			std::string result = str;
			size_t start_pos = result.find(from);

			if (start_pos == std::string::npos)
			{
				return result;
			}

			result.replace(start_pos, from.length(), to);
			return result;
		}
	};
}