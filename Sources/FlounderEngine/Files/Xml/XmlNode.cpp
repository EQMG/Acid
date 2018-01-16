#include "XmlNode.hpp"

namespace Flounder
{
	XmlNode::XmlNode(const std::string &name) :
		m_name(name),
		m_attributes(std::map<std::string, std::string>()),
		m_data(""),
		m_childNodes(std::map<std::string, std::vector<XmlNode>>())
	{
	}
	
	XmlNode::~XmlNode()
	{
	}
}
