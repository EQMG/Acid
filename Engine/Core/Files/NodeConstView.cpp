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

NodeConstView NodeConstView::GetPropertyWithBackup(const std::string &key, const std::string &backupKey) const {
	if (!has_value())
		return {this, key};
	return value->GetPropertyWithBackup(key, backupKey);
}

NodeConstView NodeConstView::GetPropertyWithValue(const std::string &key, const NodeValue &propertyValue) const {
	if (!has_value())
		return {this, key};
	return value->GetPropertyWithValue(key, propertyValue);
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

NodeProperties NodeConstView::GetProperties() const {
	if (!has_value())
		return NodeProperties{};
	return value->GetProperties();
}

NodeType NodeConstView::GetType() const {
	if (!has_value())
		return NodeType::Null;
	return value->GetType();
}
}
