#pragma once

#include "StdAfx.hpp"

//#include "Serialized/Node.hpp"

namespace acid {
/**
 * @brief A managed resource object. Implementations contain Create functions that can take a node object or pass parameters to the constructor.
 */
class ACID_EXPORT Resource {
public:
	Resource() = default;

	virtual ~Resource() = default;

	/*template<typename T>
	friend std::enable_if_t<std::is_base_of_v<Resource, T>, const Node &> operator>>(const Node &node, std::shared_ptr<T> &object) {
		object = T::Create(node);
		return node;
	}*/
};
}
