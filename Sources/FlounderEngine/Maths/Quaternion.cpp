#include "Quaternion.hpp"

#include <cassert>
#include "Maths.hpp"

namespace Flounder
{
	const Quaternion Quaternion::ZERO = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	const Quaternion Quaternion::ONE = Quaternion(1.0f, 1.0f, 1.0f, 1.0f);
	const Quaternion Quaternion::POSITIVE_INFINITY = Quaternion(+INFINITY, +INFINITY, +INFINITY, +INFINITY);
	const Quaternion Quaternion::NEGATIVE_INFINITY = Quaternion(-INFINITY, -INFINITY, -INFINITY, -INFINITY);

	Quaternion::Quaternion() :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f),
		m_w(0.0f)
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
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f),
		m_w(0.0f)
	{
		Set(source);
	}

	Quaternion::Quaternion(const Matrix4 &source) :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f),
		m_w(0.0f)
	{
		Set(source);
	}

	Quaternion::Quaternion(LoadedValue *value) :
		m_x(value->GetChild("x")->Get<float>()),
		m_y(value->GetChild("y")->Get<float>()),
		m_z(value->GetChild("z")->Get<float>()),
		m_w(value->GetChild("w")->Get<float>())
	{
	}

	Quaternion::~Quaternion()
	{
	}

	Quaternion *Quaternion::Set(const float &x, const float &y, const float &z, const float &w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
		return this;
	}

	Quaternion *Quaternion::Set(const Quaternion &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		m_w = source.m_w;
		return this;
	}

	Quaternion *Quaternion::Set(const Vector4 &source)
	{
		const float n = std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
		const float s = std::sin(0.5f * source.m_w) / n;
		m_x = source.m_x * s;
		m_y = source.m_y * s;
		m_z = source.m_z * s;
		m_w = std::cos(0.5f * source.m_w);
		return this;
	}

	Quaternion *Quaternion::Set(const Matrix4 &source)
	{
		m_x = 0.0f;
		m_y = 0.0f;
		m_z = 0.0f;
		m_w = 0.0f;

		const float diagonal = source.m_00 + source.m_11 + source.m_22;

		if (diagonal > 0.0f)
		{
			const float w4 = std::sqrt(diagonal + 1.0f) * 2.0f;
			m_w = w4 / 4.0f;
			m_x = (source.m_21 - source.m_12) / w4;
			m_y = (source.m_02 - source.m_20) / w4;
			m_z = (source.m_10 - source.m_01) / w4;
		}
		else if ((source.m_00 > source.m_11) && (source.m_00 > source.m_22))
		{
			const float x4 = std::sqrt(1.0f + source.m_00 - source.m_11 - source.m_22) * 2.0f;
			m_w = (source.m_21 - source.m_12) / x4;
			m_x = x4 / 4.0f;
			m_y = (source.m_01 + source.m_10) / x4;
			m_z = (source.m_02 + source.m_20) / x4;
		}
		else if (source.m_11 > source.m_22)
		{
			const float y4 = std::sqrt(1.0f + source.m_11 - source.m_00 - source.m_22 * 2.0f);
			m_w = (source.m_02 - source.m_20) / y4;
			m_x = (source.m_01 + source.m_10) / y4;
			m_y = y4 / 4.0f;
			m_z = (source.m_12 + source.m_21) / y4;
		}
		else
		{
			const float z4 = std::sqrt(1.0f + source.m_22 - source.m_00 - source.m_11) * 2.0f;
			m_w = (source.m_10 - source.m_01) / z4;
			m_x = (source.m_02 + source.m_20) / z4;
			m_y = (source.m_12 + source.m_21) / z4;
			m_z = z4 / 4.0f;
		}

		return this;
	}

	Quaternion *Quaternion::Set(const Matrix3 &source)
	{
		m_x = 0.0f;
		m_y = 0.0f;
		m_z = 0.0f;
		m_w = 0.0f;

		const float tr = source.m_00 + source.m_11 + source.m_22;

		if (tr >= 0.0)
		{
			float s = std::sqrt(tr + 1.0f);
			m_w = s * 0.5f;
			s = 0.5f / s;
			m_x = (source.m_21 - source.m_12) * s;
			m_y = (source.m_02 - source.m_20) * s;
			m_z = (source.m_10 - source.m_01) * s;
		}
		else
		{
			const float max = Maths::Max(Maths::Max(source.m_00, source.m_11), source.m_22);

			if (max == source.m_00)
			{
				float s = std::sqrt(source.m_00 - (source.m_11 + source.m_22) + 1.0f);
				m_x = s * 0.5f;
				s = 0.5f / s;
				m_y = (source.m_01 + source.m_10) * s;
				m_z = (source.m_20 + source.m_02) * s;
				m_w = (source.m_21 - source.m_12) * s;
			}
			else if (max == source.m_11)
			{
				float s = std::sqrt(source.m_11 - (source.m_22 + source.m_00) + 1.0f);
				m_y = s * 0.5f;
				s = 0.5f / s;
				m_z = (source.m_12 + source.m_21) * s;
				m_x = (source.m_01 + source.m_10) * s;
				m_w = (source.m_02 - source.m_20) * s;
			}
			else
			{
				float s = std::sqrt(source.m_22 - (source.m_00 + source.m_11) + 1.0f);
				m_z = s * 0.5f;
				s = 0.5f / s;
				m_x = (source.m_20 + source.m_02) * s;
				m_y = (source.m_12 + source.m_21) * s;
				m_w = (source.m_10 - source.m_01) * s;
			}
		}

		return this;
	}

	Quaternion *Quaternion::Set(LoadedValue *value)
	{
		m_x = value->GetChild("x")->Get<float>();
		m_y = value->GetChild("y")->Get<float>();
		m_z = value->GetChild("z")->Get<float>();
		m_w = value->GetChild("w")->Get<float>();
		return this;
	}

	void Quaternion::Write(LoadedValue *destination)
	{
		destination->SetChild<float>("x", m_x);
		destination->SetChild<float>("y", m_y);
		destination->SetChild<float>("z", m_z);
		destination->SetChild<float>("w", m_w);
	}

	Quaternion *Quaternion::SetIdentity(Quaternion *source)
	{
		source->m_x = 0.0f;
		source->m_y = 0.0f;
		source->m_z = 0.0f;
		source->m_w = 1.0f;
		return source;
	}

	Quaternion *Quaternion::Multiply(const Quaternion &left, const Quaternion &right, Quaternion *destination)
	{
		if (destination == nullptr)
		{
			destination = new Quaternion();
		}

		return destination->Set(
			left.m_x * right.m_w + left.m_w * right.m_x + left.m_y * right.m_z - left.m_z * right.m_y,
			left.m_y * right.m_w + left.m_w * right.m_y + left.m_z * right.m_x - left.m_x * right.m_z,
			left.m_z * right.m_w + left.m_w * right.m_z + left.m_x * right.m_y - left.m_y * right.m_x,
			left.m_w * right.m_w - left.m_x * right.m_x - left.m_y * right.m_y - left.m_z * right.m_z);
	}

	Quaternion *Quaternion::MultiplyInverse(const Quaternion &left, const Quaternion &right, Quaternion *destination)
	{
		if (destination == nullptr)
		{
			destination = new Quaternion();
		}

		float n = LengthSquared(right);
		n = (n == 0.0f ? n : 1.0f / n);
		return destination->Set(
			(left.m_x * right.m_w - left.m_w * right.m_x - left.m_y * right.m_z + left.m_z * right.m_y) * n,
			(left.m_y * right.m_w - left.m_w * right.m_y - left.m_z * right.m_x + left.m_x * right.m_z) * n,
			(left.m_z * right.m_w - left.m_w * right.m_z - left.m_x * right.m_y + left.m_y * right.m_x) * n,
			(left.m_w * right.m_w + left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z) * n);
	}

	float Quaternion::Dot(const Quaternion &left, const Quaternion &right)
	{
		return left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z + left.m_w * right.m_w;
	}

	Quaternion *Quaternion::Slerp(const Quaternion &left, const Quaternion &right, const float &progression, Quaternion *destination)
	{
		if (destination == nullptr)
		{
			destination = new Quaternion();
		}

		const float &d = left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z + left.m_w * right.m_w;
		const float absDot = d < 0.0f ? -d : d;
		float scale0 = 1.0f - progression;
		float scale1 = progression;

		if ((1.0f - absDot) > 0.1f)
		{
			const float &angle = std::acos(absDot);
			const float &invSinTheta = 1.0f / std::sin(angle);
			scale0 = std::sin((1.0f - progression) * angle) * invSinTheta;
			scale1 = std::sin(progression * angle) * invSinTheta;
		}

		if (d < 0.0f)
		{
			scale1 = -scale1;
		}

		const float newX = (scale0 * left.m_x) + (scale1 * right.m_x);
		const float newY = (scale0 * left.m_y) + (scale1 * right.m_y);
		const float newZ = (scale0 * left.m_z) + (scale1 * right.m_z);
		const float newW = (scale0 * left.m_w) + (scale1 * right.m_w);
		return destination->Set(newX, newY, newZ, newW);
	}

	Quaternion *Quaternion::Scale(const Quaternion &source, const float &scalar, Quaternion *destination)
	{
		if (destination == nullptr)
		{
			destination = new Quaternion();
		}

		return destination->Set(source.m_x * scalar, source.m_y * scalar, source.m_z * scalar, source.m_w * scalar);
	}

	Quaternion *Quaternion::Negate(const Quaternion &source, Quaternion *destination)
	{
		if (destination == nullptr)
		{
			destination = new Quaternion();
		}

		return destination->Set(-source.m_x, -source.m_y, -source.m_z, -source.m_w);
	}

	Quaternion *Quaternion::Normalize(const Quaternion &source, Quaternion *destination)
	{
		if (destination == nullptr)
		{
			destination = new Quaternion();
		}

		const float l = Length(source);
		assert(l != 0.0f && "Zero length vector!");
		return destination->Set(source.m_x / l, source.m_y / l, source.m_z / l, source.m_w / l);
	}

	float Quaternion::Length(const Quaternion &source)
	{
		return std::sqrt(LengthSquared(source));
	}

	float Quaternion::LengthSquared(const Quaternion &source)
	{
		return source.m_x * source.m_x + source.m_y * source.m_y + source.m_z * source.m_z + source.m_w * source.m_w;
	}

	Matrix4 *Quaternion::ToMatrix(const Quaternion &source, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		const float xSquared = source.m_x * source.m_x;
		const float twoXY = 2.0f * source.m_x * source.m_y;
		const float twoXZ = 2.0f * source.m_x * source.m_z;
		const float twoXW = 2.0f * source.m_x * source.m_w;
		const float ySquared = source.m_y * source.m_y;
		const float twoYZ = 2.0f * source.m_y * source.m_z;
		const float twoYW = 2.0f * source.m_y * source.m_w;
		const float twoZW = 2.0f * source.m_z * source.m_w;
		const float zSquared = source.m_z * source.m_z;
		const float wSquared = source.m_w * source.m_w;

		destination->m_00 = wSquared + xSquared - ySquared - zSquared;
		destination->m_01 = twoXY - twoZW;
		destination->m_02 = twoXZ + twoYW;
		destination->m_03 = 0.0f;
		destination->m_10 = twoXY + twoZW;
		destination->m_11 = wSquared - xSquared + ySquared - zSquared;
		destination->m_12 = twoYZ - twoXW;
		destination->m_13 = 0.0f;
		destination->m_20 = twoXZ - twoYW;
		destination->m_21 = twoYZ + twoXW;
		destination->m_22 = wSquared - xSquared - ySquared + zSquared;
		destination->m_23 = 0.0f;
		destination->m_30 = 0.0f;
		destination->m_31 = 0.0f;
		destination->m_32 = 0.0f;
		destination->m_33 = 1.0f;
		return destination;
	}

	Matrix4 *Quaternion::ToRotationMatrix(const Quaternion &source, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		const float xy = source.m_x * source.m_y;
		const float xz = source.m_x * source.m_z;
		const float xw = source.m_x * source.m_w;
		const float yz = source.m_y * source.m_z;
		const float yw = source.m_y * source.m_w;
		const float zw = source.m_z * source.m_w;
		const float xSquared = source.m_x * source.m_x;
		const float ySquared = source.m_y * source.m_y;
		const float zSquared = source.m_z * source.m_z;

		destination->m_00 = 1.0f - 2.0f * (ySquared + zSquared);
		destination->m_01 = 2.0f * (xy - zw);
		destination->m_02 = 2.0f * (xz + yw);
		destination->m_03 = 0.0f;
		destination->m_10 = 2.0f * (xy + zw);
		destination->m_11 = 1.0f - 2.0f * (xSquared + zSquared);
		destination->m_12 = 2.0f * (yz - xw);
		destination->m_13 = 0.0f;
		destination->m_20 = 2.0f * (xz - yw);
		destination->m_21 = 2.0f * (yz + xw);
		destination->m_22 = 1.0f - 2.0f * (xSquared + ySquared);
		destination->m_23 = 0.0f;
		destination->m_30 = 0.0f;
		destination->m_31 = 0.0f;
		destination->m_32 = 0.0f;
		destination->m_33 = 1.0f;
		return destination;
	}

	Quaternion *Quaternion::SetIdentity()
	{
		return SetIdentity(this);
	}

	Quaternion *Quaternion::Negate()
	{
		return Negate(*this, this);
	}

	Quaternion *Quaternion::Normalize()
	{
		return Normalize(*this, this);
	}

	Quaternion *Quaternion::Scale(const float &scalar)
	{
		return Scale(*this, scalar, this);
	}

	bool Quaternion::IsZero() const
	{
		return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f && m_w == 0.0f;
	}

	float Quaternion::Length() const
	{
		return Length(*this);
	}

	float Quaternion::LengthSquared() const
	{
		return LengthSquared(*this);
	}

	Quaternion &Quaternion::operator=(const Quaternion &other)
	{
		return *Set(other);
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

	Quaternion &Quaternion::operator-()
	{
		return *Quaternion(*this).Negate();
	}

	Quaternion operator*(Quaternion left, const Quaternion &right)
	{
		return *Quaternion::Multiply(left, right, &left);
	}

	Quaternion &Quaternion::operator*=(const Quaternion &other)
	{
		Quaternion result = Quaternion();
		return *Quaternion::Multiply(*this, other, &result);
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
