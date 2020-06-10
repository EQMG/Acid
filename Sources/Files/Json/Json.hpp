#pragma once

#include "Files/Node.hpp"

namespace acid {
class ACID_EXPORT Json : public NodeFormat {
public:
	void ParseString(Node &node, std::string_view string) override;
	void WriteStream(const Node &node, std::ostream &stream, Format format) override;

private:
	static void AddToken(std::string_view view, std::vector<Token> &tokens);
	static void Convert(Node &current, const std::vector<Token> &tokens, int32_t &k);

	static void AppendData(const Node &node, std::ostream &stream, Format format, int32_t indent);
};
}
