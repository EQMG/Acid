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
		
		~XmlNode();
	};
}
