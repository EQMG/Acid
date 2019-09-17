#include "Node.hpp"

namespace acid {
static const Node NullNode = Node("null", Node::Type::Null);

Node::Node(std::string value, Type type) :
	m_value(std::move(value)),
	m_type(type) {
}

Node::Node(std::string value, std::vector<Node> &&properties) :
	m_properties(std::move(properties)),
	m_value(std::move(value)) {
}

void Node::LoadString(std::string_view string) {
}

void Node::WriteStream(std::ostream &stream, Format format) const {
}

void Node::Clear() {
	m_properties.clear();
}

bool Node::IsValid() const {
	switch (m_type) {
	case Type::Object:
	case Type::Array:
		return !m_properties.empty();
	default:
		return !m_value.empty();
	}
}

bool Node::HasProperty(const std::string &name) const {
	for (const auto &property : m_properties) {
		if (property.m_name == name) {
			return true;
		}
	}

	return false;
}

NodeReturn Node::GetProperty(const std::string &name) const {
	for (const auto &property : m_properties) {
		if (property.m_name == name) {
			return {this, name, &property};
		}
	}

	return {this, name, nullptr};
}

NodeReturn Node::GetProperty(uint32_t index) const {
	if (index < m_properties.size()) {
		return {this, index, &m_properties[index]};
	}

	return {this, index, nullptr};
}

Node &Node::AddProperty() {
	return m_properties.emplace_back();
}

Node &Node::AddProperty(const std::string &name, Node &&node) {
	node.m_name = name;
	return m_properties.emplace_back(std::move(node));
}

Node &Node::AddProperty(uint32_t index, Node &&node) {
	m_properties.resize(std::max(m_properties.size(), static_cast<std::size_t>(index + 1)), NullNode);
	return m_properties[index] = std::move(node);
}

void Node::RemoveProperty(const std::string &name) {
	//node.m_parent = nullptr;
	m_properties.erase(std::remove_if(m_properties.begin(), m_properties.end(), [name](const auto &n) {
		return n.GetName() == name;
	}), m_properties.end());
}

void Node::RemoveProperty(const Node &node) {
	//node.m_parent = nullptr;
	m_properties.erase(std::remove_if(m_properties.begin(), m_properties.end(), [node](const auto &n) {
		return n == node;
	}), m_properties.end());
}

NodeReturn Node::operator[](const std::string &key) const {
	return GetProperty(key);
}

NodeReturn Node::operator[](uint32_t index) const {
	return GetProperty(index);
}

bool Node::operator==(const Node &other) const {
	return m_value == other.m_value && m_properties.size() == other.m_properties.size() &&
		std::equal(m_properties.begin(), m_properties.end(), other.m_properties.begin(), [](const auto &left, const auto &right) {
			return left == right;
		});
}

bool Node::operator!=(const Node &other) const {
	return !(*this == other);
}

bool Node::operator<(const Node &other) const {
	if (m_name < other.m_name) return true;
	if (other.m_name < m_name) return false;

	if (m_value < other.m_value) return true;
	if (other.m_value < m_value) return false;

	if (m_properties < other.m_properties) return true;
	if (other.m_properties < m_properties) return false;
	
	return false;
}
}
