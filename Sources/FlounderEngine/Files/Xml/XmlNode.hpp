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

		const std::string &GetName() const { return m_name; };

		const std::string &GetData() const { return m_data; };

		void SetData(const std::string &data) { m_data = data; };

		std::string &GetAttribute(const std::string &attribute);

		void AddAttribute(const std::string &attribute, const std::string &value);

		std::vector<XmlNode> GetChildren(const std::string &name);

		XmlNode *GetChild(const std::string &name);

		XmlNode *GetChildrenWithAttribute(const std::string &name, const std::string &attribute, const std::string &value);

		void AddChild(const XmlNode &child);
	};
}
