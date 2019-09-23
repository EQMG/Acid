#pragma once

#include "Node.hpp"

namespace acid {
template<typename Base>
class Encodable {
public:
	void Decode(Node &node, const Base &base) {
		node << base;
	}
	void Encode(const Node &node, Base &base) {
		node >> base;
	}
};
}
