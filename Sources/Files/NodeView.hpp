#pragma once

#include "NodeConstView.hpp"

namespace acid {
/**
 * @brief Class that extends the usage of {@link NodeConstView} to mutable nodes.
 */
class ACID_EXPORT NodeView : public NodeConstView {
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
	void SetName(const T &value);

	template<typename T>
	void Set(const T &value);

	std::vector<NodeView> GetProperties(std::string_view name);
	NodeView GetPropertyWithBackup(std::string_view name, std::string_view backupName);
	NodeView GetPropertyWithValue(std::string_view propertyName, std::string_view propertyValue);

	NodeView operator[](std::string_view key);
	NodeView operator[](uint32_t index);

	template<typename T>
	Node &operator=(const T &rhs);

	std::vector<Node> &GetProperties();

	void SetName(const std::string &name);
};
}
