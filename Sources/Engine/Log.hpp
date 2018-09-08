#pragma once

#include <memory>
#include <sstream>
#include <string>
#include "Exports.hpp"

namespace acid
{
	/// <summary>
	/// A logging class used in Acid, will write output to a file one the application has closed.
	/// </summary>
	class ACID_EXPORT Log
	{
	private:
		static std::ostringstream STREAM;
	public:
		/// <summary>
		/// Outputs a message into the console.
		/// </summary>
		/// <param name="string"> The string to output. </param>
		static void Out(const std::string &string);
		
		/// <summary>
		/// Outputs a message into the console.
		/// </summary>
		/// <param name="format"> The format to output into. </param>
		/// <param name="args"> The args to be added into the format. </param>
		template<typename... Args>
		static void Out(const std::string &format, Args &&... args)
		{
			Out(StringFormat(format, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Outputs a error into the console.
		/// </summary>
		/// <param name="string"> The string to output. </param>
		static void Error(const std::string &string);
		
		/// <summary>
		/// Outputs a error into the console.
		/// </summary>
		/// <param name="format"> The format to output into. </param>
		/// <param name="args"> The args to be added into the format. </param>
		template<typename... Args>
		static void Error(const std::string &format, Args &&... args)
		{
			Error(StringFormat(format, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Outputs all logs into a file.
		/// </summary>
		/// <param name="filename"> The filename to output into. </param>
		static void CreateLog(const std::string &filename);
	private:
		template<typename... Args>
		static std::string StringFormat(const std::string &format, Args &&... args)
		{
			size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
			std::unique_ptr<char[]> buf(new char[size]);
			snprintf(buf.get(), size, format.c_str(), args ...);
			return std::string(buf.get(), buf.get() + size - 1); // Excludes the '\0'
		}
	};
}
