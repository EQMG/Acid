#pragma once

#include <string>
#include <map>
#include "FileCsv.hpp"

namespace Flounder
{
	class ConfigCsv
	{
	private:
		FileCsv *m_fileCsv;
		std::map<std::string, std::string> *m_values;
	public:
		ConfigCsv(const std::string &filename);

		~ConfigCsv();

		void Load();

		void Save();

		std::string Get(const std::string &key, const std::string &normal = "");

		void Set(const std::string &key, const std::string &value);

		bool Get(const std::string &key, const bool &normal = false);

	//	void Set(const std::string &key, const bool &value);

		int Get(const std::string &key, const int &normal = 0);

	//	void Set(const std::string &key, const int &value);

		float Get(const std::string &key, const float &normal = 0.0f);

	//	void Set(const std::string &key, const float &value);

		double Get(const std::string &key, const double &normal = 0.0);

	//	void Set(const std::string &key, const double &value);
	};
}