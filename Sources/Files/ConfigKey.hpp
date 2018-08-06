#pragma once

#include <functional>
#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	class ACID_EXPORT ConfigKey
	{
	private:
		std::string m_value;
		bool m_fromFile;
		std::function<std::string()> m_getter;
	public:
		ConfigKey(const std::string &value, const bool &fromFile = false);

		~ConfigKey();

		std::string GetValue() const { return m_value; }

		void SetValue(const std::string &value) { m_value = value; }

		bool IsFromFile() const { return m_fromFile; }

		std::function<std::string()> GetGetter() const { return m_getter; }

		void SetGetter(std::function<std::string()> getter) { m_getter = getter; }
	};
}
