#include "Vector2.hpp"

#include <cassert>
#include <sstream>
#include "Maths.hpp"
#include "Vector3.hpp"

namespace fl
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

	Vector2::~Vector2()
	{
	}

	Vector2 Vector2::Add(const Vector2 &other) const
	{
		return Vector2(m_x + other.m_x, m_y + other.m_y);
	}

	Vector2 Vector2::Subtract(const Vector2 &other) const
	{
		return Vector2(m_x - other.m_x, m_y - other.m_y);
	}

	Vector2 Vector2::Multiply(const Vector2 &other) const
	{
		return Vector2(m_x * other.m_x, m_y * other.m_y);
	}

	Vector2 Vector2::Divide(const Vector2 &other) const
	{
		return Vector2(m_x / other.m_x, m_y / other.m_y);
	}

	float Vector2::Angle(const Vector2 &other) const
	{
		float dls = Dot(other) / (Length() * other.Length());

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

	float Vector2::Dot(const Vector2 &other) const
	{
		return m_x * other.m_x + m_y * other.m_y;
	}

	Vector2 Vector2::Scale(const float &scalar) const
	{
		return Vector2(m_x * scalar, m_y * scalar);
	}

	Vector2 Vector2::Rotate(const float &angle) const
	{
		return Vector2(m_x * std::cos(angle) - m_y * std::sin(angle), m_x * std::sin(angle) + m_y * std::cos(angle));
	}

	Vector2 Vector2::Rotate(const float &angle, const Vector2 &rotationAxis) const
	{
		return Vector2(((m_x - rotationAxis.m_x) * std::cos(angle)) - ((m_y - rotationAxis.m_y) * std::sin(angle) + rotationAxis.m_x),
			((m_x - rotationAxis.m_x) * std::sin(angle)) + ((m_y - rotationAxis.m_y) * std::cos(angle) + rotationAxis.m_y));
	}

	Vector2 Vector2::Negate() const
	{
		return Vector2(-m_x, -m_y);
	}

	Vector2 Vector2::Normalize() const
	{
		float l = Length();
		assert(l != 0.0f && "Zero length vector!");
		return Vector2(m_x / l, m_y / l);
	}

	float Vector2::LengthSquared() const
	{
		return m_x * m_x + m_y * m_y;
	}

	float Vector2::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	float Vector2::MaxComponent() const
	{
		return std::max(m_x, m_y);
	}

	float Vector2::MinComponent() const
	{
		return std::min(m_x, m_y);
	}

	float Vector2::DistanceSquared(const Vector2 &other) const
	{
		float dx = m_x - other.m_x;
		float dy = m_y - other.m_y;
		return dx * dx + dy * dy;
	}

	float Vector2::Distance(const Vector2 &other) const
	{
		return std::sqrt(DistanceSquared(other));
	}

	Vector2 Vector2::DistanceVector(const Vector2 &other) const
	{
		float dx = m_x - other.m_x;
		float dy = m_y - other.m_y;
		return Vector2(dx * dx, dy * dy);
	}

	bool Vector2::InTriangle(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3) const
	{
		bool b1 = ((m_x - v2.m_x) * (v1.m_y - v2.m_y) - (v1.m_x - v2.m_x) * (m_y - v2.m_y)) < 0.0f;
		bool b2 = ((m_x - v3.m_x) * (v2.m_y - v3.m_y) - (v2.m_x - v3.m_x) * (m_y - v3.m_y)) < 0.0f;
		bool b3 = ((m_x - v1.m_x) * (v3.m_y - v1.m_y) - (v3.m_x - v1.m_x) * (m_y - v1.m_y)) < 0.0f;
		return ((b1 == b2) & (b2 == b3));
	}

	Vector2 Vector2::SmoothDamp(const Vector2 &target, const Vector2 &rate) const
	{
		return Vector2(Maths::SmoothDamp(m_x, target.m_x, rate.m_x), Maths::SmoothDamp(m_y, target.m_y, rate.m_y));
	}

	Vector2 Vector2::CartesianToPolar() const
	{
		float radius = std::sqrt(m_x * m_x + m_y * m_y);
		float theta = std::atan2(m_y, m_x);
		return Vector2(radius, theta);
	}

	Vector2 Vector2::PolarToCartesian() const
	{
		float x = m_x * std::cos(m_y);
		float y = m_x * std::sin(m_y);
		return Vector2(x, y);
	}

	void Vector2::Write(LoadedValue *destination)
	{
		destination->SetChild<float>("x", m_x);
		destination->SetChild<float>("y", m_y);
	}

	Vector2 &Vector2::operator=(const Vector2 &other)
	{
		m_x = other.m_x;
		m_y = other.m_y;
		return *this;
	}

	Vector2 &Vector2::operator=(LoadedValue *source)
	{
		m_x = source->GetChild("x")->Get<float>();
		m_y = source->GetChild("y")->Get<float>();
		return *this;
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

	Vector2 Vector2::operator-()
	{
		return Negate();
	}

	Vector2 operator+(Vector2 left, const Vector2 &right)
	{
		return left.Add(right);
	}

	Vector2 operator-(Vector2 left, const Vector2 &right)
	{
		return left.Subtract(right);
	}

	Vector2 operator*(Vector2 left, const Vector2 &right)
	{
		return left.Multiply(right);
	}

	Vector2 operator/(Vector2 left, const Vector2 &right)
	{
		return left.Divide(right);
	}

	Vector2 operator+(Vector2 left, float value)
	{
		return left.Add(Vector2(value, value));
	}

	Vector2 operator-(Vector2 left, float value)
	{
		return left.Subtract(Vector2(value, value));
	}

	Vector2 operator*(Vector2 left, float value)
	{
		return left.Multiply(Vector2(value, value));
	}

	Vector2 operator/(Vector2 left, float value)
	{
		return left.Divide(Vector2(value, value));
	}

	Vector2 operator+(float value, Vector2 left)
	{
		return Vector2(value, value).Add(left);
	}

	Vector2 operator-(float value, Vector2 left)
	{
		return Vector2(value, value).Subtract(left);
	}

	Vector2 operator*(float value, Vector2 left)
	{
		return Vector2(value, value).Multiply(left);
	}

	Vector2 operator/(float value, Vector2 left)
	{
		return Vector2(value, value).Divide(left);
	}

	Vector2 &Vector2::operator+=(const Vector2 &other)
	{
		return *this = Add(other);
	}

	Vector2 &Vector2::operator-=(const Vector2 &other)
	{
		return *this = Subtract(other);
	}

	Vector2 &Vector2::operator*=(const Vector2 &other)
	{
		return *this = Multiply(other);
	}

	Vector2 &Vector2::operator/=(const Vector2 &other)
	{
		return *this = Divide(other);
	}

	Vector2 &Vector2::operator+=(float value)
	{
		return *this = Add(Vector2(value, value));
	}

	Vector2 &Vector2::operator-=(float value)
	{
		return *this = Subtract(Vector2(value, value));
	}

	Vector2 &Vector2::operator*=(float value)
	{
		return *this = Multiply(Vector2(value, value));
	}

	Vector2 &Vector2::operator/=(float value)
	{
		return *this = Divide(Vector2(value, value));
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
