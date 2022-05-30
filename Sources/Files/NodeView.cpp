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
		for (const auto &key : keys) {
			if (const auto name = std::get_if<std::string>(&key))
				value = &const_cast<Node *>(parent)->AddProperty(*name);
			else if (const auto index = std::get_if<std::uint32_t>(&key))
				value = &const_cast<Node *>(parent)->AddProperty(*index);
			else
				throw std::runtime_error("Key for node return is neither a int or a string");
			
			// Because the last key will set parent to the value parent usage should be avoided.
			parent = value;
		}

		keys.erase(keys.begin(), keys.end() - 1);
	}

	return const_cast<Node *>(value);
}

NodeView NodeView::GetPropertyWithBackup(const std::string &key, const std::string &backupKey) {
	if (!has_value())
		return {this, key};
	return const_cast<Node *>(value)->GetPropertyWithBackup(key, backupKey);
}

NodeView NodeView::GetPropertyWithValue(const std::string &key, const NodeValue &propertyValue) {
	if (!has_value())
		return {this, key};
	return const_cast<Node *>(value)->GetPropertyWithValue(key, propertyValue);
}

NodeView NodeView::operator[](const std::string &key) {
	if (!has_value())
		return {this, key};
	return const_cast<Node *>(value)->operator[](key);
}

NodeView NodeView::operator[](uint32_t index) {
	if (!has_value())
		return {this, index};
	return const_cast<Node *>(value)->operator[](index);
}

NodeProperties &NodeView::GetProperties() {
	if (!has_value())
		return get()->GetProperties();
	return const_cast<Node *>(value)->GetProperties();
}
}
