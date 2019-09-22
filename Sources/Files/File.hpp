#pragma once

#include "Files/Node.hpp"

namespace acid {
/**
 * @brief Class that represents a readable and writable file format using {@link Node} as storage.
 */
class ACID_EXPORT File {
public:
	explicit File(std::unique_ptr<Node> &&node);

	void Load(const std::filesystem::path &filename);
	void Write(const std::filesystem::path &filename, Node::Format format = Node::Format::Minified) const;
	void Clear();

	Node *GetNode() const { return m_node.get(); }

private:
	std::unique_ptr<Node> m_node;
};
}
