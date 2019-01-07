#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT JsonSection
	{
	private:
		JsonSection *m_parent;
		std::vector<std::unique_ptr<JsonSection>> m_children;

		std::string m_name;
		std::string m_content;
	public:
		JsonSection(JsonSection *parent, const std::string &name, const std::string &content);

		JsonSection(const JsonSection&) = delete;

		JsonSection& operator=(const JsonSection&) = delete;

		JsonSection *GetParent() const { return m_parent; }

		const std::vector<std::unique_ptr<JsonSection>> &GetChildren() const { return m_children; }

		void AddChild(JsonSection *child) { m_children.emplace_back(child); }

		const std::string &GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		const std::string &GetContent() const { return m_content; }

		void SetContent(const std::string &content) { m_content = content; }

		static void AppendData(const Metadata &source, std::stringstream &builder, const int32_t &indentation, const bool &end = false);

		static Metadata *Convert(const JsonSection &source, Metadata *parent, const bool &isTopSection = true);
	};
}
