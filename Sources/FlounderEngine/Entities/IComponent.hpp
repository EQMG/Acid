#pragma once

#include "EntityRender.hpp"
#include "Prefabs/EntityPrefab.hpp"

namespace Flounder
{
	class Entity;

	class IComponent
	{
	protected:
		Entity *m_entity;
	public:
		IComponent()
		{
			m_entity = nullptr;
		}

		virtual ~IComponent()
		{
		}

		virtual void Update() = 0;

		virtual void CmdRender(EntityRender *entityRender) = 0;

		virtual void Save(ComponentPrefab* prefab) = 0;

		virtual ICollider *GetCollider() { return nullptr; }

		Entity *GetEntity() const { return m_entity; }

		void SetEntity(Entity *entity) { m_entity = entity; }
	};
}
