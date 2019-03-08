#pragma once

#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include "Engine/Exports.hpp"
#include "TypeTraits.hpp"

namespace acid
{
	/// <summary>
	/// A helper for C++ strings.
	/// </summary>
	class ACID_EXPORT String
	{
	public:
		/// <summary>
		/// Splits a string by a separator.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="sep"> The separator. </param>
		/// <param name="trim"> If each object should be trimmed. </param>
		/// <returns> The split string vector. </returns>
		static std::vector<std::string> Split(const std::string &str, const std::string &sep, const bool &trim = false);

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
		static int32_t FindCharPos(const std::string &str, const char &c);

		/// <summary>
		/// Trims the left and right side of a string of whitespace.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="whitespace"> The whitespace type. </param>
		/// <returns> The trimmed string. </returns>
		static std::string Trim(const std::string &str, const std::string &whitespace = " \t\n\r");

		/// <summary>
		/// Takes a substring of a string between two bounds.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="start"> The left bound. </param>
		/// <param name="end"> The right bound. </param>
		/// <returns> The substring of the string. </returns>
		static std::string Substring(const std::string &str, const uint32_t &start, const uint32_t &end);

		/// <summary>
		/// Removes all tokens from a string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <returns> The string with the tokens removed. </returns>
		static std::string RemoveAll(const std::string &str, const char &token);

		/// <summary>
		/// Removes the last token from a string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <returns> The string with the last token removed. </returns>
		static std::string RemoveLast(const std::string &str, const char &token);

		/// <summary>
		/// Replaces all tokens from a string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <param name="to"> The string to replace the tokens with. </param>
		/// <returns> The string with the tokens replaced. </returns>
		static std::string ReplaceAll(const std::string &str, const std::string &token, const std::string &to);

		/// <summary>
		/// Replaces the first token from a string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <param name="token"> The token. </param>
		/// <param name="to"> The string to replace the tokens with. </param>
		/// <returns> The string with the tokens replaced. </returns>
		static std::string ReplaceFirst(const std::string &str, const std::string &token, const std::string &to);

		/// <summary>
		/// Lowercases a string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <returns> The lowercased string. </returns>
		static std::string Lowercase(const std::string &str);

		/// <summary>
		/// Uppercases a string.
		/// </summary>
		/// <param name="str"> The string. </param>
		/// <returns> The uppercased string. </returns>
		static std::string Uppercase(const std::string &str);

		/// <summary>
		/// Converts a type to a string.
		/// </summary>
		/// <param name="val"> The value to convert. </param>
		/// <returns> The value as a string. </returns>
		template<typename T>
		static std::string To(const T &val)
		{
			if constexpr (std::is_same_v<std::string, T>)
			{
				return val;
			}
			else if constexpr (std::is_enum_v<T>)
			{
				typedef typename std::underlying_type<T>::type safe_type;
				return std::to_string(static_cast<safe_type>(val));
			}
			else if constexpr (std::is_same_v<bool, T>)
			{
				return val ? "true" : "false";
			}
			else
			{
				return std::to_string(static_cast<T>(val));
			}
		}

		/// <summary>
		/// Converts a string to a type.
		/// </summary>
		/// <param name="str"> The string to convert. </param>
		/// <returns> The string as a value. </returns>
		template<typename T>
		static T From(const std::string &str)
		{
			if constexpr (std::is_same_v<std::string, T>)
			{
				return str;
			}
			else if constexpr (std::is_enum_v<T>)
			{
				typedef typename std::underlying_type<T>::type safe_type;
				return static_cast<T>(From<safe_type>(str));
			}
			else if constexpr (std::is_same_v<bool, T>)
			{
				return Lowercase(str) == "true" || From<int32_t>(str) == 1;
			}
			else
			{
				std::istringstream iss(str);

				if constexpr (TypeTraits::is_optional<T>::value)
				{
					typedef typename T::value_type base_type;
					base_type temp;

					if ((iss >> temp).fail())
					{
						return {};
					}

					return temp;
				}
				else
				{
					T temp;
					iss >> temp;
					return temp;
				}
			}
		}
	};
}
