#pragma once

#include <iostream>
#include "../framework/framework.h"

namespace flounder {
	class logger : public imodule
	{
	private:

	public:
		static logger *get()
		{
			return (logger*) framework::get()->getInstance("logger");
		}

		logger();

		~logger();

		/// <summary>
		/// Logs strings sent into a .log file, and if {@code LOG_TO_CONSOLE} is enabled it will also be logged to the IDEs console.
		/// </summary>
		/// <param name="value"> Text or numbers being added to the log file and possibly to the IDEs console. </param>
		void log(std::string value);

		/// <summary>
		/// Warning logs strings sent into javas console, and if {@code LOG_TO_FILE} is enabled it will also be logged to a log file.
		/// </summary>
		/// <param name="value"> Warnings being added to the log file and possibly to your IDES console. </param>
		void warning(std::string value);

		/// <summary>
		/// Error logs strings sent into javas console, and if {@code LOG_TO_FILE} is enabled it will also be logged to a log file.
		/// </summary>
		/// <param name="value"> Errors being added to the log file and possibly to your IDES console. </param>
		void error(std::string value);
	};
}
