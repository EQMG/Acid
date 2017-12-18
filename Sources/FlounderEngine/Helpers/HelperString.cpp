#include "HelperString.hpp"

#include <algorithm>
#include <cctype>

namespace Flounder
{
	std::vector<std::string> HelperString::Split(const std::string &str, const std::string &sep)
	{
		char *cstr = const_cast<char*>(str.c_str());
		std::vector<std::string> arr;
		char *current = strtok(cstr, sep.c_str());

		while (current != nullptr)
		{
			arr.push_back(current);
			current = strtok(nullptr, sep.c_str());
		}

		return arr;
	}

	bool HelperString::StartsWith(const std::string &str, const std::string &token)
	{
		if (str.length() < token.length())
		{
			return false;
		}

		return str.compare(0, token.length(), token) == 0;
	}

	bool HelperString::Contains(const std::string &str, const std::string &token)
	{
		return str.find(token) != std::string::npos;
	}

	bool HelperString::IsInteger(const std::string &str)
	{
		if (str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+')))
		{
			return false;
		}

		char *p;
		strtol(str.c_str(), &p, 10);

		return *p == 0;
	}

	int HelperString::FindCharPos(const std::string &str, const char &c)
	{
		for (int i = 0; i < static_cast<int>(str.length()); i++)
		{
			if (str.at(i) == c)
			{
				return i;
			}
		}

		return -1;
	}

	std::string HelperString::Trim(const std::string &str, const std::string &whitespace)
	{
		const auto strBegin = str.find_first_not_of(whitespace);

		if (strBegin == std::string::npos)
		{
			// No content.
			return "";
		}

		const auto strEnd = str.find_last_not_of(whitespace);
		const auto strRange = strEnd - strBegin + 1;

		std::string result = str;
		result = result.substr(strBegin, strRange);
		return result;
	}

	std::string HelperString::Substring(const std::string &str, const int &start, const int &end)
	{
		std::string result = str;
		result = result.substr(start, end - start);
		return result;
	}

	std::string HelperString::RemoveAll(const std::string &str, const char &token)
	{
		std::string result = str;
		result.erase(remove(result.begin(), result.end(), token), result.end());
		return result;
	}

	std::string HelperString::Replace(const std::string &str, const std::string &token, const std::string &to)
	{
		std::string result = str;
		const size_t startPos = result.find(token);

		if (startPos == std::string::npos)
		{
			return result;
		}

		result.replace(startPos, token.length(), to);
		return result;
	}
}
