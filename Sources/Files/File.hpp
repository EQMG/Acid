#pragma once

#include "Files/Node.hpp"

namespace acid {
/**
 * @brief Class that represents a readable and writable file format using {@link Node} as storage.
 */
class ACID_EXPORT File {
public:
	// TODO: Implement a more dynamic, less hard-coded, file parse/write.
	enum class Type {
		Json, Xml
	};
	
	File() = default;
	File(Type type, const Node &node);
	explicit File(Type type, Node &&node = {});
	File(std::filesystem::path filename, Type type, const Node &node);
	File(std::filesystem::path filename, Type type, Node &&node = {});

	void Load(const std::filesystem::path &filename);
	void Load();
	
	void Write(const std::filesystem::path &filename, Node::Format format = Node::Format::Minified) const;
	void Write(Node::Format format = Node::Format::Minified) const;
	
	void Clear();

	const Node &GetNode() const { return node; }
	Node &GetNode() { return node; }

	const std::filesystem::path &GetFilename() const { return filename; }
	void SetFilename(const std::filesystem::path &filename) { this->filename = filename; }

private:
	Node node;
	Type type;
	std::filesystem::path filename;
};
}
