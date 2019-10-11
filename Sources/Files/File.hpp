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
	explicit File(const std::filesystem::path &filename);
	File(std::filesystem::path filename, std::unique_ptr<Node> &&node);

	void Load(const std::filesystem::path &filename);
	void Load();
	
	void Write(const std::filesystem::path &filename, Node::Format format = Node::Format::Minified) const;
	void Write(Node::Format format = Node::Format::Minified) const;
	
	void Clear();

	Node *GetNode() const { return m_node.get(); }

	const std::filesystem::path &GetFilename() const { return m_filename; }
	void SetFilename(const std::filesystem::path &filename) { m_filename = filename; }

private:
	std::unique_ptr<Node> m_node;
	std::filesystem::path m_filename;
};
}
