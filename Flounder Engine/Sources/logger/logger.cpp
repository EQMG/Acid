#include "logger.h"

namespace flounder {
	logger::logger()
	{
	}

	logger::~logger()
	{
	}

	void logger::log(std::string value)
	{
		std::cout << "Log: " << value << std::endl;
	}

	void logger::warning(std::string value)
	{
		std::cout << "Log: " << value << std::endl;
	}

	void logger::error(std::string value)
	{
		std::cout << "Log: " << value << std::endl;
	}
}
