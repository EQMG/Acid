#include "aabb.h"

namespace flounder
{
	aabb::aabb()
	{
		m_minExtents = new vector3();
		m_maxExtents = new vector3();
	}

	aabb::aabb(vector3 *minExtents, vector3 *maxExtents)
	{
		m_minExtents = minExtents;
		m_maxExtents = maxExtents;;
	}

	aabb::aabb(const aabb &source)
	{
		m_minExtents = new vector3(*source.m_minExtents);
		m_maxExtents = new vector3(*source.m_maxExtents);
	}

	aabb::~aabb()
	{
		delete m_minExtents;
		delete m_maxExtents;
	}

	collider *aabb::update(const vector3 &position, const vector3 &rotation, const float &scale, collider *destination)
	{
		return nullptr;
	}

	vector3 *aabb::resolveCollision(const collider &other, const vector3 &positionDelta, vector3 *destination)
	{
		return nullptr;
	}

	intersect *aabb::intersects(const collider &other)
	{
		return nullptr;
	}

	intersect *aabb::intersects(const ray &ray)
	{
		return nullptr;
	}

	bool aabb::inFrustum(const frustum &frustum)
	{
		return false;
	}

	bool aabb::contains(const collider &other)
	{
		return false;
	}

	bool aabb::contains(const vector3 &point)
	{
		return false;
	}
}
