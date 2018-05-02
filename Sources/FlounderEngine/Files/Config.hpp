#pragma once

#include <string>
#include <map>
#include <functional>
#include "Tasks/Tasks.hpp"
#include "IFile.hpp"
#include "ConfigKey.hpp"

namespace fl
{
	class FL_EXPORT Config
	{
	private:
		IFile *m_file;
		std::map<std::string, ConfigKey> *m_values;
	public:
		// TODO: Clean up defines.
#define CONFIG_GET(f) (new std::function<std::string()>([&]() -> std::string { return std::to_string(f); }))
#define CONFIG_GET_STR(f) (new std::function<std::string()>([&]() -> std::string { return f; }))
#define CONFIG_SET(t, f) (new std::function<void(t)>([&](const t &v) -> void { f; }))

		Config(IFile *file);

		~Config();

		void Load();

		void Update();

		void Save();

		ConfigKey GetRaw(const std::string &key, const std::string &normal);

		template<typename T>
		void Set(const std::string &key, const T &value)
		{
			if (m_values->find(key) == m_values->end())
			{
				m_values->insert(std::make_pair(key, std::to_string(value)));
				return;
			}

			m_values->at(key) = ConfigKey(std::to_string(value));
		}

		// TODO: Convert to templates.
		std::string Get(const std::string &key, const std::string &normal = "nullptr");

		bool Get(const std::string &key, const bool &normal = false);

		int Get(const std::string &key, const int &normal = 0);

		float Get(const std::string &key, const float &normal = 0.0f);

		double Get(const std::string &key, const double &normal = 0.0);

		template<typename T>
		void Link(const std::string &key, const T &normal, std::function<std::string()> *getter, std::function<void(T)> *setter = nullptr)
		{
			ConfigKey configKey = GetRaw(key, std::to_string(normal));

			if (getter != nullptr)
			{
				configKey.SetGetter(getter);
			}

			if (setter != nullptr && configKey.IsFromFile())
			{
				(*setter)(Get(key, normal));
			}
		}
	};
}