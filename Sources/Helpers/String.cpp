#include "String.hpp"

namespace acid
{
std::vector<std::string> String::Split(const std::string &str, const std::string &sep, bool trim)
{
	std::unique_ptr<char[]> copy{new char[strlen(str.c_str()) + 1]};
	std::strcpy(copy.get(), str.c_str());

	std::vector<std::string> splitVector;
	auto current = std::strtok(copy.get(), sep.c_str());

	while (current != nullptr)
	{
		auto currentS = std::string{current};

		if (trim)
		{
			currentS = Trim(currentS);
		}

		splitVector.emplace_back(currentS);
		current = std::strtok(nullptr, sep.c_str());
	}

	return splitVector;
}

bool String::StartsWith(std::string_view str, std::string_view token)
{
	if (str.length() < token.length())
	{
		return false;
	}

	return str.compare(0, token.length(), token) == 0;
}

bool String::Contains(std::string_view str, std::string_view token)
{
	return str.find(token) != std::string::npos;
}

bool String::IsInteger(std::string_view str)
{
	return str.find_first_not_of("0123456789") == std::string::npos;
}

int String::FindCharPos(std::string_view str, char c)
{
	auto res = str.find(c);
	return res == std::string::npos ? -1 : res;
}

std::string String::Trim(std::string str, std::string_view whitespace)
{
	auto strBegin = str.find_first_not_of(whitespace);

	if (strBegin == std::string::npos)
	{
		return "";
	}

	auto strEnd = str.find_last_not_of(whitespace);
	auto strRange = strEnd - strBegin + 1;

	auto trimmed = str;
	trimmed = trimmed.substr(strBegin, strRange);
	return trimmed;
}

std::string String::Substring(std::string str, uint32_t start, uint32_t end)
{
	str = str.substr(start, end - start);
	return str;
}

std::string String::RemoveAll(std::string str, char token)
{
	str.erase(remove(str.begin(), str.end(), token), str.end());
	return str;
}

std::string String::RemoveLast(std::string str, char token)
{
	for (auto it = str.end(); it != str.begin(); --it)
	{
		if (*it == token)
		{
			str.erase(it);
			return str;
		}
	}

	return str;
}

std::string String::ReplaceAll(std::string str, std::string_view token, std::string_view to)
{
	auto pos = str.find(token);

	while (pos != std::string::npos)
	{
		str.replace(pos, token.size(), to);
		pos = str.find(token, pos + token.size());
	}

	return str;
}

std::string String::ReplaceFirst(std::string str, std::string_view token, std::string_view to)
{
	const auto startPos = str.find(token);

	if (startPos == std::string::npos)
	{
		return str;
	}

	str.replace(startPos, token.length(), to);
	return str;
}

std::string String::FixReturnTokens(const std::string &str)
{
	// TODO: Optimize.
	return ReplaceAll(ReplaceAll(str, "\n", "\\n"), "\r", "\\r");
}

std::string String::Lowercase(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}

std::string String::Uppercase(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
	return str;
}
}
