#include "Quaternion.hpp"

#include <cassert>
#include <sstream>
#include "Maths.hpp"

namespace fl
{
	const Quaternion Quaternion::ZERO = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	const Quaternion Quaternion::ONE = Quaternion(1.0f, 1.0f, 1.0f, 1.0f);
	const Quaternion Quaternion::POSITIVE_INFINITY = Quaternion(+INFINITY, +INFINITY, +INFINITY, +INFINITY);
	const Quaternion Quaternion::NEGATIVE_INFINITY = Quaternion(-INFINITY, -INFINITY, -INFINITY, -INFINITY);

	Quaternion::Quaternion() :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f),
		m_w(1.0f)
	{
	}

	Quaternion::Quaternion(const float &x, const float &y, const float &z, const float &w) :
		m_x(x),
		m_y(y),
		m_z(z),
		m_w(w)
	{
	}

	Quaternion::Quaternion(const Vector4 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z),
		m_w(source.m_w)
	{
	}

	Quaternion::Quaternion(const Quaternion &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z),
		m_w(source.m_w)
	{
	}

	Quaternion::Quaternion(const Matrix4 &source) :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f),
		m_w(0.0f)
	{
		*this = source;
	}

	Quaternion::~Quaternion()
	{
	}

	Quaternion Quaternion::Multiply(const Quaternion &other) const
	{
		return Quaternion(m_x * other.m_w + m_w * other.m_x + m_y * other.m_z - m_z * other.m_y,
			m_y * other.m_w + m_w * other.m_y + m_z * other.m_x - m_x * other.m_z,
			m_z * other.m_w + m_w * other.m_z + m_x * other.m_y - m_y * other.m_x,
			m_w * other.m_w - m_x * other.m_x - m_y * other.m_y - m_z * other.m_z);
	}

	Quaternion Quaternion::MultiplyInverse(const Quaternion &other) const
	{
		float n = other.LengthSquared();
		n = (n == 0.0f ? n : 1.0f / n);
		return Quaternion(
			(m_x * other.m_w - m_w * other.m_x - m_y * other.m_z + m_z * other.m_y) * n,
			(m_y * other.m_w - m_w * other.m_y - m_z * other.m_x + m_x * other.m_z) * n,
			(m_z * other.m_w - m_w * other.m_z - m_x * other.m_y + m_y * other.m_x) * n,
			(m_w * other.m_w + m_x * other.m_x + m_y * other.m_y + m_z * other.m_z) * n);
	}

	float Quaternion::Dot(const Quaternion &other) const
	{
		return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z + m_w * other.m_w;
	}

	Quaternion Quaternion::Slerp(const Quaternion &other, const float &progression)
	{
		Quaternion start = this->Normalize();
		Quaternion end = other.Normalize();

		float d = start.m_x * end.m_x + start.m_y * end.m_y + start.m_z * end.m_z + start.m_w * end.m_w;
		float absDot = d < 0.0f ? -d : d;
		float scale0 = 1.0f - progression;
		float scale1 = progression;

		if ((1.0f - absDot) > 0.1f)
		{
			float angle = std::acos(absDot);
			float invSinTheta = 1.0f / std::sin(angle);
			scale0 = std::sin((1.0f - progression) * angle) * invSinTheta;
			scale1 = std::sin(progression * angle) * invSinTheta;
		}

		if (d < 0.0f)
		{
			scale1 = -scale1;
		}

		float x = (scale0 * start.m_x) + (scale1 * end.m_x);
		float y = (scale0 * start.m_y) + (scale1 * end.m_y);
		float z = (scale0 * start.m_z) + (scale1 * end.m_z);
		float w = (scale0 * start.m_w) + (scale1 * end.m_w);
		return Quaternion(x, y, z, w);
	}

	Quaternion Quaternion::Scale(const float &scalar) const
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
		assert(l != 0.0f && "Zero length quaternion!");
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

	Matrix4 Quaternion::ToMatrix()
	{
		float xSquared = m_x * m_x;
		float twoXY = 2.0f * m_x * m_y;
		float twoXZ = 2.0f * m_x * m_z;
		float twoXW = 2.0f * m_x * m_w;
		float ySquared = m_y * m_y;
		float twoYZ = 2.0f * m_y * m_z;
		float twoYW = 2.0f * m_y * m_w;
		float twoZW = 2.0f * m_z * m_w;
		float zSquared = m_z * m_z;
		float wSquared = m_w * m_w;

		Matrix4 result = Matrix4();
		result.m_00 = wSquared + xSquared - ySquared - zSquared;
		result.m_01 = twoXY - twoZW;
		result.m_02 = twoXZ + twoYW;
		result.m_03 = 0.0f;
		result.m_10 = twoXY + twoZW;
		result.m_11 = wSquared - xSquared + ySquared - zSquared;
		result.m_12 = twoYZ - twoXW;
		result.m_13 = 0.0f;
		result.m_20 = twoXZ - twoYW;
		result.m_21 = twoYZ + twoXW;
		result.m_22 = wSquared - xSquared - ySquared + zSquared;
		result.m_23 = 0.0f;
		result.m_30 = 0.0f;
		result.m_31 = 0.0f;
		result.m_32 = 0.0f;
		result.m_33 = 1.0f;
		return result;
	}

	Matrix4 Quaternion::ToRotationMatrix()
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
		result.m_00 = 1.0f - 2.0f * (ySquared + zSquared);
		result.m_01 = 2.0f * (xy - zw);
		result.m_02 = 2.0f * (xz + yw);
		result.m_03 = 0.0f;
		result.m_10 = 2.0f * (xy + zw);
		result.m_11 = 1.0f - 2.0f * (xSquared + zSquared);
		result.m_12 = 2.0f * (yz - xw);
		result.m_13 = 0.0f;
		result.m_20 = 2.0f * (xz - yw);
		result.m_21 = 2.0f * (yz + xw);
		result.m_22 = 1.0f - 2.0f * (xSquared + ySquared);
		result.m_23 = 0.0f;
		result.m_30 = 0.0f;
		result.m_31 = 0.0f;
		result.m_32 = 0.0f;
		result.m_33 = 1.0f;
		return result;
	}

	void Quaternion::Write(std::shared_ptr<LoadedValue> destination)
	{
		destination->SetChild<float>("x", m_x);
		destination->SetChild<float>("y", m_y);
		destination->SetChild<float>("z", m_z);
		destination->SetChild<float>("w", m_w);
	}

	Quaternion &Quaternion::operator=(const Quaternion &other)
	{
		m_x = other.m_x;
		m_y = other.m_y;
		m_z = other.m_z;
		return *this;
	}

	Quaternion &Quaternion::operator=(std::shared_ptr<LoadedValue> source)
	{
		m_x = source->GetChild("x")->Get<float>();
		m_y = source->GetChild("y")->Get<float>();
		m_z = source->GetChild("z")->Get<float>();
		m_w = source->GetChild("w")->Get<float>();
		return *this;
	}

	Quaternion &Quaternion::operator=(const Matrix4 &other)
	{
		float diagonal = other.m_00 + other.m_11 + other.m_22;

		if (diagonal > 0.0f)
		{
			float w4 = std::sqrt(diagonal + 1.0f) * 2.0f;
			m_w = w4 / 4.0f;
			m_x = (other.m_21 - other.m_12) / w4;
			m_y = (other.m_02 - other.m_20) / w4;
			m_z = (other.m_10 - other.m_01) / w4;
		}
		else if ((other.m_00 > other.m_11) && (other.m_00 > other.m_22))
		{
			float x4 = std::sqrt(1.0f + other.m_00 - other.m_11 - other.m_22) * 2.0f;
			m_w = (other.m_21 - other.m_12) / x4;
			m_x = x4 / 4.0f;
			m_y = (other.m_01 + other.m_10) / x4;
			m_z = (other.m_02 + other.m_20) / x4;
		}
		else if (other.m_11 > other.m_22)
		{
			float y4 = std::sqrt(1.0f + other.m_11 - other.m_00 - other.m_22 * 2.0f);
			m_w = (other.m_02 - other.m_20) / y4;
			m_x = (other.m_01 + other.m_10) / y4;
			m_y = y4 / 4.0f;
			m_z = (other.m_12 + other.m_21) / y4;
		}
		else
		{
			float z4 = std::sqrt(1.0f + other.m_22 - other.m_00 - other.m_11) * 2.0f;
			m_w = (other.m_10 - other.m_01) / z4;
			m_x = (other.m_02 + other.m_20) / z4;
			m_y = (other.m_12 + other.m_21) / z4;
			m_z = z4 / 4.0f;
		}

		return *this;
	}

	bool Quaternion::operator==(const Quaternion &other) const
	{
		return m_x == other.m_x && m_y == other.m_x && m_z == other.m_z && m_w == other.m_w;
	}

	bool Quaternion::operator!=(const Quaternion &other) const
	{
		return !(*this == other);
	}

	bool Quaternion::operator<(const Quaternion &other) const
	{
		return m_x < other.m_x && m_y < other.m_y && m_z < other.m_z && m_w < other.m_w;
	}

	bool Quaternion::operator<=(const Quaternion &other) const
	{
		return m_x <= other.m_x && m_y <= other.m_y && m_z <= other.m_z && m_w <= other.m_w;
	}

	bool Quaternion::operator>(const Quaternion &other) const
	{
		return m_x > other.m_x && m_y > other.m_y && m_z > other.m_z && m_w > other.m_w;
	}

	bool Quaternion::operator>=(const Quaternion &other) const
	{
		return m_x >= other.m_x && m_y >= other.m_y && m_z >= other.m_z && m_w >= other.m_w;
	}

	bool Quaternion::operator==(const float &value) const
	{
		return m_x == value && m_y == value && m_z == value && m_w == value;
	}

	bool Quaternion::operator!=(const float &value) const
	{
		return !(*this == value);
	}

	Quaternion Quaternion::operator-()
	{
		return Negate();
	}

	Quaternion operator*(Quaternion left, const Quaternion &right)
	{
		return left.Multiply(right);
	}

	Quaternion operator*(Quaternion left, float value)
	{
		return left.Multiply(Quaternion(value, value, value, value));
	}

	Quaternion operator*(float value, Quaternion left)
	{
		return Quaternion(value, value, value, value).Multiply(left);
	}

	Quaternion &Quaternion::operator*=(const Quaternion &other)
	{
		return *this = Multiply(other);
	}

	Quaternion &Quaternion::operator*=(float value)
	{
		return *this = Multiply(Quaternion(value, value, value, value));
	}

	std::ostream &operator<<(std::ostream &stream, const Quaternion &vector)
	{
		stream << vector.ToString();
		return stream;
	}

	std::string Quaternion::ToString() const
	{
		std::stringstream result;
		result << "Quaternion(" << m_x << ", " << m_y << ", " << m_z << ", " << m_w << ")";
		return result.str();
	}
}
