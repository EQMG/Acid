#include "Node.hpp"

namespace acid
{
Node::Node(std::string value, const Type &type) :
	m_value{std::move(value)},
	m_type{type}
{
}

Node::Node(std::string value, std::vector<Property> &&properties) :
	m_value{std::move(value)},
	m_properties{std::move(properties)}
{
	for (auto &[propertyName, property] : m_properties)
	{
		property.m_parent = this;
	}
}

void Node::Load(std::istream &stream)
{
}

void Node::Write(std::ostream &stream, const Format &format) const
{
}

void Node::Remove()
{
	if (m_parent == nullptr)
	{
		throw std::runtime_error("Cannot remove from parent properties if parent is null");
	}

	m_parent->RemoveProperty(*this);
}

bool Node::IsValid() const
{
	return !m_value.empty();
}

std::string Node::GetName() const
{
	if (m_parent == nullptr)
	{
		throw std::runtime_error("Cannot get name if parent is null");
	}

	for (const auto &[propertyName, property] : m_parent->m_properties)
	{
		if (property == *this)
		{
			return propertyName;
		}
	}

	return "";
}

void Node::SetName(const std::string &name)
{
	if (m_parent == nullptr)
	{
		throw std::runtime_error("Cannot set name if parent is null");
	}

	for (auto it{m_parent->m_properties.begin()}; it < m_parent->m_properties.end(); ++it)
	{
		if (it->second == *this)
		{
			it->first = name;
			return;
		}
	}
}

bool Node::HasProperty(const std::string &name) const
{
	for (const auto &[propertyName, property] : m_properties)
	{
		if (propertyName == name)
		{
			return true;
		}
	}

	return false;
}

NodeReturn Node::GetProperty(const std::string &name) const
{
	for (const auto &property : m_properties)
	{
		if (property.first == name)
		{
			return {this, name, &property.second};
		}
	}

	return {this, name, nullptr};
}

NodeReturn Node::GetProperty(const uint32_t &index) const
{
	if (index < m_properties.size())
	{
		return {this, index, &m_properties[index].second};
	}

	return {this, index, nullptr};
}

Node &Node::AddProperty()
{
	Node node;
	node.m_parent = this;
	return m_properties.emplace_back("", std::move(node)).second;
}

Node &Node::AddProperty(const std::string &name, Node &&node)
{
	node.m_parent = this;
	return m_properties.emplace_back(name, std::move(node)).second;
}

Node &Node::AddProperty(const uint32_t &index, Node &&node)
{
	m_properties.resize(std::max(m_properties.size(), static_cast<std::size_t>(index + 1)), {"", Node{"null", Type::Null}});
	return m_properties[index].second = node;
}

void Node::RemoveProperty(const std::string &name)
{
	//node.m_parent = nullptr;
	m_properties.erase(std::remove_if(m_properties.begin(), m_properties.end(), [name](const auto &n)
	{
		return n.first == name;
	}), m_properties.end());
}

void Node::RemoveProperty(const Node &node)
{
	//node.m_parent = nullptr;
	m_properties.erase(std::remove_if(m_properties.begin(), m_properties.end(), [node](const auto &n)
	{
		return n.second == node;
	}), m_properties.end());
}

NodeReturn Node::operator[](const std::string &key) const
{
	return GetProperty(key);
}

NodeReturn Node::operator[](const uint32_t &index) const
{
	return GetProperty(index);
}

bool Node::operator==(const Node &other) const
{
	return m_value == other.m_value && m_properties.size() == other.m_properties.size() && 
		std::equal(m_properties.begin(), m_properties.end(), other.m_properties.begin(), [](const auto &left, const auto &right)
		{
			return left == right;
		});
}

bool Node::operator!=(const Node &other) const
{
	return !(*this == other);
}

bool Node::operator<(const Node &other) const
{
	return m_value < other.m_value || m_properties < other.m_properties;
}
}
