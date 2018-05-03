#include "ColliderSphere.hpp"

#include <cmath>

namespace fl
{
	ColliderSphere::ColliderSphere() :
		Collider(),
		m_radius(1.0f),
		m_position(new Vector3())
	{
	}

	ColliderSphere::ColliderSphere(const float &radius, const Vector3 &position) :
		Collider(),
		m_radius(radius),
		m_position(new Vector3(position))
	{
	}

	ColliderSphere::ColliderSphere(const ColliderSphere &source) :
		Collider(),
		m_radius(source.m_radius),
		m_position(new Vector3(*source.m_position))
	{
	}

	ColliderSphere::~ColliderSphere()
	{
		delete m_position;
	}

	Collider *ColliderSphere::Update(const Transform &transform, Collider *destination)
	{
		if (destination == nullptr)
		{
			destination = new ColliderSphere();
		}

		ColliderSphere *source = dynamic_cast<ColliderSphere *>(destination);

		if (source == nullptr)
		{
			delete source;
			source = new ColliderSphere();
		}

		source->m_radius = m_radius * transform.m_scaling->m_x;
		*source->m_position = *transform.m_position;

		return source;
	}

	void ColliderSphere::Load(LoadedValue *value)
	{
	}

	void ColliderSphere::Write(LoadedValue *value)
	{
	}

	Vector3 *ColliderSphere::ResolveCollision(const Collider &other, const Vector3 &positionDelta, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		//	const ColliderSphere &sphere2 = dynamic_cast<const ColliderSphere&>(other);
		//	float d = sphere2.m_radius + m_radius;

		//	float xDif = m_position->m_x - sphere2.m_position->m_x;
		//	float yDif = m_position->m_y - sphere2.m_position->m_y;
		//	float zDif = m_position->m_z - sphere2.m_position->m_z;
		//	float distance = xDif * xDif + yDif * yDif + zDif * zDif;
		// TODO: Resolve!
		return destination;
	}

	Intersect ColliderSphere::Intersects(const Collider &other)
	{
		/*if (dynamic_cast<aabb*>(other) != 0)
		{
			AABB *aabb = dynamic_cast<AABB*>(other);

			float distanceSquared = m_radius * m_radius;

			if (m_position->x < aabb->getMinExtents()->x)
			{
				distanceSquared -= pow(m_position->x - aabb->getMinExtents()->x, 2);
			}
			else if (m_position->x > aabb->getMaxExtents()->x)
			{
				distanceSquared -= pow(m_position->x - aabb->getMaxExtents()->x, 2);
			}

			if (m_position->y < aabb->getMinExtents()->x)
			{
				distanceSquared -= pow(m_position->y - aabb->getMinExtents()->y, 2);
			}
			else if (m_position->x > aabb->getMaxExtents()->x)
			{
				distanceSquared -= pow(m_position->y - aabb->getMaxExtents()->y, 2);
			}

			if (m_position->z < aabb->getMinExtents()->x)
			{
				distanceSquared -= pow(m_position->z - aabb->getMinExtents()->z, 2);
			}
			else if (m_position->z > aabb->getMaxExtents()->x)
			{
				distanceSquared -= pow(m_position->z - aabb->getMaxExtents()->z, 2);
			}

			return new intersect(distanceSquared > 0.0f, dynamic_cast<float>(sqrt(distanceSquared)));
		}
		else */
		const ColliderSphere &sphere2 = dynamic_cast<const ColliderSphere &>(other);

		float d = sphere2.m_radius + m_radius;

		float xDif = m_position->m_x - sphere2.m_position->m_x;
		float yDif = m_position->m_y - sphere2.m_position->m_y;
		float zDif = m_position->m_z - sphere2.m_position->m_z;
		float distance = xDif * xDif + yDif * yDif + zDif * zDif;

		bool intersects = d * d > distance;
		return Intersect(intersects, (d * d) - distance);
	}

	Intersect ColliderSphere::Intersects(const Ray &ray)
	{
		Vector3 L = *ray.m_origin - *m_position;

		float a = ray.m_currentRay->Dot(*ray.m_currentRay);
		float b = 2.0f * ray.m_currentRay->Dot(L);
		float c = L.Dot(L) - (m_radius * m_radius);

		float disc = b * b - 4.0f * a * c;

		if (disc < 0.0f)
		{
			return Intersect(false, -1.0f);
		}

		float distSqrt = std::sqrt(disc);
		float q;

		if (b < 0.0f)
		{
			q = (-b - distSqrt) / 2.0f;
		}
		else
		{
			q = (-b + distSqrt) / 2.0f;
		}

		float t0 = q / a;
		float t1 = c / q;

		if (t0 > t1)
		{
			float temp = t0;
			t0 = t1;
			t1 = temp;
		}

		if (t1 < 0.0f)
		{
			return Intersect(false, -1.0f);
		}

		float t;

		if (t0 < 0.0f)
		{
			t = t1;
		}
		else
		{
			t = t0;
		}

		return Intersect(true, t);
	}

	bool ColliderSphere::InFrustum(const Frustum &frustum)
	{
		return frustum.SphereInFrustum(*m_position, m_radius);
	}

	bool ColliderSphere::Contains(const Collider &other)
	{
		const ColliderSphere &sphere2 = dynamic_cast<const ColliderSphere &>(other);

		return sphere2.m_position->m_x + sphere2.m_radius - 1.0f <= m_position->m_x + m_radius - 1.0f &&
			sphere2.m_position->m_x - sphere2.m_radius + m_radius >= m_position->m_x - m_radius + 1.0f &&
			sphere2.m_position->m_y + sphere2.m_radius - 1.0f <= m_position->m_y + m_radius - 1.0f &&
			sphere2.m_position->m_y - sphere2.m_radius + 1.0f >= m_position->m_y - m_radius + 1.0f &&
			sphere2.m_position->m_z + sphere2.m_radius - 1.0f <= m_position->m_z + m_radius - 1.0f &&
			sphere2.m_position->m_z - sphere2.m_radius + 1.0f >= m_position->m_z - m_radius + 1.0f;
	}

	bool ColliderSphere::Contains(const Vector3 &point)
	{
		return m_position->DistanceSquared(point) <= m_radius * m_radius;
	}
}
