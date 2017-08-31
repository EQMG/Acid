#include "vector2.hpp"

#include "vector3.hpp"

namespace Flounder
{
	vector2::vector2() :
		m_x(0.0f),
		m_y(0.0f)
	{
	}

	vector2::vector2(const float &x, const float &y) :
		m_x(x),
		m_y(y)
	{
	}

	vector2::vector2(const vector2 &source) :
		m_x(source.m_x),
		m_y(source.m_y)
	{
	}

	vector2::vector2(const vector3 &source) :
		m_x(source.m_x),
		m_y(source.m_y)
	{
	}

	vector2::~vector2()
	{
	}

	vector2 *vector2::set(const float &x, const float &y)
	{
		m_x = x;
		m_y = y;
		return this;
	}

	vector2 *vector2::set(const vector2 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		return this;
	}

	vector2 *vector2::set(const vector3 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		return this;
	}

	vector2 *vector2::add(const vector2 &left, const vector2 &right, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		return destination->set(left.m_x + right.m_x, left.m_y + right.m_y);
	}

	vector2 *vector2::subtract(const vector2 &left, const vector2 &right, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		return destination->set(left.m_x - right.m_x, left.m_y - right.m_y);
	}

	vector2 *vector2::multiply(const vector2 &left, const vector2 &right, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		return destination->set(left.m_x * right.m_x, left.m_y * right.m_y);
	}

	vector2 *vector2::divide(const vector2 &left, const vector2 &right, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		return destination->set(left.m_x / right.m_x, left.m_y / right.m_y);
	}

	float vector2::angle(const vector2 &left, const vector2 &right)
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

	float vector2::dot(const vector2 &left, const vector2 &right)
	{
		return left.m_x * right.m_x + left.m_y * right.m_y;
	}

	vector2 *vector2::scale(const vector2 &source, const float &scalar, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		return destination->set(source.m_x * scalar, source.m_y * scalar);
	}

	vector2 *vector2::rotate(const vector2 &source, const float &angle, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		float theta = __radians(angle);
		return destination->set(source.m_x * cos(theta) - source.m_y * sin(theta), source.m_x * sin(theta) + source.m_y * cos(theta));
	}

	vector2 *vector2::rotate(const vector2 &source, const float &angle, const vector2 &rotationAxis, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		float theta = __radians(angle);
		return destination->set(((source.m_x - rotationAxis.m_x) * cos(theta)) - ((source.m_y - rotationAxis.m_y) * sin(theta) + rotationAxis.m_x), ((source.m_x - rotationAxis.m_x) * sin(theta)) + ((source.m_y - rotationAxis.m_y) * cos(theta) + rotationAxis.m_y));
	}

	vector2 *vector2::negate(const vector2 &source, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		return destination->set(-source.m_x, -source.m_y);
	}

	vector2 *vector2::normalize(const vector2 &source, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		float l = length(source);

		if (l != 0.0f)
		{
			return destination->set(source.m_x / l, source.m_y / l);
		}
		else
		{
			throw std::invalid_argument("Zero length vector");
		}
	}

	float vector2::length(const vector2 &source)
	{
		return sqrt(lengthSquared(source));
	}

	float vector2::lengthSquared(const vector2 &source)
	{
		return source.m_x * source.m_x + source.m_y * source.m_y;
	}

	vector2 *vector2::maxVector(const vector2 &a, const vector2 &b, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		return destination->set(__max(a.m_x, b.m_x), __max(a.m_y, b.m_y));
	}

	vector2 *vector2::minVector(const vector2 &a, const vector2 &b, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		return destination->set(__min(a.m_x, b.m_x), __min(a.m_y, b.m_y));
	}

	float vector2::maxComponent(const vector2 &vector)
	{
		return __max(vector.m_x, vector.m_y);
	}

	float vector2::minComponent(const vector2 &vector)
	{
		return __min(vector.m_x, vector.m_y);
	}

	float vector2::getDistanceSquared(const vector2 &point1, const vector2 &point2)
	{
		float dx = point1.m_x - point2.m_x;
		float dy = point1.m_y - point2.m_y;
		return dx * dx + dy * dy;
	}

	float vector2::getDistance(const vector2 &point1, const vector2 &point2)
	{
		return sqrt(pow(point2.m_x - point1.m_x, 2) + pow(point2.m_y - point1.m_y, 2));
	}

	vector2 *vector2::getVectorDistance(const vector2 &point1, const vector2 &point2, vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new vector2();
		}

		return destination->set(pow(point2.m_x - point1.m_x, 2), pow(point2.m_y - point1.m_y, 2));
	}

	bool vector2::pointInTriangle(const vector2 &point, const vector2 &v1, const vector2 &v2, const vector2 &v3)
	{
		bool b1 = (point.m_x - v2.m_x) * (v1.m_y - v2.m_y) - (v1.m_x - v2.m_x) * (point.m_y - v2.m_y);
		bool b2 = (point.m_x - v3.m_x) * (v2.m_y - v3.m_y) - (v2.m_x - v3.m_x) * (point.m_y - v3.m_y);
		bool b3 = (point.m_x - v1.m_x) * (v3.m_y - v1.m_y) - (v3.m_x - v1.m_x) * (point.m_y - v1.m_y);
		return ((b1 == b2) & (b2 == b3));
	}

	vector2 *vector2::translate(const float &x, const float &y)
	{
		m_x += x;
		m_y += y;
		return this;
	}

	vector2 *vector2::negate()
	{
		return negate(*this, this);
	}

	vector2 *vector2::normalize()
	{
		return normalize(*this, this);
	}

	vector2 *vector2::scale(const float &scalar)
	{
		return scale(*this, scalar, this);
	}

	bool vector2::isZero() const
	{
		return m_x == 0.0f && m_y == 0.0f;
	}

	float vector2::length() const
	{
		return length(*this);
	}

	float vector2::lengthSquared() const
	{
		return lengthSquared(*this);
	}
}
