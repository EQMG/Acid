#include "Vector2.hpp"

#include <cassert>
#include "Maths.hpp"
#include "Vector3.hpp"

namespace Flounder
{
	const Vector2 Vector2::ZERO = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::ONE = Vector2(1.0f, 1.0f);
	const Vector2 Vector2::LEFT = Vector2(-1.0f, 0.0f);
	const Vector2 Vector2::RIGHT = Vector2(1.0f, 0.0f);
	const Vector2 Vector2::UP = Vector2(0.0f, 1.0f);
	const Vector2 Vector2::DOWN = Vector2(0.0f, -1.0f);
	const Vector2 Vector2::POSITIVE_INFINITY = Vector2(+INFINITY, +INFINITY);
	const Vector2 Vector2::NEGATIVE_INFINITY = Vector2(-INFINITY, -INFINITY);

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

	Vector2::Vector2(LoadedValue *value)
	{
		Set(value);
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

	Vector2 *Vector2::Set(LoadedValue *value)
	{
		m_x = value->GetChild("x")->Get<float>();
		m_y = value->GetChild("y")->Get<float>();
		return this;
	}

	void Vector2::Write(LoadedValue *destination)
	{
		destination->SetChild<float>("x", m_x);
		destination->SetChild<float>("y", m_y);
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

		return std::acos(dls);
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

		const float theta = Maths::Radians(angle);
		return destination->Set(source.m_x * std::cos(theta) - source.m_y * std::sin(theta),
			source.m_x * std::sin(theta) + source.m_y * std::cos(theta));
	}

	Vector2 *Vector2::Rotate(const Vector2 &source, const float &angle, const Vector2 &rotationAxis, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		const float theta = Maths::Radians(angle);
		return destination->Set(((source.m_x - rotationAxis.m_x) * std::cos(theta)) - ((source.m_y - rotationAxis.m_y) * std::sin(theta) + rotationAxis.m_x),
			((source.m_x - rotationAxis.m_x) * std::sin(theta)) + ((source.m_y - rotationAxis.m_y) * std::cos(theta) + rotationAxis.m_y));
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

		const float l = Length(source);
		assert(l != 0.0f && "Zero length vector!");
		return destination->Set(source.m_x / l, source.m_y / l);
	}

	float Vector2::Length(const Vector2 &source)
	{
		return std::sqrt(LengthSquared(source));
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
		const float dx = point1.m_x - point2.m_x;
		const float dy = point1.m_y - point2.m_y;
		return dx * dx + dy * dy;
	}

	float Vector2::GetDistance(const Vector2 &point1, const Vector2 &point2)
	{
		return std::sqrt(std::pow(point2.m_x - point1.m_x, 2.0f) + std::pow(point2.m_y - point1.m_y, 2.0f));
	}

	Vector2 *Vector2::GetVectorDistance(const Vector2 &point1, const Vector2 &point2, Vector2 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector2();
		}

		return destination->Set(std::pow(point2.m_x - point1.m_x, 2.0f), std::pow(point2.m_y - point1.m_y, 2.0f));
	}

	bool Vector2::PointInTriangle(const Vector2 &point, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3)
	{
		const bool b1 = ((point.m_x - v2.m_x) * (v1.m_y - v2.m_y) - (v1.m_x - v2.m_x) * (point.m_y - v2.m_y)) < 0.0f;
		const bool b2 = ((point.m_x - v3.m_x) * (v2.m_y - v3.m_y) - (v2.m_x - v3.m_x) * (point.m_y - v3.m_y)) < 0.0f;
		const bool b3 = ((point.m_x - v1.m_x) * (v3.m_y - v1.m_y) - (v3.m_x - v1.m_x) * (point.m_y - v1.m_y)) < 0.0f;
		return ((b1 == b2) & (b2 == b3));
	}

	Vector2 Vector2::SmoothDamp(const Vector2 &current, const Vector2 &target, const Vector2 &rate)
	{
		return Vector2(Maths::SmoothDamp(current.m_x, target.m_x, rate.m_x),
			Maths::SmoothDamp(current.m_y, target.m_y, rate.m_y));
	}

	Vector2 Vector2::CartesianToPolar(const Vector2 &cartesian)
	{
		float radius = std::sqrt(cartesian.m_x * cartesian.m_x + cartesian.m_y * cartesian.m_y);
		float theta = std::atan(cartesian.m_y / cartesian.m_x);
		return Vector2(radius, theta);
	}

	Vector2 Vector2::PolarToCartesian(const Vector2 &polar)
	{
		float x = polar.m_x * std::cos(polar.m_y);
		float y = polar.m_x * std::sin(polar.m_y);
		return Vector2(x, y);
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

	float Vector2::Length() const
	{
		return Length(*this);
	}

	float Vector2::LengthSquared() const
	{
		return LengthSquared(*this);
	}

	Vector2 &Vector2::operator=(const Vector2 &other)
	{
		return *Set(other);
	}

	bool Vector2::operator==(const Vector2 &other) const
	{
		return m_x == other.m_x && m_y == other.m_x;
	}

	bool Vector2::operator!=(const Vector2 &other) const
	{
		return !(*this == other);
	}

	bool Vector2::operator<(const Vector2 &other) const
	{
		return m_x < other.m_x && m_y < other.m_y;
	}

	bool Vector2::operator<=(const Vector2 &other) const
	{
		return m_x <= other.m_x && m_y <= other.m_y;
	}

	bool Vector2::operator>(const Vector2 &other) const
	{
		return m_x > other.m_x && m_y > other.m_y;
	}

	bool Vector2::operator>=(const Vector2 &other) const
	{
		return m_x >= other.m_x && m_y >= other.m_y;
	}

	bool Vector2::operator==(const float &value) const
	{
		return m_x == value && m_y == value;
	}

	bool Vector2::operator!=(const float &value) const
	{
		return !(*this == value);
	}

	Vector2 &Vector2::operator-()
	{
		return *Vector2(*this).Negate();
	}

	Vector2 operator+(Vector2 left, const Vector2 &right)
	{
		return *Vector2::Add(left, right, &left);
	}

	Vector2 operator-(Vector2 left, const Vector2 &right)
	{
		return *Vector2::Subtract(left, right, &left);
	}

	Vector2 operator*(Vector2 left, const Vector2 &right)
	{
		return *Vector2::Multiply(left, right, &left);
	}

	Vector2 operator/(Vector2 left, const Vector2 &right)
	{
		return *Vector2::Divide(left, right, &left);
	}

	Vector2 operator+(Vector2 left, float value)
	{
		return *Vector2::Add(left, Vector2(value, value), &left);
	}

	Vector2 operator-(Vector2 left, float value)
	{
		return *Vector2::Subtract(left, Vector2(value, value), &left);
	}

	Vector2 operator*(Vector2 left, float value)
	{
		return *Vector2::Multiply(left, Vector2(value, value), &left);
	}

	Vector2 operator/(Vector2 left, float value)
	{
		return *Vector2::Divide(left, Vector2(value, value), &left);
	}

	Vector2 operator+(float value, Vector2 left)
	{
		return *Vector2::Add(Vector2(value, value), left, &left);
	}

	Vector2 operator-(float value, Vector2 left)
	{
		return *Vector2::Subtract(Vector2(value, value), left, &left);
	}

	Vector2 operator*(float value, Vector2 left)
	{
		return *Vector2::Multiply(Vector2(value, value), left, &left);
	}

	Vector2 operator/(float value, Vector2 left)
	{
		return *Vector2::Divide(Vector2(value, value), left, &left);
	}

	Vector2 &Vector2::operator+=(const Vector2 &other)
	{
		Vector2 result = Vector2();
		return *Vector2::Add(*this, other, &result);
	}

	Vector2 &Vector2::operator-=(const Vector2 &other)
	{
		Vector2 result = Vector2();
		return *Vector2::Subtract(*this, other, &result);
	}

	Vector2 &Vector2::operator*=(const Vector2 &other)
	{
		Vector2 result = Vector2();
		return *Vector2::Multiply(*this, other, &result);
	}

	Vector2 &Vector2::operator/=(const Vector2 &other)
	{
		Vector2 result = Vector2();
		return *Vector2::Divide(*this, other, &result);
	}

	Vector2 &Vector2::operator+=(float value)
	{
		return *Vector2::Add(*this, Vector2(value, value), this);
	}

	Vector2 &Vector2::operator-=(float value)
	{
		return *Vector2::Subtract(*this, Vector2(value, value), this);
	}

	Vector2 &Vector2::operator*=(float value)
	{
		return *Vector2::Multiply(*this, Vector2(value, value), this);
	}

	Vector2 &Vector2::operator/=(float value)
	{
		return *Vector2::Divide(*this, Vector2(value, value), this);
	}

	std::ostream &operator<<(std::ostream &stream, const Vector2 &vector)
	{
		stream << vector.ToString();
		return stream;
	}

	std::string Vector2::ToString() const
	{
		std::stringstream result;
		result << "Vector2(" << m_x << ", " << m_y << ")";
		return result.str();
	}
}
