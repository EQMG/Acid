#include "String.hpp"

#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>

namespace acid
{
std::vector<std::string> String::Split(const std::string &str, const char &sep)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream{str};

	while (std::getline(tokenStream, token, sep))
	{
		tokens.push_back(token);
	}

	return tokens;
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

bool String::IsWhitespace(const char &c)
{
	return std::string{" \n\r  "}.find(c) != std::string::npos;
}

bool String::IsNumber(std::string_view str)
{
	return !str.empty() && std::find_if(str.begin(), str.end(), [](const auto c)
	{
		return std::string{"0123456789.-"}.find(c) == std::string::npos;
	}) == str.end();
}

int32_t String::FindCharPos(std::string_view str, char c)
{
	auto res{str.find(c)};
	return res == std::string::npos ? -1 : static_cast<int32_t>(res);
}

std::string String::Trim(std::string str, std::string_view whitespace)
{
	auto strBegin{str.find_first_not_of(whitespace)};

	if (strBegin == std::string::npos)
	{
		return "";
	}

	auto strEnd{str.find_last_not_of(whitespace)};
	auto strRange{strEnd - strBegin + 1};

	auto trimmed{str};
	trimmed = trimmed.substr(strBegin, strRange);
	return trimmed;
}

std::string String::RemoveAll(std::string str, char token)
{
	str.erase(std::remove(str.begin(), str.end(), token), str.end());
	return str;
}

std::string String::RemoveLast(std::string str, char token)
{
	for (auto it{str.end()}; it != str.begin(); --it)
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
	auto pos{str.find(token)};

	while (pos != std::string::npos)
	{
		str.replace(pos, token.size(), to);
		pos = str.find(token, pos + token.size());
	}

	return str;
}

std::string String::ReplaceFirst(std::string str, std::string_view token, std::string_view to)
{
	const auto startPos{str.find(token)};

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

std::string String::UnfixReturnTokens(const std::string &str)
{
	// TODO: Optimize.
	return ReplaceAll(ReplaceAll(str, "\\n", "\n"), "\\r", "\r");
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
