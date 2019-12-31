#include "NodeConstView.hpp"

#include "Node.hpp"

namespace acid {
NodeConstView::NodeConstView(const Node *parent, Key key, const Node *value) :
	m_parent(parent),
	m_value(value),
	m_keys{std::move(key)} {
}

NodeConstView::NodeConstView(const NodeConstView *parent, Key key) :
	m_parent(parent->m_parent),
	m_keys(parent->m_keys) {
	m_keys.emplace_back(std::move(key));
}

std::vector<NodeConstView> NodeConstView::GetProperties(std::string_view name) const {
	if (!has_value())
		return {};
	return m_value->GetProperties(name);
}

NodeConstView NodeConstView::GetPropertyWithBackup(std::string_view name, std::string_view backupName) const {
	if (!has_value())
		return {this, std::string(name)};
	return m_value->GetPropertyWithBackup(name, backupName);
}

NodeConstView NodeConstView::GetPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) const {
	if (!has_value())
		return {this, std::string(propertyName)};
	return m_value->GetPropertyWithValue(propertyName, propertyValue);
}

NodeConstView NodeConstView::operator[](std::string_view key) const {
	if (!has_value())
		return {this, std::string(key)};
	return m_value->operator[](key);
}

NodeConstView NodeConstView::operator[](uint32_t index) const {
	if (!has_value())
		return {this, index};
	return m_value->operator[](index);
}

std::vector<Node> NodeConstView::GetProperties() const {
	if (!has_value())
		return {};
	return m_value->GetProperties();
}

std::string NodeConstView::GetName() const {
	if (!has_value())
		return *std::get_if<std::string>(&m_keys.back());
	return m_value->GetName();
}

NodeView::Type NodeConstView::GetType() const {
	if (!has_value())
		return Type::Unknown;
	return m_value->GetType();
}

}
