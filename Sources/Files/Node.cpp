#include "Node.hpp"

#include <algorithm>

namespace acid {
static const NodeProperty NullNode = NodeProperty("", Node() = nullptr);

void Node::Clear() {
	properties.clear();
}

bool Node::IsValid() const {
	switch (type) {
	case NodeType::Token:
	case NodeType::Unknown:
		return false;
	case NodeType::Object:
	case NodeType::Array:
		return !properties.empty();
	case NodeType::Null:
		return true;
	default:
		return !value.empty();
	}
}

bool Node::HasProperty(const std::string &name) const {
	for (const auto &[propertyName, property] : properties) {
		if (propertyName == name)
			return true;
	}

	return false;
}

bool Node::HasProperty(uint32_t index) const {
	return index < properties.size();
}

NodeConstView Node::GetProperty(const std::string &name) const {
	for (const auto &[propertyName, property] : properties) {
		if (propertyName == name)
			return {this, name, &property};
	}
	return {this, name, nullptr};
}

NodeConstView Node::GetProperty(uint32_t index) const {
	if (index < properties.size())
		return {this, index, &properties[index].second};

	return {this, index, nullptr};
}

// TODO: Duplicate
NodeView Node::GetProperty(const std::string &name) {
	for (auto &[propertyName, property] : properties) {
		if (propertyName == name)
			return {this, name, &property};
	}
	return {this, name, nullptr};
}

// TODO: Duplicate
NodeView Node::GetProperty(uint32_t index) {
	if (index < properties.size())
		return {this, index, &properties[index].second};

	return {this, index, nullptr};
}

Node &Node::AddProperty(const Node &node) {
	type = NodeType::Array;
	return properties.emplace_back(NodeProperty("", node)).second;
}

Node &Node::AddProperty(Node &&node) {
	type = NodeType::Array;
	return properties.emplace_back(NodeProperty("", std::move(node))).second;
}

Node &Node::AddProperty(const std::string &name, const Node &node) {
	return properties.emplace_back(NodeProperty(name, node)).second;
}

Node &Node::AddProperty(const std::string &name, Node &&node) {
	return properties.emplace_back(NodeProperty(name, std::move(node))).second;
}

Node &Node::AddProperty(uint32_t index, const Node &node) {
	type = NodeType::Array;
	properties.resize(std::max(properties.size(), static_cast<std::size_t>(index + 1)), NullNode);
	return properties[index].second = node;
}

Node &Node::AddProperty(uint32_t index, Node &&node) {
	type = NodeType::Array;
	properties.resize(std::max(properties.size(), static_cast<std::size_t>(index + 1)), NullNode);
	return properties[index].second = std::move(node);
}

Node Node::RemoveProperty(const std::string &name) {
	for (auto it = properties.begin(); it != properties.end(); ) {
		if (it->first == name) {
			auto result = std::move(it->second);
			properties.erase(it);
			return result;
		}
		++it;
	}
	return {};
}

Node Node::RemoveProperty(const Node &node) {
	for (auto it = properties.begin(); it != properties.end(); ) {
		if (it->second == node) {
			auto result = std::move(it->second);
			it = properties.erase(it);
			return result;
		}
		++it;
	}
	return {};
}

NodeConstView Node::GetPropertyWithBackup(const std::string &name, const std::string &backupName) const {
	if (auto p1 = GetProperty(name))
		return p1;
	if (auto p2 = GetProperty(backupName))
		return p2;
	return {this, name, nullptr};
}

NodeConstView Node::GetPropertyWithValue(const std::string &name, const NodeValue &propertyValue) const {
	for (const auto &[propertyName, property] : properties) {
		if (auto property1 = property.GetProperty(name); property1->GetValue() == propertyValue)
			return {this, name, &property};
	}

	return {this, name, nullptr};
}

// TODO: Duplicate
NodeView Node::GetPropertyWithBackup(const std::string &name, const std::string &backupName) {
	if (auto p1 = GetProperty(name))
		return p1;
	if (auto p2 = GetProperty(backupName))
		return p2;
	return {this, name, nullptr};
}

// TODO: Duplicate
NodeView Node::GetPropertyWithValue(const std::string &name, const NodeValue &propertyValue) {
	for (auto &[propertyName, property] : properties) {
		if (auto property1 = property.GetProperty(name); property1->GetValue() == propertyValue)
			return {this, name, &property};
		return {this, name, nullptr};
	}

	return {this, name, nullptr};
}

NodeConstView Node::operator[](const std::string &name) const {
	return GetProperty(name);
}

NodeConstView Node::operator[](uint32_t index) const {
	return GetProperty(index);
}

// TODO: Duplicate
NodeView Node::operator[](const std::string &name) {
	return GetProperty(name);
}

// TODO: Duplicate
NodeView Node::operator[](uint32_t index) {
	return GetProperty(index);
}

Node &Node::operator=(const NodeConstView &rhs) {
	return operator=(*rhs);
}

Node &Node::operator=(NodeConstView &&rhs) {
	return operator=(*rhs);
}

Node &Node::operator=(NodeView &rhs) {
	return operator=(*rhs);
}

Node &Node::operator=(NodeView &&rhs) {
	return operator=(*rhs);
}

bool Node::operator==(const Node &rhs) const {
	return value == rhs.value && properties.size() == rhs.properties.size() &&
		std::equal(properties.begin(), properties.end(), rhs.properties.begin(), [](const auto &left, const auto &right) {
		return left == right;
	});
}

bool Node::operator!=(const Node &rhs) const {
	return !operator==(rhs);
}

bool Node::operator<(const Node &rhs) const {
	if (value < rhs.value) return true;
	if (rhs.value < value) return false;

	if (properties < rhs.properties) return true;
	if (rhs.properties < properties) return false;

	return false;
}
}
