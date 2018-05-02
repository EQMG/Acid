#pragma once

#include <vector>
#include "Engine/Exports.hpp"
#include "Physics/Space/ISpatialStructure.hpp"
#include "Maths/Transform.hpp"
#include "Component.hpp"

namespace fl
{
	class FL_EXPORT GameObject
	{
	protected:
		std::string m_name;
		Transform *m_transform;
	private:
		std::vector<Component *> *m_components;
		ISpatialStructure<GameObject *> *m_structure;
		bool m_removed;
	public:
		GameObject(const Transform &transform, ISpatialStructure<GameObject *> *structure = nullptr, const std::string &name = "Unnamed");

		GameObject(const std::string &prefabName, const Transform &transform, ISpatialStructure<GameObject *> *structure = nullptr);

		virtual ~GameObject();

		virtual void Update();

		std::vector<Component *> *GetComponents() const { return m_components; }

		template<typename T>
		T *GetComponent()
		{
			for (auto c : *m_components)
			{
				T *casted = dynamic_cast<T *>(c);

				if (casted != nullptr)
				{
					return casted;
				}
			}

			return nullptr;
		}

		Component *AddComponent(Component *component);

		template<typename T, typename... Args>
		T *AddComponent(Args &&... args)
		{
			T *created = new T(std::forward<Args>(args)...);
			AddComponent(created);
			return created;
		}

		Component *RemoveComponent(Component *component);

		template<typename T>
		T *RemoveComponent()
		{
			for (auto c : *m_components)
			{
				T *casted = dynamic_cast<T *>(c);

				if (casted != nullptr)
				{
					RemoveComponent(c);
					return c;
				}
			}

			return nullptr;
		}

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		Transform *GetTransform() const { return m_transform; }

		void SetTransform(const Transform &transform) const { m_transform->Set(transform); }

		ISpatialStructure<GameObject *> *GetStructure() const { return m_structure; }

		bool GetRemoved() const { return m_removed; }

		void StructureSwitch(ISpatialStructure<GameObject *> *structure);

		void StructureRemove();
	};
}
