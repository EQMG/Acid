#pragma once

#include <locale>
#include "ConstExpr.hpp"

namespace acid
{
/**
 * @brief Helper class for C++ strings.
 */
class String
{
public:
	/**
	 * Splits a string by a separator.
	 * @param str The string.
	 * @param sep The separator.
	 * @param trim If each object should be trimmed.
	 * @return The split string vector.
	 */
	static std::vector<std::string> Split(const std::string &str, const std::string &sep, bool trim = false);

	/**
	 * Gets if a string starts with a token.
	 * @param str The string.
	 * @param token The token.
	 * @return If a string starts with the token.
	 */
	static bool StartsWith(std::string_view str, std::string_view token);

	/**
	 * Gets if a string contains a token.
	 * @param str The string.
	 * @param token The token.
	 * @return If a string contains the token.
	 */
	static bool Contains(std::string_view str, std::string_view token);

	/**
	 * Gets if a string is a integer.
	 * @param str The string.
	 * @return If a string is a integer.
	 */
	static bool IsInteger(std::string_view str);

	/**
	 * Gets the first char index in the string.
	 * @param str The string.
	 * @param c The char to look for.
	 * @return The char index.
	 */
	static int32_t FindCharPos(std::string_view str, char c);

	/**
	 * Trims the left and right side of a string of whitespace.
	 * @param str The string.
	 * @param whitespace The whitespace type.
	 * @return The trimmed string.
	 */
	static std::string Trim(std::string str, std::string_view whitespace = " \t\n\r");

	/**
	 * Takes a substring of a string between two bounds.
	 * @param str The string.
	 * @param start The left bound.
	 * @param end The right bound.
	 * @return The substring of the string.
	 */
	static std::string Substring(std::string str, uint32_t start, uint32_t end);

	/**
	 * Removes all tokens from a string.
	 * @param str The string.
	 * @param token The token.
	 * @return The string with the tokens removed.
	 */
	static std::string RemoveAll(std::string str, char token);

	/**
	 * Removes the last token from a string.
	 * @param str The string.
	 * @param token The token.
	 * @return The string with the last token removed.
	 */
	static std::string RemoveLast(std::string str, char token);

	/**
	 * Replaces all tokens from a string.
	 * @param str The string.
	 * @param token The token.
	 * @param to The string to replace the tokens with.
	 * @return The string with the tokens replaced.
	 */
	static std::string ReplaceAll(std::string str, std::string_view token, std::string_view to);

	/**
	 * Replaces the first token from a string.
	 * @param str The string.
	 * @param token The token.
	 * @param to The string to replace the tokens with.
	 * @return The string with the tokens replaced.
	 */
	static std::string ReplaceFirst(std::string str, std::string_view token, std::string_view to);

	/**
	 * Fixes all tokens return line tokens from a string so it is file write-able.
	 * @param str The string.
	 * @return The string with return lines fixed.
	 */
	static std::string FixReturnTokens(const std::string &str);

	/**
	 * Lower cases a string.
	 * @param str The string.
	 * @return The lowercased string.
	 */
	static std::string Lowercase(std::string str);

	/**
	 * Uppercases a string.
	 * @param str The string.
	 * @return The uppercased string.
	 */
	static std::string Uppercase(std::string str);

	/**
	 * Converts a type to a string.
	 * @tparam T The type to convert from.
	 * @param val The value to convert.
	 * @return The value as a string.
	 */
	template<typename T>
	static std::string To(const T &val)
	{
		if constexpr (std::is_enum_v<T>)
		{
			typedef typename std::underlying_type<T>::type safe_type;
			return std::to_string(static_cast<safe_type>(val));
		}
		else if constexpr (std::is_same_v<bool, T>)
		{
			return val ? "true" : "false";
		}
		else if constexpr (std::is_same_v<std::string, T>)
		{
			return val;
		}
		else
		{
			return std::to_string(static_cast<T>(val));
		}
	}

	/**
	 * Converts a string to a type.
	 * @tparam T The type to convert to.
	 * @param str The string to convert.
	 * @return The string as a value.
	 */
	template<typename T>
	static T From(const std::string &str)
	{
		if constexpr (std::is_enum_v<T>)
		{
			typedef typename std::underlying_type<T>::type safe_type;
			return static_cast<T>(From<safe_type>(str));
		}
		else if constexpr (std::is_same_v<bool, T>)
		{
			return Lowercase(str) == "true" || From<std::optional<int32_t>>(str) == 1;
		}
		else if constexpr (std::is_same_v<std::string, T>)
		{
			return str;
		}
		else if constexpr (is_optional_v<T>)
		{
			typedef typename T::value_type base_type;
			base_type temp;
			std::istringstream iss(str);

			if ((iss >> temp).fail())
			{
				return std::nullopt;
			}

			return temp;
		}
		else
		{
			T temp;
			std::istringstream iss(str);
			iss >> temp;
			return temp;
		}
	}
};
}
