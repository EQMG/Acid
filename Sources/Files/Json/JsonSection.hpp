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
	private:
		JsonSection *m_parent;
		std::vector<JsonSection *> m_children;

		std::string m_name;
		std::string m_content;
	public:
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

		JsonSection *GetParent() const { return m_parent; }

		std::vector<JsonSection *> GetChildren() const { return m_children; }

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		std::string GetContent() const { return m_content; }

		void SetContent(const std::string &content) { m_content = content; }

		static void AppendData(LoadedValue *loadedValue, std::string *data, const int &indentation, const bool &end = false);

		static LoadedValue *Convert(JsonSection *source, LoadedValue *parent, const bool &isTopSection);
	};
}
