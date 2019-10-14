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

	/**
	 * Checks whether the Entity has the Component or not.
	 * @tparam T The Component type.
	 * @param id The Entity ID.
	 * @return If the Entity has the Component.
	 */
	template<typename T>
	bool HasComponent(Entity::Id id) const {
		// Is the Entity ID and the Component type ID known.
		if (id < m_components.size()) {
			auto typeId = GetComponentTypeId<T>();

			// Is the Component type ID known
			if (typeId < m_components[id].size()) {
				return m_components[id][typeId] != nullptr;
			}
		}

		return false;
	}

	/**
	 * Gets the Component from the Entity.
	 * @tparam T The Component type.
	 * @param id The Entity ID.
	 * @return The Component.
	 */
	template<typename T>
	T *GetComponent(Entity::Id id) const {
		if (!HasComponent<T>(id)) {
			//throw std::runtime_error("Entity does not have requested Component");
			return nullptr;
		}

		auto &component = m_components[id][GetComponentTypeId<T>()];

		if (!component.get()) {
			//throw std::runtime_error("Entity does not have requested Component");
			return nullptr;
		}

		return static_cast<T *>(component.get());
	}

	/**
	 * Adds the Component to the Entity.
	 * @tparam T The Component type.
	 * @param id The Entity ID.
	 * @param component The component.
	 */
	template<typename T>
	void AddComponent(Entity::Id id, std::unique_ptr<T> &&component) {
		if (id >= m_components.size()) {
			throw std::runtime_error("Entity ID is out of range");
		}

		const auto typeId = GetComponentTypeId<T>();

		if (typeId >= m_components[id].size()) {
			throw std::runtime_error("Component type ID is out of range");
		}

		m_components[id][typeId] = std::move(component);
		m_componentsMasks[id].set(typeId);
	}

	/**
	 * Removes the Component from the Entity.
	 * @tparam T The Component type.
	 * @param id The Entity ID.
	 */
	template<typename T>
	void RemoveComponent(Entity::Id id) {
		if (!HasComponent<T>(id)) {
			return;
		}

		auto &component = m_components[id][GetComponentTypeId<T>()];
		component.reset();
		m_componentsMasks[id].reset(GetComponentTypeId<T>());
	}

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
