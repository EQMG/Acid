#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Helpers/TypeInfo.hpp"
#include "Scenes/System.hpp"

namespace acid {
class ACID_EXPORT SystemHolder : public NonCopyable {
public:
	SystemHolder() = default;

	~SystemHolder();

	/**
	 * Checks whether a System exists or not.
	 * @tparam T The System type.
	 * @return If the System exists.
	 */
	template<typename T>
	bool HasSystem() const;

	/**
	 * Gets a System.
	 * @tparam T The System type.
	 * @return The System.
	 */
	template<typename T>
	T *GetSystem() const;

	/**
	 * Adds a System.
	 * @tparam T The System type.
	 * @param priority The System priority.
	 * @param system The System.
	 */
	template<typename T>
	void AddSystem(std::size_t priority, std::unique_ptr<T> &&system);

	/**
	 * Removes a System.
	 * @tparam T The System type.
	 */
	template<typename T>
	void RemoveSystem();

	/**
	 * Removes all Systems.
	 */
	void RemoveAllSystems();

	/**
	 * Iterates through all valid Systems.
	 * @tparam Func The function type.
	 * @param func The function to pass each System into, System object and System ID.
	 */
	template<typename Func>
	void ForEach(Func &&func);

private:
	// Remove System from the priority list.
	void RemoveSystemPriority(TypeId id);

	// List of all Systems.
	std::unordered_map<TypeId, std::unique_ptr<System>> m_systems;

	// List of systems priorities.
	std::multimap<std::size_t, TypeId, std::greater<>> m_priorities;
};
}

#include "SystemHolder.inl"
