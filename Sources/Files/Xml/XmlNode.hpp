#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Helpers/NonCopyable.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT XmlNode :
		public NonCopyable
	{
	private:
		XmlNode *m_parent;
		std::vector<std::unique_ptr<XmlNode>> m_children;

		std::string m_attributes;
		std::string m_content;
	public:
		XmlNode(XmlNode *parent, const std::string &attributes, const std::string &content);

		XmlNode *GetParent() const { return m_parent; }

		const std::vector<std::unique_ptr<XmlNode>> &GetChildren() const { return m_children; }

		void AddChild(XmlNode *child) { m_children.emplace_back(child); }

		const std::string &GetAttributes() const { return m_attributes; }

		void SetAttributes(const std::string &attributes) { m_attributes = attributes; }

		const std::string &GetContent() const { return m_content; }

		void SetContent(const std::string &content) { m_content = content; }

		static void AppendData(const Metadata &source, std::stringstream &builder, const int32_t &indentation);

		static Metadata *Convert(const XmlNode &source, Metadata *parent, const bool &isTopSection);
	};
}
