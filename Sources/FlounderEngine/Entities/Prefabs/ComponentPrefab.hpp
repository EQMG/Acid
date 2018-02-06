#pragma once

#include <string>
#include <vector>
#include "../../Helpers/FormatString.hpp"

namespace Flounder
{
	class ComponentPrefab
	{
	private:
		std::vector<std::string> m_data;
	public:
		ComponentPrefab(const std::vector<std::string> &data);

		std::string GetRaw(const unsigned int &i);

		std::string GetString(const unsigned int &i);

		int GetInt(const unsigned int &i);

		float GetFloat(const unsigned int &i);

		bool GetBool(const unsigned int &i);

		std::vector<std::string> GetData() const { return m_data; }
	};
}
