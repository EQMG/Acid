#include "vector4.hpp"

#include "vector3.hpp"

namespace flounder
{
	vector4::vector4() :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f),
		m_w(0.0f)
	{
	}

	vector4::vector4(const float &x, const float &y, const float &z, const float &w) :
		m_x(x),
		m_y(y),
		m_z(z),
		m_w(z)
	{
	}

	vector4::vector4(const vector3 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z),
		m_w(1.0f)
	{
	}

	vector4::vector4(const vector4 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z),
		m_w(source.m_w)
	{
	}

	vector4::~vector4()
	{
	}

	vector4 *vector4::set(const float &x, const float &y, const float &z, const float &w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
		return this;
	}

	vector4 *vector4::set(const vector3 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		m_w = 0.0f;
		return this;
	}

	vector4 *vector4::set(const vector4 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		m_w = source.m_w;
		return this;
	}

	vector4 *vector4::add(const vector4 &left, const vector4 &right, vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector4();
		}

		return destination->set(left.m_x + right.m_x, left.m_y + right.m_y, left.m_z + right.m_z, left.m_w + right.m_w);
	}

	vector4 *vector4::subtract(const vector4 &left, const vector4 &right, vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector4();
		}

		return destination->set(left.m_x - right.m_x, left.m_y - right.m_y, left.m_z - right.m_z, left.m_w - right.m_w);
	}

	vector4 *vector4::multiply(const vector4 &left, const vector4 &right, vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector4();
		}

		return destination->set(left.m_x * right.m_x, left.m_y * right.m_y, left.m_z * right.m_z, left.m_w * right.m_w);
	}

	vector4 *vector4::divide(const vector4 &left, const vector4 &right, vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector4();
		}

		return destination->set(left.m_x / right.m_x, left.m_y / right.m_y, left.m_z / right.m_z, left.m_w / right.m_w);
	}

	float vector4::angle(const vector4 &left, const vector4 &right)
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

	float vector4::dot(const vector4 &left, const vector4 &right)
	{
		return left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z + left.m_w * right.m_w;
	}

	vector4 *vector4::scale(const vector4 &source, const float &scalar, vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector4();
		}

		return destination->set(source.m_x * scalar, source.m_y * scalar, source.m_z * scalar, source.m_w * scalar);
	}

	vector4 *vector4::negate(const vector4 &source, vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector4();
		}

		return destination->set(-source.m_x, -source.m_y, -source.m_z, -source.m_w);
	}

	vector4 *vector4::normalize(const vector4 &source, vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector4();
		}

		float l = length(source);

		if (l != 0.0f)
		{
			return destination->set(source.m_x / l, source.m_y / l, source.m_z / l, source.m_w / l);
		}
		else
		{
			throw std::invalid_argument("Zero length vector");
		}
	}

	float vector4::length(const vector4 &source)
	{
		return sqrt(lengthSquared(source));
	}

	float vector4::lengthSquared(const vector4 &source)
	{
		return source.m_x * source.m_x + source.m_y * source.m_y + source.m_z * source.m_z + source.m_w * source.m_w;
	}

	vector4 *vector4::maxVector(const vector4 &a, const vector4 &b, vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector4();
		}

		return destination->set(__max(a.m_x, b.m_x), __max(a.m_y, b.m_y), __max(a.m_z, b.m_z), __max(a.m_w, b.m_w));
	}

	vector4 *vector4::minVector(const vector4 &a, const vector4 &b, vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector4();
		}

		return destination->set(__min(a.m_x, b.m_x), __min(a.m_y, b.m_y), __min(a.m_z, b.m_z), __min(a.m_w, b.m_w));
	}

	float vector4::maxComponent(const vector4 &vector)
	{
		return __max(vector.m_x, __max(vector.m_y, __max(vector.m_z, vector.m_w)));
	}

	float vector4::minComponent(const vector4 &vector)
	{
		return __min(vector.m_x, __min(vector.m_y, __min(vector.m_z, vector.m_w)));
	}

	float vector4::getDistanceSquared(const vector4 &point1, const vector4 &point2)
	{
		float dx = point1.m_x - point2.m_x;
		float dy = point1.m_y - point2.m_y;
		float dz = point1.m_z - point2.m_z;
		float dw = point1.m_w - point2.m_w;
		return dx * dx + dy * dy + dz * dz + dw * dw;
	}

	float vector4::getDistance(const vector4 &point1, const vector4 &point2)
	{
		return sqrt(pow(point2.m_x - point1.m_x, 2) + pow(point2.m_y - point1.m_y, 2) + pow(point2.m_z - point1.m_z, 2) + pow(point2.m_w - point1.m_w, 2));
	}

	vector4 *vector4::getVectorDistance(const vector4 &point1, const vector4 &point2, vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector4();
		}

		return destination->set(pow(point2.m_x - point1.m_x, 2), pow(point2.m_y - point1.m_y, 2), pow(point2.m_z - point1.m_z, 2), pow(point2.m_w - point1.m_w, 2));
	}

	vector4 *vector4::translate(const float &x, const float &y, const float &z, const float &w)
	{
		m_x += x;
		m_y += y;
		m_z += z;
		m_w += w;
		return this;
	}

	vector4 *vector4::negate()
	{
		m_x = -m_x;
		m_y = -m_y;
		m_z = -m_z;
		m_w = -m_w;
		return this;
	}

	vector4 *vector4::normalize()
	{
		return normalize(*this, this);
	}

	vector4 *vector4::scale(const float &scalar)
	{
		return scale(*this, scalar, this);
	}

	bool vector4::isZero() const
	{
		return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f && m_w == 0.0f;
	}

	float vector4::length() const
	{
		return length(*this);
	}

	float vector4::lengthSquared() const
	{
		return lengthSquared(*this);
	}
}
