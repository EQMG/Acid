#include "Config.hpp"

#include <sstream>

namespace fl
{
	Config::Config(std::shared_ptr<IFile> file) :
		m_file(file),
		m_values(std::map<std::string, std::shared_ptr<ConfigKey>>())
	{
	}

	Config::~Config()
	{
	}

	void Config::Load()
	{
		m_file->Load();

		auto fileMap = m_file->ConfigReadValues();
		m_values.clear();

		for (auto &fm : fileMap)
		{
			m_values.emplace(fm.first, new ConfigKey(fm.second, true));
		}
	}

	void Config::Update()
	{
		// TODO: Implement.
		//	for (auto &value : m_values)
		//	{
		//		value.second.SetValue(value.second.GetGetter()());
		//	}
	}

	void Config::Save()
	{
		Update();
		m_file->Clear();

		for (auto &value : m_values)
		{
			m_file->ConfigPushValue(value.first, value.second->GetValue());
		}

		m_file->Save();
	}

	std::shared_ptr<ConfigKey> Config::GetRaw(const std::string &key, const std::string &normal)
	{
		if (m_values.find(key) == m_values.end())
		{
			auto configKey = std::make_shared<ConfigKey>(normal, false);
			m_values.emplace(key, configKey);
			return configKey;
		}

		return m_values.at(key);
	}

	void Config::SetRaw(const std::string &key, const std::string &value)
	{
		if (m_values.find(key) == m_values.end())
		{
			m_values.emplace(key, std::make_shared<ConfigKey>(value, false));
			return;
		}

		m_values.at(key) = std::make_shared<ConfigKey>(value);
	}

	void Config::Remove(const std::string &key)
	{
		auto value = m_values.find(key);

		if (value == m_values.end())
		{
			return;
		}

		m_values.erase(key);
	}
}
