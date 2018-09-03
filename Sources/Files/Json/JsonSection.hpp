#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Helpers/FormatString.hpp"
#include "Serialized/Serialized.hpp"

namespace acid
{
	class ACID_EXPORT JsonSection
	{
	private:
		JsonSection *m_parent;
		std::vector<JsonSection *> m_children;

		std::string m_name;
		std::string m_content;
	public:
		JsonSection(JsonSection *parent, const std::string &name, const std::string &content);

		~JsonSection();

		JsonSection *GetParent() const { return m_parent; }

		std::vector<JsonSection *> GetChildren() const { return m_children; }

		void AddChild(JsonSection * child) { m_children.emplace_back(child); }

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		std::string GetContent() const { return m_content; }

		void SetContent(const std::string &content) { m_content = content; }

		static void AppendData(const std::shared_ptr<Serialized> &Serialized, std::stringstream &builder, const int &indentation, const bool &end = false);

		static std::shared_ptr<Serialized> Convert(const JsonSection &source, std::shared_ptr<Serialized> &parent, const bool &isTopSection = true);
	};
}
