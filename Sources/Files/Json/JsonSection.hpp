#pragma once

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include "Helpers/FormatString.hpp"
#include "Files/LoadedValue.hpp"

namespace fl
{
	class FL_EXPORT JsonSection
	{
	private:
		std::shared_ptr<JsonSection> m_parent;
		std::vector<std::shared_ptr<JsonSection>> m_children;

		std::string m_name;
		std::string m_content;
	public:
		JsonSection(std::shared_ptr<JsonSection> parent, const std::string &name, const std::string &content);

		~JsonSection();

		std::shared_ptr<JsonSection> GetParent() const { return m_parent; }

		std::vector<std::shared_ptr<JsonSection>> GetChildren() const { return m_children; }

		void AddChild(std::shared_ptr<JsonSection> child) { m_children.emplace_back(child); }

		void Clear();

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		std::string GetContent() const { return m_content; }

		void SetContent(const std::string &content) { m_content = content; }

		static void AppendData(LoadedValue * loadedValue, std::string &data, const int &indentation, const bool &end = false);

		static LoadedValue * Convert(const JsonSection &source, LoadedValue * parent, const bool &isTopSection);
	};
}
