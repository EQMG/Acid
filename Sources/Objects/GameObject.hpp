#pragma once

#include <memory>
#include <vector>
#include "Engine/Exports.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/ISpatialStructure.hpp"
#include "IComponent.hpp"

namespace acid
{
	/// <summary>
	/// A class that represents a structured game object.
	/// </summary>
	class ACID_EXPORT GameObject
	{
	private:
		std::string m_name;
		Transform m_transform;
		std::vector<std::shared_ptr<IComponent>> m_components;
		std::weak_ptr<GameObject> m_parent;
		bool m_removed;
	public:
		/// <summary>
		/// Will create a new Game Object and store it into a structure.
		/// </summary>
		/// <param name="transform"> The objects inital world position, rotation, and scale. </param>
		/// <param name="structure"> The structure to store the object into, if null it will be stored in the scenes structure. </param>
		static std::shared_ptr<GameObject> Resource(const Transform &transform, ISpatialStructure *structure = nullptr);

		/// <summary>
		/// Will create a new Game Object and store it into a structure.
		/// </summary>
		/// <param name="filepath"> The file to load the component data from. </param>
		/// <param name="transform"> The objects inital world position, rotation, and scale. </param>
		/// <param name="structure"> The structure to store the object into, if null it will be stored in the scenes structure. </param>
		static std::shared_ptr<GameObject> Resource(const std::string &filepath, const Transform &transform, ISpatialStructure *structure = nullptr);

		GameObject(const Transform &transform);

		GameObject(const std::string &filepath, const Transform &transform);

		virtual ~GameObject();

		virtual void Update();

		/// <summary>
		/// Gets all components attached to this game object.
		/// </summary>
		/// <returns> The list of components. </returns>
		std::vector<std::shared_ptr<IComponent>> GetComponents() const { return m_components; }

		/// <summary>
		/// Gets a component by type.
		/// </summary>
		/// <param name="T"> The component type to find. </param>
		/// <returns> The found component. </returns>
		template<typename T>
		std::shared_ptr<T> GetComponent(const bool &allowDisabled = false)
		{
			std::shared_ptr<T> alternative = nullptr;

			for (auto &component : m_components)
			{
				auto casted = std::dynamic_pointer_cast<T>(component);

				if (casted != nullptr)
				{
					if (!allowDisabled && !casted->IsEnabled())
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
		/// Adds a component to this game object.
		/// </summary>
		/// <param name="component"> The component to add. </param>
		/// <returns> The added component. </returns>
		std::shared_ptr<IComponent> AddComponent(const std::shared_ptr<IComponent> &component);

		/// <summary>
		/// Creates a component by type to be added this game object.
		/// </summary>
		/// <param name="T"> The type of component to add. </param>
		/// <param name="args"> The type constructor arguments. </param>
		/// <returns> The added component. </returns>
		template<typename T, typename... Args>
		std::shared_ptr<T> AddComponent(Args &&... args)
		{
			auto created = std::make_shared<T>(std::forward<Args>(args)...);
			AddComponent(created);
			return created;
		}

		/// <summary>
		/// Removes a component from this game object.
		/// </summary>
		/// <param name="component"> The component to remove. </param>
		/// <returns> If the component was removed. </returns>
		bool RemoveComponent(const std::shared_ptr<IComponent> &component);

		/// <summary>
		/// Removes a component from this game object.
		/// </summary>
		/// <param name="name"> The name of the component to remove. </param>
		/// <returns> If the component was removed. </returns>
		bool RemoveComponent(const std::string &name);

		/// <summary>
		/// Removes a component by type from this game object.
		/// </summary>
		/// <param name="T"> The type of component to remove. </param>
		/// <returns> If the component was removed. </returns>
		template<typename T>
		bool RemoveComponent()
		{
			for (auto &component : m_components)
			{
				auto casted = std::dynamic_pointer_cast<T>(component);

				if (casted != nullptr)
				{
					RemoveComponent(component);
					return true;
				}
			}

			return false;
		}

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		Transform &GetTransform() { return m_transform; }

		std::weak_ptr<GameObject> GetParent() const { return m_parent; }

		void SetParent(const std::shared_ptr<GameObject> &parent) { m_parent = parent; }

		bool IsRemoved() const { return m_removed; }

		void SetRemoved(const bool &removed) { m_removed = removed; }
	};
}
