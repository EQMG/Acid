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

	XmlNode::XmlNode(const XmlNode &source) :
		m_name(source.m_name),
		m_attributes(std::map<std::string, std::string>(source.m_attributes)),
		m_data(source.m_data),
		m_childNodes(std::map<std::string, std::vector<XmlNode>>(source.m_childNodes))
	{
	}

	XmlNode::~XmlNode()
	{
	}

	std::string &XmlNode::GetAttribute(const std::string &attribute)
	{
		return m_attributes.at(attribute);
	}

	void XmlNode::AddAttribute(const std::string &attribute, const std::string &value)
	{
		m_attributes.insert(std::make_pair(attribute, value));
	}

	std::vector<XmlNode> XmlNode::GetChildren(const std::string &name)
	{
		std::vector<XmlNode> nodes = m_childNodes.at(name);
		return std::vector<XmlNode>(nodes);
	}

	XmlNode *XmlNode::GetChild(const std::string &name)
	{
		if (m_childNodes.find(name) == m_childNodes.end())
		{
			return nullptr;
		}

		std::vector<XmlNode> nodes = m_childNodes.at(name);
		return !nodes.empty() ? &nodes.at(0) : nullptr;
	}

	XmlNode *XmlNode::GetChildrenWithAttribute(const std::string &name, const std::string &attribute, const std::string &value)
	{
		std::vector<XmlNode> children = GetChildren(name);

		if (children.empty())
		{
			return nullptr;
		}

		for (XmlNode child : children)
		{
			std::string val = child.GetAttribute(attribute);

			if (value == val)
			{
				return &child;
			}
		}

		return nullptr;
	}

	void XmlNode::AddChild(const XmlNode &child)
	{
		if (m_childNodes.find(child.m_name) == m_childNodes.end())
		{
			m_childNodes.insert(std::make_pair(child.m_name, std::vector<XmlNode>()));
			return;
		}

		m_childNodes.at(child.m_name).push_back(child);
	}
}
