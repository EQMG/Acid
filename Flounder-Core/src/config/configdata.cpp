#include "configdata.h"

namespace flounder
{
	template<typename t>
	configdata<t>::configdata(const std::string &key, const std::string &data, t *reference)
	{
		m_key = fixDataString(key);
		m_data = fixDataString(data);
		m_reference = reference;
	}

	template<typename t>
	std::string configdata<t>::getString()
	{
		return data;
	}

	template<typename t>
	bool configdata<t>::getBool()
	{
		return false;
	}

	template<typename t>
	int configdata<t>::getInt()
	{
		return stoi(data);
	}

	template<typename t>
	double configdata<t>::getDouble()
	{
		return stod(data);
	}

	template<typename t>
	float configdata<t>::getFloat()
	{
		return stof(data);
	}

	template<typename t>
	std::string configdata<t>::fixDataString(const std::string &string)
	{
		std::string result = string;
		result = helperstring::replace(result, "#", "");
		result = helperstring::replace(result, "$", "");
		result = helperstring::replace(result, ",", "");
		result = helperstring::replace(result, ";", "");
		result = helperstring::replace(result, "{", "");
		result = helperstring::replace(result, "}", "");
		return result;
	}
}
