#include "Vector3.hpp"

#include <cassert>
#include "Network/Packet.hpp"
#include "Serialized/Metadata.hpp"
#include "Colour.hpp"
#include "Matrix4.hpp"
#include "Quaternion.hpp"
#include "Vector2.hpp"
#include "Maths.hpp"

namespace acid
{
	const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);
	const Vector3 Vector3::ONE = Vector3(1.0f, 1.0f, 1.0f);
	const Vector3 Vector3::LEFT = Vector3(-1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::RIGHT = Vector3(1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::UP = Vector3(0.0f, 1.0f, 0.0f);
	const Vector3 Vector3::DOWN = Vector3(0.0f, -1.0f, 0.0f);
	const Vector3 Vector3::FRONT = Vector3(0.0f, 0.0f, 1.0f);
	const Vector3 Vector3::BACK = Vector3(0.0f, 0.0f, -1.0f);
	const Vector3 Vector3::POSITIVE_INFINITY = Vector3(+std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());
	const Vector3 Vector3::NEGATIVE_INFINITY = Vector3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

	Vector3::Vector3() :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f)
	{
	}

	Vector3::Vector3(const float &x, const float &y, const float &z) :
		m_x(x),
		m_y(y),
		m_z(z)
	{
	}

	Vector3::Vector3(const Vector2 &source, const float &z) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(z)
	{
	}

	Vector3::Vector3(const Vector3 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z)
	{
	}

	Vector3::Vector3(const Vector4 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z)
	{
	}

	Vector3::Vector3(const Colour &source) :
		m_x(source.m_r),
		m_y(source.m_g),
		m_z(source.m_b)
	{
	}

	Vector3 Vector3::Add(const Vector3 &other) const
	{
		return Vector3(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
	}

	Vector3 Vector3::Subtract(const Vector3 &other) const
	{
		return Vector3(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
	}

	Vector3 Vector3::Multiply(const Vector3 &other) const
	{
		return Vector3(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z);
	}

	Vector3 Vector3::Divide(const Vector3 &other) const
	{
		return Vector3(m_x / other.m_x, m_y / other.m_y, m_z / other.m_z);
	}

	float Vector3::Angle(const Vector3 &other) const
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

	float Vector3::Dot(const Vector3 &other) const
	{
		return m_x * other.m_x + m_y * other.m_y;
	}

	Vector3 Vector3::Cross(const Vector3 &other) const
	{
		return Vector3(m_y * other.m_z - m_z * other.m_y, other.m_x * m_z - other.m_z * m_x, m_x * other.m_y - m_y * other.m_x);
	}

	Vector3 Vector3::Lerp(const Vector3 &other, const float &progression) const
	{
		Vector3 ta = *this * (1.0f - progression);
		Vector3 tb = other * progression;
		return ta + tb;
	}

	Vector3 Vector3::Scale(const float &scalar) const
	{
		return Vector3(m_x * scalar, m_y * scalar, m_z * scalar);
	}

	Vector3 Vector3::Rotate(const Vector3 &rotation) const
	{
		Matrix4 matrix = Matrix4::TransformationMatrix(Vector3::ZERO, rotation, Vector3::ONE);
		Vector4 direction4 = Vector4(m_x, m_y, m_z, 1.0f);
		direction4 = matrix.Transform(direction4);
		return Vector3(direction4.m_x, direction4.m_y, direction4.m_z);
	}

	Vector3 Vector3::Negate() const
	{
		return Vector3(-m_x, -m_y, -m_z);
	}

	Vector3 Vector3::Normalize() const
	{
		float l = Length();
		return Vector3(m_x / l, m_y / l, m_z / l);
	}

	float Vector3::LengthSquared() const
	{
		return m_x * m_x + m_y * m_y + m_z * m_z;
	}

	float Vector3::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	float Vector3::MaxComponent() const
	{
		return std::max(m_x, std::max(m_y, m_z));
	}

	float Vector3::MinComponent() const
	{
		return std::min(m_x, std::min(m_y, m_z));
	}

	Quaternion Vector3::ToQuaternion() const
	{
		return Quaternion(m_x, m_y, m_z);
	}

	float Vector3::DistanceSquared(const Vector3 &other) const
	{
		float dx = m_x - other.m_x;
		float dy = m_y - other.m_y;
		float dz = m_z - other.m_z;
		return dx * dx + dy * dy + dz * dz;
	}

	float Vector3::Distance(const Vector3 &other) const
	{
		return std::sqrt(DistanceSquared(other));
	}

	Vector3 Vector3::DistanceVector(const Vector3 &other) const
	{
		float dx = m_x - other.m_x;
		float dy = m_y - other.m_y;
		float dz = m_z - other.m_z;
		return Vector3(dx * dx, dy * dy, dz * dz);
	}

	Vector3 Vector3::SmoothDamp(const Vector3 &target, const Vector3 &rate) const
	{
		return Vector3(Maths::SmoothDamp(m_x, target.m_x, rate.m_x), Maths::SmoothDamp(m_y, target.m_y, rate.m_y), Maths::SmoothDamp(m_z, target.m_z, rate.m_z));
	}

	Vector3 Vector3::CartesianToPolar() const
	{
		float radius = std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
		float theta = std::atan2(m_y, m_x);
		float phi = std::atan2(std::sqrt(m_x * m_x + m_y * m_y), m_z);
		return Vector3(radius, theta, phi);
	}

	Vector3 Vector3::PolarToCartesian() const
	{
		float x = m_x * std::sin(m_z) * std::cos(m_y);
		float y = m_x * std::sin(m_z) * std::sin(m_y);
		float z = m_x * std::cos(m_z);
		return Vector3(x, y, z);
	}

	Vector3 Vector3::ProjectCubeToSphere(const float &radius) const
	{
		if (radius == 0.0f)
		{
			return *this;
		}

		Vector3 cube = *this / radius;
		float dx = cube.m_x * cube.m_x;
		float dy = cube.m_y * cube.m_y;
		float dz = cube.m_z * cube.m_z;
		float sx = cube.m_x * std::sqrt(1.0f - (dy / 2.0f) - (dz / 2.0f) + (dy * dz / 3.0f));
		float sy = cube.m_y * std::sqrt(1.0f - (dz / 2.0f) - (dx / 2.0f) + (dz * dx / 3.0f));
		float sz = cube.m_z * std::sqrt(1.0f - (dx / 2.0f) - (dy / 2.0f) + (dx * dy / 3.0f));
		return Vector3(sx * radius, sy * radius, sz * radius);
	}

	float Vector3::BaryCentric(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
	{
		float det = (p2.m_z - p3.m_z) * (p1.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (p1.m_z - p3.m_z);
		float l1 = ((p2.m_z - p3.m_z) * (m_x - p3.m_x) + (p3.m_x - p2.m_x) * (m_y - p3.m_z)) / det;
		float l2 = ((p3.m_z - p1.m_z) * (m_x - p3.m_x) + (p1.m_x - p3.m_x) * (m_y - p3.m_z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.m_y + l2 * p2.m_y + l3 * p3.m_y;
	}

	Vector3 Vector3::MinVector(const Vector3 &a, const Vector3 &b)
	{
		return Vector3(std::min(a.m_x, b.m_x), std::min(a.m_y, b.m_y), std::min(a.m_z, b.m_z));
	}

	Vector3 Vector3::MaxVector(const Vector3 &a, const Vector3 &b)
	{
		return Vector3(std::max(a.m_x, b.m_x), std::max(a.m_y, b.m_y), std::max(a.m_z, b.m_z));
	}

	Vector3 Vector3::RandomUnitVector()
	{
		float theta = Maths::Random(0.0f, 1.0f) * 2.0f * PI;
		float z = Maths::Random(0.0f, 1.0f) * 2.0f - 1.0f;
		float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * std::cos(theta);
		float y = rootOneMinusZSquared * std::sin(theta);
		return Vector3(x, y, z);
	}

	Vector3 Vector3::RandomPointOnCircle(const Vector3 &normal, const float &radius)
	{
		Vector3 direction = Vector3();

		do
		{
			Vector3 randomVector = RandomUnitVector();
			direction = randomVector.Cross(normal);
		} while (direction.Length() == 0.0f);

		direction.Normalize();
		direction *= radius;

		float a = Maths::Random(0.0f, 1.0f);
		float b = Maths::Random(0.0f, 1.0f);

		if (a > b)
		{
			float temp = a;
			a = b;
			b = temp;
		}

		float randX = b * std::cos(2.0f * PI * (a / b));
		float randY = b * std::sin(2.0f * PI * (a / b));
		float distance = Vector3(randX, randY, 0.0f).Length();
		return Vector3(direction * distance);
	}

	Vector3 Vector3::RandomUnitVectorWithinCone(const Vector3 &coneDirection, const float &angle)
	{
		float cosAngle = std::cos(angle);
		float theta = Maths::Random(0.0f, 1.0f) * 2.0f * PI;
		float z = (cosAngle + Maths::Random(0.0f, 1.0f)) * (1.0f - cosAngle);
		float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * std::cos(theta);
		float y = rootOneMinusZSquared * std::sin(theta);

		Vector4 direction = Vector4(x, y, z, 1.0f);

		if (coneDirection.m_x != 0.0f || coneDirection.m_y != 0.0f || (coneDirection.m_z != 1.0f && coneDirection.m_z != -1.0f))
		{
			Vector3 rotateAxis = coneDirection.Cross(Vector3::FRONT);
			rotateAxis.Normalize();
			float rotateAngle = std::acos(coneDirection.Dot(Vector3::FRONT));
			Matrix4 rotationMatrix = Matrix4();
			rotationMatrix = rotationMatrix.Rotate(-rotateAngle, rotateAxis);
			direction = rotationMatrix.Transform(direction);
		}
		else if (coneDirection.m_z == -1.0f)
		{
			direction.m_z *= -1.0f;
		}

		return Vector3(direction);
	}

	void Vector3::Decode(const Metadata &metadata)
	{
		m_x = metadata.GetChild<float>("x");
		m_y = metadata.GetChild<float>("y");
		m_z = metadata.GetChild<float>("z");
	}

	void Vector3::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("x", m_x);
		metadata.SetChild<float>("y", m_y);
		metadata.SetChild<float>("z", m_z);
	}

	bool Vector3::operator==(const Vector3 &other) const
	{
		return m_x == other.m_x && m_y == other.m_x && m_z == other.m_z;
	}

	bool Vector3::operator!=(const Vector3 &other) const
	{
		return !(*this == other);
	}

	bool Vector3::operator<(const Vector3 &other) const
	{
		return m_x < other.m_x && m_y < other.m_y && m_z < other.m_z;
	}

	bool Vector3::operator<=(const Vector3 &other) const
	{
		return m_x <= other.m_x && m_y <= other.m_y && m_z <= other.m_z;
	}

	bool Vector3::operator>(const Vector3 &other) const
	{
		return m_x > other.m_x && m_y > other.m_y && m_z > other.m_z;
	}

	bool Vector3::operator>=(const Vector3 &other) const
	{
		return m_x >= other.m_x && m_y >= other.m_y && m_z >= other.m_z;
	}

	bool Vector3::operator==(const float &value) const
	{
		return m_x == value && m_y == value && m_z == value;
	}

	bool Vector3::operator!=(const float &value) const
	{
		return !(*this == value);
	}

	Vector3 Vector3::operator-() const
	{
		return Negate();
	}

	const float &Vector3::operator[](const uint32_t &index) const
	{
		assert(index < 3);
		return m_elements[index];
	}

	float &Vector3::operator[](const uint32_t &index)
	{
		assert(index < 3);
		return m_elements[index];
	}

	Vector3 operator+(const Vector3 &left, const Vector3 &right)
	{
		return left.Add(right);
	}

	Vector3 operator-(const Vector3 &left, const Vector3 &right)
	{
		return left.Subtract(right);
	}

	Vector3 operator*(const Vector3 &left, const Vector3 &right)
	{
		return left.Multiply(right);
	}

	Vector3 operator/(const Vector3 &left, const Vector3 &right)
	{
		return left.Divide(right);
	}

	Vector3 operator+(const float &left, const Vector3 &right)
	{
		return Vector3(left, left, left).Add(right);
	}

	Vector3 operator-(const float &left, const Vector3 &right)
	{
		return Vector3(left, left, left).Subtract(right);
	}

	Vector3 operator*(const float &left, const Vector3 &right)
	{
		return Vector3(left, left, left).Multiply(right);
	}

	Vector3 operator/(const float &left, const Vector3 &right)
	{
		return Vector3(left, left, left).Divide(right);
	}

	Vector3 operator+(const Vector3 &left, const float &right)
	{
		return left.Add(Vector3(right, right, right));
	}

	Vector3 operator-(const Vector3 &left, const float &right)
	{
		return left.Subtract(Vector3(right, right, right));
	}

	Vector3 operator*(const Vector3 &left, const float &right)
	{
		return left.Multiply(Vector3(right, right, right));
	}

	Vector3 operator/(const Vector3 &left, const float &right)
	{
		return left.Divide(Vector3(right, right, right));
	}

	Vector3 &Vector3::operator+=(const Vector3 &other)
	{
		return *this = Add(other);
	}

	Vector3 &Vector3::operator-=(const Vector3 &other)
	{
		return *this = Subtract(other);
	}

	Vector3 &Vector3::operator*=(const Vector3 &other)
	{
		return *this = Multiply(other);
	}

	Vector3 &Vector3::operator/=(const Vector3 &other)
	{
		return *this = Divide(other);
	}

	Vector3 &Vector3::operator+=(const float &other)
	{
		return *this = Add(Vector3(other, other, other));
	}

	Vector3 &Vector3::operator-=(const float &other)
	{
		return *this = Subtract(Vector3(other, other, other));
	}

	Vector3 &Vector3::operator*=(const float &other)
	{
		return *this = Multiply(Vector3(other, other, other));
	}

	Vector3 &Vector3::operator/=(const float &other)
	{
		return *this = Divide(Vector3(other, other, other));
	}

	std::ostream &operator<<(std::ostream &stream, const Vector3 &vector)
	{
		stream << vector.ToString();
		return stream;
	}

	Packet &operator<<(Packet &packet, const Vector3 &vector)
	{
		return packet << vector.m_x << vector.m_y << vector.m_z;
	}

	Packet &operator>>(Packet &packet, Vector3 &vector)
	{
		return packet >> vector.m_x >> vector.m_y >> vector.m_z;
	}

	std::string Vector3::ToString() const
	{
		std::stringstream result;
		result << "Vector3(" << m_x << ", " << m_y << ", " << m_z << ")";
		return result.str();
	}
}
