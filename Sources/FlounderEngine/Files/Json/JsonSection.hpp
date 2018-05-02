#pragma once

#include <string>
#include <utility>
#include <vector>
#include "Helpers/FormatString.hpp"
#include "Files/LoadedValue.hpp"

namespace fl
{
	class FL_EXPORT JsonSection
	{
	public:
		JsonSection *m_parent;
		std::vector<JsonSection *> m_children;

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

		static void AppendData(LoadedValue *loadedValue, std::string *data, const int &indentation, const bool &end = false);

		static LoadedValue *Convert(JsonSection *source, LoadedValue *parent, const bool &isTopSection);
	};
}
