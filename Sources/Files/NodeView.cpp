#include "NodeView.hpp"

#include "Node.hpp"

namespace acid {
NodeView::NodeView(Node *parent, Key key, Node *value) :
	NodeConstView(parent, std::move(key), value) {
}

NodeView::NodeView(NodeView *parent, Key key) :
	NodeConstView(parent, std::move(key)) {
}

Node *NodeView::get() {
	if (!has_value()) {
		// This will build the tree of nodes from the return keys tree.
		for (const auto &key : m_keys) {
			if (std::holds_alternative<int32_t>(key)) {
				const auto &index = std::get<std::int32_t>(key);
				m_value = &const_cast<Node *>(m_parent)->AddProperty(index, {});
			} else if (std::holds_alternative<std::string>(key)) {
				const auto &name = std::get<std::string>(key);
				m_value = &const_cast<Node *>(m_parent)->AddProperty(name, {});
			} else {
				throw std::runtime_error("Key for node return is neither a int or a string");
			}

			// Because the last key will set parent to the value parent usage should be avoided.
			m_parent = m_value;
		}

		m_keys.erase(m_keys.begin(), m_keys.end() - 1);
	}

	return const_cast<Node *>(m_value);
}

std::vector<NodeView> NodeView::GetProperties(std::string_view name) {
	if (!has_value())
		return {};
	return const_cast<Node *>(m_value)->GetProperties(name);
}

NodeView NodeView::GetPropertyWithBackup(std::string_view name, std::string_view backupName) {
	if (!has_value())
		return {this, std::string(name)};
	return const_cast<Node *>(m_value)->GetPropertyWithBackup(name, backupName);
}

NodeView NodeView::GetPropertyWithValue(std::string_view propertyName, std::string_view propertyValue) {
	if (!has_value())
		return {this, std::string(propertyName)};
	return const_cast<Node *>(m_value)->GetPropertyWithValue(propertyName, propertyValue);
}

NodeView NodeView::operator[](std::string_view key) {
	if (!has_value())
		return {this, std::string(key)};
	return const_cast<Node *>(m_value)->operator[](key);
}

NodeView NodeView::operator[](uint32_t index) {
	if (!has_value())
		return {this, index};
	return const_cast<Node *>(m_value)->operator[](index);
}

std::vector<Node> &NodeView::GetProperties() {
	if (!has_value())
		return get()->GetProperties();
	return const_cast<Node *>(m_value)->GetProperties();
}

void NodeView::SetName(const std::string &name) {
	if (!has_value()) {
		m_keys.back() = name;
		return;
	}

	const_cast<Node *>(m_value)->SetName(name);
}
}
