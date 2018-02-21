#pragma once

#include <vector>
#include "../Prerequisites.hpp"
#include "../Maths/Transform.hpp"
#include "Component.hpp"

namespace Flounder
{
	class F_EXPORT GameObject
	{
	protected:
		Transform *m_transform;
	private:
		std::vector<Component*> *m_components;
	public:
		GameObject(const Transform &transform);

		virtual ~GameObject();

		virtual void Update();

		std::vector<Component *> *GetComponents() const { return m_components; }

		void AddComponent(Component *component);

		void RemoveComponent(Component *component);

		Transform *GetTransform() const { return m_transform; }

		void SetTransform(const Transform &transform) const { m_transform->Set(transform); }

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
	};
}
