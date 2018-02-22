#pragma once

#include <string>
#include <map>
#include <functional>
#include "../Tasks/Tasks.hpp"
#include "IFile.hpp"
#include "ConfigKey.hpp"

namespace Flounder
{
	class F_EXPORT Config
	{
	private:
		IFile *m_file;
		std::map<std::string, ConfigKey> *m_values;
	public:
#define CONFIG_GET(f) [&]() -> std::string { return std::to_string(f); }
#define CONFIG_GET_STR(f) [&]() -> std::string { return f; }
#define CONFIG_SET(t, f) [&](const t &v) -> void { f; }

		Config(IFile *file);

		~Config();

		void Load();

		void Update();

		void Save();

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

		ConfigKey GetRaw(const std::string &key, const std::string &normal);

		std::string Get(const std::string &key, const std::string &normal = "nullptr");

		bool Get(const std::string &key, const bool &normal = false);

		int Get(const std::string &key, const int &normal = 0);

		float Get(const std::string &key, const float &normal = 0.0f);

		double Get(const std::string &key, const double &normal = 0.0);

		template<typename T>
		void Link(const std::string &key, const T &normal, const std::function<std::string()> &getter, const std::function<void(T)> &setter = [](const T &v) -> void { })
		{
			ConfigKey configKey = GetRaw(key, std::to_string(normal));

			if (getter != nullptr)
			{
				configKey.SetGetter(new std::function<std::string()>([&]() -> std::string { return getter(); }));
			}

			if (setter != nullptr && configKey.IsFromFile())
			{
				setter(Get(key, normal));
			}
		}
	};
}