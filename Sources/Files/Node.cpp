#include "Node.hpp"

namespace acid {
const Node::Format Node::Format::Beautified = Format(2, '\n', ' ', true);
const Node::Format Node::Format::Minified = Format(0, '\0', '\0', false);

static const Node NullNode = Node("null", Node::Type::Null);

Node::Node(std::string value, Type type) :
	m_value(std::move(value)),
	m_type(type) {
}

Node::Node(std::string value, std::vector<Node> &&properties) :
	m_properties(std::move(properties)),
	m_value(std::move(value)) {
}

void Node::ParseString(std::string_view string) {
}

void Node::WriteStream(std::ostream &stream, const Format &format) const {
}

void Node::Clear() {
	m_properties.clear();
}

bool Node::IsValid() const {
	switch (m_type) {
	case Type::Token:
	case Type::Unknown:
		return false;
	case Type::Object:
	case Type::Array:
		return !m_properties.empty();
	case Type::Null:
		return true;
	default:
		return !m_value.empty();
	}
}

bool Node::HasProperty(std::string_view name) const {
	for (const auto &property : m_properties) {
		if (property.m_name == name)
			return true;
	}

	return false;
}

NodeConstView Node::GetProperty(std::string_view name) const {
	for (const auto &property : m_properties) {
		if (property.m_name == name)
			return {this, std::string(name), &property};
	}

	return {this, std::string(name), nullptr};
}

NodeConstView Node::GetProperty(uint32_t index) const {
	if (index < m_properties.size())
		return {this, index, &m_properties[index]};

	return {this, index, nullptr};
}

// TODO: Duplicate
NodeView Node::GetProperty(std::string_view name) {
	for (auto &property : m_properties) {
		if (property.m_name == name)
			return {this, std::string(name), &property};
	}

	return {this, std::string(name), nullptr};
}

// TODO: Duplicate
NodeView Node::GetProperty(uint32_t index) {
	if (index < m_properties.size())
		return {this, index, &m_properties[index]};

	return {this, index, nullptr};
}

Node &Node::AddProperty() {
	return m_properties.emplace_back();
}

Node &Node::AddProperty(std::string_view name, Node &&node) {
	node.m_name = name;
	return m_properties.emplace_back(std::move(node));
}

Node &Node::AddProperty(std::string_view name) {
	Node node;
	node.m_name = name;
	return m_properties.emplace_back(std::move(node));
}

Node &Node::AddProperty(uint32_t index, Node &&node) {
	m_properties.resize(std::max(m_properties.size(), static_cast<std::size_t>(index + 1)), NullNode);
	return m_properties[index] = std::move(node);
}

Node &Node::AddProperty(uint32_t index) {
	m_properties.resize(std::max(m_properties.size(), static_cast<std::size_t>(index + 1)), NullNode);
	return m_properties[index];
}

void Node::RemoveProperty(std::string_view name) {
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

std::vector<NodeConstView> Node::GetProperties(std::string_view name) const {
	std::vector<NodeConstView> properties;

	for (const auto &property : m_properties) {
		if (property.m_name == name)
			properties.emplace_back(NodeConstView(this, std::string(name), &property));
	}

	return properties;
}

NodeConstView Node::GetPropertyWithBackup(std::string_view name, std::string_view backupName) const {
	if (auto p1 = GetProperty(name))
		return p1;
	if (auto p2 = GetProperty(backupName))
		return p2;
	return {this, std::string(name), nullptr};
}

NodeConstView Node::GetPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) const {
	for (const auto &property : m_properties) {
		auto properties1 = property.GetProperties(propertyName);
		if (properties1.empty())
			return {this, std::string(propertyName), nullptr};

		for (auto &property1 : properties1) {
			if (property1 && property1->GetValue() == propertyValue)
				return {this, std::string(propertyName), &property};
		}
	}

	return {this, std::string(propertyName), nullptr};
}

// TODO: Duplicate
std::vector<NodeView> Node::GetProperties(std::string_view name) {
	std::vector<NodeView> properties;

	for (auto &property : m_properties) {
		if (property.m_name == name)
			properties.emplace_back(NodeView(this, std::string(name), &property));
	}

	return properties;
}

// TODO: Duplicate
NodeView Node::GetPropertyWithBackup(std::string_view name, std::string_view backupName) {
	if (auto p1 = GetProperty(name))
		return p1;
	if (auto p2 = GetProperty(backupName))
		return p2;
	return {this, std::string(name), nullptr};
}

// TODO: Duplicate
NodeView Node::GetPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) {
	for (auto &property : m_properties) {
		auto properties1 = property.GetProperties(propertyName);
		if (properties1.empty())
			return {this, std::string(propertyName), nullptr};

		for (auto &property1 : properties1) {
			if (property1 && property1->GetValue() == propertyValue)
				return {this, std::string(propertyName), &property};
		}
	}

	return {this, std::string(propertyName), nullptr};
}

NodeConstView Node::operator[](std::string_view key) const {
	return GetProperty(key);
}

NodeConstView Node::operator[](uint32_t index) const {
	return GetProperty(index);
}

NodeView Node::operator[](std::string_view key) {
	return GetProperty(key);
}

NodeView Node::operator[](uint32_t index) {
	return GetProperty(index);
}

bool Node::operator==(const Node &other) const {
	return m_value == other.m_value && m_properties.size() == other.m_properties.size() &&
		std::equal(m_properties.begin(), m_properties.end(), other.m_properties.begin(), [](const auto &left, const auto &right) {
			return left == right;
		});
}

bool Node::operator!=(const Node &other) const {
	return !operator==(other);
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
