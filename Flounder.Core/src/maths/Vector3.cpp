#include "Vector3.hpp"

#include "Vector2.hpp"
#include "Vector4.hpp"

namespace Flounder
{
	Vector3::Vector3() :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f)
	{
	}

	Vector3::Vector3(const Vector2 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(0.0f)
	{
	}

	Vector3::Vector3(const Vector3 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z)
	{
	}

	Vector3::Vector3(const Vector4 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z)
	{
	}

	Vector3::Vector3(const float &x, const float &y, const float &z) :
		m_x(x),
		m_y(y),
		m_z(z)
	{
	}

	Vector3::~Vector3()
	{
	}

	Vector3 *Vector3::set(const Vector2 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = 0.0f;
		return this;
	}

	Vector3 *Vector3::set(const Vector3 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		return this;
	}

	Vector3 *Vector3::set(const Vector4 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		return this;
	}

	Vector3 *Vector3::set(const float &x, const float &y, const float &z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		return this;
	}

	Vector3 *Vector3::add(const Vector3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->set(left.m_x + right.m_x, left.m_y + right.m_y, left.m_z + right.m_z);
	}

	Vector3 *Vector3::subtract(const Vector3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->set(left.m_x - right.m_x, left.m_y - right.m_y, left.m_z - right.m_z);
	}

	Vector3 *Vector3::multiply(const Vector3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->set(left.m_x * right.m_x, left.m_y * right.m_y, left.m_z * right.m_z);
	}

	Vector3 *Vector3::divide(const Vector3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->set(left.m_x / right.m_x, left.m_y / right.m_y, left.m_z / right.m_z);
	}

	float Vector3::angle(const Vector3 &left, const Vector3 &right)
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

		return acos(dls);
	}

	float Vector3::dot(const Vector3 &left, const Vector3 &right)
	{
		return left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z;
	}

	Vector3 *Vector3::cross(const Vector3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->set(left.m_y * right.m_z - left.m_z * right.m_y, right.m_x * left.m_z - right.m_z * left.m_x, left.m_x * right.m_y - left.m_y * right.m_x);
	}

	Vector3 *Vector3::scale(const Vector3 &source, const float &scalar, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->set(source.m_x * scalar, source.m_y * scalar, source.m_z * scalar);
	}

	Vector3 *Vector3::rotate(const Vector3 &source, const Vector3 &rotation, Vector3 *destination)
	{
		throw std::logic_error("Instead of calling Vector3::rotate, call Matrix4::rotate!");
	}

	Vector3 *Vector3::negate(const Vector3 &source, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->set(-source.m_x, -source.m_y, -source.m_z);
	}

	Vector3 *Vector3::normalize(const Vector3 &source, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		float l = length(source);

		if (l != 0.0f)
		{
			return destination->set(source.m_x / l, source.m_y / l, source.m_z / l);
		}
		throw std::invalid_argument("Zero length vector");
	}

	float Vector3::length(const Vector3 &source)
	{
		return sqrt(lengthSquared(source));
	}

	float Vector3::lengthSquared(const Vector3 &source)
	{
		return source.m_x * source.m_x + source.m_y * source.m_y + source.m_z * source.m_z;
	}

	Vector3 *Vector3::maxVector(const Vector3 &a, const Vector3 &b, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->set(Maths::Max(a.m_x, b.m_x), Maths::Max(a.m_y, b.m_y), Maths::Max(a.m_z, b.m_z));
	}

	Vector3 *Vector3::minVector(const Vector3 &a, const Vector3 &b, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->set(Maths::Min(a.m_x, b.m_x), Maths::Min(a.m_y, b.m_y), Maths::Min(a.m_z, b.m_z));
	}

	float Vector3::maxComponent(const Vector3 &vector)
	{
		return Maths::Max(vector.m_x, Maths::Max(vector.m_y, vector.m_z));
	}

	float Vector3::minComponent(const Vector3 &vector)
	{
		return Maths::Min(vector.m_x, Maths::Min(vector.m_y, vector.m_z));
	}

	float Vector3::getDistanceSquared(const Vector3 &point1, const Vector3 &point2)
	{
		float dx = point1.m_x - point2.m_x;
		float dy = point1.m_y - point2.m_y;
		float dz = point1.m_z - point2.m_z;
		return dx * dx + dy * dy + dz * dz;
	}

	float Vector3::getDistance(const Vector3 &point1, const Vector3 &point2)
	{
		return sqrt(pow(point2.m_x - point1.m_x, 2) + pow(point2.m_y - point1.m_y, 2) + pow(point2.m_z - point1.m_z, 2));
	}

	Vector3 *Vector3::getVectorDistance(const Vector3 &point1, const Vector3 &point2, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->set(pow(point2.m_x - point1.m_x, 2), pow(point2.m_y - point1.m_y, 2), pow(point2.m_z - point1.m_z, 2));
	}

	Vector3 *Vector3::generateRandomUnitVector(Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		float theta = Maths::RandomInRange(0.0f, 1.0f) * 2.0f * PI;
		float z = Maths::RandomInRange(0.0f, 1.0f) * 2.0f - 1.0f;
		float rootOneMinusZSquared = sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);
		return destination->set(x, y, z);
	}

	Vector3 *Vector3::randomPointOnCircle(const Vector3 &normal, const float &radius, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		do
		{
			Vector3 *randomVector = generateRandomUnitVector(nullptr);
			cross(*randomVector, normal, destination);
			delete randomVector;
		}
		while (length(*destination) == 0.0f);

		destination->normalize();
		destination->scale(radius);
		float a = Maths::RandomInRange(0.0f, 1.0f);
		float b = Maths::RandomInRange(0.0f, 1.0f);

		if (a > b)
		{
			float temp = a;
			a = b;
			b = temp;
		}

		float randX = b * cos(2.0f * PI * (a / b));
		float randY = b * sin(2.0f * PI * (a / b));
		float distance = Vector3(randX, randY, 0.0f).length();
		destination->scale(distance);
		return destination;
	}

	float Vector3::baryCentric(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &pos)
	{
		float det = (p2.m_z - p3.m_z) * (p1.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (p1.m_z - p3.m_z);
		float l1 = ((p2.m_z - p3.m_z) * (pos.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (pos.m_y - p3.m_z)) / det;
		float l2 = ((p3.m_z - p1.m_z) * (pos.m_x - p3.m_x) + (p1.m_x - p3.m_x) * (pos.m_y - p3.m_z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.m_y + l2 * p2.m_y + l3 * p3.m_y;
	}

	Vector3 *Vector3::translate(const float &x, const float &y, const float &z)
	{
		m_x += x;
		m_y += y;
		m_z += z;
		return this;
	}

	Vector3 *Vector3::negate()
	{
		return negate(*this, this);
	}

	Vector3 *Vector3::normalize()
	{
		return normalize(*this, this);
	}

	Vector3 *Vector3::scale(const float &scalar)
	{
		return scale(*this, scalar, this);
	}

	bool Vector3::isZero() const
	{
		return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f;
	}

	float Vector3::length() const
	{
		return length(*this);
	}

	float Vector3::lengthSquared() const
	{
		return lengthSquared(*this);
	}
}
