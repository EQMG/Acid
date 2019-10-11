#pragma once

#include "Files/Node.hpp"

namespace acid {
/**
 * @brief Class that represents a readable and writable file format using {@link Node} as storage.
 */
class ACID_EXPORT File {
public:
	File() = default;
	explicit File(std::unique_ptr<Node> &&node);
	File(std::filesystem::path filename, std::unique_ptr<Node> &&node);
	explicit File(const std::filesystem::path &filename);

	void Load(const std::filesystem::path &filename);
	void Write(const std::filesystem::path &filename, Node::Format format = Node::Format::Minified) const;
	
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
