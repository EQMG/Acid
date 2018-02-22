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
}
