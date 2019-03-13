#include "Vector4.hpp"

#include "Colour.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

namespace acid
{
const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::One = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 Vector4::PositiveInfinity = Vector4(+std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());
const Vector4 Vector4::NegativeInfinity = Vector4(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

Vector4::Vector4(const float& x, const float& y, const float& z, const float& w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

Vector4::Vector4(const Vector2& a, const Vector2& b) : m_x(a.m_x), m_y(a.m_y), m_z(b.m_x), m_w(b.m_y) {}

Vector4::Vector4(const Vector3& source, const float& w) : m_x(source.m_x), m_y(source.m_y), m_z(source.m_z), m_w(w) {}

Vector4::Vector4(const Colour& source) : m_x(source.m_r), m_y(source.m_g), m_z(source.m_b), m_w(source.m_a) {}

Vector4 Vector4::Add(const Vector4& other) const
{
	return Vector4(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
}

Vector4 Vector4::Subtract(const Vector4& other) const
{
	return Vector4(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
}

Vector4 Vector4::Multiply(const Vector4& other) const
{
	return Vector4(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z, m_w * other.m_w);
}

Vector4 Vector4::Divide(const Vector4& other) const
{
	return Vector4(m_x / other.m_x, m_y / other.m_y, m_z / other.m_z, m_w / other.m_w);
}

float Vector4::Angle(const Vector4& other) const
{
	float dls = Dot(other) / (Length() * other.Length());

	if(dls < -1.0f)
		{
			dls = -1.0f;
		}
	else if(dls > 1.0f)
		{
			dls = 1.0f;
		}

	return std::acos(dls);
}

float Vector4::Dot(const Vector4& other) const
{
	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z + m_w * other.m_w;
}

Vector4 Vector4::Lerp(const Vector4& other, const float& progression) const
{
	Vector4 ta = *this * (1.0f - progression);
	Vector4 tb = other * progression;
	return ta + tb;
}

Vector4 Vector4::Scale(const float& scalar) const
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

	if(l == 0.0f)
		{
			throw std::runtime_error("Can't normalize a zero length vector");
		}

	return *this / l;
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

float Vector4::DistanceSquared(const Vector4& other) const
{
	float dx = m_x - other.m_x;
	float dy = m_y - other.m_y;
	float dz = m_z - other.m_z;
	float dw = m_w - other.m_w;
	return dx * dx + dy * dy + dz * dz + dw * dw;
}

float Vector4::Distance(const Vector4& other) const
{
	return std::sqrt(DistanceSquared(other));
}

Vector4 Vector4::DistanceVector(const Vector4& other) const
{
	float dx = m_x - other.m_x;
	float dy = m_y - other.m_y;
	float dz = m_z - other.m_z;
	float dw = m_w - other.m_w;
	return Vector4(dx * dx, dy * dy, dz * dz, dw * dw);
}

Vector4 Vector4::SmoothDamp(const Vector4& target, const Vector4& rate) const
{
	return Vector4(Maths::SmoothDamp(m_x, target.m_x, rate.m_x), Maths::SmoothDamp(m_y, target.m_y, rate.m_y), Maths::SmoothDamp(m_z, target.m_z, rate.m_z), Maths::SmoothDamp(m_w, target.m_w, rate.m_w));
}

Vector4 Vector4::MinVector(const Vector4& a, const Vector4& b)
{
	return Vector4(std::min(a.m_x, b.m_x), std::min(a.m_y, b.m_y), std::min(a.m_z, b.m_z), std::min(a.m_w, b.m_w));
}

Vector4 Vector4::MaxVector(const Vector4& a, const Vector4& b)
{
	return Vector4(std::max(a.m_x, b.m_x), std::max(a.m_y, b.m_y), std::max(a.m_z, b.m_z), std::max(a.m_w, b.m_w));
}

void Vector4::Decode(const Metadata& metadata)
{
	metadata.GetChild("x", m_x);
	metadata.GetChild("y", m_y);
	metadata.GetChild("z", m_z);
	metadata.GetChild("w", m_w);
}

void Vector4::Encode(Metadata& metadata) const
{
	metadata.SetChild("x", m_x);
	metadata.SetChild("y", m_y);
	metadata.SetChild("z", m_z);
	metadata.SetChild("w", m_w);
}

bool Vector4::operator==(const Vector4& other) const
{
	return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z && m_w == other.m_w;
}

bool Vector4::operator!=(const Vector4& other) const
{
	return !(*this == other);
}

Vector4 Vector4::operator-() const
{
	return Negate();
}

const float& Vector4::operator[](const uint32_t& index) const
{
	assert(index < 4);
	return m_elements[index];
}

float& Vector4::operator[](const uint32_t& index)
{
	assert(index < 4);
	return m_elements[index];
}

Vector4 operator+(const Vector4& left, const Vector4& right)
{
	return left.Add(right);
}

Vector4 operator-(const Vector4& left, const Vector4& right)
{
	return left.Subtract(right);
}

Vector4 operator*(const Vector4& left, const Vector4& right)
{
	return left.Multiply(right);
}

Vector4 operator/(const Vector4& left, const Vector4& right)
{
	return left.Divide(right);
}

Vector4 operator+(const float& left, const Vector4& right)
{
	return Vector4(left, left, left, left).Add(right);
}

Vector4 operator-(const float& left, const Vector4& right)
{
	return Vector4(left, left, left, left).Subtract(right);
}

Vector4 operator*(const float& left, const Vector4& right)
{
	return Vector4(left, left, left, left).Multiply(right);
}

Vector4 operator/(const float& left, const Vector4& right)
{
	return Vector4(left, left, left, left).Divide(right);
}

Vector4 operator+(const Vector4& left, const float& right)
{
	return left.Add(Vector4(right, right, right, right));
}

Vector4 operator-(const Vector4& left, const float& right)
{
	return left.Subtract(Vector4(right, right, right, right));
}

Vector4 operator*(const Vector4& left, const float& right)
{
	return left.Multiply(Vector4(right, right, right, right));
}

Vector4 operator/(const Vector4& left, const float& right)
{
	return left.Divide(Vector4(right, right, right, right));
}

Vector4& Vector4::operator+=(const Vector4& other)
{
	return *this = Add(other);
}

Vector4& Vector4::operator-=(const Vector4& other)
{
	return *this = Subtract(other);
}

Vector4& Vector4::operator*=(const Vector4& other)
{
	return *this = Multiply(other);
}

Vector4& Vector4::operator/=(const Vector4& other)
{
	return *this = Divide(other);
}

Vector4& Vector4::operator+=(const float& other)
{
	return *this = Add(Vector4(other, other, other, other));
}

Vector4& Vector4::operator-=(const float& other)
{
	return *this = Subtract(Vector4(other, other, other, other));
}

Vector4& Vector4::operator*=(const float& other)
{
	return *this = Multiply(Vector4(other, other, other, other));
}

Vector4& Vector4::operator/=(const float& other)
{
	return *this = Divide(Vector4(other, other, other, other));
}

std::ostream& operator<<(std::ostream& stream, const Vector4& vector)
{
	stream << vector.ToString();
	return stream;
}

std::string Vector4::ToString() const
{
	std::stringstream result;
	result.precision(10);
	result << "Vector4(" << m_x << ", " << m_y << ", " << m_z << ", " << m_w << ")";
	return result.str();
}
}
