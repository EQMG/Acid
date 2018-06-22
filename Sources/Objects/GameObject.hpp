#pragma once

#include <vector>
#include <memory>
#include "Engine/Exports.hpp"
#include "Scenes/ISpatialStructure.hpp"
#include "Maths/Transform.hpp"
#include "IComponent.hpp"

namespace fl
{
	class FL_EXPORT GameObject :
		public std::enable_shared_from_this<GameObject>
	{
	private:
		std::string m_name;
		Transform *m_transform;
		std::vector<std::shared_ptr<IComponent>> m_components;
		ISpatialStructure *m_structure;
		GameObject* m_parent;
		bool m_removed;
	public:
		GameObject(const Transform &transform, ISpatialStructure *structure = nullptr);

		GameObject(const std::string &filepath, const Transform &transform, ISpatialStructure *structure = nullptr);

		virtual ~GameObject();

		virtual void Update();

		std::vector<std::shared_ptr<IComponent>> GetComponents() const { return m_components; }

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

		std::shared_ptr<IComponent> AddComponent(std::shared_ptr<IComponent> component);

		template<typename T, typename... Args>
		std::shared_ptr<T> AddComponent(Args &&... args)
		{
			auto created = std::make_shared<T>(std::forward<Args>(args)...);
			AddComponent(created);
			return created;
		}

		std::shared_ptr<IComponent> RemoveComponent(std::shared_ptr<IComponent> component);

		template<typename T>
		std::shared_ptr<T> RemoveComponent()
		{
			for (auto &component : m_components)
			{
				auto casted = std::dynamic_pointer_cast<T>(component);

				if (casted != nullptr)
				{
					RemoveComponent(component);
					return component;
				}
			}

			return nullptr;
		}

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		Transform *GetTransform() const { return m_transform; }

		void SetTransform(const Transform &transform) const { *m_transform = transform; }

		ISpatialStructure *GetStructure() const { return m_structure; }

		bool IsRemoved() const { return m_removed; }

		GameObject *GetParent() const { return m_parent; }

		void SetParent(GameObject *parent) { m_parent = parent; }

		void StructureSwitch(ISpatialStructure *structure);

		void StructureRemove();
	};
}
