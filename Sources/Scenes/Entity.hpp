#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Maths/Transform.hpp"
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
	 * Creates a new entity and stores it into a structure.
	 * @param transform The objects initial world position, rotation, and scale.
	 */
	explicit Entity(const Transform &transform = Transform::Zero);

	/**
	 * Creates a new entity and stores it into a structure.
	 * @param filename The file to load the component data from.
	 * @param transform The objects initial world position, rotation, and scale.
	 */
	explicit Entity(const std::string &filename, const Transform &transform = Transform::Zero);

	~Entity();

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
	T *GetComponent(const bool &allowDisabled = false) const
	{
		T *alternative = nullptr;

		for (const auto &component : m_components)
		{
			auto casted = dynamic_cast<T *>(component.get());

			if (casted != nullptr)
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
	std::vector<T *> GetComponents(const bool &allowDisabled = false) const
	{
		std::vector<T *> components;

		for (const auto &component : m_components)
		{
			auto casted = dynamic_cast<T *>(component.get());

			if (casted != nullptr)
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

			if (casted != nullptr)
			{
				(*it)->SetParent(nullptr);
				m_components.erase(it);
			}
		}
	}

	const std::string GetName() const { return m_name; }

	void SetName(const std::string &name) { m_name = name; }

	Transform &GetLocalTransform() { return m_localTransform; }

	void SetLocalTransform(const Transform &localTransform) { m_localTransform = localTransform; }

	Transform GetWorldTransform() const;

	Matrix4 GetWorldMatrix() const;

	const bool &IsRemoved() const { return m_removed; }

	void SetRemoved(const bool &removed) { m_removed = removed; }

	Entity *GetParent() const { return m_parent; }

	void SetParent(Entity *parent);

	const std::vector<Entity *> &GetChildren() const { return m_children; }

	void AddChild(Entity *child);

	void RemoveChild(Entity *child);

private:
	std::string m_name;
	Transform m_localTransform;
	mutable Transform m_worldTransform;
	std::vector<std::unique_ptr<Component>> m_components;
	Entity *m_parent;
	std::vector<Entity *> m_children;
	bool m_removed;
};
}
