#pragma once

#include "Node.hpp"

namespace acid {
template<typename Base>
class Encodable {
public:
	virtual const Node &Decode(Node &node, const Base &base); // operator>>
	virtual Node &Encode(const Node &node, Base &base); // operator<<
};
}
