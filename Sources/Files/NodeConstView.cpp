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

std::vector<NodeConstView> NodeConstView::GetProperties(std::string_view name) const {
	if (!has_value())
		return {};
	return value->GetProperties(name);
}

NodeConstView NodeConstView::GetPropertyWithBackup(std::string_view name, std::string_view backupName) const {
	if (!has_value())
		return {this, std::string(name)};
	return value->GetPropertyWithBackup(name, backupName);
}

NodeConstView NodeConstView::GetPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) const {
	if (!has_value())
		return {this, std::string(propertyName)};
	return value->GetPropertyWithValue(propertyName, propertyValue);
}

NodeConstView NodeConstView::operator[](std::string_view key) const {
	if (!has_value())
		return {this, std::string(key)};
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
		return *std::get_if<std::string>(&keys.back());
	return value->GetName();
}

NodeView::Type NodeConstView::GetType() const {
	if (!has_value())
		return Type::Unknown;
	return value->GetType();
}
}
