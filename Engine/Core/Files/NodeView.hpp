#pragma once

#include "NodeConstView.hpp"

namespace acid {
/**
 * @brief Class that extends the usage of {@link NodeConstView} to mutable nodes.
 */
class ACID_CORE_EXPORT NodeView : public NodeConstView {
	friend class Node;
protected:
	NodeView() = default;
	NodeView(Node *parent, Key key, Node *value);
	NodeView(NodeView *parent, Key key);

public:
	Node *get();

	operator Node &() { return *get(); }

	Node &operator*() { return *get(); }
	Node *operator->() { return get(); }

	template<typename T>
	void Set(const T &value);
	template<typename T>
	void Set(T &&value);
	
	NodeView GetPropertyWithBackup(const std::string &key, const std::string &backupKey);
	NodeView GetPropertyWithValue(const std::string &key, const NodeValue &propertyValue);

	NodeView operator[](const std::string &key);
	NodeView operator[](uint32_t index);

	NodeView operator=(const NodeConstView &) = delete;
	NodeView operator=(const NodeView &) = delete;
	template<typename T>
	Node &operator=(const T &rhs);
	template<typename T>
	Node &operator=(T &&rhs);

	NodeProperties &GetProperties();
};
}
