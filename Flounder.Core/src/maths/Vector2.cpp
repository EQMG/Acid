#include "Vector2.hpp"

#include "Vector3.hpp"

namespace Flounder
{
	Vector2::Vector2() :
		m_x(0.0f),
		m_y(0.0f)
	{
	}

	Vector2::Vector2(const float &x, const float &y) :
		m_x(x),
		m_y(y)
	{
	}

	Vector2::Vector2(const Vector2 &source) :
		m_x(source.m_x),
		m_y(source.m_y)
	{
	}

	Vector2::Vector2(const Vector3 &source) :
		m_x(source.m_x),
		m_y(source.m_y)
	{
	}

	Vector2::~Vector2()
	{
	}

	Vector2 *Vector2::Set(const float &x, const float &y)
	{
		m_x = x;
		m_y = y;
		return this;
	}

	Vector2 *Vector2::Set(const Vector2 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		return this;
	}

	Vector2 *Vector2::Set(const Vector3 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		return this;
	}

	Vector2 *Vector2::Add(const Vector2 &left, const Vector2 &right, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		return destination->Set(left.m_x + right.m_x, left.m_y + right.m_y);
	}

	Vector2 *Vector2::Subtract(const Vector2 &left, const Vector2 &right, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		return destination->Set(left.m_x - right.m_x, left.m_y - right.m_y);
	}

	Vector2 *Vector2::Multiply(const Vector2 &left, const Vector2 &right, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		return destination->Set(left.m_x * right.m_x, left.m_y * right.m_y);
	}

	Vector2 *Vector2::Divide(const Vector2 &left, const Vector2 &right, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		return destination->Set(left.m_x / right.m_x, left.m_y / right.m_y);
	}

	float Vector2::Angle(const Vector2 &left, const Vector2 &right)
	{
		float dls = Dot(left, right) / (Length(left) * Length(right));

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

	float Vector2::Dot(const Vector2 &left, const Vector2 &right)
	{
		return left.m_x * right.m_x + left.m_y * right.m_y;
	}

	Vector2 *Vector2::Scale(const Vector2 &source, const float &scalar, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		return destination->Set(source.m_x * scalar, source.m_y * scalar);
	}

	Vector2 *Vector2::Rotate(const Vector2 &source, const float &angle, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		float theta = Maths::Radians(angle);
		return destination->Set(source.m_x * cos(theta) - source.m_y * sin(theta), source.m_x * sin(theta) + source.m_y * cos(theta));
	}

	Vector2 *Vector2::Rotate(const Vector2 &source, const float &angle, const Vector2 &rotationAxis, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		float theta = Maths::Radians(angle);
		return destination->Set(((source.m_x - rotationAxis.m_x) * cos(theta)) - ((source.m_y - rotationAxis.m_y) * sin(theta) + rotationAxis.m_x), ((source.m_x - rotationAxis.m_x) * sin(theta)) + ((source.m_y - rotationAxis.m_y) * cos(theta) + rotationAxis.m_y));
	}

	Vector2 *Vector2::Negate(const Vector2 &source, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		return destination->Set(-source.m_x, -source.m_y);
	}

	Vector2 *Vector2::Normalize(const Vector2 &source, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		float l = Length(source);

		if (l != 0.0f)
		{
			return destination->Set(source.m_x / l, source.m_y / l);
		}
		throw std::invalid_argument("Zero length vector");
	}

	float Vector2::Length(const Vector2 &source)
	{
		return sqrt(LengthSquared(source));
	}

	float Vector2::LengthSquared(const Vector2 &source)
	{
		return source.m_x * source.m_x + source.m_y * source.m_y;
	}

	Vector2 *Vector2::MaxVector(const Vector2 &a, const Vector2 &b, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		return destination->Set(Maths::Max(a.m_x, b.m_x), Maths::Max(a.m_y, b.m_y));
	}

	Vector2 *Vector2::MinVector(const Vector2 &a, const Vector2 &b, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		return destination->Set(Maths::Min(a.m_x, b.m_x), Maths::Min(a.m_y, b.m_y));
	}

	float Vector2::MaxComponent(const Vector2 &vector)
	{
		return Maths::Max(vector.m_x, vector.m_y);
	}

	float Vector2::MinComponent(const Vector2 &vector)
	{
		return Maths::Min(vector.m_x, vector.m_y);
	}

	float Vector2::GetDistanceSquared(const Vector2 &point1, const Vector2 &point2)
	{
		float dx = point1.m_x - point2.m_x;
		float dy = point1.m_y - point2.m_y;
		return dx * dx + dy * dy;
	}

	float Vector2::GetDistance(const Vector2 &point1, const Vector2 &point2)
	{
		return sqrt(pow(point2.m_x - point1.m_x, 2) + pow(point2.m_y - point1.m_y, 2));
	}

	Vector2 *Vector2::GetVectorDistance(const Vector2 &point1, const Vector2 &point2, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		return destination->Set(pow(point2.m_x - point1.m_x, 2), pow(point2.m_y - point1.m_y, 2));
	}

	bool Vector2::PointInTriangle(const Vector2 &point, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3)
	{
		bool b1 = (point.m_x - v2.m_x) * (v1.m_y - v2.m_y) - (v1.m_x - v2.m_x) * (point.m_y - v2.m_y);
		bool b2 = (point.m_x - v3.m_x) * (v2.m_y - v3.m_y) - (v2.m_x - v3.m_x) * (point.m_y - v3.m_y);
		bool b3 = (point.m_x - v1.m_x) * (v3.m_y - v1.m_y) - (v3.m_x - v1.m_x) * (point.m_y - v1.m_y);
		return ((b1 == b2) & (b2 == b3));
	}

	Vector2 *Vector2::Translate(const float &x, const float &y)
	{
		m_x += x;
		m_y += y;
		return this;
	}

	Vector2 *Vector2::Negate()
	{
		return Negate(*this, this);
	}

	Vector2 *Vector2::Normalize()
	{
		return Normalize(*this, this);
	}

	Vector2 *Vector2::Scale(const float &scalar)
	{
		return Scale(*this, scalar, this);
	}

	bool Vector2::IsZero() const
	{
		return m_x == 0.0f && m_y == 0.0f;
	}

	float Vector2::Length() const
	{
		return Length(*this);
	}

	float Vector2::LengthSquared() const
	{
		return LengthSquared(*this);
	}
}
