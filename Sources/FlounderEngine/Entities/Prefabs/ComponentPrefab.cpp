#include "ComponentPrefab.hpp"

namespace Flounder
{
	ComponentPrefab::ComponentPrefab(const std::vector<std::string> &data) :
		m_data(std::vector<std::string>(data))
	{
	}

	std::string ComponentPrefab::GetRaw(const unsigned int &i)
	{
		return i >= m_data.size() ? "" : m_data.at(i);
	}

	std::string ComponentPrefab::GetString(const unsigned int &i)
	{
		return FormatString::RemoveAll(GetRaw(i), '\"');
	}

	int ComponentPrefab::GetInt(const unsigned int &i)
	{
		std::string data = GetRaw(i);
		return data.empty() ? 0 : atoi(data.c_str());
	}

	float ComponentPrefab::GetFloat(const unsigned int &i)
	{
		std::string data = GetRaw(i);
		return data.empty() ? 0.0f : static_cast<float>(atof(data.c_str()));
	}

	bool ComponentPrefab::GetBool(const unsigned int &i)
	{
		return GetInt(i) == 1;
	}
}
