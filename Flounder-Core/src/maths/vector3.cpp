#include "vector3.h"

namespace flounder
{
	vector3::vector3()
	{
		this->set(0.0f, 0.0f, 0.0f);
	}

	vector3::vector3(const float &x, const float &y, const float &z)
	{
		this->set(x, y, z);
	}

	vector3::vector3(const vector3 &source)
	{
		this->set(source);
	}

	vector3::vector3(const vector4 &source)
	{
		this->set(source);
	}

	vector3::~vector3()
	{
	}

	vector3 *vector3::set(const float &x, const float &y, const float &z)
	{
		this->m_x = x;
		this->m_y = y;
		this->m_z = z;
		return this;
	}

	vector3 *vector3::set(const vector3 &source)
	{
		this->m_x = source.m_x;
		this->m_y = source.m_y;
		this->m_z = source.m_z;
		return this;
	}

	vector3 *vector3::set(const vector4 &source)
	{
		this->m_x = source.m_x;
		this->m_y = source.m_y;
		this->m_z = source.m_z;
		return this;
	}

	vector3 *vector3::add(const vector3 &left, const vector3 &right, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(left.m_x + right.m_x, left.m_y + right.m_y, left.m_z + right.m_z);
	}

	vector3 *vector3::subtract(const vector3 &left, const vector3 &right, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(left.m_x - right.m_x, left.m_y - right.m_y, left.m_z - right.m_z);
	}

	vector3 *vector3::multiply(const vector3 &left, const vector3 &right, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(left.m_x * right.m_x, left.m_y * right.m_y, left.m_z * right.m_z);
	}

	vector3 *vector3::divide(const vector3 &left, const vector3 &right, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(left.m_x / right.m_x, left.m_y / right.m_y, left.m_z / right.m_z);
	}

	float vector3::angle(const vector3 &left, const vector3 &right)
	{
		float dls = dot(left, right) / (length(left) * length(right));

		if (dls < -1.0f)
		{
			dls = -1.0f;
		}
		else if (dls > 1.0f)
		{
			dls = 1.0f;
		}

		return (float) acos(dls);
	}

	float vector3::dot(const vector3 &left, const vector3 &right)
	{
		return left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z;
	}

	vector3 *vector3::cross(const vector3 &left, const vector3 &right, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(left.m_y * right.m_z - left.m_z * right.m_y, right.m_x * left.m_z - right.m_z * left.m_x, left.m_x * right.m_y - left.m_y * right.m_x);
	}

	vector3 *vector3::scale(const vector3 &source, const float &scalar, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(source.m_x * scalar, source.m_y * scalar, source.m_z * scalar);
	}

	vector3 *vector3::rotate(const vector3 &source, vector3 *rotation, vector3 *destination)
	{
		throw std::logic_error("Instead of calling vector3::rotate, call matrix4x4::rotate!");
	}

	vector3 *vector3::negate(const vector3 &source, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(-source.m_x, -source.m_y, -source.m_z);
	}

	vector3 *vector3::normalize(const vector3 &source, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		float l = length(source);

		if (l != 0.0f)
		{
			return destination->set(source.m_x / l, source.m_y / l, source.m_z / l);
		}
		else
		{
			throw std::invalid_argument("Zero length vector");
		}
	}

	float vector3::length(const vector3 &source)
	{
		return sqrt(lengthSquared(source));
	}

	float vector3::lengthSquared(const vector3 &source)
	{
		return source.m_x * source.m_x + source.m_y * source.m_y + source.m_z * source.m_z;
	}

	vector3 *vector3::maxVector(const vector3 &a, const vector3 &b, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(__max(a.m_x, b.m_x), __max(a.m_y, b.m_y), __max(a.m_z, b.m_z));
	}

	vector3 *vector3::minVector(const vector3 &a, const vector3 &b, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(__min(a.m_x, b.m_x), __min(a.m_y, b.m_y), __min(a.m_z, b.m_z));
	}

	float vector3::maxComponent(const vector3 &vector)
	{
		return __max(vector.m_x, __max(vector.m_y, vector.m_z));
	}

	float vector3::minComponent(const vector3 &vector)
	{
		return __min(vector.m_x, __min(vector.m_y, vector.m_z));
	}

	float vector3::getDistanceSquared(const vector3 &point1, const vector3 &point2)
	{
		float dx = point1.m_x - point2.m_x;
		float dy = point1.m_y - point2.m_y;
		float dz = point1.m_z - point2.m_z;
		return dx * dx + dy * dy + dz * dz;
	}

	float vector3::getDistance(const vector3 &point1, const vector3 &point2)
	{
		return sqrt(pow(point2.m_x - point1.m_x, 2) + pow(point2.m_y - point1.m_y, 2) + pow(point2.m_z - point1.m_z, 2));
	}

	vector3 *vector3::getVectorDistance(const vector3 &point1, const vector3 &point2, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(pow(point2.m_x - point1.m_x, 2), pow(point2.m_y - point1.m_y, 2), pow(point2.m_z - point1.m_z, 2));
	}

	vector3 *vector3::generateRandomUnitVector(vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		float theta = (float) __random();
		theta *= 2.0f * PI;
		float z = (float) __random();
		z *= 2.0f - 1.0f;
		float rootOneMinusZSquared = sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);
		return destination->set(x, y, z);
	}

	vector3 *vector3::randomPointOnCircle(const vector3 &normal, const float &radius, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		do
		{
			vector3 *randomVector = generateRandomUnitVector(NULL);
			vector3::cross(*randomVector, normal, destination);
			delete randomVector;
		}
		while (length(*destination) == 0.0f);

		destination->normalize();
		destination->scale(radius);
		float a = (float) __random();
		float b = (float) __random();

		if (a > b)
		{
			float temp = a;
			a = b;
			b = temp;
		}

		float randX = b * cos(2.0f * PI * (a / b));
		float randY = b * sin(2.0f * PI * (a / b));
		float distance = vector3(randX, randY, 0.0f).length();
		destination->scale(distance);
		return destination;
	}

	float vector3::baryCentric(const vector3 &p1, const vector3 &p2, const vector3 &p3, const vector3 &pos)
	{
		float det = (p2.m_z - p3.m_z) * (p1.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (p1.m_z - p3.m_z);
		float l1 = ((p2.m_z - p3.m_z) * (pos.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (pos.m_y - p3.m_z)) / det;
		float l2 = ((p3.m_z - p1.m_z) * (pos.m_x - p3.m_x) + (p1.m_x - p3.m_x) * (pos.m_y - p3.m_z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.m_y + l2 * p2.m_y + l3 * p3.m_y;
	}

	vector3 *vector3::translate(const float &x, const float &y, const float &z)
	{
		this->m_x += x;
		this->m_y += y;
		this->m_z += z;
		return this;
	}

	vector3 *vector3::negate()
	{
		return negate(*this, this);
	}

	vector3 *vector3::normalize()
	{
		return normalize(*this, this);
	}

	vector3 *vector3::scale(const float &scalar)
	{
		return scale(*this, scalar, this);
	}

	bool vector3::isZero() const
	{
		return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f;
	}

	float vector3::length() const
	{
		return length(*this);
	}

	float vector3::lengthSquared() const
	{
		return lengthSquared(*this);
	}
}
