#include "Vector3.hpp"

#include <cassert>
#include <sstream>
#include "Colour.hpp"
#include "Maths.hpp"
#include "Vector2.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"

namespace Flounder
{
	const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);
	const Vector3 Vector3::ONE = Vector3(1.0f, 1.0f, 1.0f);
	const Vector3 Vector3::LEFT = Vector3(-1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::RIGHT = Vector3(1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::UP = Vector3(0.0f, 1.0f, 0.0f);
	const Vector3 Vector3::DOWN = Vector3(0.0f, -1.0f, 0.0f);
	const Vector3 Vector3::FRONT = Vector3(0.0f, 0.0f, 1.0f);
	const Vector3 Vector3::BACK = Vector3(0.0f, 0.0f, -1.0f);
	const Vector3 Vector3::POSITIVE_INFINITY = Vector3(+INFINITY, +INFINITY, +INFINITY);
	const Vector3 Vector3::NEGATIVE_INFINITY = Vector3(-INFINITY, -INFINITY, -INFINITY);

	Vector3::Vector3() :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f)
	{
	}

	Vector3::Vector3(const Vector2 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(0.0f)
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

	Vector3::Vector3(const float &x, const float &y, const float &z) :
		m_x(x),
		m_y(y),
		m_z(z)
	{
	}

	Vector3::Vector3(LoadedValue *value)
	{
		Set(value);
	}

	Vector3::~Vector3()
	{
	}

	Vector3 *Vector3::Set(const Vector2 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = 0.0f;
		return this;
	}

	Vector3 *Vector3::Set(const Vector3 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		return this;
	}

	Vector3 *Vector3::Set(const Vector4 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		return this;
	}

	Vector3 *Vector3::Set(const float &x, const float &y, const float &z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		return this;
	}

	Vector3 *Vector3::Set(LoadedValue *value)
	{
		m_x = value->GetChild("x")->Get<float>();
		m_y = value->GetChild("y")->Get<float>();
		m_z = value->GetChild("z")->Get<float>();
		return this;
	}

	void Vector3::Write(LoadedValue *destination)
	{
		destination->SetChild<float>("x", m_x);
		destination->SetChild<float>("y", m_y);
		destination->SetChild<float>("z", m_z);
	}

	Vector3 *Vector3::Add(const Vector3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(left.m_x + right.m_x, left.m_y + right.m_y, left.m_z + right.m_z);
	}

	Vector3 *Vector3::Subtract(const Vector3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(left.m_x - right.m_x, left.m_y - right.m_y, left.m_z - right.m_z);
	}

	Vector3 *Vector3::Multiply(const Vector3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(left.m_x * right.m_x, left.m_y * right.m_y, left.m_z * right.m_z);
	}

	Vector3 *Vector3::Divide(const Vector3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(left.m_x / right.m_x, left.m_y / right.m_y, left.m_z / right.m_z);
	}

	float Vector3::Angle(const Vector3 &left, const Vector3 &right)
	{
		float dls = Dot(left, right) / (Length(left) * Length(right));

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

	float Vector3::Dot(const Vector3 &left, const Vector3 &right)
	{
		return left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z;
	}

	Vector3 *Vector3::Cross(const Vector3 &left, const Vector3 &right, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(left.m_y * right.m_z - left.m_z * right.m_y,
			right.m_x * left.m_z - right.m_z * left.m_x, left.m_x * right.m_y - left.m_y * right.m_x);
	}

	Vector3 *Vector3::Scale(const Vector3 &source, const float &scalar, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(source.m_x * scalar, source.m_y * scalar, source.m_z * scalar);
	}

	Vector3 *Vector3::Rotate(const Vector3 &source, const Vector3 &rotation, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		Matrix4 *matrix = Matrix4::TransformationMatrix(Vector3::ZERO, rotation, Vector3::ONE, nullptr);
		Vector4 direction4 = Vector4(source.m_x, source.m_y, source.m_z, 1.0f);
		Matrix4::Transform(*matrix, direction4, &direction4);
		delete matrix;
		return destination->Set(direction4.m_x, direction4.m_y, direction4.m_z);
	}

	Vector3 *Vector3::Negate(const Vector3 &source, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(-source.m_x, -source.m_y, -source.m_z);
	}

	Vector3 *Vector3::Normalize(const Vector3 &source, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		const float l = Length(source);
		assert(l != 0.0f && "Zero length vector!");
		return destination->Set(source.m_x / l, source.m_y / l, source.m_z / l);
	}

	float Vector3::Length(const Vector3 &source)
	{
		return std::sqrt(LengthSquared(source));
	}

	float Vector3::LengthSquared(const Vector3 &source)
	{
		return source.m_x * source.m_x + source.m_y * source.m_y + source.m_z * source.m_z;
	}

	Vector3 *Vector3::MaxVector(const Vector3 &a, const Vector3 &b, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(Maths::Max(a.m_x, b.m_x), Maths::Max(a.m_y, b.m_y), Maths::Max(a.m_z, b.m_z));
	}

	Vector3 *Vector3::MinVector(const Vector3 &a, const Vector3 &b, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(Maths::Min(a.m_x, b.m_x), Maths::Min(a.m_y, b.m_y), Maths::Min(a.m_z, b.m_z));
	}

	float Vector3::MaxComponent(const Vector3 &vector)
	{
		return Maths::Max(vector.m_x, Maths::Max(vector.m_y, vector.m_z));
	}

	float Vector3::MinComponent(const Vector3 &vector)
	{
		return Maths::Min(vector.m_x, Maths::Min(vector.m_y, vector.m_z));
	}

	float Vector3::GetDistanceSquared(const Vector3 &point1, const Vector3 &point2)
	{
		const float dx = point1.m_x - point2.m_x;
		const float dy = point1.m_y - point2.m_y;
		const float dz = point1.m_z - point2.m_z;
		return dx * dx + dy * dy + dz * dz;
	}

	float Vector3::GetDistance(const Vector3 &point1, const Vector3 &point2)
	{
		return std::sqrt(std::pow(point2.m_x - point1.m_x, 2.0f) +
			std::pow(point2.m_y - point1.m_y, 2.0f) +
			std::pow(point2.m_z - point1.m_z, 2.0f));
	}

	Vector3 *Vector3::GetVectorDistance(const Vector3 &point1, const Vector3 &point2, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(std::pow(point2.m_x - point1.m_x, 2.0f), std::pow(point2.m_y - point1.m_y, 2.0f),
			std::pow(point2.m_z - point1.m_z, 2.0f));
	}

	Vector3 *Vector3::RandomUnitVector(Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		const float theta = Maths::RandomInRange(0.0f, 1.0f) * 2.0f * PI;
		const float z = Maths::RandomInRange(0.0f, 1.0f) * 2.0f - 1.0f;
		const float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
		const float x = rootOneMinusZSquared * std::cos(theta);
		const float y = rootOneMinusZSquared * std::sin(theta);
		return destination->Set(x, y, z);
	}

	Vector3 *Vector3::RandomPointOnCircle(const Vector3 &normal, const float &radius, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		do
		{
			Vector3 *randomVector = RandomUnitVector(nullptr);
			Cross(*randomVector, normal, destination);
			delete randomVector;
		} while (Length(*destination) == 0.0f);

		destination->Normalize();
		destination->Scale(radius);
		float a = Maths::RandomInRange(0.0f, 1.0f);
		float b = Maths::RandomInRange(0.0f, 1.0f);

		if (a > b)
		{
			const float temp = a;
			a = b;
			b = temp;
		}

		const float randX = b * std::cos(2.0f * PI * (a / b));
		const float randY = b * std::sin(2.0f * PI * (a / b));
		const float distance = Vector3(randX, randY, 0.0f).Length();
		destination->Scale(distance);
		return destination;
	}

	float Vector3::BaryCentric(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &pos)
	{
		const float det = (p2.m_z - p3.m_z) * (p1.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (p1.m_z - p3.m_z);
		const float l1 = ((p2.m_z - p3.m_z) * (pos.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (pos.m_y - p3.m_z)) / det;
		const float l2 = ((p3.m_z - p1.m_z) * (pos.m_x - p3.m_x) + (p1.m_x - p3.m_x) * (pos.m_y - p3.m_z)) / det;
		const float l3 = 1.0f - l1 - l2;
		return l1 * p1.m_y + l2 * p2.m_y + l3 * p3.m_y;
	}

	Vector3 *Vector3::RandomUnitVectorWithinCone(const Vector3 &coneDirection, const float &angle, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		const float cosAngle = std::cos(angle);
		const float theta = Maths::RandomInRange(0.0f, 1.0f) * 2.0f * PI;
		const float z = (cosAngle + Maths::RandomInRange(0.0f, 1.0f)) * (1.0f - cosAngle);
		const float rootOneMinusZSquared = std::sqrt(1.0f - z * z);
		const float x = rootOneMinusZSquared * std::cos(theta);
		const float y = rootOneMinusZSquared * std::sin(theta);

		Vector4 direction = Vector4(x, y, z, 1.0f);

		if ((coneDirection.m_x != 0.0F) || (coneDirection.m_y != 0.0F) ||
			((coneDirection.m_z != 1.0f) && (coneDirection.m_z != -1.0f)))
		{
			Vector3 *rotateAxis = Vector3::Cross(coneDirection, Vector3::FRONT, nullptr);
			rotateAxis->Normalize();
			const float rotateAngle = std::acos(Vector3::Dot(coneDirection, Vector3::FRONT));
			Matrix4 rotationMatrix = Matrix4();
			rotationMatrix.SetIdentity();
			Matrix4::Rotate(rotationMatrix, *rotateAxis, -rotateAngle, &rotationMatrix);
			Matrix4::Transform(rotationMatrix, direction, &direction);
			delete rotateAxis;
		}
		else if (coneDirection.m_z == -1.0f)
		{
			direction.m_z *= -1.0f;
		}

		return destination->Set(direction);
	}

	Vector3 *Vector3::Translate(const float &x, const float &y, const float &z)
	{
		m_x += x;
		m_y += y;
		m_z += z;
		return this;
	}

	Vector3 *Vector3::Negate()
	{
		return Negate(*this, this);
	}

	Vector3 *Vector3::Normalize()
	{
		return Normalize(*this, this);
	}

	Vector3 *Vector3::Scale(const float &scalar)
	{
		return Scale(*this, scalar, this);
	}

	float Vector3::Length() const
	{
		return Length(*this);
	}

	float Vector3::LengthSquared() const
	{
		return LengthSquared(*this);
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

	Vector3 &Vector3::operator-()
	{
		return *Vector3(*this).Negate();
	}

	Vector3 operator+(Vector3 left, const Vector3 &right)
	{
		return *Vector3::Add(left, right, &left);
	}

	Vector3 operator-(Vector3 left, const Vector3 &right)
	{
		return *Vector3::Subtract(left, right, &left);
	}

	Vector3 operator*(Vector3 left, const Vector3 &right)
	{
		return *Vector3::Multiply(left, right, &left);
	}

	Vector3 operator/(Vector3 left, const Vector3 &right)
	{
		return *Vector3::Divide(left, right, &left);
	}

	Vector3 operator+(Vector3 left, float value)
	{
		return *Vector3::Add(left, Vector3(value, value, value), &left);
	}

	Vector3 operator-(Vector3 left, float value)
	{
		return *Vector3::Subtract(left, Vector3(value, value, value), &left);
	}

	Vector3 operator*(Vector3 left, float value)
	{
		return *Vector3::Multiply(left, Vector3(value, value, value), &left);
	}

	Vector3 operator/(Vector3 left, float value)
	{
		return *Vector3::Divide(left, Vector3(value, value, value), &left);
	}

	Vector3 operator+(float value, Vector3 left)
	{
		return *Vector3::Add(Vector3(value, value, value), left, &left);
	}

	Vector3 operator-(float value, Vector3 left)
	{
		return *Vector3::Subtract(Vector3(value, value, value), left, &left);
	}

	Vector3 operator*(float value, Vector3 left)
	{
		return *Vector3::Multiply(Vector3(value, value, value), left, &left);
	}

	Vector3 operator/(float value, Vector3 left)
	{
		return *Vector3::Divide(Vector3(value, value, value), left, &left);
	}

	Vector3 &Vector3::operator+=(const Vector3 &other)
	{
		Vector3 result = Vector3();
		return *Vector3::Add(*this, other, &result);
	}

	Vector3 &Vector3::operator-=(const Vector3 &other)
	{
		Vector3 result = Vector3();
		return *Vector3::Subtract(*this, other, &result);
	}

	Vector3 &Vector3::operator*=(const Vector3 &other)
	{
		Vector3 result = Vector3();
		return *Vector3::Multiply(*this, other, &result);
	}

	Vector3 &Vector3::operator/=(const Vector3 &other)
	{
		Vector3 result = Vector3();
		return *Vector3::Divide(*this, other, &result);
	}

	Vector3 &Vector3::operator+=(float value)
	{
		return *Vector3::Add(*this, Vector3(value, value, value), this);
	}

	Vector3 &Vector3::operator-=(float value)
	{
		return *Vector3::Subtract(*this, Vector3(value, value, value), this);
	}

	Vector3 &Vector3::operator*=(float value)
	{
		return *Vector3::Multiply(*this, Vector3(value, value, value), this);
	}

	Vector3 &Vector3::operator/=(float value)
	{
		return *Vector3::Divide(*this, Vector3(value, value, value), this);
	}

	std::ostream &operator<<(std::ostream &stream, const Vector3 &vector)
	{
		stream << vector.ToString();
		return stream;
	}

	std::string Vector3::ToString() const
	{
		std::stringstream result;
		result << "Vector3(" << m_x << ", " << m_y << ", " << m_z << ")";
		return result.str();
	}
}
