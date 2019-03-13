#include "Quaternion.hpp"

namespace acid
{
const Quaternion Quaternion::Zero = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
const Quaternion Quaternion::One = Quaternion(1.0f, 1.0f, 1.0f, 1.0f);
const Quaternion Quaternion::OneW = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
const Quaternion Quaternion::PositiveInfinity = Quaternion(+std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());
const Quaternion Quaternion::NegativeInfinity = Quaternion(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

Quaternion::Quaternion(const float& x, const float& y, const float& z, const float& w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

Quaternion::Quaternion(const Vector3& source, const float& w) : m_x(source.m_x), m_y(source.m_y), m_z(source.m_z), m_w(w) {}

Quaternion::Quaternion(const Matrix4& source)
{
	float diagonal = source[0][0] + source[1][1] + source[2][2];

	if(diagonal > 0.0f)
		{
			float w4 = std::sqrt(diagonal + 1.0f) * 2.0f;
			m_w = w4 / 4.0f;
			m_x = (source[2][1] - source[1][2]) / w4;
			m_y = (source[0][2] - source[2][0]) / w4;
			m_z = (source[1][0] - source[0][1]) / w4;
		}
	else if((source[0][0] > source[1][1]) && (source[0][0] > source[2][2]))
		{
			float x4 = std::sqrt(1.0f + source[0][0] - source[1][1] - source[2][2]) * 2.0f;
			m_w = (source[2][1] - source[1][2]) / x4;
			m_x = x4 / 4.0f;
			m_y = (source[0][1] + source[1][0]) / x4;
			m_z = (source[0][2] + source[2][0]) / x4;
		}
	else if(source[1][1] > source[2][2])
		{
			float y4 = std::sqrt(1.0f + source[1][1] - source[0][0] - source[2][2]) * 2.0f;
			m_w = (source[0][2] - source[2][0]) / y4;
			m_x = (source[0][1] + source[1][0]) / y4;
			m_y = y4 / 4.0f;
			m_z = (source[1][2] + source[2][1]) / y4;
		}
	else
		{
			float z4 = std::sqrt(1.0f + source[2][2] - source[0][0] - source[1][1]) * 2.0f;
			m_w = (source[1][0] - source[0][1]) / z4;
			m_x = (source[0][2] + source[2][0]) / z4;
			m_y = (source[1][2] + source[2][1]) / z4;
			m_z = z4 / 4.0f;
		}
}

Quaternion::Quaternion(const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ)
{
	Matrix4 rotation = Matrix4();
	rotation[0][0] = axisX.m_x;
	rotation[1][0] = axisX.m_y;
	rotation[2][0] = axisX.m_z;
	rotation[0][1] = axisY.m_x;
	rotation[1][1] = axisY.m_y;
	rotation[2][1] = axisY.m_z;
	rotation[0][2] = axisZ.m_x;
	rotation[1][2] = axisZ.m_y;
	rotation[2][2] = axisZ.m_z;

	*this = rotation;
}

Quaternion Quaternion::Add(const Quaternion& other) const
{
	return Quaternion(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
}

Quaternion Quaternion::Subtract(const Quaternion& other) const
{
	return Quaternion(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
}

Quaternion Quaternion::Multiply(const Quaternion& other) const
{
	return Quaternion(m_x * other.m_w + m_w * other.m_x + m_y * other.m_z - m_z * other.m_y, m_y * other.m_w + m_w * other.m_y + m_z * other.m_x - m_x * other.m_z, m_z * other.m_w + m_w * other.m_z + m_x * other.m_y - m_y * other.m_x,
					  m_w * other.m_w - m_x * other.m_x - m_y * other.m_y - m_z * other.m_z);
}

Vector3 Quaternion::Multiply(const Vector3& other) const
{
	Vector3 q = Vector3(m_x, m_y, m_z);
	Vector3 cross1 = q.Cross(other);
	Vector3 cross2 = q.Cross(cross1);
	return other + 2.0f * (cross1 * m_w + cross2);
}

Quaternion Quaternion::MultiplyInverse(const Quaternion& other) const
{
	float n = other.LengthSquared();
	n = (n == 0.0f ? n : 1.0f / n);
	return Quaternion((m_x * other.m_w - m_w * other.m_x - m_y * other.m_z + m_z * other.m_y) * n, (m_y * other.m_w - m_w * other.m_y - m_z * other.m_x + m_x * other.m_z) * n, (m_z * other.m_w - m_w * other.m_z - m_x * other.m_y + m_y * other.m_x) * n,
					  (m_w * other.m_w + m_x * other.m_x + m_y * other.m_y + m_z * other.m_z) * n);
}

float Quaternion::Dot(const Quaternion& other) const
{
	return m_w * other.m_w + m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

Quaternion Quaternion::Slerp(const Quaternion& other, const float& progression) const
{
	float cosom = m_x * other.m_x + m_y * other.m_y + m_z * other.m_z + m_w * other.m_w;
	float absCosom = std::abs(cosom);
	float scale0, scale1;

	if(1.0f - absCosom > 1E-6f)
		{
			float sinSqr = 1.0f - absCosom * absCosom;
			float sinom = 1.0f / std::sqrt(sinSqr);
			float omega = std::atan2(sinSqr * sinom, absCosom);
			scale0 = std::sin((1.0f - progression) * omega) * sinom;
			scale1 = std::sin(progression * omega) * sinom;
		}
	else
		{
			scale0 = 1.0f - progression;
			scale1 = progression;
		}

	scale1 = cosom >= 0.0f ? scale1 : -scale1;
	Quaternion result = Quaternion();
	result.m_x = scale0 * m_x + scale1 * other.m_x;
	result.m_y = scale0 * m_y + scale1 * other.m_y;
	result.m_z = scale0 * m_z + scale1 * other.m_z;
	result.m_w = scale0 * m_w + scale1 * other.m_w;
	return result;
}

Quaternion Quaternion::Scale(const float& scalar) const
{
	return Quaternion(m_x * scalar, m_y * scalar, m_z * scalar, m_w * scalar);
}

Quaternion Quaternion::Negate() const
{
	return Quaternion(-m_x, -m_y, -m_z, -m_w);
}

Quaternion Quaternion::Normalize() const
{
	float l = Length();
	return Quaternion(m_x / l, m_y / l, m_z / l, m_w / l);
}

float Quaternion::LengthSquared() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

float Quaternion::Length() const
{
	return std::sqrt(LengthSquared());
}

float Quaternion::MaxComponent() const
{
	return std::max(m_x, std::max(m_y, std::max(m_z, m_w)));
}

float Quaternion::MinComponent() const
{
	return std::min(m_x, std::min(m_y, std::min(m_z, m_w)));
}

Matrix4 Quaternion::ToMatrix() const
{
	float w2 = m_w * m_w;
	float x2 = m_x * m_x;
	float y2 = m_y * m_y;
	float z2 = m_z * m_z;
	float zw = m_z * m_w;
	float xy = m_x * m_y;
	float xz = m_x * m_z;
	float yw = m_y * m_w;
	float yz = m_y * m_z;
	float xw = m_x * m_w;

	Matrix4 result = Matrix4();
	result[0][0] = w2 + x2 - z2 - y2;
	result[0][1] = xy + zw + zw + xy;
	result[0][2] = xz - yw + xz - yw;
	result[1][0] = -zw + xy - zw + xy;
	result[1][1] = y2 - z2 + w2 - x2;
	result[1][2] = yz + yz + xw + xw;
	result[2][0] = yw + xz + xz + yw;
	result[2][1] = yz + yz - xw - xw;
	result[2][2] = z2 - y2 - x2 + w2;
	return result;
}

Matrix4 Quaternion::ToRotationMatrix() const
{
	float xy = m_x * m_y;
	float xz = m_x * m_z;
	float xw = m_x * m_w;
	float yz = m_y * m_z;
	float yw = m_y * m_w;
	float zw = m_z * m_w;
	float xSquared = m_x * m_x;
	float ySquared = m_y * m_y;
	float zSquared = m_z * m_z;

	Matrix4 result = Matrix4();
	result[0][0] = 1.0f - 2.0f * (ySquared + zSquared);
	result[0][1] = 2.0f * (xy - zw);
	result[0][2] = 2.0f * (xz + yw);
	result[0][3] = 0.0f;
	result[1][0] = 2.0f * (xy + zw);
	result[1][1] = 1.0f - 2.0f * (xSquared + zSquared);
	result[1][2] = 2.0f * (yz - xw);
	result[1][3] = 0.0f;
	result[2][0] = 2.0f * (xz - yw);
	result[2][1] = 2.0f * (yz + xw);
	result[2][2] = 1.0f - 2.0f * (xSquared + ySquared);
	result[2][3] = 0.0f;
	result[3][0] = 0.0f;
	result[3][1] = 0.0f;
	result[3][2] = 0.0f;
	result[3][3] = 1.0f;
	return result;
}

Vector3 Quaternion::ToEuler() const
{
	Vector3 result = Vector3();
	result.m_x = std::atan2(2.0f * (m_x * m_w - m_y * m_z), 1.0f - 2.0f * (m_x * m_x + m_y * m_y));
	result.m_y = std::asin(2.0f * (m_x * m_z + m_y * m_w));
	result.m_z = std::atan2(2.0f * (m_z * m_w - m_x * m_y), 1.0f - 2.0f * (m_y * m_y + m_z * m_z));
	return result * Maths::RadToDeg;
}

void Quaternion::Decode(const Metadata& metadata)
{
	metadata.GetChild("x", m_x);
	metadata.GetChild("y", m_y);
	metadata.GetChild("z", m_z);
	metadata.GetChild("w", m_w);
}

void Quaternion::Encode(Metadata& metadata) const
{
	metadata.SetChild("x", m_x);
	metadata.SetChild("y", m_y);
	metadata.SetChild("z", m_z);
	metadata.SetChild("w", m_w);
}

bool Quaternion::operator==(const Quaternion& other) const
{
	return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z && m_w == other.m_w;
}

bool Quaternion::operator!=(const Quaternion& other) const
{
	return !(*this == other);
}

Quaternion Quaternion::operator-() const
{
	return Negate();
}

const float& Quaternion::operator[](const uint32_t& index) const
{
	assert(index < 4);
	return m_elements[index];
}

float& Quaternion::operator[](const uint32_t& index)
{
	assert(index < 4);
	return m_elements[index];
}

Quaternion operator+(const Quaternion& left, const Quaternion& right)
{
	return left.Add(right);
}

Quaternion operator-(const Quaternion& left, const Quaternion& right)
{
	return left.Subtract(right);
}

Quaternion operator*(const Quaternion& left, const Quaternion& right)
{
	return left.Multiply(right);
}

Vector3 operator*(const Vector3& left, const Quaternion& right)
{
	return right.Multiply(left);
}

Vector3 operator*(const Quaternion& left, const Vector3& right)
{
	return left.Multiply(right);
}

Quaternion operator*(const float& left, const Quaternion& right)
{
	return right.Scale(left);
}

Quaternion operator*(const Quaternion& left, const float& right)
{
	return left.Scale(right);
}

Quaternion& Quaternion::operator*=(const Quaternion& other)
{
	return *this = Multiply(other);
}

Quaternion& Quaternion::operator*=(const float& other)
{
	return *this = Scale(other);
}

std::ostream& operator<<(std::ostream& stream, const Quaternion& quaternion)
{
	stream << quaternion.ToString();
	return stream;
}

std::string Quaternion::ToString() const
{
	std::stringstream result;
	result.precision(10);
	result << "Quaternion(" << m_x << ", " << m_y << ", " << m_z << ", " << m_w << ")";
	return result.str();
}
}
