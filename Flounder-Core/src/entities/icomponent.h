#pragma once

#include "entityrender.h"

namespace flounder
{
	class entity;

	class icomponent
	{
	protected:
		entity *m_entity;
	public:
		icomponent()
		{
			m_entity = NULL;
		}

		virtual ~icomponent()
		{
		}

		virtual void update() = 0;

		virtual void render(entityrender *render) = 0;

		entity *getEntity() const { return m_entity; }

		void setEntity(entity *entity) { m_entity = entity; }
	};
}
