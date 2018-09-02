#pragma once

#include <map>
#include <string>
#include <memory>
#include <optional>
#include <utility>
#include <vector>
#include "Helpers/FormatString.hpp"

namespace acid
{
	class ACID_EXPORT LoadedValue
	{
	private:
		std::vector<std::shared_ptr<LoadedValue>> m_children;

		std::string m_name;
		std::string m_value;
		std::map<std::string, std::string> m_attributes;
	public:
		LoadedValue(const std::string &name = "", const std::string &value = "", const std::map<std::string, std::string> &attributes = {});

		LoadedValue(const LoadedValue &source);

		~LoadedValue();

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		std::string GetValue() const { return m_value; }

		void SetValue(const std::string &data) { m_value = data; }

		std::vector<std::shared_ptr<LoadedValue>> &GetChildren() { return m_children; }

		std::vector<std::shared_ptr<LoadedValue>> FindChildren(const std::string &name) const;

		std::shared_ptr<LoadedValue> FindChild(const std::string &name, const bool &addIfNull = false,
		                                       const bool &reportError = true);

		std::shared_ptr<LoadedValue> FindChild(const uint32_t &index, const bool &reportError = true) const;

		std::shared_ptr<LoadedValue> FindChildWithAttribute(const std::string &childName, const std::string &attribute,
		                                                    const std::string &value, const bool &reportError = true) const;

		std::map<std::string, std::string> GetAttributes() const { return m_attributes; }

		void AddChild(const std::shared_ptr<LoadedValue> &value);

		bool RemoveChild(const std::shared_ptr<LoadedValue> &value);

		template<typename T>
		void SetChild(const std::string &name, const T &value)
		{
			std::string strValue = std::to_string(value);
			auto child = FindChild(name);

			if (child)
			{
				child = std::make_shared<LoadedValue>(name, strValue);
				m_children.emplace_back(child);
				return;
			}

			child->m_value = strValue;
		}

		void SetAttributes(const std::map<std::string, std::string> &attributes) { m_attributes = attributes; }

		void AddAttribute(const std::string &attribute, const std::string &value);

		bool RemoveAttribute(const std::string &attribute);

		std::string FindAttribute(const std::string &attribute) const;

		template<typename T>
		T Get()
		{
			std::string data = GetValue();
			return FormatString::ConvertTo<T>(data);
		}

		template<typename T>
		void Set(const T &data)
		{
			SetValue(std::to_string(data));
		}

		std::string GetString();

		void SetString(const std::string &data);
	};
}
