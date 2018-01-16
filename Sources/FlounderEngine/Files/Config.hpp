#pragma once

#include <string>
#include <map>
#include "IFile.hpp"

namespace Flounder
{
	class Config
	{
	private:
		IFile *m_file;
		std::map<std::string, std::string> *m_values;
	public:
		Config(IFile *file);

		~Config();

		void Load();

		void Save();

		template<typename T>
		void Set(const std::string &key, const T &value)
		{
			if (m_values->find(key) == m_values->end())
			{
				m_values->insert(std::make_pair(key, std::to_string(value)));
				return;
			}

			m_values->at(key) = std::to_string(value);
		}

		std::string Get(const std::string &key, const std::string &normal = "nullptr");

		bool Get(const std::string &key, const bool &normal = false);

		int Get(const std::string &key, const int &normal = 0);

		float Get(const std::string &key, const float &normal = 0.0f);

		double Get(const std::string &key, const double &normal = 0.0);
	};
}