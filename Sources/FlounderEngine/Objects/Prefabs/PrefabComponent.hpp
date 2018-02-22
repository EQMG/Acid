#pragma once

#include <string>
#include <vector>
#include "../../Prerequisites.hpp"
#include "../../Helpers/FormatString.hpp"

namespace Flounder
{
	class F_EXPORT PrefabComponent
	{
	private:
		std::vector<std::string> m_data;
	public:
		PrefabComponent(const std::vector<std::string> &data = std::vector<std::string>());

		std::string GetRaw(const unsigned int &i);

		void SetRaw(const unsigned int &i, const std::string &data);

		template<typename T>
		T Get(const unsigned int &i)
		{
			std::string data = GetRaw(i);
			return FormatString::ConvertTo<T>(data);
		}

		template<typename T>
		void Set(const unsigned int &i, const T &data)
		{
			SetRaw(i, std::to_string(data));
		}

		std::vector<std::string> GetData() const { return m_data; }
	};
}
