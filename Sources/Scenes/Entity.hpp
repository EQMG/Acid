#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Component.hpp"

namespace acid
{
/**
 * @brief Class that represents a objects that acts as a component container.
 */
class ACID_EXPORT Entity :
	public NonCopyable
{
public:
	/**
	 * Creates a new entity.
	 */
	Entity() = default;

	/**
	 * Creates a new entity from a entity prefab file.
	 * @param filename The file to load components from.
	 */
	Entity(const std::filesystem::path &filename);

	void Update();

	/**
	 * Gets all components attached to this entity.
	 * @return The list of components.
	 */
	const std::vector<std::unique_ptr<Component>> &GetComponents() const { return m_components; }

	/**
	 * Gets the count of components attached to this entity.
	 * @return The count of components.
	 */
	uint32_t GetComponentCount() const { return static_cast<uint32_t>(m_components.size()); }

	/**
	 * Gets a component by type.
	 * @tparam T The component type to find.
	 * @param allowDisabled If disabled components will be returned.
	 * @return The found component.
	 */
	template<typename T>
	T *GetComponent(bool allowDisabled = false) const
	{
		T *alternative = nullptr;

		for (const auto &component : m_components)
		{
			auto casted = dynamic_cast<T *>(component.get());

			if (casted)
			{
				if (allowDisabled && !casted->IsEnabled())
				{
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
	std::vector<T *> GetComponents(bool allowDisabled = false) const
	{
		std::vector<T *> components;

		for (const auto &component : m_components)
		{
			auto casted = dynamic_cast<T *>(component.get());

			if (casted)
			{
				if (allowDisabled && !casted->IsEnabled())
				{
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
	Component *AddComponent(Component *component);

	/**
	 * Creates a component by type to be added this entity.
	 * @tparam T The type of component to add.
	 * @tparam Args The argument types/
	 * @param args The type constructor arguments.
	 * @return The added component.
	 */
	template<typename T, typename... Args>
	T *AddComponent(Args &&... args)
	{
		auto created = new T(std::forward<Args>(args)...);
		AddComponent(created);
		return created;
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
	void RemoveComponent()
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			auto casted = dynamic_cast<T *>((*it).get());

			if (casted)
			{
				(*it)->SetEntity(nullptr);
				m_components.erase(it);
			}
		}
	}

	const std::string &GetName() const { return m_name; }

	void SetName(const std::string &name) { m_name = name; }

	bool IsRemoved() const { return m_removed; }

	void SetRemoved(bool removed) { m_removed = removed; }

private:
	std::string m_name;
	std::vector<std::unique_ptr<Component>> m_components;
	bool m_removed = false;
};
}
