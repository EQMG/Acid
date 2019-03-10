#include "String.hpp"

#include <algorithm>
#include <cstring>

namespace acid
{
	std::vector<std::string> String::Split(const std::string &str, const std::string &sep, const bool &trim)
	{
		std::unique_ptr<char[]> copy(new char[strlen(str.c_str()) + 1]);
		std::strcpy(copy.get(), str.c_str());

		std::vector<std::string> result = {};
		auto current = std::strtok(copy.get(), sep.c_str());

		while (current != nullptr)
		{
			auto currentS = std::string(current);

			if (trim)
			{
				currentS = Trim(currentS);
			}

			result.emplace_back(currentS);
			current = std::strtok(nullptr, sep.c_str());
		}

		return result;
	}

	bool String::StartsWith(const std::string &str, const std::string &token)
	{
		if (str.length() < token.length())
		{
			return false;
		}

		return str.compare(0, token.length(), token) == 0;
	}

	bool String::Contains(const std::string &str, const std::string &token)
	{
		return str.find(token) != std::string::npos;
	}

	bool String::IsInteger(const std::string &str)
	{
		if (str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+')))
		{
			return false;
		}

		char *p;
		std::strtol(str.c_str(), &p, 10);

		return *p == 0;
	}

	int String::FindCharPos(const std::string &str, const char &c)
	{
		for (uint32_t i = 0; i < str.length(); i++)
		{
			if (str.at(i) == c)
			{
				return i;
			}
		}

		return -1;
	}

	std::string String::Trim(const std::string &str, const std::string &whitespace)
	{
		auto strBegin = str.find_first_not_of(whitespace);

		if (strBegin == std::string::npos)
		{
			return "";
		}

		auto strEnd = str.find_last_not_of(whitespace);
		auto strRange = strEnd - strBegin + 1;

		auto result = str;
		result = result.substr(strBegin, strRange);
		return result;
	}

	std::string String::Substring(const std::string &str, const uint32_t &start, const uint32_t &end)
	{
		auto result = str;
		result = result.substr(start, end - start);
		return result;
	}

	std::string String::RemoveAll(const std::string &str, const char &token)
	{
		auto result = str;
		result.erase(remove(result.begin(), result.end(), token), result.end());
		return result;
	}

	std::string String::RemoveLast(const std::string &str, const char &token)
	{
		auto result = str;

		for (auto it = result.end(); it != result.begin(); --it)
		{
			if (*it == token)
			{
				result.erase(it);
				return result;
			}
		}

		return result;
	}

	std::string String::ReplaceAll(const std::string &str, const std::string &token, const std::string &to)
	{
		auto result = str;
		auto pos = result.find(token);

		while (pos != std::string::npos)
		{
			result.replace(pos, token.size(), to);
			pos = result.find(token, pos + token.size());
		}

		return result;
	}

	std::string String::ReplaceFirst(const std::string &str, const std::string &token, const std::string &to)
	{
		auto result = str;
		const auto startPos = result.find(token);

		if (startPos == std::string::npos)
		{
			return result;
		}

		result.replace(startPos, token.length(), to);
		return result;
	}

	std::string String::FixReturnTokens(const std::string &str)
	{
		// TODO: Optimize.
		return ReplaceAll(ReplaceAll(str, "\n", "\\n"), "\r", "\\r");
	}

	std::string String::Lowercase(const std::string &str)
	{
		auto result = str;
		std::transform(result.begin(), result.end(), result.begin(), tolower);
		return result;
	}

	std::string String::Uppercase(const std::string &str)
	{
		auto result = str;
		std::transform(result.begin(), result.end(), result.begin(), toupper);
		return result;
	}
}
