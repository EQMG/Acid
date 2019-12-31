#include "NodeView.hpp"

#include "Node.hpp"

namespace acid {
NodeView::NodeView(const Node *parent, Key key, const Node *value) :
	m_parent(const_cast<Node *>(parent)),
	m_keys{std::move(key)},
	m_value(const_cast<Node *>(value)) {
}

NodeView::NodeView(const NodeView *parent, Key key) :
	m_parent(parent->m_parent),
	m_keys(parent->m_keys) {
	m_keys.emplace_back(std::move(key));
}

bool NodeView::has_value() const noexcept {
	return m_value != nullptr;
}

Node *NodeView::get() {
	if (!has_value()) {
		// This will build the tree of nodes from the return keys tree.
		for (const auto &key : m_keys) {
			if (std::holds_alternative<int32_t>(key)) {
				const auto &index = std::get<std::int32_t>(key);
				m_value = &m_parent->AddProperty(index, {});
			} else if (std::holds_alternative<std::string>(key)) {
				const auto &name = std::get<std::string>(key);
				m_value = &m_parent->AddProperty(name, {});
			} else {
				throw std::runtime_error("Key for node return is neither a int or a string");
			}

			// Because the last key will set parent to the value parent usage should be avoided.
			m_parent = m_value;
		}

		m_keys.erase(m_keys.begin(), m_keys.end() - 1);
	}

	return m_value;
}

std::vector<NodeView> NodeView::GetProperties(std::string_view name) const {
	if (!has_value())
		return {};
	return m_value->GetProperties(name);
}

NodeView NodeView::GetPropertyWithBackup(std::string_view name, std::string_view backupName) const {
	if (!has_value())
		return {this, std::string(name)};
	return m_value->GetPropertyWithBackup(name, backupName);
}

NodeView NodeView::GetPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) const {
	if (!has_value())
		return {this, std::string(propertyName)};
	return m_value->GetPropertyWithValue(propertyName, propertyValue);
}

NodeView NodeView::operator[](uint32_t index) const {
	if (!has_value())
		return {this, index};
	return m_value->operator[](index);
}

NodeView NodeView::operator[](std::string_view key) const {
	if (!has_value())
		return {this, std::string(key)};
	return m_value->operator[](key);
}

std::vector<Node> NodeView::GetProperties() const {
	if (!has_value())
		return {};
	return m_value->GetProperties();
}

std::vector<Node> &NodeView::GetProperties() {
	if (!has_value())
		return get()->GetProperties();
	return m_value->GetProperties();
}

std::string NodeView::GetName() const {
	if (!has_value())
		return *std::get_if<std::string>(&m_keys.back());
	return m_value->GetName();
}

void NodeView::SetName(const std::string &name) {
	if (!has_value()) {
		m_keys.back() = name;
		return;
	}

	m_value->SetName(name);
}

NodeView::Type NodeView::GetType() const {
	if (!has_value())
		return Type::Unknown;
	return m_value->GetType();
}
}
