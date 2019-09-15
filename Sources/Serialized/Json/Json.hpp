#pragma once

#include "Serialized/Node.hpp"

namespace acid {
class ACID_EXPORT Json : public Node {
public:
	Json() = default;

	explicit Json(const Node &node);

	void Load(std::istream &stream) override;
	void Write(std::ostream &stream, const Format &format = Format::Beautified) const override;
	void Load(const std::string &string);
	std::string Write(const Format &format = Format::Beautified) const;

private:
	static void AddToken(std::vector<std::pair<Type, std::string>> &tokens, std::string &current);
	static void Convert(Node &current, const std::vector<std::pair<Type, std::string>> &v, int32_t i, int32_t &r);
	static void AppendData(const Node &source, std::ostream &outStream, int32_t indentation, const Format &format);
};
}
