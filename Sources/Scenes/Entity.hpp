#pragma once

#include <memory>
#include <vector>
#include "Helpers/NonCopyable.hpp"
#include "Maths/Transform.hpp"
#include "Component.hpp"

namespace acid
{
	/// <summary>
	/// A class that represents a objects that acts as a component container.
	/// </summary>
	class ACID_EXPORT Entity :
		public NonCopyable
	{
	public:
		/// <summary>
		/// Creates a new entity and stores it into a structure.
		/// </summary>
		/// <param name="transform"> The objects initial world position, rotation, and scale. </param>
		explicit Entity(const Transform &transform);

		/// <summary>
		/// Creates a new entity and stores it into a structure.
		/// </summary>
		/// <param name="filename"> The file to load the component data from. </param>
		/// <param name="transform"> The objects initial world position, rotation, and scale. </param>
		explicit Entity(const std::string &filename, const Transform &transform = Transform::Identity);

		~Entity();

		void Update();

		/// <summary>
		/// Gets all components attached to this entity.
		/// </summary>
		/// <returns> The list of components. </returns>
		const std::vector<std::unique_ptr<Component>> &GetComponents() const { return m_components; }

		/// <summary>
		/// Gets the count of components attached to this entity.
		/// </summary>
		/// <returns> The count of components. </returns>
		uint32_t GetComponentCount() const { return static_cast<uint32_t>(m_components.size()); }

		/// <summary>
		/// Gets a component by type.
		/// </summary>
		/// <param name="T"> The component type to find. </param>
		/// <param name="allowDisabled"> If disabled components will be returned. </param>
		/// <returns> The found component. </returns>
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

		/// <summary>
		/// Gets components by type.
		/// </summary>
		/// <param name="T"> The component type to find. </param>
		/// <param name="allowDisabled"> If disabled components will be returned. </param>
		/// <returns> The components. </returns>
		template<typename T>
		std::vector<T *> GetComponents(const bool &allowDisabled = false) const
		{
			std::vector<T *> result = {};

			for (const auto &component : m_components)
			{
				auto casted = dynamic_cast<T *>(component.get());

				if (casted != nullptr)
				{
					if (allowDisabled && !casted->IsEnabled())
					{
						result.emplace_back(casted);
						continue;
					}

					result.emplace_back(casted);
				}
			}

			return result;
		}

		/// <summary>
		/// Adds a component to this entity.
		/// </summary>
		/// <param name="component"> The component to add. </param>
		/// <returns> The added component. </returns>
		Component *AddComponent(Component *component);

		/// <summary>
		/// Creates a component by type to be added this entity.
		/// </summary>
		/// <param name="T"> The type of component to add. </param>
		/// <param name="args"> The type constructor arguments. </param>
		/// <returns> The added component. </returns>
		template<typename T, typename... Args>
		T *AddComponent(Args &&... args)
		{
			auto created = new T(std::forward<Args>(args)...);
			AddComponent(created);
			return created;
		}

		/// <summary>
		/// Removes a component from this entity.
		/// </summary>
		/// <param name="component"> The component to remove. </param>
		/// <returns> If the component was removed. </returns>
		void RemoveComponent(Component *component);

		/// <summary>
		/// Removes a component from this entity.
		/// </summary>
		/// <param name="name"> The name of the component to remove. </param>
		/// <returns> If the component was removed. </returns>
		void RemoveComponent(const std::string &name);

		/// <summary>
		/// Removes a component by type from this entity.
		/// </summary>
		/// <param name="T"> The type of component to remove. </param>
		/// <returns> If the component was removed. </returns>
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
