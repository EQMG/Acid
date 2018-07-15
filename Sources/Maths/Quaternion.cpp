#include "Quaternion.hpp"

#include <cassert>
#include "Maths.hpp"
#include "Vector3.hpp"

namespace fl
{
	const Quaternion Quaternion::ZERO = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	const Quaternion Quaternion::ONE = Quaternion(1.0f, 1.0f, 1.0f, 1.0f);
	const Quaternion Quaternion::W_ONE = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	const Quaternion Quaternion::POSITIVE_INFINITY = Quaternion(+std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());
	const Quaternion Quaternion::NEGATIVE_INFINITY = Quaternion(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

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

	Quaternion::Quaternion(const float &pitch, const float &yaw, const float &roll)
	{
		float halfPitch = Maths::Radians(pitch) * 0.5f;
		float halfYaw = Maths::Radians(yaw) * 0.5f;
		float halfRoll = Maths::Radians(roll) * 0.5f;

		float sinPitch = std::sin(halfPitch);
		float cosPitch = std::cos(halfPitch);
		float sinYaw = std::sin(halfYaw);
		float cosYaw = std::cos(halfYaw);
		float sinRoll = std::sin(halfRoll);
		float cosRoll = std::cos(halfRoll);

		m_x = cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll;
		m_y = sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll;
		m_z = cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll;
		m_w = cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll;
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

	Quaternion::Quaternion(const Matrix4 &source)
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

	Vector3 Quaternion::Multiply(const Vector3 &other) const
	{
	//	Matrix3 rotation = left.ToRotationMatrix3();
	//	return right * rotation;

		Vector3 q = Vector3(m_x, m_y, m_z);
		Vector3 cross1 = q.Cross(other);
		Vector3 cross2 = q.Cross(cross1);

		return other + 2.0f * (cross1 * m_w + cross2);
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

	Matrix4 Quaternion::ToMatrix() const
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
		result[0][0] = wSquared + xSquared - ySquared - zSquared;
		result[0][1] = twoXY - twoZW;
		result[0][2] = twoXZ + twoYW;
		result[0][3] = 0.0f;
		result[1][0] = twoXY + twoZW;
		result[1][1] = wSquared - xSquared + ySquared - zSquared;
		result[1][2] = twoYZ - twoXW;
		result[1][3] = 0.0f;
		result[2][0] = twoXZ - twoYW;
		result[2][1] = twoYZ + twoXW;
		result[2][2] = wSquared - xSquared - ySquared + zSquared;
		result[2][3] = 0.0f;
		result[3][0] = 0.0f;
		result[3][1] = 0.0f;
		result[3][2] = 0.0f;
		result[3][3] = 1.0f;
		return result;
	}

	Matrix3 Quaternion::ToRotationMatrix3() const
	{
		float tx = m_x + m_x;
		float ty = m_y + m_y;
		float fTz = m_z + m_z;
		float twx = tx * m_w;
		float twy = ty * m_w;
		float twz = fTz * m_w;
		float txx = tx * m_x;
		float txy = ty * m_x;
		float txz = fTz * m_x;
		float tyy = ty * m_y;
		float tyz = fTz * m_y;
		float tzz = fTz * m_z;

		Matrix3 result = Matrix3();
		result[0][0] = 1.0f - (tyy + tzz);
		result[0][1] = txy - twz;
		result[0][2] = txz + twy;
		result[1][0] = txy + twz;
		result[1][1] = 1.0f - (txx + tzz);
		result[1][2] = tyz - twx;
		result[2][0] = txz - twy;
		result[2][1] = tyz + twx;
		result[2][2] = 1.0f - (txx + tyy);
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
		Matrix3 matrix = ToRotationMatrix3();
		Vector3 result = Vector3();

		result.m_x = -Maths::Degrees(std::asin(matrix[1][2]));

		if (result.m_x < 90.0f)
		{
			if (result.m_x > -90.0f)
			{
				result.m_y = Maths::Degrees(std::atan2(matrix[0][2], matrix[2][2]));
				result.m_z = Maths::Degrees(std::atan2(matrix[1][0], matrix[1][1]));

				return result;
			}
			else
			{
				// Note: Not an unique solution.
				result.m_x = -90.0f;
				result.m_y = Maths::Degrees(std::atan2(-matrix[0][1], matrix[0][0]));
				result.m_z = 0.0f;

				return result;
			}
		}
		else
		{
			// Note: Not an unique solution.
			result.m_x = 90.0f;
			result.m_y = Maths::Degrees(std::atan2(matrix[0][1], matrix[0][0]));
			result.m_z = 0.0f;

			return result;
		}

		return result;
	}

	void Quaternion::Write(LoadedValue *destination)
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

	Quaternion &Quaternion::operator=(LoadedValue *source)
	{
		m_x = source->GetChild("x")->Get<float>();
		m_y = source->GetChild("y")->Get<float>();
		m_z = source->GetChild("z")->Get<float>();
		m_w = source->GetChild("w")->Get<float>();
		return *this;
	}

	Quaternion &Quaternion::operator=(const Matrix4 &other)
	{
		float diagonal = other[0][0] + other[1][1] + other[2][2];

		if (diagonal > 0.0f)
		{
			float w4 = std::sqrt(diagonal + 1.0f) * 2.0f;
			m_w = w4 / 4.0f;
			m_x = (other[2][1] - other[1][2]) / w4;
			m_y = (other[0][2] - other[2][0]) / w4;
			m_z = (other[1][0] - other[0][1]) / w4;
		}
		else if ((other[0][0] > other[1][1]) && (other[0][0] > other[2][2]))
		{
			float x4 = std::sqrt(1.0f + other[0][0] - other[1][1] - other[2][2]) * 2.0f;
			m_w = (other[2][1] - other[1][2]) / x4;
			m_x = x4 / 4.0f;
			m_y = (other[0][1] + other[1][0]) / x4;
			m_z = (other[0][2] + other[2][0]) / x4;
		}
		else if (other[1][1] > other[2][2])
		{
			float y4 = std::sqrt(1.0f + other[1][1] - other[0][0] - other[2][2] * 2.0f);
			m_w = (other[0][2] - other[2][0]) / y4;
			m_x = (other[0][1] + other[1][0]) / y4;
			m_y = y4 / 4.0f;
			m_z = (other[1][2] + other[2][1]) / y4;
		}
		else
		{
			float z4 = std::sqrt(1.0f + other[2][2] - other[0][0] - other[1][1]) * 2.0f;
			m_w = (other[1][0] - other[0][1]) / z4;
			m_x = (other[0][2] + other[2][0]) / z4;
			m_y = (other[1][2] + other[2][1]) / z4;
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

	Quaternion Quaternion::operator-() const
	{
		return Negate();
	}

	const float Quaternion::operator[](const uint32_t &index) const
	{
		assert(index < 4);
		return m_elements[index];
	}

	float Quaternion::operator[](const uint32_t &index)
	{
		assert(index < 4);
		return m_elements[index];
	}

	Quaternion operator*(const Quaternion &left, const Quaternion &right)
	{
		return left.Multiply(right);
	}

	Vector3 operator*(const Vector3 &left, const Quaternion &right)
	{
		return right.Multiply(left);
	}

	Vector3 operator*(const Quaternion &left, const Vector3 &right)
	{
		return left.Multiply(right);
	}

	Quaternion operator*(const float &left, const Quaternion &right)
	{
		return right.Scale(left);
	}

	Quaternion operator*(const Quaternion &left, const float &right)
	{
		return left.Scale(right);
	}

	Quaternion &Quaternion::operator*=(const Quaternion &other)
	{
		return *this = Multiply(other);
	}

	Quaternion &Quaternion::operator*=(const float &other)
	{
		return *this = Scale(other);
	}

	std::ostream &operator<<(std::ostream &stream, const Quaternion &quaternion)
	{
		stream << quaternion.ToString();
		return stream;
	}

	std::string Quaternion::ToString() const
	{
		std::stringstream result;
		result << "Quaternion(" << m_x << ", " << m_y << ", " << m_z << ", " << m_w << ")";
		return result.str();
	}
}
