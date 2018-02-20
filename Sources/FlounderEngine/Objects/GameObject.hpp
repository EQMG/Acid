#pragma once

#include <vector>
#include "../Prerequisites.hpp"
#include "Component.hpp"

namespace Flounder
{
	class F_EXPORT GameObject
	{
	private:
		std::vector<Component*> *m_components;
	public:
		GameObject();

		virtual ~GameObject();

		std::vector<Component *> *GetComponents() const { return m_components; }

		void AddComponent(Component *component);

		void RemoveComponent(Component *component);

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
