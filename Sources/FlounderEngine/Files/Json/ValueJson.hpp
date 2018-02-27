#pragma once

#include <string>
#include <utility>
#include <vector>
#include "../../Helpers/FormatString.hpp"

namespace Flounder
{

	class F_EXPORT JsonSection
	{
	public:
		JsonSection *m_parent;
		std::vector<JsonSection*> m_children;

		std::string m_name;
		std::string m_content;

		JsonSection(JsonSection *parent, const std::string &name, const std::string &content) :
			m_parent(parent),
			m_name(name),
			m_content(content)
		{
		}

		~JsonSection()
		{
			for (auto child : m_children)
			{
				delete child;
			}
		}
	};

	class F_EXPORT ValueJson
	{
	public:
		ValueJson *m_parent;
		std::vector<ValueJson*> m_children;

		std::string m_name;
		std::string m_value;

		ValueJson(ValueJson *parent, const std::string &name, const std::string &value);

		~ValueJson();

		std::string GetRaw() const { return m_value; }

		void SetRaw(const std::string &data) { m_value = data; }

		ValueJson *GetChild(const std::string &name);

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

		void AppendData(std::string *data, const int &indentation, const bool &end = false);

		static ValueJson *Convert(JsonSection *source, ValueJson *destination);
	};
}
