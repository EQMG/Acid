#pragma once

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <bits/shared_ptr.h>
#include "Helpers/FormatString.hpp"

namespace fl
{
	class FL_EXPORT LoadedValue :
		public std::enable_shared_from_this<LoadedValue>
	{
	private:
		std::shared_ptr<LoadedValue> m_parent;
		std::vector<std::shared_ptr<LoadedValue>> m_children;

		std::string m_name;
		std::string m_value;
	public:
		LoadedValue(std::shared_ptr<LoadedValue> parent, const std::string &name, const std::string &value);

		~LoadedValue();

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		std::string GetValue() const { return m_value; }

		void SetValue(const std::string &data) { m_value = data; }

		std::vector<std::shared_ptr<LoadedValue>> &GetChildren() { return m_children; }

		std::shared_ptr<LoadedValue> GetChild(const std::string &name, const bool &addIfNull = false);

		std::shared_ptr<LoadedValue> GetChild(const unsigned int &index, const bool &addIfNull = false);

		template<typename T>
		void AddChild(std::shared_ptr<LoadedValue> value)
		{
			auto child = GetChild(value->m_name);

			if (child != nullptr)
			{
				child->m_value = value->m_value;
				return;
			}

			child = value;
			m_children.emplace_back(child);
		}

		template<typename T>
		void SetChild(const std::string &name, const T &value)
		{
			std::string strValue = std::to_string(value);
			auto child = GetChild(name);

			if (child == nullptr)
			{
				child = std::make_shared<LoadedValue>(shared_from_this(), name, strValue);
				m_children.emplace_back(child);
				return;
			}

			child->m_value = strValue;
		}

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

		std::shared_ptr<LoadedValue> GetChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value);

		std::string GetString();

		void SetString(const std::string &data);
	};
}
