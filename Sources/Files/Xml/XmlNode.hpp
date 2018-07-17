#pragma once

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include "Helpers/FormatString.hpp"
#include "Files/LoadedValue.hpp"

namespace fl
{
	class FL_EXPORT XmlNode
	{
	private:
		std::shared_ptr<XmlNode> m_parent;
		std::vector<std::shared_ptr<XmlNode>> m_children;

		std::string m_attributes;
		std::string m_content;
	public:
		XmlNode(std::shared_ptr<XmlNode> parent, const std::string &attributes, const std::string &content);

		~XmlNode();

		std::shared_ptr<XmlNode> GetParent() const { return m_parent; }

		std::vector<std::shared_ptr<XmlNode>> GetChildren() const { return m_children; }

		void AddChild(std::shared_ptr<XmlNode> child) { m_children.emplace_back(child); }

		void Clear();

		std::string GetAttributes() const { return m_attributes; }

		void SetAttributes(const std::string &attributes) { m_attributes = attributes; }

		std::string GetContent() const { return m_content; }

		void SetContent(const std::string &content) { m_content = content; }

		static void AppendData(LoadedValue * loadedValue, std::string &data, const int &indentation, const bool &end = false);

		static LoadedValue * Convert(const XmlNode &source, LoadedValue * parent, const bool &isTopSection);
	};
}
