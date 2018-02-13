#pragma once

#include <string>
#include <vector>
#include "../../Prerequisites.hpp"
#include "../../Helpers/FormatString.hpp"

namespace Flounder
{
	class F_EXPORT ComponentPrefab
	{
	private:
		std::vector<std::string> m_data;
	public:
		ComponentPrefab(const std::vector<std::string> &data);

		std::string GetRaw(const unsigned int &i);

		void SetRaw(const unsigned int &i, const std::string &data);

		std::string GetString(const unsigned int &i);

		void SetString(const unsigned int &i, const std::string &data);

		int GetInt(const unsigned int &i);

		void SetInt(const unsigned int &i, const int &data);

		float GetFloat(const unsigned int &i);

		void SetFloat(const unsigned int &i, const float &data);

		bool GetBool(const unsigned int &i);

		void SetBool(const unsigned int &i, const bool &data);

		std::vector<std::string> GetData() const { return m_data; }
	};
}
