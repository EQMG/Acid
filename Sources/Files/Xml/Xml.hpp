#pragma once

#include "Files/Node.hpp"

namespace acid {
class Xml : public Node {
public:
	explicit Xml(const std::string &rootName);
	Xml(const std::string &rootName, const Node &node);
	Xml(const std::string &rootName, Node &&node);

	void ParseString(std::string_view string) override;
	void WriteStream(std::ostream &stream, Format format = Format::Minified) const override;

	Xml &operator=(const Xml &node) = default;
	Xml &operator=(Xml && node) = default;
	template<typename T>
	Xml &operator=(const T & rhs) {
		Set(rhs);
		return *this;
	}

private:
	static void AddToken(std::string_view view, std::vector<Token> &tokens);
	static void Convert(Node &current, const std::vector<Token> &tokens, int32_t i, int32_t &r);

	static void AppendData(const Node &source, std::ostream &stream, Format format, int32_t indent);
};
}
