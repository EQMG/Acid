#pragma once

#include <vector>
#include "../Prerequisites.hpp"
#include "../Space/ISpatialStructure.hpp"
#include "../Maths/Transform.hpp"
#include "Component.hpp"

namespace Flounder
{
	class F_EXPORT GameObject
	{
	protected:
		std::string m_name;
		Transform *m_transform;
	private:
		std::vector<Component*> *m_components;
		ISpatialStructure<GameObject *> *m_structure;
		bool m_removed;
	public:
		GameObject(const Transform &transform, ISpatialStructure<GameObject *> *structure = nullptr);

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

		void AddComponent(Component *component);

		void RemoveComponent(Component *component);

		std::string GetName() const { return m_name; }

		void SetName(const std::string &name) { m_name = name; }

		Transform *GetTransform() const { return m_transform; }

		void SetTransform(const Transform &transform) const { m_transform->Set(transform); }

		ISpatialStructure<GameObject *> *GetStructure() const { return m_structure; }

		bool GetRemoved() const { return m_removed; }

		void StructureSwitch(ISpatialStructure<GameObject *> *structure);

		void StructureRemove();
	private:
		static Component *CreateComponent(const std::string &name, PrefabComponent *prefab);
	};
}
