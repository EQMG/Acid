#pragma once

#include "Node.hpp"

namespace acid {
template<typename Base>
class Encodable {
public:
	virtual ~Encodable() = default;
	
	friend const Node &operator>>(const Node &node, Base &base) {
		return base.Decode(node, base);
	}
	friend Node &operator<<(Node &node, const Base &base) {
		return base.Encode(node, base);
	}
	
	virtual const Node &Decode(const Node &node, Base &base) = 0; // operator>>
	virtual Node &Encode(Node &node, const Base &base) const = 0; // operator<<
};
}
