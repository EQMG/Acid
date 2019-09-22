#pragma once

#include "Files/Node.hpp"

namespace acid {
class Xml : public Node {
public:
	Xml(const std::string &rootName);
	Xml(const std::string &rootName, const Node &node);
	Xml(const std::string &rootName, Node &&node);

	void LoadString(std::string_view string) override;
	void WriteStream(std::ostream &stream, Format format = Format::Minified) const override;

private:
	static void AddToken(std::string_view view, std::vector<Token> &tokens);
	static void Convert(Node &current, const std::vector<Token> &tokens, int32_t i, int32_t &r);

	static void AppendData(const Node &source, std::ostream &stream, Node::Format format, int32_t indent);
};
}
