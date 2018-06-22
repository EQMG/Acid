#pragma once

#include <string>
#include <map>
#include <functional>
#include "Helpers/FormatString.hpp"
#include "Tasks/Tasks.hpp"
#include "IFile.hpp"
#include "ConfigKey.hpp"

namespace fl
{
	class FL_EXPORT Config
	{
	private:
		std::shared_ptr<IFile> m_file;
		std::map<std::string, std::shared_ptr<ConfigKey>> m_values;
	public:
#define CONFIG_GET(f) (std::function<std::string()>([&]() -> std::string { return std::to_string(f); }))
#define CONFIG_SET(t, f) (std::function<void(t)>([&](const t &v) -> void { f; }))

		Config(std::shared_ptr<IFile> file);

		~Config();

		void Load();

		void Update();

		void Save();

		std::shared_ptr<ConfigKey> GetRaw(const std::string &key, const std::string &normal);

		void SetRaw(const std::string &key, const std::string &value);

		void Remove(const std::string &key);

		template<typename T>
		T Get(const std::string &key, const T &normal)
		{
			return FormatString::ConvertTo<T>(GetRaw(key, std::to_string(normal))->GetValue());
		}

		template<typename T>
		void Set(const std::string &key, const T &value)
		{
			SetRaw(key, std::to_string(value));
		}

		template<typename T>
		void Link(const std::string &key, const T &normal, std::function<std::string()> getter, std::function<void(T)> setter = nullptr)
		{
			auto configKey = GetRaw(key, std::to_string(normal));

			if (getter != nullptr)
			{
				configKey->SetGetter(getter);
			}

			if (setter != nullptr)
			{
				setter(Get<T>(key, FormatString::ConvertTo<T>(configKey->GetValue())));
			}
		}
	};
}