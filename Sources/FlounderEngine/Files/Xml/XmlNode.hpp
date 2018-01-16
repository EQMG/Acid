#pragma once

#include <string>
#include <map>
#include <vector>

namespace Flounder
{
	class XmlNode
	{
	private:
		std::string m_name;
		std::map<std::string, std::string> m_attributes;
		std::string m_data;
		std::map<std::string, std::vector<XmlNode>> m_childNodes;
	public:
		XmlNode(const std::string &name);

		XmlNode(const XmlNode &source);

		~XmlNode();

		const std::string &getName() const { return m_name; };

		const std::string &getData() const { return m_data; };

		void setData(const std::string &data) { m_data = data; };

		std::string &getAttribute(const std::string &attribute);

		void addAttribute(const std::string &attribute, const std::string &value);

		std::vector<XmlNode> getChildren(const std::string &name);

		XmlNode *getChild(const std::string &name);

		XmlNode *getChildrenWithAttribute(const std::string &name, const std::string &attribute, const std::string &value);

		void addChild(const XmlNode &child);
	};
}
