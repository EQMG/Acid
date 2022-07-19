#pragma once

#include "Utils/NonCopyable.hpp"
#include "Component.hpp"

namespace acid {
/**
 * @brief Class that represents a objects that acts as a component container.
 */
class ACID_SCENE_EXPORT Entity final : NonCopyable {
public:
	void Update();

	const std::string &GetName() const { return name; }
	void SetName(const std::string &name) { this->name = name; }

	bool IsRemoved() const { return removed; }
	void SetRemoved(bool removed) { this->removed = removed; }

	/**
	 * Gets all components attached to this entity.
	 * @return The list of components.
	 */
	const std::vector<std::unique_ptr<Component>> &GetComponents() const { return components; }

	/**
	 * Gets the count of components attached to this entity.
	 * @return The count of components.
	 */
	uint32_t GetComponentCount() const { return static_cast<uint32_t>(components.size()); }

	/**
	 * Gets a component by type.
	 * @tparam T The component type to find.
	 * @param allowDisabled If disabled components will be returned.
	 * @return The found component.
	 */
	template<typename T>
	T *GetComponent(bool allowDisabled = false) const {
		T *alternative = nullptr;

		for (const auto &component : components) {
			auto casted = dynamic_cast<T *>(component.get());

			if (casted) {
				if (allowDisabled && !component->IsEnabled()) {
					alternative = casted;
					continue;
				}

				return casted;
			}
		}

		return alternative;
	}

	/**
	 * Gets components by type.
	 * @tparam T The component type to find.
	 * @param allowDisabled If disabled components will be returned.
	 * @return The components.
	 */
	template<typename T>
	std::vector<T *> GetComponents(bool allowDisabled = false) const {
		std::vector<T *> components;

		for (const auto &component : this->components) {
			auto casted = dynamic_cast<T *>(component.get());

			if (casted) {
				if (allowDisabled && !component->IsEnabled()) {
					components.emplace_back(casted);
					continue;
				}

				components.emplace_back(casted);
			}
		}

		return components;
	}

	/**
	 * Adds a component to this entity.
	 * @param component The component to add.
	 * @return The added component.
	 */
	Component *AddComponent(std::unique_ptr<Component> &&component);

	/**
	 * Creates a component by type to be added this entity.
	 * @tparam T The type of component to add.
	 * @tparam Args The argument types/
	 * @param args The type constructor arguments.
	 * @return The added component.
	 */
	template<typename T, typename... Args>
	T *AddComponent(Args &&... args) {
		return dynamic_cast<T *>(AddComponent(std::make_unique<T>(std::forward<Args>(args)...)));
	}

	/**
	 * Removes a component from this entity.
	 * @param component The component to remove.
	 */
	void RemoveComponent(Component *component);

	/**
	 * Removes a component from this entity.
	 * @param name The name of the component to remove.
	 */
	void RemoveComponent(const std::string &name);

	/**
	 * Removes a component by type from this entity.
	 * @tparam T The type of component to remove.
	 */
	template<typename T>
	void RemoveComponent() {
		for (auto it = components.begin(); it != components.end(); ++it) {
			auto casted = dynamic_cast<T *>((*it).get());

			if (casted) {
				(*it)->SetEntity(nullptr);
				components.erase(it);
			}
		}
	}

private:
	std::string name;
	bool removed = false;
	std::vector<std::unique_ptr<Component>> components;
};
}
