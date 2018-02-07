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

	void ComponentPrefab::SetRaw(const unsigned int &i, const std::string &data)
	{
		// TODO: if (i >= m_data.size())
		m_data.at(i) = data;
	}

	std::string ComponentPrefab::GetString(const unsigned int &i)
	{
		return FormatString::RemoveAll(GetRaw(i), '\"');
	}

	void ComponentPrefab::SetString(const unsigned int &i, const std::string &data)
	{
		SetRaw(i, "\"" + data + "\"");
	}

	int ComponentPrefab::GetInt(const unsigned int &i)
	{
		std::string data = GetRaw(i);
		return data.empty() ? 0 : atoi(data.c_str());
	}

	void ComponentPrefab::SetInt(const unsigned int &i, const int &data)
	{
		SetRaw(i, std::to_string(data));
	}

	float ComponentPrefab::GetFloat(const unsigned int &i)
	{
		std::string data = GetRaw(i);
		return data.empty() ? 0.0f : static_cast<float>(atof(data.c_str()));
	}

	void ComponentPrefab::SetFloat(const unsigned int &i, const float &data)
	{
		SetRaw(i, std::to_string(data));
	}

	bool ComponentPrefab::GetBool(const unsigned int &i)
	{
		return GetInt(i) == 1;
	}

	void ComponentPrefab::SetBool(const unsigned int &i, const bool &data)
	{
		SetInt(i, data ? 1 : 0);
	}
}
