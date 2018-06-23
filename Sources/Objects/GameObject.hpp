#pragma once

#include <vector>
#include <memory>
#include "Engine/Exports.hpp"
#include "Scenes/ISpatialStructure.hpp"
#include "Maths/Transform.hpp"
#include "IComponent.hpp"

namespace fl
{
	/// <summary>
	/// A class that represents a structured game object.
	/// </summary>
	class FL_EXPORT GameObject :
		public std::enable_shared_from_this<GameObject>
	{
	private:
		std::string m_name;
		Transform m_transform;
		std::vector<std::shared_ptr<IComponent>> m_components;
		std::shared_ptr<ISpatialStructure> m_structure;
		GameObject *m_parent;
		bool m_removed;
	public:
		GameObject(const Transform &transform, std::shared_ptr<ISpatialStructure> structure = nullptr);

		GameObject(const std::string &filepath, const Transform &transform, std::shared_ptr<ISpatialStructure> structure = nullptr);

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
		std::shared_ptr<IComponent> AddComponent(std::shared_ptr<IComponent> component);

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
		/// <returns> The removed component. </returns>
		std::shared_ptr<IComponent> RemoveComponent(std::shared_ptr<IComponent> component);

		/// <summary>
		/// Removes a component by type from this game object.
		/// </summary>
		/// <param name="T"> The type of component to remove. </param>
		/// <returns> The removed component. </returns>
		template<typename T>
		std::shared_ptr<T> RemoveComponent()
		{
			for (auto &component : m_components)
			{
				auto casted = std::dynamic_pointer_cast<T>(component);

				if (casted != nullptr)
				{
					RemoveComponent(component);
					return casted;
				}
			}

			return nullptr;
		}

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		Transform &GetTransform() { return m_transform; }

		void SetTransform(const Transform &transform) { m_transform = transform; }

		std::shared_ptr<ISpatialStructure> GetStructure() const { return m_structure; }

		void SetStructure(std::shared_ptr<ISpatialStructure> structure);

		bool IsRemoved() const { return m_removed; }

		GameObject *GetParent() const { return m_parent; }

		void SetParent(GameObject *parent) { m_parent = parent; }

		void StructureRemove();
	};
}
