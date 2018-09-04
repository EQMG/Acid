#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Helpers/String.hpp"
#include "Serialized/Node.hpp"

namespace acid
{
	class ACID_EXPORT XmlNode
	{
	private:
		XmlNode *m_parent;
		std::vector<XmlNode *> m_children;

		std::string m_attributes;
		std::string m_content;
	public:
		XmlNode(XmlNode *parent, const std::string &attributes, const std::string &content);

		~XmlNode();

		XmlNode *GetParent() const { return m_parent; }

		std::vector<XmlNode *> GetChildren() const { return m_children; }

		void AddChild(XmlNode *child) { m_children.emplace_back(child); }

		std::string GetAttributes() const { return m_attributes; }

		void SetAttributes(const std::string &attributes) { m_attributes = attributes; }

		std::string GetContent() const { return m_content; }

		void SetContent(const std::string &content) { m_content = content; }

		static void AppendData(const std::shared_ptr<Node> &source, std::stringstream &builder, const int &indentation);

		static std::shared_ptr<Node> &Convert(const XmlNode &source, std::shared_ptr<Node> &parent, const bool &isTopSection);
	};
}
