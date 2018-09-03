#pragma once

#include <cstdio>
#include <utility>
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
		/// <param name="format"> The format to output into. </param>
		/// <param name="args"> The args to be added into the format. </param>
		template<typename... Args>
		static void Out(const char *format, Args &&... args)
		{
			fprintf(stdout, format, std::forward<Args>(args)...);
			STREAM << StringFormat(format, std::forward<Args>(args)...);
		}

		/// <summary>
		/// Outputs a error into the console.
		/// </summary>
		/// <param name="format"> The format to output into. </param>
		/// <param name="args"> The args to be added into the format. </param>
		template<typename... Args>
		static void Error(const char *format, Args &&... args)
		{
			fprintf(stderr, format, std::forward<Args>(args)...);
			STREAM << StringFormat(format, std::forward<Args>(args)...);
		}

		static void CreateLog(const std::string &filename);
	private:
		static std::string StringFormat(const std::string &format, ...);
	};
}
