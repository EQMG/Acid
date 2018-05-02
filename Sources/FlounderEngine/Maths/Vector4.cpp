#include "Vector4.hpp"

#include <cassert>
#include <sstream>
#include "Maths.hpp"
#include "Colour.hpp"
#include "Vector3.hpp"

namespace fl
{
	const Vector4 Vector4::ZERO = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::ONE = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	const Vector4 Vector4::POSITIVE_INFINITY = Vector4(+INFINITY, +INFINITY, +INFINITY, +INFINITY);
	const Vector4 Vector4::NEGATIVE_INFINITY = Vector4(-INFINITY, -INFINITY, -INFINITY, -INFINITY);

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
		m_w(w)
	{
	}

	Vector4::Vector4(const Vector3 &source, const float &w) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z),
		m_w(w)
	{
	}

	Vector4::Vector4(const Vector4 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z),
		m_w(source.m_w)
	{
	}

	Vector4::Vector4(const Colour &source) :
		m_x(source.m_r),
		m_y(source.m_g),
		m_z(source.m_b),
		m_w(source.m_a)
	{
	}

	Vector4::~Vector4()
	{
	}

	Vector4 Vector4::Add(const Vector4 &other) const
	{
		return Vector4(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
	}

	Vector4 Vector4::Subtract(const Vector4 &other) const
	{
		return Vector4(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
	}

	Vector4 Vector4::Multiply(const Vector4 &other) const
	{
		return Vector4(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z, m_w * other.m_w);
	}

	Vector4 Vector4::Divide(const Vector4 &other) const
	{
		return Vector4(m_x / other.m_x, m_y / other.m_y, m_z / other.m_z, m_w / other.m_w);
	}

	float Vector4::Angle(const Vector4 &other) const
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

	float Vector4::Dot(const Vector4 &other) const
	{
		return m_x * other.m_x + m_y * other.m_y + m_w * other.m_w;
	}

	Vector4 Vector4::Scale(const float &scalar) const
	{
		return Vector4(m_x * scalar, m_y * scalar, m_z * scalar, m_w * scalar);
	}

	Vector4 Vector4::Negate() const
	{
		return Vector4(-m_x, -m_y, -m_z, -m_w);
	}

	Vector4 Vector4::Normalize() const
	{
		float l = Length();
		assert(l != 0.0f && "Zero length vector!");
		return Vector4(m_x / l, m_y / l, m_z / l, m_w / l);
	}

	float Vector4::LengthSquared() const
	{
		return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
	}

	float Vector4::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	float Vector4::MaxComponent() const
	{
		return std::max(m_x, std::max(m_y, std::max(m_z, m_w)));
	}

	float Vector4::MinComponent() const
	{
		return std::min(m_x, std::min(m_y, std::min(m_z, m_w)));
	}

	float Vector4::DistanceSquared(const Vector4 &other) const
	{
		float dx = m_x - other.m_x;
		float dy = m_y - other.m_y;
		float dz = m_z - other.m_z;
		float dw = m_w - other.m_w;
		return dx * dx + dy * dy + dz * dz + dw * dw;
	}

	float Vector4::Distance(const Vector4 &other) const
	{
		return std::sqrt(DistanceSquared(other));
	}

	Vector4 Vector4::DistanceVector(const Vector4 &other) const
	{
		float dx = m_x - other.m_x;
		float dy = m_y - other.m_y;
		float dz = m_z - other.m_z;
		float dw = m_w - other.m_w;
		return Vector4(dx * dx, dy * dy, dz * dz, dw * dw);
	}

	Vector4 Vector4::SmoothDamp(const Vector4 &target, const Vector4 &rate) const
	{
		return Vector4(Maths::SmoothDamp(m_x, target.m_x, rate.m_x), Maths::SmoothDamp(m_y, target.m_y, rate.m_y), Maths::SmoothDamp(m_z, target.m_z, rate.m_z), Maths::SmoothDamp(m_w, target.m_w, rate.m_w));
	}

	void Vector4::Write(LoadedValue *destination)
	{
		destination->SetChild<float>("x", m_x);
		destination->SetChild<float>("y", m_y);
		destination->SetChild<float>("z", m_z);
		destination->SetChild<float>("w", m_w);
	}

	Vector4 &Vector4::operator=(const Vector4 &other)
	{
		m_x = other.m_x;
		m_y = other.m_y;
		m_z = other.m_z;
		m_w = other.m_w;
		return *this;
	}

	Vector4 &Vector4::operator=(LoadedValue *source)
	{
		m_x = source->GetChild("x")->Get<float>();
		m_y = source->GetChild("y")->Get<float>();
		m_z = source->GetChild("z")->Get<float>();
		m_w = source->GetChild("w")->Get<float>();
		return *this;
	}

	bool Vector4::operator==(const Vector4 &other) const
	{
		return m_x == other.m_x && m_y == other.m_x && m_z == other.m_z && m_w == other.m_w;
	}

	bool Vector4::operator!=(const Vector4 &other) const
	{
		return !(*this == other);
	}

	bool Vector4::operator<(const Vector4 &other) const
	{
		return m_x < other.m_x && m_y < other.m_y && m_z < other.m_z && m_w < other.m_w;
	}

	bool Vector4::operator<=(const Vector4 &other) const
	{
		return m_x <= other.m_x && m_y <= other.m_y && m_z <= other.m_z && m_w <= other.m_w;
	}

	bool Vector4::operator>(const Vector4 &other) const
	{
		return m_x > other.m_x && m_y > other.m_y && m_z > other.m_z && m_w > other.m_w;
	}

	bool Vector4::operator>=(const Vector4 &other) const
	{
		return m_x >= other.m_x && m_y >= other.m_y && m_z >= other.m_z && m_w >= other.m_w;
	}

	bool Vector4::operator==(const float &value) const
	{
		return m_x == value && m_y == value && m_z == value && m_w == value;
	}

	bool Vector4::operator!=(const float &value) const
	{
		return !(*this == value);
	}

	Vector4 Vector4::operator-()
	{
		return Negate();
	}

	Vector4 operator+(Vector4 left, const Vector4 &right)
	{
		return left.Add(right);
	}

	Vector4 operator-(Vector4 left, const Vector4 &right)
	{
		return left.Subtract(right);
	}

	Vector4 operator*(Vector4 left, const Vector4 &right)
	{
		return left.Multiply(right);
	}

	Vector4 operator/(Vector4 left, const Vector4 &right)
	{
		return left.Divide(right);
	}

	Vector4 operator+(Vector4 left, float value)
	{
		return left.Add(Vector4(value, value, value, value));
	}

	Vector4 operator-(Vector4 left, float value)
	{
		return left.Subtract(Vector4(value, value, value, value));
	}

	Vector4 operator*(Vector4 left, float value)
	{
		return left.Multiply(Vector4(value, value, value, value));
	}

	Vector4 operator/(Vector4 left, float value)
	{
		return left.Divide(Vector4(value, value, value, value));
	}

	Vector4 operator+(float value, Vector4 left)
	{
		return Vector4(value, value, value, value).Add(left);
	}

	Vector4 operator-(float value, Vector4 left)
	{
		return Vector4(value, value, value, value).Subtract(left);
	}

	Vector4 operator*(float value, Vector4 left)
	{
		return Vector4(value, value, value, value).Multiply(left);
	}

	Vector4 operator/(float value, Vector4 left)
	{
		return Vector4(value, value, value, value).Divide(left);
	}

	Vector4 &Vector4::operator+=(const Vector4 &other)
	{
		return *this = Add(other);
	}

	Vector4 &Vector4::operator-=(const Vector4 &other)
	{
		return *this = Subtract(other);
	}

	Vector4 &Vector4::operator*=(const Vector4 &other)
	{
		return *this = Multiply(other);
	}

	Vector4 &Vector4::operator/=(const Vector4 &other)
	{
		return *this = Divide(other);
	}

	Vector4 &Vector4::operator+=(float value)
	{
		return *this = Add(Vector4(value, value, value, value));
	}

	Vector4 &Vector4::operator-=(float value)
	{
		return *this = Subtract(Vector4(value, value, value, value));
	}

	Vector4 &Vector4::operator*=(float value)
	{
		return *this = Multiply(Vector4(value, value, value, value));
	}

	Vector4 &Vector4::operator/=(float value)
	{
		return *this = Divide(Vector4(value, value, value, value));
	}

	std::ostream &operator<<(std::ostream &stream, const Vector4 &vector)
	{
		stream << vector.ToString();
		return stream;
	}

	std::string Vector4::ToString() const
	{
		std::stringstream result;
		result << "Vector4(" << m_x << ", " << m_y << ", " << m_z << ", " << m_w << ")";
		return result.str();
	}
}
