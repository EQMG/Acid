#pragma once

#include <typeindex>

#include "Utils/NonCopyable.hpp"
#include "Core/Export.hpp"

namespace acid {
/**
 * @brief A managed resource object. Implementations contain Create functions that can take a node object or pass parameters to the constructor.
 */
class ACID_CORE_EXPORT Resource : NonCopyable {
public:
	Resource() = default;
	virtual ~Resource() = default;

	virtual std::type_index GetTypeIndex() const = 0;
	
	/*template<typename T>
	friend auto operator>>(const Node &node, std::shared_ptr<T> &object) -> std::enable_if_t<std::is_base_of_v<Resource, T>, const Node &> {
		object = T::Create(node);
		return node;
	}*/
};
}
