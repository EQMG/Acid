#include "sphere.h"

namespace flounder {
	sphere::sphere()
	{
		m_radius = 1.0f;
		m_position = new vector3();
	}

	sphere::sphere(const float radius)
	{
		m_radius = radius;
		m_position = new vector3();
	}

	sphere::sphere(const float radius, vector3 *position)
	{
		m_radius = radius;
		m_position = position;
	}

	sphere::sphere(sphere *source)
	{
		m_radius = source->m_radius;
		m_position = new vector3(*source->m_position);
	}

	sphere::~sphere()
	{
		delete m_position;
	}

	collider *sphere::update(vector3 *position, vector3 *rotation, const float scale, collider *destination)
	{
		if (destination == NULL)
		{
			destination = new sphere();
		}

		sphere *source = static_cast<sphere*>(destination);

		source->m_radius = m_radius * scale;
		source->m_position->set(*position);

		return source;
	}

	vector3 *sphere::resolveCollision(collider *other, vector3 *positionDelta, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		if (other == NULL || this == other)
		{
			return destination;
		}

		sphere *sphere2 = static_cast<sphere*>(other);
		float d = sphere2->m_radius + m_radius;

		float xDif = m_position->x - sphere2->m_position->x;
		float yDif = m_position->y - sphere2->m_position->y;
		float zDif = m_position->z - sphere2->m_position->z;
		float distance = xDif * xDif + yDif * yDif + zDif * zDif;
		// TODO: Resolve!
		return destination;
	}

	collider *sphere::clone()
	{
		return new sphere(m_radius, new vector3(*m_position));
	}

	intersect *sphere::intersects(collider *other)
	{
		return nullptr;
	}

	intersect *sphere::intersects(ray *other)
	{
		float t;

		vector3 *L = vector3::subtract(*other->getOrigin(), *m_position, NULL);

		float a = vector3::dot(*other->getCurrentRay(), *other->getCurrentRay());
		float b = 2.0f * (vector3::dot(*other->getCurrentRay(), *L));
		float c = (vector3::dot(*L, *L)) - (m_radius * m_radius);

		float disc = b * b - 4.0f * a * c;

		delete L;

		if (disc < 0.0f)
		{
			return new intersect(false, -1.0f);
		}

		float distSqrt = (float)(sqrt(disc));
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

	bool sphere::inFrustum(frustum *frustum)
	{
		return frustum->sphereInFrustum(m_position->x, m_position->y, m_position->z, m_radius);
	}

	bool sphere::contains(collider *other)
	{
		if (other == NULL || this == other)
		{
			return false;
		}

		if (dynamic_cast<sphere*>(other) != 0)
		{
			sphere *sphere2 = static_cast<sphere*>(other);

			return 
				sphere2->m_position->x + sphere2->m_radius - 1.0f <= m_position->x + m_radius - 1.0f &&
				sphere2->m_position->x - sphere2->m_radius + m_radius >= m_position->x - m_radius + 1.0f &&
				sphere2->m_position->y + sphere2->m_radius - 1.0f <= m_position->y + m_radius - 1.0f &&
				sphere2->m_position->y - sphere2->m_radius + 1.0f >= m_position->y - m_radius + 1.0f &&
				sphere2->m_position->z + sphere2->m_radius - 1.0f <= m_position->z + m_radius - 1.0f &&
				sphere2->m_position->z - sphere2->m_radius + 1.0f >= m_position->z - m_radius + 1.0f;
		}

		return false;
	}

	bool sphere::contains(vector3 *point)
	{
		return vector3::getDistanceSquared(*m_position, *point) <= m_radius * m_radius;
	}
}
