#pragma once

#include <string>
#include <utility>
#include <vector>
#include "Helpers/FormatString.hpp"

namespace fl
{
	class FL_EXPORT LoadedValue
	{
	public:
		LoadedValue *m_parent;
		std::vector<LoadedValue *> *m_children;

		std::string m_name;
		std::string m_value;

		LoadedValue(LoadedValue *parent, const std::string &name, const std::string &value);

		~LoadedValue();

		std::string GetRaw() const { return m_value; }

		void SetRaw(const std::string &data) { m_value = data; }

		std::vector<LoadedValue *> *GetChildren() const { return m_children; }

		unsigned int ChildrenCount() const { return m_children->size(); }

		LoadedValue *GetChild(const std::string &name, const bool &addIfNull = false);

		LoadedValue *GetChild(const unsigned int &index, const bool &addIfNull = false);

		template<typename T>
		void AddChild(LoadedValue *value)
		{
			LoadedValue *child = GetChild(value->m_name);

			if (child != nullptr)
			{
				child->m_value = value->m_value;
				return;
			}

			child = value;
			m_children->push_back(child);
		}

		template<typename T>
		void SetChild(const std::string &name, const T &value)
		{
			std::string strValue = std::to_string(value);
			LoadedValue *child = GetChild(name);

			if (child == nullptr)
			{
				child = new LoadedValue(this, name, strValue);
				m_children->push_back(child);
				return;
			}

			child->m_value = strValue;
		}

		template<typename T>
		T Get()
		{
			std::string data = GetRaw();
			return FormatString::ConvertTo<T>(data);
		}

		template<typename T>
		void Set(const T &data)
		{
			SetRaw(std::to_string(data));
		}

		LoadedValue *GetChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value);

		std::string GetString()
		{
			return FormatString::RemoveAll(m_value, '\"');
		}

		void SetString(const std::string &data)
		{
			m_value = "\"" + data + "\"";
		}
	};
}
