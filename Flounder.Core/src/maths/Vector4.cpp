#include "Vector4.hpp"

#include "Vector3.hpp"

namespace Flounder
{
	Vector4::Vector4() :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f),
		m_w(1.0f)
	{
	}

	Vector4::Vector4(const float &x, const float &y, const float &z, const float &w) :
		m_x(x),
		m_y(y),
		m_z(z),
		m_w(z)
	{
	}

	Vector4::Vector4(const Vector3 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z),
		m_w(1.0f)
	{
	}

	Vector4::Vector4(const Vector4 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z),
		m_w(source.m_w)
	{
	}

	Vector4::~Vector4()
	{
	}

	Vector4 *Vector4::set(const float &x, const float &y, const float &z, const float &w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
		return this;
	}

	Vector4 *Vector4::set(const Vector3 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		m_w = 1.0f;
		return this;
	}

	Vector4 *Vector4::set(const Vector4 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		m_w = source.m_w;
		return this;
	}

	Vector4 *Vector4::add(const Vector4 &left, const Vector4 &right, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->set(left.m_x + right.m_x, left.m_y + right.m_y, left.m_z + right.m_z, left.m_w + right.m_w);
	}

	Vector4 *Vector4::subtract(const Vector4 &left, const Vector4 &right, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->set(left.m_x - right.m_x, left.m_y - right.m_y, left.m_z - right.m_z, left.m_w - right.m_w);
	}

	Vector4 *Vector4::multiply(const Vector4 &left, const Vector4 &right, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->set(left.m_x * right.m_x, left.m_y * right.m_y, left.m_z * right.m_z, left.m_w * right.m_w);
	}

	Vector4 *Vector4::divide(const Vector4 &left, const Vector4 &right, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->set(left.m_x / right.m_x, left.m_y / right.m_y, left.m_z / right.m_z, left.m_w / right.m_w);
	}

	float Vector4::angle(const Vector4 &left, const Vector4 &right)
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

	float Vector4::dot(const Vector4 &left, const Vector4 &right)
	{
		return left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z + left.m_w * right.m_w;
	}

	Vector4 *Vector4::scale(const Vector4 &source, const float &scalar, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->set(source.m_x * scalar, source.m_y * scalar, source.m_z * scalar, source.m_w * scalar);
	}

	Vector4 *Vector4::negate(const Vector4 &source, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->set(-source.m_x, -source.m_y, -source.m_z, -source.m_w);
	}

	Vector4 *Vector4::normalize(const Vector4 &source, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		float l = length(source);

		if (l != 0.0f)
		{
			return destination->set(source.m_x / l, source.m_y / l, source.m_z / l, source.m_w / l);
		}
		throw std::invalid_argument("Zero length vector");
	}

	float Vector4::length(const Vector4 &source)
	{
		return sqrt(lengthSquared(source));
	}

	float Vector4::lengthSquared(const Vector4 &source)
	{
		return source.m_x * source.m_x + source.m_y * source.m_y + source.m_z * source.m_z + source.m_w * source.m_w;
	}

	Vector4 *Vector4::maxVector(const Vector4 &a, const Vector4 &b, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->set(Maths::Max(a.m_x, b.m_x), Maths::Max(a.m_y, b.m_y), Maths::Max(a.m_z, b.m_z), Maths::Max(a.m_w, b.m_w));
	}

	Vector4 *Vector4::minVector(const Vector4 &a, const Vector4 &b, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->set(Maths::Min(a.m_x, b.m_x), Maths::Min(a.m_y, b.m_y), Maths::Min(a.m_z, b.m_z), Maths::Min(a.m_w, b.m_w));
	}

	float Vector4::maxComponent(const Vector4 &vector)
	{
		return Maths::Max(vector.m_x, Maths::Max(vector.m_y, Maths::Max(vector.m_z, vector.m_w)));
	}

	float Vector4::minComponent(const Vector4 &vector)
	{
		return Maths::Min(vector.m_x, Maths::Min(vector.m_y, Maths::Min(vector.m_z, vector.m_w)));
	}

	float Vector4::getDistanceSquared(const Vector4 &point1, const Vector4 &point2)
	{
		float dx = point1.m_x - point2.m_x;
		float dy = point1.m_y - point2.m_y;
		float dz = point1.m_z - point2.m_z;
		float dw = point1.m_w - point2.m_w;
		return dx * dx + dy * dy + dz * dz + dw * dw;
	}

	float Vector4::getDistance(const Vector4 &point1, const Vector4 &point2)
	{
		return sqrt(pow(point2.m_x - point1.m_x, 2) + pow(point2.m_y - point1.m_y, 2) + pow(point2.m_z - point1.m_z, 2) + pow(point2.m_w - point1.m_w, 2));
	}

	Vector4 *Vector4::getVectorDistance(const Vector4 &point1, const Vector4 &point2, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->set(pow(point2.m_x - point1.m_x, 2), pow(point2.m_y - point1.m_y, 2), pow(point2.m_z - point1.m_z, 2), pow(point2.m_w - point1.m_w, 2));
	}

	Vector4 *Vector4::translate(const float &x, const float &y, const float &z, const float &w)
	{
		m_x += x;
		m_y += y;
		m_z += z;
		m_w += w;
		return this;
	}

	Vector4 *Vector4::negate()
	{
		m_x = -m_x;
		m_y = -m_y;
		m_z = -m_z;
		m_w = -m_w;
		return this;
	}

	Vector4 *Vector4::normalize()
	{
		return normalize(*this, this);
	}

	Vector4 *Vector4::scale(const float &scalar)
	{
		return scale(*this, scalar, this);
	}

	bool Vector4::isZero() const
	{
		return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f && m_w == 0.0f;
	}

	float Vector4::length() const
	{
		return length(*this);
	}

	float Vector4::lengthSquared() const
	{
		return lengthSquared(*this);
	}
}
