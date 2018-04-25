#include "ConfigKey.hpp"

namespace fl
{
	ConfigKey::ConfigKey(const std::string &value, const bool &fromFile) :
		m_value(value),
		m_fromFile(fromFile),
		m_getter(nullptr)
	{
	}

	ConfigKey::~ConfigKey()
	{
	}
}
