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
		std::vector<std::unique_ptr<IComponent>> m_components;
		ISpatialStructure *m_structure;
		GameObject *m_parent;
	public:
		/// <summary>
		/// Creates a new Game Object and store it into a structure.
		/// </summary>
		/// <param name="transform"> The objects initial world position, rotation, and scale. </param>
		/// <param name="structure"> The structure to store the object into, if null it will be stored in the scenes structure. </param>
		GameObject(const Transform &transform, ISpatialStructure *structure = nullptr);

		/// <summary>
		/// Creates a new Game Object and store it into a structure.
		/// </summary>
		/// <param name="filepath"> The file to load the component data from. </param>
		/// <param name="transform"> The objects initial world position, rotation, and scale. </param>
		/// <param name="structure"> The structure to store the object into, if null it will be stored in the scenes structure. </param>
		GameObject(const std::string &filepath, const Transform &transform, ISpatialStructure *structure = nullptr);

		~GameObject();

		GameObject(const GameObject&) = delete; // FIXME: Temp Fix.

		GameObject& operator=(const GameObject&) = delete;

		void Update();

		/// <summary>
		/// Gets all components attached to this game object.
		/// </summary>
		/// <returns> The list of components. </returns>
		const std::vector<std::unique_ptr<IComponent>> &GetComponents() const { return m_components; }

		/// <summary>
		/// Gets the count of components attached to this Game Object.
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
		T *GetComponent(const bool &allowDisabled = false)
		{
			T *alternative = nullptr;

			for (auto &component : m_components)
			{
				auto casted = dynamic_cast<T *>(component.get());

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
		IComponent *AddComponent(IComponent *component);

		/// <summary>
		/// Creates a component by type to be added this game object.
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
		/// Removes a component from this game object.
		/// </summary>
		/// <param name="component"> The component to remove. </param>
		/// <returns> If the component was removed. </returns>
		bool RemoveComponent(IComponent *component);

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
				auto casted = dynamic_cast<T *>(component.get());

				if (casted != nullptr)
				{
					RemoveComponent(component.get());
					return true;
				}
			}

			return false;
		}

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		Transform &GetTransform() { return m_transform; }

		ISpatialStructure *GetStructure() const { return m_structure; }

		void SetStructure(ISpatialStructure *structure);

		void StructureRemove();

		GameObject *GetParent() const { return m_parent; }

		void SetParent(GameObject *parent) { m_parent = parent; }
	};
}
