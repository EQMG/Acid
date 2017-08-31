#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <locale>

namespace Flounder
{
	/// <summary>
	/// A helper for C++ strings.
	/// </summary>
	class helperstring
	{
	public:
		/// <summary>
		/// Splits a string by a seperator.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="sep"> The seperator. </param>
		/// <returns> The split string vector. </returns>
		static std::vector<std::string> split(const std::string &str, const std::string &sep)
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

		/// <summary>
		/// Gets if a string starts with a token.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <returns> If a string starts with the token. </returns>
		static bool startsWith(const std::string &str, const std::string &token)
		{
			if (str.length() < token.length())
			{
				return false;
			}

			return (str.compare(0, token.length(), token) == 0);
		}

		/// <summary>
		/// Gets if a string contains a token.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <returns> If a string contains the token. </returns>
		static bool contains(const std::string &str, const std::string &token)
		{
			return str.find(token) != std::string::npos;
		}

		/// <summary>
		/// Gets if a string is a integer.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <returns> If a string is a integer. </returns>
		static bool isInteger(const std::string &str)
		{
			if (str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+')))
			{
				return false;
			}

			char *p;
			strtol(str.c_str(), &p, 10);

			return (*p == 0);
		}

		/// <summary>
		/// Gets the first char index in the string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="c"> The char to look for. </param>
		/// <returns> The char index. </returns>
		static int findCharPos(const std::string &str, const char &c)
		{
			for (int i = 0; i < str.length(); i++)
			{
				if (str.at(i) == c)
				{
					return i;
				}
			}

			return 0;
		}

		/// <summary>
		/// Trims the left and right side of a string of whitespace.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="whitespace"> The whitespace type. </param>
		/// <returns> The trimmed string. </returns>
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

		/// <summary>
		/// Takes a substring of a string between two bounds.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="start"> The left bound. </param>
		/// <param name="end"> The right bound. </param>
		/// <returns> The substring of the string. </returns>
		static std::string substring(const std::string &str, const int &start, const int &end)
		{
			std::string result = str;
			result = result.substr(start, end - start);
			return result;
		}

		/// <summary>
		/// Removes all tokens from a string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <returns> The string with the tokens removed. </returns>
		static std::string removeAll(const std::string &str, const char &token)
		{
			std::string result = str;
			result.erase(remove(result.begin(), result.end(), token), result.end());
			return result;
		}

		/// <summary>
		/// Replaces all tokens from a string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <param name="to"> The string to replace the tokens with. </param>
		/// <returns> The string with the tokens replaced. </returns>
		static std::string replace(const std::string &str, const std::string &token, const std::string &to)
		{
			std::string result = str;
			size_t start_pos = result.find(token);

			if (start_pos == std::string::npos)
			{
				return result;
			}

			result.replace(start_pos, token.length(), to);
			return result;
		}
	};
}
