#include "Sphere.hpp"

#include <cmath>

namespace Flounder
{
	Sphere::Sphere() :
		ICollider(),
		m_radius(1.0f),
		m_position(new Vector3())
	{
	}

	Sphere::Sphere(const float &radius, const Vector3 &position) :
		ICollider(),
		m_radius(radius),
		m_position(new Vector3(position))
	{
	}

	Sphere::Sphere(const Sphere &source) :
		ICollider(),
		m_radius(source.m_radius),
		m_position(new Vector3(*source.m_position))
	{
	}

	Sphere::~Sphere()
	{
		delete m_position;
	}

	ICollider *Sphere::Update(const Vector3 &position, const Vector3 &rotation, const float &scale, ICollider *destination)
	{
		if (destination == nullptr)
		{
			destination = new Sphere();
		}

		Sphere *source = dynamic_cast<Sphere *>(destination);

		source->m_radius = m_radius * scale;
		source->m_position->Set(position);

		return source;
	}

	Vector3 *Sphere::ResolveCollision(const ICollider &other, const Vector3 &positionDelta, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		//	const Sphere &sphere2 = dynamic_cast<const Sphere&>(other);
		//	float d = sphere2.m_radius + m_radius;

		//	float xDif = m_position->m_x - sphere2.m_position->m_x;
		//	float yDif = m_position->m_y - sphere2.m_position->m_y;
		//	float zDif = m_position->m_z - sphere2.m_position->m_z;
		//	float distance = xDif * xDif + yDif * yDif + zDif * zDif;
		// TODO: Resolve!
		return destination;
	}

	Intersect Sphere::Intersects(const ICollider &other)
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
		const Sphere &sphere2 = dynamic_cast<const Sphere &>(other);

		float d = sphere2.m_radius + m_radius;

		float xDif = m_position->m_x - sphere2.m_position->m_x;
		float yDif = m_position->m_y - sphere2.m_position->m_y;
		float zDif = m_position->m_z - sphere2.m_position->m_z;
		float distance = xDif * xDif + yDif * yDif + zDif * zDif;

		bool intersects = d * d > distance;
		return Intersect(intersects, (d * d) - distance);
	}

	Intersect Sphere::Intersects(const Ray &ray)
	{
		Vector3 *L = Vector3::Subtract(*ray.m_origin, *m_position, nullptr);

		float a = Vector3::Dot(*ray.m_currentRay, *ray.m_currentRay);
		float b = 2.0f * (Vector3::Dot(*ray.m_currentRay, *L));
		float c = (Vector3::Dot(*L, *L)) - (m_radius * m_radius);

		float disc = b * b - 4.0f * a * c;

		delete L;

		if (disc < 0.0f)
		{
			return Intersect(false, -1.0f);
		}

		float distSqrt = sqrt(disc);
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

	bool Sphere::InFrustum(const Frustum &frustum)
	{
		return frustum.SphereInFrustum(*m_position, m_radius);
	}

	bool Sphere::Contains(const ICollider &other)
	{
		const Sphere &sphere2 = dynamic_cast<const Sphere &>(other);

		return sphere2.m_position->m_x + sphere2.m_radius - 1.0f <= m_position->m_x + m_radius - 1.0f &&
			sphere2.m_position->m_x - sphere2.m_radius + m_radius >= m_position->m_x - m_radius + 1.0f &&
			sphere2.m_position->m_y + sphere2.m_radius - 1.0f <= m_position->m_y + m_radius - 1.0f &&
			sphere2.m_position->m_y - sphere2.m_radius + 1.0f >= m_position->m_y - m_radius + 1.0f &&
			sphere2.m_position->m_z + sphere2.m_radius - 1.0f <= m_position->m_z + m_radius - 1.0f &&
			sphere2.m_position->m_z - sphere2.m_radius + 1.0f >= m_position->m_z - m_radius + 1.0f;
	}

	bool Sphere::Contains(const Vector3 &point)
	{
		return Vector3::GetDistanceSquared(*m_position, point) <= m_radius * m_radius;
	}
}
