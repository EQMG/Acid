#include "String.hpp"

#include <codecvt>
#include <sstream>
#include <algorithm>

namespace acid {
std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> UTF8_TO_UTF16_CONVERTER;

std::string String::ConvertUtf8(const std::wstring_view &string) {
	return UTF8_TO_UTF16_CONVERTER.to_bytes(string.data(), string.data() + string.length());
}

char String::ConvertUtf8(wchar_t c) {
	return UTF8_TO_UTF16_CONVERTER.to_bytes(c)[0];
}

std::wstring String::ConvertUtf16(const std::string_view &string) {
	return UTF8_TO_UTF16_CONVERTER.from_bytes(string.data(), string.data() + string.length());
}

wchar_t String::ConvertUtf16(char c) {
	return UTF8_TO_UTF16_CONVERTER.from_bytes(c)[0];
}

std::vector<std::string> String::Split(const std::string &str, char sep) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);

	while (std::getline(tokenStream, token, sep))
		tokens.push_back(token);

	return tokens;
}

bool String::StartsWith(std::string_view str, std::string_view token) {
	if (str.length() < token.length()) 
		return false;

	return str.compare(0, token.length(), token) == 0;
}

bool String::Contains(std::string_view str, std::string_view token) noexcept {
	return str.find(token) != std::string::npos;
}

bool String::IsWhitespace(char c) noexcept {
	return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

bool String::IsNumber(std::string_view str) noexcept {
	// This is the fastest way to tell if a string holds a decimal number.
	return std::all_of(str.cbegin(), str.cend(), [](const auto c) {
		return (c >= '0' && c <= '9') || c == '.' || c == '-';
	});
}

int32_t String::FindCharPos(std::string_view str, char c) noexcept {
	auto res = str.find(c);
	return res == std::string::npos ? -1 : static_cast<int32_t>(res);
}

std::string String::Trim(std::string str, std::string_view whitespace) {
	auto strBegin = str.find_first_not_of(whitespace);

	if (strBegin == std::string::npos)
		return "";

	auto strEnd = str.find_last_not_of(whitespace);
	auto strRange = strEnd - strBegin + 1;

	auto trimmed = str;
	trimmed = trimmed.substr(strBegin, strRange);
	return trimmed;
}

std::string String::RemoveAll(std::string str, char token) {
	str.erase(std::remove(str.begin(), str.end(), token), str.end());
	return str;
}

std::string String::RemoveLast(std::string str, char token) {
	for (auto it = str.end(); it != str.begin(); --it) {
		if (*it == token) {
			str.erase(it);
			return str;
		}
	}

	return str;
}

std::string String::ReplaceAll(std::string str, std::string_view token, std::string_view to) {
	auto pos = str.find(token);

	while (pos != std::string::npos) {
		str.replace(pos, token.size(), to);
		pos = str.find(token, pos + token.size());
	}

	return str;
}

std::string String::ReplaceFirst(std::string str, std::string_view token, std::string_view to) {
	const auto startPos = str.find(token);

	if (startPos == std::string::npos)
		return str;

	str.replace(startPos, token.length(), to);
	return str;
}

std::string String::FixEscapedChars(std::string str) {
	static const std::vector<std::pair<char, std::string_view>> replaces = { {'\n', "\\n"}, {'\r', "\\r"}, {'\t', "\\t"} };

	for (const auto &[from, to] : replaces) {
		auto pos = str.find(from);
		while (pos != std::string::npos) {
			str.replace(pos, 1, to);
			pos = str.find(from, pos + 2);
		}
	}
	
	return str;
}

std::string String::UnfixEscapedChars(std::string str) {
	static const std::vector<std::pair<std::string_view, char>> replaces = { {"\\r", '\r'}, {"\\n", '\n'}, {"\\t", '\t'} };

	for (const auto &[from, to] : replaces) {
		auto pos = str.find(from);
		while (pos != std::string::npos) {
			if (pos != 0 && str[pos - 1] == '\\')
				str.erase(str.begin() + --pos);
			else
				str.replace(pos, from.size(), 1, to);
			pos = str.find(from, pos + 1 + from.size());
		}
	}

	return str;
}

std::string String::Lowercase(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}

std::string String::Uppercase(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), toupper);
	return str;
}
}
