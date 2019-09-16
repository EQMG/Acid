#pragma once

#include "Serialized/Node.hpp"

namespace acid {
class ACID_EXPORT Json : public Node {
public:
	Json() = default;
	explicit Json(const Node &node);
	explicit Json(Node &&node);
	
protected:
	void LoadStructure(const std::string &string) override;
	void WriteStructure(std::ostream &stream, Format format) const override;
	
private:
	static void AddToken(std::vector<std::pair<Type, std::string>> &tokens, std::string &current);
	static void Convert(Node &current, const std::vector<std::pair<Type, std::string>> &v, int32_t i, int32_t &r);

	static void AppendData(const Node &source, std::ostream &stream, int32_t indentation, Format format);
};
}
