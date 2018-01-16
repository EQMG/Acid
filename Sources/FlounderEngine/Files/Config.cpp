#include "Config.hpp"

#include <sstream>
#include <algorithm>

namespace Flounder
{

	Config::Config(IFile *file) :
		m_file(file),
		m_values(new std::map<std::string, std::string>())
	{
	}

	Config::~Config()
	{
		delete m_file;
		delete m_values;
	}

	void Config::Load()
	{
		m_file->Load();

		auto fileMap = m_file->ConfigReadValues();
		m_values->clear();
		m_values->insert(fileMap.begin(), fileMap.end());
	}

	void Config::Save()
	{
		m_file->Clear();

		for (auto value : *m_values)
		{
			m_file->ConfigPushValue(value.first, value.second);
		}
		
		m_file->Save();
	}

	std::string Config::Get(const std::string &key, const std::string &normal)
	{
		if (m_values->find(key) == m_values->end())
		{
			m_values->insert(std::make_pair(key, normal));
			return normal;
		}

		return m_values->at(key);
	}

	bool Config::Get(const std::string &key, const bool &normal)
	{
		std::string value = Get(key, std::to_string(normal));
		return atoi(value.c_str()) == 1; // atob(value.c_str());
	}

	int Config::Get(const std::string &key, const int &normal)
	{
		std::string value = Get(key, std::to_string(normal));
		return atoi(value.c_str());
	}

	float Config::Get(const std::string &key, const float &normal)
	{
		std::string value = Get(key, std::to_string(normal));
		return static_cast<float>(atof(value.c_str()));
	}

	double Config::Get(const std::string &key, const double &normal)
	{
		std::string value = Get(key, std::to_string(normal));
		return atof(value.c_str());
	}
}
