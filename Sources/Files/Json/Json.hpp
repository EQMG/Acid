#pragma once

#include "Files/Node.hpp"

namespace acid {
class Json : public Node {
public:
	Json() = default;
	explicit Json(const Node &node);
	explicit Json(Node &&node);

	void ParseString(std::string_view string) override;
	void WriteStream(std::ostream &stream, Format format = Format::Minified) const override;

	Json& operator=(const Json& node) = default;
	Json& operator=(Json && node) = default;
	template<typename T>
	Json& operator=(const T & rhs) {
		Set(rhs);
		return *this;
	}

private:
	static void AddToken(std::string_view view, Tokens &tokens);
	static void Convert(Node &current, const Tokens &tokens, int32_t i, int32_t &r);

	static void AppendData(const Node &source, std::ostream &stream, Format format, int32_t indent);
};
}
