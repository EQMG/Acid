#pragma once

#include "../space/ISpatialStructure.hpp"
#include "IComponent.hpp"

namespace Flounder
{
	class Entity :
		public ISpatialObject
	{
	private:
		ISpatialStructure<Entity*> *m_structure;

		std::vector<IComponent*> *m_components;

		Vector3 *m_position;
		Vector3 *m_rotation;

		bool m_removed;
	public:
		Entity(ISpatialStructure<Entity*> *structure, const Vector3 &position, const Vector3 &rotation);

		~Entity();

		void Update();

		ISpatialStructure<Entity*> *GetStructure() const { return m_structure; }

		void MoveStructure(ISpatialStructure<Entity*> *structure);

		std::vector<IComponent*> *GetComponents() const { return m_components; }

		void AddComponent(IComponent *component);

		void RemoveComponent(IComponent *component);

		template<class T>
		T GetComponent();

		Vector3 *GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) const { m_position->Set(position); }

		Vector3 *GetRotation() const { return m_rotation; }

		void SetRotation(const Vector3 &rotation) const { m_rotation->Set(rotation); }

		bool GetRemoved() const { return m_removed; }

		void Remove();

		ICollider *GetCollider() override;
	};
}
