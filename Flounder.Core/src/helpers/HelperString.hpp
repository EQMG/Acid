#pragma once

#include <vector>
#include <locale>

namespace Flounder
{
	/// <summary>
	/// A helper for C++ strings.
	/// </summary>
	class HelperString
	{
	public:
		/// <summary>
		/// Splits a string by a seperator.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="sep"> The seperator. </param>
		/// <returns> The split string vector. </returns>
		static std::vector<std::string> Split(const std::string &str, const std::string &sep);

		/// <summary>
		/// Gets if a string starts with a token.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <returns> If a string starts with the token. </returns>
		static bool StartsWith(const std::string &str, const std::string &token);

		/// <summary>
		/// Gets if a string contains a token.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <returns> If a string contains the token. </returns>
		static bool Contains(const std::string &str, const std::string &token);

		/// <summary>
		/// Gets if a string is a integer.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <returns> If a string is a integer. </returns>
		static bool IsInteger(const std::string &str);

		/// <summary>
		/// Gets the first char index in the string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="c"> The char to look for. </param>
		/// <returns> The char index. </returns>
		static int FindCharPos(const std::string &str, const char &c);

		/// <summary>
		/// Trims the left and right side of a string of whitespace.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="whitespace"> The whitespace type. </param>
		/// <returns> The trimmed string. </returns>
		static std::string Trim(const std::string &str, const std::string &whitespace = " \t");

		/// <summary>
		/// Takes a substring of a string between two bounds.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="start"> The left bound. </param>
		/// <param name="end"> The right bound. </param>
		/// <returns> The substring of the string. </returns>
		static std::string Substring(const std::string &str, const int &start, const int &end);

		/// <summary>
		/// Removes all tokens from a string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <returns> The string with the tokens removed. </returns>
		static std::string RemoveAll(const std::string &str, const char &token);

		/// <summary>
		/// Replaces all tokens from a string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <param name="to"> The string to replace the tokens with. </param>
		/// <returns> The string with the tokens replaced. </returns>
		static std::string Replace(const std::string &str, const std::string &token, const std::string &to);
	};
}
