#include "Node.hpp"

#include <algorithm>

namespace acid {
const NodeFormat::Format NodeFormat::Beautified = Format(2, '\n', ' ', true);
const NodeFormat::Format NodeFormat::Minified = Format(0, '\0', '\0', false);

static const Node NullNode = (Node() = nullptr);

Node::Node() :
	type(Type::Object) {
}

Node::Node(const std::string &name) :
	name(name),
	type(Type::Object) {
}

Node::Node(const std::string &name, const Node &node) :
	Node(node) {
	SetName(name);
}

void Node::Clear() {
	properties.clear();
}

bool Node::IsValid() const {
	switch (type) {
	case Type::Token:
	case Type::Unknown:
		return false;
	case Type::Object:
	case Type::Array:
		return !properties.empty();
	case Type::Null:
		return true;
	default:
		return !value.empty();
	}
}

bool Node::HasProperty(const std::string &name) const {
	for (const auto &property : properties) {
		if (property.name == name)
			return true;
	}

	return false;
}

bool Node::HasProperty(uint32_t index) const {
	return index < properties.size();
}

NodeConstView Node::GetProperty(const std::string &name) const {
	for (const auto &property : properties) {
		if (property.name == name)
			return {this, name, &property};
	}

	return {this, name, nullptr};
}

NodeConstView Node::GetProperty(uint32_t index) const {
	if (index < properties.size())
		return {this, index, &properties[index]};

	return {this, index, nullptr};
}

// TODO: Duplicate
NodeView Node::GetProperty(const std::string &name) {
	for (auto &property : properties) {
		if (property.name == name)
			return {this, name, &property};
	}

	return {this, name, nullptr};
}

// TODO: Duplicate
NodeView Node::GetProperty(uint32_t index) {
	if (index < properties.size())
		return {this, index, &properties[index]};

	return {this, index, nullptr};
}

Node &Node::AddProperty(const Node &node) {
	return properties.emplace_back(node);
}

Node &Node::AddProperty(Node &&node) {
	return properties.emplace_back(std::move(node));
}

Node &Node::AddProperty(const std::string &name, const Node &node) {
	Node newNode = node;
	newNode.name = name;
	return properties.emplace_back(std::move(newNode));
}

Node &Node::AddProperty(const std::string &name, Node &&node) {
	node.name = name;
	return properties.emplace_back(std::move(node));
}

Node &Node::AddProperty(uint32_t index, const Node &node) {
	properties.resize(std::max(properties.size(), static_cast<std::size_t>(index + 1)), NullNode);
	return properties[index] = node;
}

Node &Node::AddProperty(uint32_t index, Node &&node) {
	properties.resize(std::max(properties.size(), static_cast<std::size_t>(index + 1)), NullNode);
	return properties[index] = std::move(node);
}

void Node::RemoveProperty(const std::string &name) {
	//node.parent = nullptr;
	properties.erase(std::remove_if(properties.begin(), properties.end(), [name](const auto &n) {
		return n.name == name;
	}), properties.end());
}

void Node::RemoveProperty(const Node &node) {
	//node.parent = nullptr;
	properties.erase(std::remove_if(properties.begin(), properties.end(), [node](const auto &n) {
		return n == node;
	}), properties.end());
}

std::vector<NodeConstView> Node::GetProperties(const std::string &name) const {
	std::vector<NodeConstView> properties;

	for (const auto &property : this->properties) {
		if (property.name == name)
			properties.emplace_back(NodeConstView(this, name, &property));
	}

	return properties;
}

NodeConstView Node::GetPropertyWithBackup(const std::string &name, const std::string &backupName) const {
	if (auto p1 = GetProperty(name))
		return p1;
	if (auto p2 = GetProperty(backupName))
		return p2;
	return {this, name, nullptr};
}

NodeConstView Node::GetPropertyWithValue(const std::string &name, const std::string &value) const {
	for (const auto &property : properties) {
		auto properties1 = property.GetProperties(name);
		if (properties1.empty())
			return {this, name, nullptr};

		for (auto &property1 : properties1) {
			if (property1 && property1->GetValue() == value)
				return {this, name, &property};
		}
	}

	return {this, name, nullptr};
}

// TODO: Duplicate
std::vector<NodeView> Node::GetProperties(const std::string &name) {
	std::vector<NodeView> properties;

	for (auto &property : this->properties) {
		if (property.name == name)
			properties.emplace_back(NodeView(this, name, &property));
	}

	return properties;
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
NodeView Node::GetPropertyWithValue(const std::string &name, const std::string &value) {
	for (auto &property : properties) {
		auto properties1 = property.GetProperties(name);
		if (properties1.empty())
			return {this, name, nullptr};

		for (auto &property1 : properties1) {
			if (property1 && property1->GetValue() == value)
				return {this, name, &property};
		}
	}

	return {this, std::string(name), nullptr};
}

NodeConstView Node::operator[](const std::string &key) const {
	return GetProperty(key);
}

NodeConstView Node::operator[](uint32_t index) const {
	return GetProperty(index);
}

// TODO: Duplicate
NodeView Node::operator[](const std::string &key) {
	return GetProperty(key);
}

// TODO: Duplicate
NodeView Node::operator[](uint32_t index) {
	return GetProperty(index);
}

Node &Node::operator=(const Node &rhs) {
	properties = rhs.properties;
	//name = rhs.name;
	value = rhs.value;
	type = rhs.type;
	return *this;
}

Node &Node::operator=(Node &&rhs) noexcept {
	properties = std::move(rhs.properties);
	//name = std::move(rhs.name);
	value = std::move(rhs.value);
	type = std::move(rhs.type);
	return *this;
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
