#include "sphere.h"

namespace flounder
{
	sphere::sphere()
	{
		m_radius = 1.0f;
		m_position = new vector3();
	}

	sphere::sphere(const float &radius)
	{
		m_radius = radius;
		m_position = new vector3();
	}

	sphere::sphere(const float &radius, vector3 *position)
	{
		m_radius = radius;
		m_position = position;
	}

	sphere::sphere(const sphere &source)
	{
		m_radius = source.m_radius;
		m_position = new vector3(*source.m_position);
	}

	sphere::~sphere()
	{
		delete m_position;
	}

	collider *sphere::update(const vector3 &position, const vector3 &rotation, const float &scale, collider *destination)
	{
		if (destination == NULL)
		{
			destination = new sphere();
		}

		sphere *source = static_cast<sphere*>(destination);

		source->m_radius = m_radius * scale;
		source->m_position->set(position);

		return source;
	}

	vector3 *sphere::resolveCollision(const collider &other, const vector3 &positionDelta, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		if (this == &other)
		{
			return destination;
		}

		const sphere &sphere2 = dynamic_cast<const sphere&>(other);
		float d = sphere2.m_radius + m_radius;

		float xDif = m_position->m_x - sphere2.m_position->m_x;
		float yDif = m_position->m_y - sphere2.m_position->m_y;
		float zDif = m_position->m_z - sphere2.m_position->m_z;
		float distance = xDif * xDif + yDif * yDif + zDif * zDif;
		// TODO: Resolve!
		return destination;
	}

	intersect *sphere::intersects(const collider &other)
	{
		if (&other == NULL || this == &other)
		{
			return new intersect(true, 0.0f);
		}

		/*if (dynamic_cast<aabb*>(other) != 0)
		{
			AABB *aabb = static_cast<AABB*>(other);

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

			return new intersect(distanceSquared > 0.0f, static_cast<float>(sqrt(distanceSquared)));
		}
		else */
		if (&dynamic_cast<const sphere&>(other) != NULL)
		{
			const sphere &sphere2 = static_cast<const sphere&>(other);

			float d = sphere2.m_radius + m_radius;

			float xDif = m_position->m_x - sphere2.m_position->m_x;
			float yDif = m_position->m_y - sphere2.m_position->m_y;
			float zDif = m_position->m_z - sphere2.m_position->m_z;
			float distance = xDif * xDif + yDif * yDif + zDif * zDif;

			bool intersects = d * d > distance;
			return new intersect(intersects, (d * d) - distance);
		}

		return new intersect(false, 0.0f);
	}

	intersect *sphere::intersects(const ray &ray)
	{
		float t;

		vector3 *L = vector3::subtract(*ray.getOrigin(), *m_position, NULL);

		float a = vector3::dot(*ray.getCurrentRay(), *ray.getCurrentRay());
		float b = 2.0f * (vector3::dot(*ray.getCurrentRay(), *L));
		float c = (vector3::dot(*L, *L)) - (m_radius * m_radius);

		float disc = b * b - 4.0f * a * c;

		delete L;

		if (disc < 0.0f)
		{
			return new intersect(false, -1.0f);
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
			return new intersect(false, -1.0f);
		}

		if (t0 < 0.0f)
		{
			t = t1;
		}
		else
		{
			t = t0;
		}

		return new intersect(true, t);
	}

	bool sphere::inFrustum(const frustum &frustum)
	{
		return frustum.sphereInFrustum(m_position->m_x, m_position->m_y, m_position->m_z, m_radius);
	}

	bool sphere::contains(const collider &other)
	{
		if (&other == NULL || this == &other)
		{
			return false;
		}

		const sphere &sphere2 = dynamic_cast<const sphere&>(other);

		if (&sphere2 != NULL)
		{
			return
				sphere2.m_position->m_x + sphere2.m_radius - 1.0f <= m_position->m_x + m_radius - 1.0f &&
				sphere2.m_position->m_x - sphere2.m_radius + m_radius >= m_position->m_x - m_radius + 1.0f &&
				sphere2.m_position->m_y + sphere2.m_radius - 1.0f <= m_position->m_y + m_radius - 1.0f &&
				sphere2.m_position->m_y - sphere2.m_radius + 1.0f >= m_position->m_y - m_radius + 1.0f &&
				sphere2.m_position->m_z + sphere2.m_radius - 1.0f <= m_position->m_z + m_radius - 1.0f &&
				sphere2.m_position->m_z - sphere2.m_radius + 1.0f >= m_position->m_z - m_radius + 1.0f;
		}

		return false;
	}

	bool sphere::contains(const vector3 &point)
	{
		return vector3::getDistanceSquared(*m_position, point) <= m_radius * m_radius;
	}
}
