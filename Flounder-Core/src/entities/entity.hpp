#pragma once

#include "../space/ispatialstructure.hpp"

#include "icomponent.hpp"

namespace flounder
{
	class entity :
		public ispatialobject
	{
	private:
		ispatialstructure<entity*> *m_structure;

		std::vector<icomponent*> *m_components;

		vector3 *m_position;
		vector3 *m_rotation;

		bool m_removed;
	public:
		entity(ispatialstructure<entity*> *structure, const vector3 &position, const vector3 &rotation);

		~entity();

		void update();

		ispatialstructure<entity*> *getStructure() const { return m_structure; }

		void moveStructure(ispatialstructure<entity*> *structure);

		std::vector<icomponent*> *getComponents() const { return m_components; }

		void addComponent(icomponent *component);

		void removeComponent(icomponent *component);

		template<class t>
		t getComponent();

		vector3 *getPosition() const { return m_position; }

		void setPosition(const vector3 &position) const { m_position->set(position); }

		vector3 *getRotation() const { return m_rotation; }

		void setRotation(const vector3 &rotation) const { m_rotation->set(rotation); }

		bool getRemoved() const { return m_removed; }

		void remove();

		icollider *getCollider() override;
	};
}
