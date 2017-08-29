#include "aabb.hpp"

namespace flounder
{
	aabb::aabb() :
		icollider()
	{
		m_minExtents = new vector3();
		m_maxExtents = new vector3();
	}

	aabb::aabb(vector3 *minExtents, vector3 *maxExtents) :
		icollider()
	{
		m_minExtents = minExtents;
		m_maxExtents = maxExtents;;
	}

	aabb::aabb(const aabb &source) :
		icollider()
	{
		m_minExtents = new vector3(*source.m_minExtents);
		m_maxExtents = new vector3(*source.m_maxExtents);
	}

	aabb::~aabb()
	{
		delete m_minExtents;
		delete m_maxExtents;
	}

	aabb *aabb::scale(const aabb &source, const vector3 &scale, aabb *destination)
	{
		if (destination == nullptr)
		{
			destination = new aabb();
		}

		destination->m_minExtents->set(source.m_minExtents->m_x * scale.m_x, source.m_minExtents->m_y * scale.m_y, source.m_minExtents->m_z * scale.m_z);
		destination->m_maxExtents->set(source.m_maxExtents->m_x * scale.m_x, source.m_maxExtents->m_y * scale.m_y, source.m_maxExtents->m_z * scale.m_z);

		return destination;
	}

	aabb *aabb::scale(const aabb &source, const float &scaleX, const float &scaleY, const float &scaleZ, aabb *destination)
	{
		return scale(source, vector3(scaleX, scaleY, scaleZ), destination);
	}

	aabb *aabb::expand(const aabb &source, const vector3 &expand, aabb *destination)
	{
		if (destination == nullptr)
		{
			destination = new aabb();
		}

		destination->m_minExtents->set(source.m_minExtents->m_x - expand.m_x, source.m_minExtents->m_y - expand.m_y, source.m_minExtents->m_z - expand.m_z);
		destination->m_maxExtents->set(source.m_maxExtents->m_x + expand.m_x, source.m_maxExtents->m_y + expand.m_y, source.m_maxExtents->m_z + expand.m_z);

		return destination;
	}

	aabb *aabb::expand(const aabb &source, const float &expandX, const float &expandY, const float &expandZ, aabb *destination)
	{
		return expand(source, vector3(expandX, expandY, expandZ), destination);
	}

	aabb *aabb::combine(const aabb &left, const aabb &right, aabb *destination)
	{
		if (destination == nullptr)
		{
			destination = new aabb();
		}

		float newMinX = __min(left.m_minExtents->m_x, right.m_minExtents->m_x);
		float newMinY = __min(left.m_minExtents->m_y, right.m_minExtents->m_y);
		float newMinZ = __min(left.m_minExtents->m_z, right.m_minExtents->m_z);
		float newMaxX = __max(left.m_maxExtents->m_x, right.m_maxExtents->m_x);
		float newMaxY = __max(left.m_maxExtents->m_y, right.m_maxExtents->m_y);
		float newMaxZ = __max(left.m_maxExtents->m_z, right.m_maxExtents->m_z);

		destination->m_minExtents->set(newMinX, newMinY, newMinZ);
		destination->m_maxExtents->set(newMaxX, newMaxY, newMaxZ);

		return destination;
	}

	aabb *aabb::stretch(const aabb &source, const vector3 &stretch, aabb *destination)
	{
		if (destination == nullptr)
		{
			destination = new aabb();
		}

		float newMinX, newMaxX, newMinY, newMaxY, newMinZ, newMaxZ;

		if (stretch.m_x < 0.0f)
		{
			newMinX = source.m_minExtents->m_x + stretch.m_x;
			newMaxX = source.m_maxExtents->m_x;
		}
		else
		{
			newMinX = source.m_minExtents->m_x;
			newMaxX = source.m_maxExtents->m_x + stretch.m_x;
		}

		if (stretch.m_y < 0.0f)
		{
			newMinY = source.m_minExtents->m_y + stretch.m_y;
			newMaxY = source.m_maxExtents->m_y;
		}
		else
		{
			newMinY = source.m_minExtents->m_y;
			newMaxY = source.m_maxExtents->m_y + stretch.m_y;
		}

		if (stretch.m_z < 0.0f)
		{
			newMinZ = source.m_minExtents->m_z + stretch.m_z;
			newMaxZ = source.m_maxExtents->m_z;
		}
		else
		{
			newMinZ = source.m_minExtents->m_z;
			newMaxZ = source.m_maxExtents->m_z + stretch.m_z;
		}

		destination->m_minExtents->set(newMinX, newMinY, newMinZ);
		destination->m_maxExtents->set(newMaxX, newMaxY, newMaxZ);

		return destination;
	}

	aabb *aabb::stretch(const aabb &source, const float &stretchX, const float &stretchY, const float &stretchZ, aabb *destination)
	{
		return stretch(source, vector3(stretchX, stretchY, stretchZ), destination);
	}

	icollider *aabb::update(const vector3 &position, const vector3 &rotation, const float &scale, icollider *destination)
	{
		if (destination == nullptr)
		{
			destination = new aabb();
		}

		aabb *aabb2 = dynamic_cast<aabb*>(destination);

		// Sets the destinations values to the sources.
		aabb2->m_minExtents->set(*m_minExtents);
		aabb2->m_maxExtents->set(*m_maxExtents);

		// Scales the dimensions for the aabb.
		if (scale != 1.0f)
		{
			aabb2->m_minExtents->set(aabb2->m_minExtents->m_x * scale, aabb2->m_minExtents->m_y * scale, aabb2->m_minExtents->m_z * scale);
			aabb2->m_maxExtents->set(aabb2->m_maxExtents->m_x * scale, aabb2->m_maxExtents->m_y * scale, aabb2->m_maxExtents->m_z * scale);
		}

		// Creates the 8 aabb corners and rotates them.
		if (!rotation.isZero())
		{
			vector3 *fll = new vector3(aabb2->m_minExtents->m_x, aabb2->m_minExtents->m_y, aabb2->m_minExtents->m_z);
			matrix4x4::rotate(*fll, rotation, fll);

			vector3 *flr = new vector3(aabb2->m_maxExtents->m_x, aabb2->m_minExtents->m_y, aabb2->m_minExtents->m_z);
			matrix4x4::rotate(*flr, rotation, flr);

			vector3 *ful = new vector3(aabb2->m_minExtents->m_x, aabb2->m_maxExtents->m_y, aabb2->m_minExtents->m_z);
			matrix4x4::rotate(*ful, rotation, ful);

			vector3 *fur = new vector3(aabb2->m_maxExtents->m_x, aabb2->m_maxExtents->m_y, aabb2->m_minExtents->m_z);
			matrix4x4::rotate(*fur, rotation, fur);

			vector3 *bur = new vector3(aabb2->m_maxExtents->m_x, aabb2->m_maxExtents->m_y, aabb2->m_maxExtents->m_z);
			matrix4x4::rotate(*bur, rotation, bur);

			vector3 *bul = new vector3(aabb2->m_minExtents->m_x, aabb2->m_maxExtents->m_y, aabb2->m_maxExtents->m_z);
			matrix4x4::rotate(*bul, rotation, bul);

			vector3 *blr = new vector3(aabb2->m_maxExtents->m_x, aabb2->m_minExtents->m_y, aabb2->m_maxExtents->m_z);
			matrix4x4::rotate(*blr, rotation, blr);

			vector3 *bll = new vector3(aabb2->m_minExtents->m_x, aabb2->m_minExtents->m_y, aabb2->m_maxExtents->m_z);
			matrix4x4::rotate(*bll, rotation, bll);

			//aabb2->m_minExtents = min(fll, min(flr, min(ful, min(fur, min(bur, min(bul, min(blr, bll)))))));
			vector3::minVector(*fll, *flr, aabb2->m_minExtents);
			vector3::minVector(*aabb2->m_minExtents, *ful, aabb2->m_minExtents);
			vector3::minVector(*aabb2->m_minExtents, *fur, aabb2->m_minExtents);
			vector3::minVector(*aabb2->m_minExtents, *bur, aabb2->m_minExtents);
			vector3::minVector(*aabb2->m_minExtents, *bul, aabb2->m_minExtents);
			vector3::minVector(*aabb2->m_minExtents, *blr, aabb2->m_minExtents);
			vector3::minVector(*aabb2->m_minExtents, *bll, aabb2->m_minExtents);

			//aabb2->m_maxExtents = max(fll, max(flr, max(ful, max(fur, max(bur, max(bul, max(blr, bll)))))));
			vector3::maxVector(*fll, *flr, aabb2->m_maxExtents);
			vector3::maxVector(*aabb2->m_maxExtents, *ful, aabb2->m_maxExtents);
			vector3::maxVector(*aabb2->m_maxExtents, *fur, aabb2->m_maxExtents);
			vector3::maxVector(*aabb2->m_maxExtents, *bur, aabb2->m_maxExtents);
			vector3::maxVector(*aabb2->m_maxExtents, *bul, aabb2->m_maxExtents);
			vector3::maxVector(*aabb2->m_maxExtents, *blr, aabb2->m_maxExtents);
			vector3::maxVector(*aabb2->m_maxExtents, *bll, aabb2->m_maxExtents);

			delete fll;
			delete flr;
			delete ful;
			delete fur;
			delete bur;
			delete bul;
			delete blr;
			delete bll;
		}

		// Transforms the aabb.
		vector3::add(*aabb2->m_minExtents, position, aabb2->m_minExtents);
		vector3::add(*aabb2->m_maxExtents, position, aabb2->m_maxExtents);

		// Returns the final aabb.
		return aabb2;
	}

	vector3 *aabb::resolveCollision(const icollider &other, const vector3 &positionDelta, vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector3();
		}

		const aabb &aabb2 = dynamic_cast<const aabb&>(other);

		if (positionDelta.m_x != 0.0f)
		{
			float newAmountX = positionDelta.m_x;

			if (positionDelta.m_x >= 0.0f)
			{
				// Our max == their min
				newAmountX = aabb2.m_minExtents->m_x - m_maxExtents->m_x;
			}
			else
			{
				// Our min == their max
				newAmountX = aabb2.m_maxExtents->m_x - m_minExtents->m_x;
			}

			if (fabs(newAmountX) < fabs(positionDelta.m_x))
			{
				destination->m_x = newAmountX;
			}
		}

		if (positionDelta.m_y != 0.0f)
		{
			float newAmountY = positionDelta.m_y;

			if (positionDelta.m_y >= 0.0f)
			{
				// Our max == their min
				newAmountY = aabb2.m_minExtents->m_y - m_maxExtents->m_y;
			}
			else
			{
				// Our min == their max
				newAmountY = aabb2.m_maxExtents->m_y - m_minExtents->m_y;
			}

			if (fabs(newAmountY) < fabs(positionDelta.m_y))
			{
				destination->m_y = newAmountY;
			}
		}

		if (positionDelta.m_z != 0.0f)
		{
			float newAmountZ = positionDelta.m_z;

			if (positionDelta.m_z >= 0.0f)
			{
				// Our max == their min
				newAmountZ = aabb2.m_minExtents->m_z - m_maxExtents->m_z;
			}
			else
			{
				// Our min == their max
				newAmountZ = aabb2.m_maxExtents->m_z - m_minExtents->m_z;
			}

			if (fabs(newAmountZ) < fabs(positionDelta.m_z))
			{
				destination->m_z = newAmountZ;
			}
		}

		return destination;
	}

	intersect *aabb::intersects(const icollider &other)
	{
		const aabb &aabb2 = dynamic_cast<const aabb&>(other);

		vector3 *distance1 = vector3::subtract(*m_minExtents, *aabb2.m_maxExtents, nullptr);
		vector3 *distance2 = vector3::subtract(*aabb2.m_minExtents, *m_maxExtents, nullptr);
		vector3 *maxDistance = vector3::maxVector(*distance1, *distance2, nullptr);
		float maxDist = vector3::maxComponent(*maxDistance);

		delete distance1;
		delete distance2;
		delete maxDistance;

		return new intersect(maxDist < 0.0f, maxDist);

		/*else if (dynamic_cast<Sphere*>(other) != 0)
		{
			Sphere *sphere = dynamic_cast<Sphere*>(other);

			float distanceSquared = sphere->getRadius() * sphere->getRadius();

			if (sphere->getPosition()->m_x < m_minExtents->m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_x - m_minExtents->m_x, 2);
			}
			else if (sphere->getPosition()->m_x > m_maxExtents->m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_x - m_maxExtents->m_x, 2);
			}

			if (sphere->getPosition()->m_y < m_minExtents->m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_y - m_minExtents->m_y, 2);
			}
			else if (sphere->getPosition()->m_x > m_maxExtents->m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_y - m_maxExtents->m_y, 2);
			}

			if (sphere->getPosition()->m_z < m_minExtents->m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_z - m_minExtents->m_z, 2);
			}
			else if (sphere->getPosition()->m_z > m_maxExtents->m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_z - m_maxExtents->m_z, 2);
			}

			return new intersect(distanceSquared > 0.0f, sqrt(distanceSquared));
		}*/
	}

	intersect *aabb::intersects(const ray &ray)
	{
		double tmin = (m_minExtents->m_x - ray.m_origin->m_x) / ray.m_currentRay->m_x;
		double tmax = (m_maxExtents->m_x - ray.m_origin->m_x) / ray.m_currentRay->m_x;

		if (tmin > tmax)
		{
			double temp = tmax;
			tmax = tmin;
			tmin = temp;
		}

		float tymin = (m_minExtents->m_y - ray.m_origin->m_y) / ray.m_currentRay->m_y;
		float tymax = (m_maxExtents->m_y - ray.m_origin->m_y) / ray.m_currentRay->m_y;

		if (tymin > tymax)
		{
			float temp = tymax;
			tymax = tymin;
			tymin = temp;
		}

		if ((tmin > tymax) || (tymin > tmax))
		{
			return new intersect(false, 0.0f);
		}

		if (tymin > tmin)
		{
			tmin = tymin;
		}

		if (tymax < tmax)
		{
			tmax = tymax;
		}

		float tzmin = (m_minExtents->m_z - ray.m_origin->m_z) / ray.m_currentRay->m_z;
		float tzmax = (m_maxExtents->m_z - ray.m_origin->m_z) / ray.m_currentRay->m_z;

		if (tzmin > tzmax)
		{
			float temp = tzmax;
			tzmax = tzmin;
			tzmin = temp;
		}

		if ((tmin > tzmax) || (tzmin > tmax))
		{
			return new intersect(false, 0.0f);
		}

		return new intersect(true, 0.0f);
	}

	bool aabb::inFrustum(const frustum &frustum)
	{
		return frustum.cubeInFrustum(m_minExtents->m_x, m_minExtents->m_y, m_minExtents->m_z, m_maxExtents->m_x, m_maxExtents->m_y, m_maxExtents->m_z);
	}

	bool aabb::contains(const icollider &other)
	{
		const aabb &aabb2 = dynamic_cast<const aabb&>(other);

		return m_minExtents->m_x <= aabb2.m_minExtents->m_x &&
			aabb2.m_maxExtents->m_x <= m_maxExtents->m_x &&
			m_minExtents->m_y <= aabb2.m_minExtents->m_y &&
			aabb2.m_maxExtents->m_y <= m_maxExtents->m_y &&
			m_minExtents->m_z <= aabb2.m_minExtents->m_z &&
			aabb2.m_maxExtents->m_z <= m_maxExtents->m_z;
	}

	bool aabb::contains(const vector3 &point)
	{
		if (point.m_x > m_maxExtents->m_x)
		{
			return false;
		}
		else if (point.m_x < m_minExtents->m_x)
		{
			return false;
		}
		else if (point.m_y > m_maxExtents->m_y)
		{
			return false;
		}
		else if (point.m_y < m_minExtents->m_y)
		{
			return false;
		}
		else if (point.m_z > m_maxExtents->m_z)
		{
			return false;
		}
		else if (point.m_z < m_minExtents->m_z)
		{
			return false;
		}

		return true;
	}

	float aabb::getCentreX()
	{
		return (m_minExtents->m_x + m_maxExtents->m_x) / 2.0f;
	}

	float aabb::getCentreY()
	{
		return (m_minExtents->m_y + m_maxExtents->m_y) / 2.0f;
	}

	float aabb::getCentreZ()
	{
		return (m_minExtents->m_z + m_maxExtents->m_z) / 2.0f;
	}

	float aabb::getWidth()
	{
		return m_maxExtents->m_x - m_minExtents->m_x;
	}

	float aabb::getHeight()
	{
		return m_maxExtents->m_y - m_minExtents->m_y;
	}

	float aabb::getDepth()
	{
		return m_maxExtents->m_z - m_minExtents->m_z;
	}
}
