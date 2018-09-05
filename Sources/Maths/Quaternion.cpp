#include "Quaternion.hpp"

#include <cassert>
#include "Maths.hpp"

namespace acid
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
		float halfPitch = pitch * DEG_TO_RAD * 0.5f;
		float halfYaw = yaw * DEG_TO_RAD * 0.5f;
		float halfRoll = roll * DEG_TO_RAD * 0.5f;

		float cosYaw = std::cos(halfYaw);
		float sinYaw = std::sin(halfYaw);
		float cosPitch = std::cos(halfPitch);
		float sinPitch = std::sin(halfPitch);
		float cosRoll = std::cos(halfRoll);
		float sinRoll = std::sin(halfRoll);

		m_x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		m_y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
		m_z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		m_w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
	}

	Quaternion::Quaternion(const Vector3 &source, const float &w) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z),
		m_w(w)
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
		float diagonal = source[0][0] + source[1][1] + source[2][2];

		if (diagonal > 0.0f)
		{
			float w4 = std::sqrt(diagonal + 1.0f) * 2.0f;
			m_w = w4 / 4.0f;
			m_x = (source[2][1] - source[1][2]) / w4;
			m_y = (source[0][2] - source[2][0]) / w4;
			m_z = (source[1][0] - source[0][1]) / w4;
		}
		else if ((source[0][0] > source[1][1]) && (source[0][0] > source[2][2]))
		{
			float x4 = std::sqrt(1.0f + source[0][0] - source[1][1] - source[2][2]) * 2.0f;
			m_w = (source[2][1] - source[1][2]) / x4;
			m_x = x4 / 4.0f;
			m_y = (source[0][1] + source[1][0]) / x4;
			m_z = (source[0][2] + source[2][0]) / x4;
		}
		else if (source[1][1] > source[2][2])
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

	Quaternion::Quaternion(const Vector3 &axisX, const Vector3 &axisY, const Vector3 &axisZ)
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

	Quaternion::~Quaternion()
	{
	}

	Quaternion Quaternion::Add(const Quaternion &other) const
	{
		return Quaternion(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
	}

	Quaternion Quaternion::Subtract(const Quaternion &other) const
	{
		return Quaternion(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
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
		//	Matrix4 rotation = left.ToRotationMatrix();
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
		return m_w * other.m_w + m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
	}

	Quaternion Quaternion::Slerp(const Quaternion &other, const float &progression)
	{
		// Favor accuracy for native code builds.
		float cosAngle = Dot(other);
		float sign = 1.0f;

		// Enable shortest path rotation.
		if (cosAngle < 0.0f)
		{
			cosAngle = -cosAngle;
			sign = -1.0f;
		}

		float angle = acosf(cosAngle);
		float sinAngle = sinf(angle);
		float t1, t2;

		if (sinAngle > 0.001f)
		{
			float invSinAngle = 1.0f / sinAngle;
			t1 = std::sin((1.0f - progression) * angle) * invSinAngle;
			t2 = std::sin(progression * angle) * invSinAngle;
		}
		else
		{
			t1 = 1.0f - progression;
			t2 = progression;
		}

		return *this * t1 + (other * sign) * t2;
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
		result[1][0] = twoXY + twoZW;
		result[1][1] = wSquared - xSquared + ySquared - zSquared;
		result[1][2] = twoYZ - twoXW;
		result[2][0] = twoXZ - twoYW;
		result[2][1] = twoYZ + twoXW;
		result[2][2] = wSquared - xSquared - ySquared + zSquared;
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
		/*float sqx = m_x * m_x;
		float sqy = m_y * m_y;
		float sqz = m_z * m_z;
		float squ = m_w * m_w;
		float rollX = std::atan2(2.0f * (m_y * m_z + m_w * m_x), squ - sqx - sqy + sqz);
		float sarg = -2.f * (m_x * m_z - m_w * m_y);
		float pitchY = sarg <= -1.0f ? -0.5f * PI : (sarg >= 1.0f ? 0.5f * PI : std::asin(sarg));
		float yawZ = std::atan2(2 * (m_x * m_y + m_w * m_z), squ + sqx - sqy - sqz);

		return Vector3(pitchY * RAD_TO_DEG, yawZ * RAD_TO_DEG, rollX * RAD_TO_DEG);*/

		float check = 2.0f * (-m_y * m_z + m_w * m_x);

		if (check < -0.995f)
		{
			return Vector3(
				-90.0f,
				0.0f,
				-atan2f(2.0f * (m_x * m_z - m_w * m_y), 1.0f - 2.0f * (m_y * m_y + m_z * m_z)) * RAD_TO_DEG
			);
		}
		else if (check > 0.995f)
		{
			return Vector3(
				90.0f,
				0.0f,
				atan2f(2.0f * (m_x * m_z - m_w * m_y), 1.0f - 2.0f * (m_y * m_y + m_z * m_z)) * RAD_TO_DEG
			);
		}

		return Vector3(
			asinf(check) * RAD_TO_DEG,
			atan2f(2.0f * (m_x * m_z + m_w * m_y), 1.0f - 2.0f * (m_x * m_x + m_y * m_y)) * RAD_TO_DEG,
			atan2f(2.0f * (m_x * m_y + m_w * m_z), 1.0f - 2.0f * (m_x * m_x + m_z * m_z)) * RAD_TO_DEG
		);
	}

	void Quaternion::Decode(const Node &node)
	{
		m_x = node.GetChild<float>("x");
		m_y = node.GetChild<float>("y");
		m_z = node.GetChild<float>("z");
		m_w = node.GetChild<float>("w");
	}

	void Quaternion::Encode(Node &node) const
	{
		node.SetChild<float>("x", m_x);
		node.SetChild<float>("y", m_y);
		node.SetChild<float>("z", m_z);
		node.SetChild<float>("w", m_w);
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

	const float &Quaternion::operator[](const uint32_t &index) const
	{
		assert(index < 4);
		return m_elements[index];
	}

	float &Quaternion::operator[](const uint32_t &index)
	{
		assert(index < 4);
		return m_elements[index];
	}

	Quaternion operator+(const Quaternion &left, const Quaternion &right)
	{
		return left.Add(right);
	}

	Quaternion operator-(const Quaternion &left, const Quaternion &right)
	{
		return left.Subtract(right);
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
