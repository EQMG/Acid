#pragma once

#include "Serialized/Node.hpp"

namespace acid {
/**
 * @brief Class that represents a readable and writable file format using {@link Node} as storage.
 */
class ACID_EXPORT File {
public:
	explicit File(std::filesystem::path filename, std::unique_ptr<Node> &&node);

	void Load();
	void Write(Node::Format format = Node::Format::Minified) const;
	void Clear();

	const std::filesystem::path &GetFilename() const { return m_filename; }
	void SetFilename(const std::filesystem::path &filename) { m_filename = filename; }

	Node *GetNode() const { return m_node.get(); }

private:
	std::filesystem::path m_filename;
	std::unique_ptr<Node> m_node;
};
}
