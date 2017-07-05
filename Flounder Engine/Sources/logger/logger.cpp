#include "logger.h"

namespace flounder {
	logger::logger()
	{
	}

	logger::~logger()
	{
	}

	template<typename T>
	void logger::log(T value)
	{
		std::cout << "Log: " << value << std::endl;
	}

	template<typename T>
	void logger::warning(T value)
	{
		std::cout << "Log: " << value << std::endl;
	}

	template<typename T>
	void logger::error(T value)
	{
		std::cout << "Log: " << value << std::endl;
	}
}
