#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Helpers/TypeInfo.hpp"

namespace acid {
/**
 * @brief A interface used for defining engine modules.
 */
class ACID_EXPORT
Module
:
public
NonCopyable
{
public:
	/**
	 * @brief Represents when a module will call <seealso cref="Module#Update()"/> in the update loop.
	 */
	enum class Stage
	{
		Always, Pre, Normal, Post, Render
	};

	virtual ~Module() = default;

	/**
	 * The update function for the module.
	 */
	virtual void Update() = 0;
};

template class ACID_EXPORT
TypeInfo<Module>;

/**
 * Gets the Type ID for the Module.
 * @tparam T The Module type.
 * @return The Type ID.
 */
template<typename T>
TypeId GetModuleTypeId() noexcept {
	static_assert(std::is_base_of<Module, T>::value, "T must be a Module.");

	return TypeInfo<Module>::GetTypeId<T>();
}
}
