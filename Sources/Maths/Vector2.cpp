#include "Vector2.hpp"

#include <cassert>
#include "Network/Packet.hpp"
#include "Serialized/Metadata.hpp"
#include "Vector3.hpp"
#include "Maths.hpp"

namespace acid
{
	const Vector2 Vector2::ZERO = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::ONE = Vector2(1.0f, 1.0f);
	const Vector2 Vector2::LEFT = Vector2(-1.0f, 0.0f);
	const Vector2 Vector2::RIGHT = Vector2(1.0f, 0.0f);
	const Vector2 Vector2::UP = Vector2(0.0f, 1.0f);
	const Vector2 Vector2::DOWN = Vector2(0.0f, -1.0f);
	const Vector2 Vector2::POSITIVE_INFINITY = Vector2(+std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());
	const Vector2 Vector2::NEGATIVE_INFINITY = Vector2(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

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

	Vector2 Vector2::Lerp(const Vector2 &other, const float &progression) const
	{
		Vector2 ta = *this * (1.0f - progression);
		Vector2 tb = other * progression;
		return ta + tb;
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

	void Vector2::Decode(const Metadata &metadata)
	{
		m_x = metadata.GetChild<float>("x");
		m_y = metadata.GetChild<float>("y");
	}

	void Vector2::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("x", m_x);
		metadata.SetChild<float>("y", m_y);
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

	Vector2 Vector2::operator-() const
	{
		return Negate();
	}

	const float &Vector2::operator[](const uint32_t &index) const
	{
		assert(index < 2);
		return m_elements[index];
	}

	float &Vector2::operator[](const uint32_t &index)
	{
		assert(index < 2);
		return m_elements[index];
	}

	Vector2 operator+(const Vector2 &left, const Vector2 &right)
	{
		return left.Add(right);
	}

	Vector2 operator-(const Vector2 &left, const Vector2 &right)
	{
		return left.Subtract(right);
	}

	Vector2 operator*(const Vector2 &left, const Vector2 &right)
	{
		return left.Multiply(right);
	}

	Vector2 operator/(const Vector2 &left, const Vector2 &right)
	{
		return left.Divide(right);
	}

	Vector2 operator+(const float &left, const Vector2 &right)
	{
		return Vector2(left, left).Add(right);
	}

	Vector2 operator-(const float &left, const Vector2 &right)
	{
		return Vector2(left, left).Subtract(right);
	}

	Vector2 operator*(const float &left, const Vector2 &right)
	{
		return Vector2(left, left).Multiply(right);
	}

	Vector2 operator/(const float &left, const Vector2 &right)
	{
		return Vector2(left, left).Divide(right);
	}

	Vector2 operator+(const Vector2 &left, const float &right)
	{
		return left.Add(Vector2(right, right));
	}

	Vector2 operator-(const Vector2 &left, const float &right)
	{
		return left.Subtract(Vector2(right, right));
	}

	Vector2 operator*(const Vector2 &left, const float &right)
	{
		return left.Multiply(Vector2(right, right));
	}

	Vector2 operator/(const Vector2 &left, const float &right)
	{
		return left.Divide(Vector2(right, right));
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

	Vector2 &Vector2::operator+=(const float &other)
	{
		return *this = Add(Vector2(other, other));
	}

	Vector2 &Vector2::operator-=(const float &other)
	{
		return *this = Subtract(Vector2(other, other));
	}

	Vector2 &Vector2::operator*=(const float &other)
	{
		return *this = Multiply(Vector2(other, other));
	}

	Vector2 &Vector2::operator/=(const float &other)
	{
		return *this = Divide(Vector2(other, other));
	}

	std::ostream &operator<<(std::ostream &stream, const Vector2 &vector)
	{
		stream << vector.ToString();
		return stream;
	}

	Packet &operator<<(Packet &packet, const Vector2 &vector)
	{
		return packet << vector.m_x << vector.m_y;
	}

	Packet &operator>>(Packet &packet, Vector2 &vector)
	{
		return packet >> vector.m_x >> vector.m_y;
	}

	std::string Vector2::ToString() const
	{
		std::stringstream result;
		result << "Vector2(" << m_x << ", " << m_y << ")";
		return result.str();
	}
}
