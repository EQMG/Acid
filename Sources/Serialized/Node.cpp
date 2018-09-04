#include "Node.hpp"

#include <algorithm>
#include <ostream>
#include "Engine/Log.hpp"

namespace acid
{
	Node::Node(const std::string &name, const std::string &value, const std::map<std::string, std::string> &attributes) :
		m_name(FormatString::Trim(FormatString::RemoveAll(name, '\"'))),
		m_value(FormatString::Trim(value)),
		m_children(std::vector<std::shared_ptr<Node>>()),
		m_attributes(attributes)
	{
	}

	Node::Node(const Node &source) :
		m_name(source.m_name),
		m_value(source.m_value),
		m_children(source.m_children),
		m_attributes(source.m_attributes)
	{
	}

	Node::~Node()
	{
	}

	std::string Node::GetString() const
	{
		return FormatString::RemoveAll(m_value, '\"'); // FIXME: Just first and last.
	}

	void Node::SetString(const std::string &data)
	{
		m_value = "\"" + data + "\"";
	}

	std::shared_ptr<Node> Node::AddChild(const std::shared_ptr<Node> &value)
	{
		/*auto child = FindChild(value->m_name);

		if (child != nullptr)
		{
			child->m_value = value->m_value;
			return child;
		}*/

		m_children.emplace_back(value);
		return value;
	}

	bool Node::RemoveChild(const std::shared_ptr<Node> &value)
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), value), m_children.end());
		return true;
	}

	std::vector<std::shared_ptr<Node>> Node::FindChildren(const std::string &name) const
	{
		auto result = std::vector<std::shared_ptr<Node>>();

		for (auto &child : m_children)
		{
			if (child->m_name == name)
			{
				result.push_back(child);
			}
		}

		return result;
	}

	std::shared_ptr<Node> Node::FindChild(const std::string &name, const bool &reportError) const
	{
		std::string nameNoSpaces = FormatString::Replace(name, " ", "_");

		for (auto &child : m_children)
		{
			if (child->m_name == name || child->m_name == nameNoSpaces)
			{
				return child;
			}
		}

		if (reportError)
		{
			Log::Error("Could not find child in node by name '%s'\n", m_name.c_str());
		}

		return nullptr;
	}

	std::shared_ptr<Node> Node::FindChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value, const bool &reportError) const
	{
		auto children = FindChildren(childName);

		if (children.empty())
		{
			return nullptr;
		}

		for (auto &child : children)
		{
			std::string attrib = child->FindAttribute(attribute);

			if (attrib == value)
			{
				return child;
			}
		}

		if (reportError)
		{
			Log::Error("Could not find child in node '%s' with '%s'\n", m_name.c_str(), attribute.c_str());
		}

		return nullptr;
	}

	void Node::AddAttribute(const std::string &attribute, const std::string &value)
	{
		auto it = m_attributes.find(attribute);

		if (it == m_attributes.end())
		{
			m_attributes.emplace(attribute, value);
		}

		(*it).second = value;
	}

	bool Node::RemoveAttribute(const std::string &attribute)
	{
		auto it = m_attributes.find(attribute);

		if (it != m_attributes.end())
		{
			m_attributes.erase(it);
			return true;
		}

		return false;
	}

	std::string Node::FindAttribute(const std::string &attribute) const
	{
		auto it = m_attributes.find(attribute);

		if (it == m_attributes.end())
		{
			return nullptr;
		}

		return (*it).second;
	}
}
