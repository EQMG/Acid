#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Scenes/Component.hpp"
#include "ComponentFilter.hpp"
#include "Scenes/Entity.hpp"

namespace acid {
class ACID_EXPORT ComponentHolder : public NonCopyable {
public:
	ComponentHolder() = default;

	~ComponentHolder() = default;

	std::vector<Component *> GetComponents(Entity::Id id) const;

	/**
	 * Checks whether the Entity has the Component or not.
	 * @tparam T The Component type.
	 * @param id The Entity ID.
	 * @return If the Entity has the Component.
	 */
	template<typename T>
	bool HasComponent(Entity::Id id) const;

	/**
	 * Gets the Component from the Entity.
	 * @tparam T The Component type.
	 * @param id The Entity ID.
	 * @return The Component.
	 */
	template<typename T>
	T *GetComponent(Entity::Id id) const;

	/**
	 * Adds the Component to the Entity.
	 * @tparam T The Component type.
	 * @param id The Entity ID.
	 * @param component The component.
	 */
	template<typename T>
	void AddComponent(Entity::Id id, std::unique_ptr<T> &&component);

	/**
	 * Adds the Component to the Entity.
	 * @tparam T The Component type.
	 * @param id The Entity ID.
	 * @param typeId The components type id.
	 * @param component The component.
	 */
	void AddComponent(Entity::Id id, TypeId typeId, std::unique_ptr<Component> &&component);
	
	/**
	 * Removes the Component from the Entity.
	 * @tparam T The Component type.
	 * @param id The Entity ID.
	 */
	template<typename T>
	void RemoveComponent(Entity::Id id);

	/**
	 * Removes all Components from the Entity.
	 * @param id The Entity ID.
	 */
	void RemoveAllComponents(Entity::Id id);

	/**
	 * Gets the Component mask for the given Entity.
	 * @param id The Entity ID.
	 * @return The Component mask.
	 */
	ComponentFilter::Mask GetComponentsMask(Entity::Id id) const;

	/**
	 * Resizes the Component array.
	 * @param size The new size.
	 */
	void Resize(std::size_t size);

	/**
	 * Clear all Components.
	 */
	void Clear() noexcept;

private:
	// The index of this array matches the Component type ID.
	using ComponentArray = std::array<std::unique_ptr<Component>, MAX_COMPONENTS>;

	// List of all Components of all Entities.
	// The index of this array matches the Entity ID.
	std::vector<ComponentArray> m_components;

	// List of all masks of all Composents of all Entities.
	// The index of this array matches the Entity ID.
	std::vector<ComponentFilter::Mask> m_componentsMasks;
};
}

#include "ComponentHolder.inl"
