#include "PrefabComponent.hpp"

namespace Flounder
{
	PrefabComponent::PrefabComponent(const std::vector<std::string> &data) :
		m_data(std::vector<std::string>(data))
	{
	}

	std::string PrefabComponent::GetRaw(const unsigned int &i)
	{
		return i >= m_data.size() ? "" : m_data.at(i);
	}

	void PrefabComponent::SetRaw(const unsigned int &i, const std::string &data)
	{
		while (m_data.empty() || i >= m_data.size())
		{
			m_data.push_back("");
		}

		m_data.at(i) = data;
	}

	std::string PrefabComponent::GetString(const unsigned int &i)
	{
		return FormatString::RemoveAll(GetRaw(i), '\"');
	}

	void PrefabComponent::SetString(const unsigned int &i, const std::string &data)
	{
		SetRaw(i, "\"" + data + "\"");
	}

	int PrefabComponent::GetInt(const unsigned int &i)
	{
		std::string data = GetRaw(i);
		return data.empty() ? 0 : atoi(data.c_str());
	}

	void PrefabComponent::SetInt(const unsigned int &i, const int &data)
	{
		SetRaw(i, std::to_string(data));
	}

	float PrefabComponent::GetFloat(const unsigned int &i)
	{
		std::string data = GetRaw(i);
		return data.empty() ? 0.0f : static_cast<float>(atof(data.c_str()));
	}

	void PrefabComponent::SetFloat(const unsigned int &i, const float &data)
	{
		SetRaw(i, std::to_string(data));
	}

	bool PrefabComponent::GetBool(const unsigned int &i)
	{
		return GetInt(i) == 1;
	}

	void PrefabComponent::SetBool(const unsigned int &i, const bool &data)
	{
		SetInt(i, data ? 1 : 0);
	}
}
