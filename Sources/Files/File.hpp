#pragma once

#include "Files/NodeFormat.hpp"

namespace acid {
/**
 * @brief Class that represents a readable and writable file format using {@link Node} as storage.
 */
class ACID_EXPORT File {
public:
	File() = default;
	File(std::unique_ptr<NodeFormat> &&type, const Node &node);
	explicit File(std::unique_ptr<NodeFormat> &&type, Node &&node = {});
	File(std::filesystem::path filename, std::unique_ptr<NodeFormat> &&type, const Node &node);
	File(std::filesystem::path filename, std::unique_ptr<NodeFormat> &&type, Node &&node = {});

	void Load(const std::filesystem::path &filename);
	void Load();
	
	void Write(const std::filesystem::path &filename, NodeFormat::Format format = NodeFormat::Minified) const;
	void Write(NodeFormat::Format format = NodeFormat::Minified) const;
	
	void Clear();

	const Node &GetNode() const { return node; }
	Node &GetNode() { return node; }

	const std::filesystem::path &GetFilename() const { return filename; }
	void SetFilename(const std::filesystem::path &filename) { this->filename = filename; }

private:
	Node node;
	std::unique_ptr<NodeFormat> type;
	std::filesystem::path filename;
};
}
