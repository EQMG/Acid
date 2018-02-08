#include "Aabb.hpp"

#include "../Maths/Maths.hpp"

namespace Flounder
{
	Aabb::Aabb() :
		ICollider(),
		m_minExtents(new Vector3()),
		m_maxExtents(new Vector3())
	{
	}

	Aabb::Aabb(const Vector3 &minExtents, const Vector3 &maxExtents) :
		ICollider(),
		m_minExtents(new Vector3(minExtents)),
		m_maxExtents(new Vector3(maxExtents))
	{
	}

	Aabb::Aabb(const Aabb &source) :
		ICollider(),
		m_minExtents(new Vector3(*source.m_minExtents)),
		m_maxExtents(new Vector3(*source.m_maxExtents))
	{
	}

	Aabb::~Aabb()
	{
		delete m_minExtents;
		delete m_maxExtents;
	}

	Aabb *Aabb::Scale(const Aabb &source, const Vector3 &scale, Aabb *destination)
	{
		if (destination == nullptr)
		{
			destination = new Aabb();
		}

		destination->m_minExtents->Set(
			source.m_minExtents->m_x * scale.m_x,
			source.m_minExtents->m_y * scale.m_y, source.m_minExtents->m_z * scale.m_z);
		destination->m_maxExtents->Set(
			source.m_maxExtents->m_x * scale.m_x,
			source.m_maxExtents->m_y * scale.m_y, source.m_maxExtents->m_z * scale.m_z);

		return destination;
	}

	Aabb *Aabb::Scale(const Aabb &source, const float &scaleX, const float &scaleY, const float &scaleZ, Aabb *destination)
	{
		return Scale(source, Vector3(scaleX, scaleY, scaleZ), destination);
	}

	Aabb *Aabb::Expand(const Aabb &source, const Vector3 &expand, Aabb *destination)
	{
		if (destination == nullptr)
		{
			destination = new Aabb();
		}

		destination->m_minExtents->Set(
			source.m_minExtents->m_x - expand.m_x,
			source.m_minExtents->m_y - expand.m_y, source.m_minExtents->m_z - expand.m_z);
		destination->m_maxExtents->Set(
			source.m_maxExtents->m_x + expand.m_x,
			source.m_maxExtents->m_y + expand.m_y, source.m_maxExtents->m_z + expand.m_z);

		return destination;
	}

	Aabb *Aabb::Expand(const Aabb &source, const float &expandX, const float &expandY, const float &expandZ, Aabb *destination)
	{
		return Expand(source, Vector3(expandX, expandY, expandZ), destination);
	}

	Aabb *Aabb::Combine(const Aabb &left, const Aabb &right, Aabb *destination)
	{
		if (destination == nullptr)
		{
			destination = new Aabb();
		}

		float newMinX = Maths::Min(left.m_minExtents->m_x, right.m_minExtents->m_x);
		float newMinY = Maths::Min(left.m_minExtents->m_y, right.m_minExtents->m_y);
		float newMinZ = Maths::Min(left.m_minExtents->m_z, right.m_minExtents->m_z);
		float newMaxX = Maths::Max(left.m_maxExtents->m_x, right.m_maxExtents->m_x);
		float newMaxY = Maths::Max(left.m_maxExtents->m_y, right.m_maxExtents->m_y);
		float newMaxZ = Maths::Max(left.m_maxExtents->m_z, right.m_maxExtents->m_z);

		destination->m_minExtents->Set(newMinX, newMinY, newMinZ);
		destination->m_maxExtents->Set(newMaxX, newMaxY, newMaxZ);

		return destination;
	}

	Aabb *Aabb::Stretch(const Aabb &source, const Vector3 &stretch, Aabb *destination)
	{
		if (destination == nullptr)
		{
			destination = new Aabb();
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

		destination->m_minExtents->Set(newMinX, newMinY, newMinZ);
		destination->m_maxExtents->Set(newMaxX, newMaxY, newMaxZ);

		return destination;
	}

	Aabb *Aabb::Stretch(const Aabb &source, const float &stretchX, const float &stretchY, const float &stretchZ, Aabb *destination)
	{
		return Stretch(source, Vector3(stretchX, stretchY, stretchZ), destination);
	}

	ICollider *Aabb::Update(const Transform &transform, ICollider *destination)
	{
		if (destination == nullptr)
		{
			destination = new Aabb();
		}

		Aabb *aabb2 = dynamic_cast<Aabb *>(destination);

		// Sets the destinations values to the sources.
		aabb2->m_minExtents->Set(*m_minExtents);
		aabb2->m_maxExtents->Set(*m_maxExtents);

		// Scales the dimensions for the aabb.
		if (*transform.m_scaling != 1.0f)
		{
			aabb2->m_minExtents->Set(aabb2->m_minExtents->m_x * transform.m_scaling->m_x, aabb2->m_minExtents->m_y * transform.m_scaling->m_y, aabb2->m_minExtents->m_z * transform.m_scaling->m_z);
			aabb2->m_maxExtents->Set(aabb2->m_maxExtents->m_x * transform.m_scaling->m_x, aabb2->m_maxExtents->m_y * transform.m_scaling->m_y, aabb2->m_maxExtents->m_z * transform.m_scaling->m_z);
		}

		// Creates the 8 aabb corners and rotates them.
		if (*transform.m_rotation != 0.0f)
		{
			Vector3 fll = Vector3(aabb2->m_minExtents->m_x, aabb2->m_minExtents->m_y, aabb2->m_minExtents->m_z);
			Vector3::Rotate(fll, *transform.m_rotation, &fll);

			Vector3 flr = Vector3(aabb2->m_maxExtents->m_x, aabb2->m_minExtents->m_y, aabb2->m_minExtents->m_z);
			Vector3::Rotate(flr, *transform.m_rotation, &flr);

			Vector3 ful = Vector3(aabb2->m_minExtents->m_x, aabb2->m_maxExtents->m_y, aabb2->m_minExtents->m_z);
			Vector3::Rotate(ful, *transform.m_rotation, &ful);

			Vector3 fur = Vector3(aabb2->m_maxExtents->m_x, aabb2->m_maxExtents->m_y, aabb2->m_minExtents->m_z);
			Vector3::Rotate(fur, *transform.m_rotation, &fur);

			Vector3 bur = Vector3(aabb2->m_maxExtents->m_x, aabb2->m_maxExtents->m_y, aabb2->m_maxExtents->m_z);
			Vector3::Rotate(bur, *transform.m_rotation, &bur);

			Vector3 bul = Vector3(aabb2->m_minExtents->m_x, aabb2->m_maxExtents->m_y, aabb2->m_maxExtents->m_z);
			Vector3::Rotate(bul, *transform.m_rotation, &bul);

			Vector3 blr = Vector3(aabb2->m_maxExtents->m_x, aabb2->m_minExtents->m_y, aabb2->m_maxExtents->m_z);
			Vector3::Rotate(blr, *transform.m_rotation, &blr);

			Vector3 bll = Vector3(aabb2->m_minExtents->m_x, aabb2->m_minExtents->m_y, aabb2->m_maxExtents->m_z);
			Vector3::Rotate(bll, *transform.m_rotation, &bll);

			//aabb2->m_minExtents = min(fll, min(flr, min(ful, min(fur, min(bur, min(bul, min(blr, bll)))))));
			Vector3::MinVector(fll, flr, aabb2->m_minExtents);
			Vector3::MinVector(*aabb2->m_minExtents, ful, aabb2->m_minExtents);
			Vector3::MinVector(*aabb2->m_minExtents, fur, aabb2->m_minExtents);
			Vector3::MinVector(*aabb2->m_minExtents, bur, aabb2->m_minExtents);
			Vector3::MinVector(*aabb2->m_minExtents, bul, aabb2->m_minExtents);
			Vector3::MinVector(*aabb2->m_minExtents, blr, aabb2->m_minExtents);
			Vector3::MinVector(*aabb2->m_minExtents, bll, aabb2->m_minExtents);

			//aabb2->m_maxExtents = max(fll, max(flr, max(ful, max(fur, max(bur, max(bul, max(blr, bll)))))));
			Vector3::MaxVector(fll, flr, aabb2->m_maxExtents);
			Vector3::MaxVector(*aabb2->m_maxExtents, ful, aabb2->m_maxExtents);
			Vector3::MaxVector(*aabb2->m_maxExtents, fur, aabb2->m_maxExtents);
			Vector3::MaxVector(*aabb2->m_maxExtents, bur, aabb2->m_maxExtents);
			Vector3::MaxVector(*aabb2->m_maxExtents, bul, aabb2->m_maxExtents);
			Vector3::MaxVector(*aabb2->m_maxExtents, blr, aabb2->m_maxExtents);
			Vector3::MaxVector(*aabb2->m_maxExtents, bll, aabb2->m_maxExtents);
		}

		// Transforms the aabb.
		if (*transform.m_position != 0.0f)
		{
			Vector3::Add(*aabb2->m_minExtents, *transform.m_position, aabb2->m_minExtents);
			Vector3::Add(*aabb2->m_maxExtents, *transform.m_position, aabb2->m_maxExtents);
		}

		// Returns the final aabb.
		return aabb2;
	}

	Vector3 *Aabb::ResolveCollision(const ICollider &other, const Vector3 &positionDelta, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		const Aabb &aabb2 = dynamic_cast<const Aabb &>(other);

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

			if (std::fabs(newAmountX) < std::fabs(positionDelta.m_x))
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

			if (std::fabs(newAmountY) < std::fabs(positionDelta.m_y))
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

			if (std::fabs(newAmountZ) < std::fabs(positionDelta.m_z))
			{
				destination->m_z = newAmountZ;
			}
		}

		return destination;
	}

	Intersect Aabb::Intersects(const ICollider &other)
	{
		const Aabb &aabb2 = dynamic_cast<const Aabb &>(other);

		Vector3 *distance1 = Vector3::Subtract(*m_minExtents, *aabb2.m_maxExtents, nullptr);
		Vector3 *distance2 = Vector3::Subtract(*aabb2.m_minExtents, *m_maxExtents, nullptr);
		Vector3 *maxDistance = Vector3::MaxVector(*distance1, *distance2, nullptr);
		const float maxDist = Vector3::MaxComponent(*maxDistance);

		delete distance1;
		delete distance2;
		delete maxDistance;

		return Intersect(maxDist < 0.0f, maxDist);

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

	Intersect Aabb::Intersects(const Ray &ray)
	{
		double tmin = (m_minExtents->m_x - ray.m_origin->m_x) / ray.m_currentRay->m_x;
		double tmax = (m_maxExtents->m_x - ray.m_origin->m_x) / ray.m_currentRay->m_x;

		if (tmin > tmax)
		{
			const double temp = tmax;
			tmax = tmin;
			tmin = temp;
		}

		float tymin = (m_minExtents->m_y - ray.m_origin->m_y) / ray.m_currentRay->m_y;
		float tymax = (m_maxExtents->m_y - ray.m_origin->m_y) / ray.m_currentRay->m_y;

		if (tymin > tymax)
		{
			const float temp = tymax;
			tymax = tymin;
			tymin = temp;
		}

		if ((tmin > tymax) || (tymin > tmax))
		{
			return Intersect(false, 0.0f);
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
			const float temp = tzmax;
			tzmax = tzmin;
			tzmin = temp;
		}

		if ((tmin > tzmax) || (tzmin > tmax))
		{
			return Intersect(false, 0.0f);
		}

		return Intersect(true, 0.0f);
	}

	bool Aabb::InFrustum(const Frustum &frustum)
	{
		return frustum.CubeInFrustum(*m_minExtents, *m_maxExtents);
	}

	bool Aabb::Contains(const ICollider &other)
	{
		const Aabb &aabb2 = dynamic_cast<const Aabb &>(other);

		return m_minExtents->m_x <= aabb2.m_minExtents->m_x &&
			aabb2.m_maxExtents->m_x <= m_maxExtents->m_x &&
			m_minExtents->m_y <= aabb2.m_minExtents->m_y &&
			aabb2.m_maxExtents->m_y <= m_maxExtents->m_y &&
			m_minExtents->m_z <= aabb2.m_minExtents->m_z &&
			aabb2.m_maxExtents->m_z <= m_maxExtents->m_z;
	}

	bool Aabb::Contains(const Vector3 &point)
	{
		if (point.m_x > m_maxExtents->m_x)
		{
			return false;
		}

		if (point.m_x < m_minExtents->m_x)
		{
			return false;
		}

		if (point.m_y > m_maxExtents->m_y)
		{
			return false;
		}

		if (point.m_y < m_minExtents->m_y)
		{
			return false;
		}

		if (point.m_z > m_maxExtents->m_z)
		{
			return false;
		}

		if (point.m_z < m_minExtents->m_z)
		{
			return false;
		}

		return true;
	}

	float Aabb::GetCentreX() const
	{
		return (m_minExtents->m_x + m_maxExtents->m_x) / 2.0f;
	}

	float Aabb::GetCentreY() const
	{
		return (m_minExtents->m_y + m_maxExtents->m_y) / 2.0f;
	}

	float Aabb::GetCentreZ() const
	{
		return (m_minExtents->m_z + m_maxExtents->m_z) / 2.0f;
	}

	float Aabb::GetWidth() const
	{
		return m_maxExtents->m_x - m_minExtents->m_x;
	}

	float Aabb::GetHeight() const
	{
		return m_maxExtents->m_y - m_minExtents->m_y;
	}

	float Aabb::GetDepth() const
	{
		return m_maxExtents->m_z - m_minExtents->m_z;
	}
}
