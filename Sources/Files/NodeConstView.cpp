#include "NodeConstView.hpp"

#include "Node.hpp"

namespace acid {
NodeConstView::NodeConstView(const Node *parent, Key key, const Node *value) :
	parent(parent),
	value(value),
	keys{std::move(key)} {
}

NodeConstView::NodeConstView(const NodeConstView *parent, Key key) :
	parent(parent->parent),
	keys(parent->keys) {
	keys.emplace_back(std::move(key));
}

std::vector<NodeConstView> NodeConstView::GetProperties(const std::string &name) const {
	if (!has_value())
		return {};
	return value->GetProperties(name);
}

NodeConstView NodeConstView::GetPropertyWithBackup(const std::string &name, const std::string &backupName) const {
	if (!has_value())
		return {this, name};
	return value->GetPropertyWithBackup(name, backupName);
}

NodeConstView NodeConstView::GetPropertyWithValue(const std::string &propertyName, const std::string &propertyValue) const {
	if (!has_value())
		return {this, propertyName};
	return value->GetPropertyWithValue(propertyName, propertyValue);
}

NodeConstView NodeConstView::operator[](const std::string &key) const {
	if (!has_value())
		return {this, key};
	return value->operator[](key);
}

NodeConstView NodeConstView::operator[](uint32_t index) const {
	if (!has_value())
		return {this, index};
	return value->operator[](index);
}

std::vector<Node> NodeConstView::GetProperties() const {
	if (!has_value())
		return {};
	return value->GetProperties();
}

std::string NodeConstView::GetName() const {
	if (!has_value())
		return "";
	return value->GetName();
}
}
