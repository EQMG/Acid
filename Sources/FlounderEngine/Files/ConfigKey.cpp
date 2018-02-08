#include "ConfigKey.hpp"

namespace Flounder
{
	ConfigKey::ConfigKey(const std::string &value, const bool &fromFile) :
		m_value(value),
		m_fromFile(fromFile)
	{
	}

	ConfigKey::~ConfigKey()
	{
	}
}
