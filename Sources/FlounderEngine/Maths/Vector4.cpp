#include "Vector4.hpp"

#include <cassert>
#include <sstream>
#include "Colour.hpp"
#include "Maths.hpp"
#include "Vector3.hpp"

namespace Flounder
{
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

	Vector4::Vector4(const Vector3 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z),
		m_w(1.0f)
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

	Vector4 *Vector4::Set(const float &x, const float &y, const float &z, const float &w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
		return this;
	}

	Vector4 *Vector4::Set(const Vector3 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		m_w = 1.0f;
		return this;
	}

	Vector4 *Vector4::Set(const Vector4 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		m_w = source.m_w;
		return this;
	}

	Vector4 *Vector4::Add(const Vector4 &left, const Vector4 &right, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->Set(left.m_x + right.m_x, left.m_y + right.m_y, left.m_z + right.m_z, left.m_w + right.m_w);
	}

	Vector4 *Vector4::Subtract(const Vector4 &left, const Vector4 &right, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->Set(left.m_x - right.m_x, left.m_y - right.m_y, left.m_z - right.m_z, left.m_w - right.m_w);
	}

	Vector4 *Vector4::Multiply(const Vector4 &left, const Vector4 &right, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->Set(left.m_x * right.m_x, left.m_y * right.m_y, left.m_z * right.m_z, left.m_w * right.m_w);
	}

	Vector4 *Vector4::Divide(const Vector4 &left, const Vector4 &right, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->Set(left.m_x / right.m_x, left.m_y / right.m_y, left.m_z / right.m_z, left.m_w / right.m_w);
	}

	float Vector4::Angle(const Vector4 &left, const Vector4 &right)
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

	float Vector4::Dot(const Vector4 &left, const Vector4 &right)
	{
		return left.m_x * right.m_x + left.m_y * right.m_y + left.m_z * right.m_z + left.m_w * right.m_w;
	}

	Vector4 *Vector4::Scale(const Vector4 &source, const float &scalar, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->Set(source.m_x * scalar, source.m_y * scalar, source.m_z * scalar, source.m_w * scalar);
	}

	Vector4 *Vector4::Negate(const Vector4 &source, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->Set(-source.m_x, -source.m_y, -source.m_z, -source.m_w);
	}

	Vector4 *Vector4::Normalize(const Vector4 &source, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		const float l = Length(source);
		assert(l != 0.0f && "Zero length vector!");
		return destination->Set(source.m_x / l, source.m_y / l, source.m_z / l, source.m_w / l);
	}

	float Vector4::Length(const Vector4 &source)
	{
		return std::sqrt(LengthSquared(source));
	}

	float Vector4::LengthSquared(const Vector4 &source)
	{
		return source.m_x * source.m_x + source.m_y * source.m_y + source.m_z * source.m_z + source.m_w * source.m_w;
	}

	Vector4 *Vector4::MaxVector(const Vector4 &a, const Vector4 &b, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->Set(Maths::Max(a.m_x, b.m_x), Maths::Max(a.m_y, b.m_y), Maths::Max(a.m_z, b.m_z), Maths::Max(a.m_w, b.m_w));
	}

	Vector4 *Vector4::MinVector(const Vector4 &a, const Vector4 &b, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->Set(Maths::Min(a.m_x, b.m_x), Maths::Min(a.m_y, b.m_y), Maths::Min(a.m_z, b.m_z), Maths::Min(a.m_w, b.m_w));
	}

	float Vector4::MaxComponent(const Vector4 &vector)
	{
		return Maths::Max(vector.m_x, Maths::Max(vector.m_y, Maths::Max(vector.m_z, vector.m_w)));
	}

	float Vector4::MinComponent(const Vector4 &vector)
	{
		return Maths::Min(vector.m_x, Maths::Min(vector.m_y, Maths::Min(vector.m_z, vector.m_w)));
	}

	float Vector4::GetDistanceSquared(const Vector4 &point1, const Vector4 &point2)
	{
		const float dx = point1.m_x - point2.m_x;
		const float dy = point1.m_y - point2.m_y;
		const float dz = point1.m_z - point2.m_z;
		const float dw = point1.m_w - point2.m_w;
		return dx * dx + dy * dy + dz * dz + dw * dw;
	}

	float Vector4::GetDistance(const Vector4 &point1, const Vector4 &point2)
	{
		return std::sqrt(std::pow(point2.m_x - point1.m_x, 2.0f) +
			std::pow(point2.m_y - point1.m_y, 2.0f) +
			std::pow(point2.m_z - point1.m_z, 2.0f) +
			std::pow(point2.m_w - point1.m_w, 2.0f));
	}

	Vector4 *Vector4::GetVectorDistance(const Vector4 &point1, const Vector4 &point2, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		return destination->Set(std::pow(point2.m_x - point1.m_x, 2.0f), std::pow(point2.m_y - point1.m_y, 2.0f),
			std::pow(point2.m_z - point1.m_z, 2.0f),
			std::pow(point2.m_w - point1.m_w, 2.0f));
	}

	Vector4 *Vector4::Translate(const float &x, const float &y, const float &z, const float &w)
	{
		m_x += x;
		m_y += y;
		m_z += z;
		m_w += w;
		return this;
	}

	Vector4 *Vector4::Negate()
	{
		m_x = -m_x;
		m_y = -m_y;
		m_z = -m_z;
		m_w = -m_w;
		return this;
	}

	Vector4 *Vector4::Normalize()
	{
		return Normalize(*this, this);
	}

	Vector4 *Vector4::Scale(const float &scalar)
	{
		return Scale(*this, scalar, this);
	}

	float Vector4::Length() const
	{
		return Length(*this);
	}

	float Vector4::LengthSquared() const
	{
		return LengthSquared(*this);
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

	Vector4 &Vector4::operator-()
	{
		return *Vector4(*this).Negate();
	}

	Vector4 operator+(Vector4 left, const Vector4 &right)
	{
		return *Vector4::Add(left, right, &left);
	}

	Vector4 operator-(Vector4 left, const Vector4 &right)
	{
		return *Vector4::Subtract(left, right, &left);
	}

	Vector4 operator*(Vector4 left, const Vector4 &right)
	{
		return *Vector4::Multiply(left, right, &left);
	}

	Vector4 operator/(Vector4 left, const Vector4 &right)
	{
		return *Vector4::Divide(left, right, &left);
	}

	Vector4 operator+(Vector4 left, float value)
	{
		return *Vector4::Add(left, Vector4(value, value, value, value), &left);
	}

	Vector4 operator-(Vector4 left, float value)
	{
		return *Vector4::Subtract(left, Vector4(value, value, value, value), &left);
	}

	Vector4 operator*(Vector4 left, float value)
	{
		return *Vector4::Multiply(left, Vector4(value, value, value, value), &left);
	}

	Vector4 operator/(Vector4 left, float value)
	{
		return *Vector4::Divide(left, Vector4(value, value, value, value), &left);
	}

	Vector4 operator+(float value, Vector4 left)
	{
		return *Vector4::Add(Vector4(value, value, value, value), left, &left);
	}

	Vector4 operator-(float value, Vector4 left)
	{
		return *Vector4::Subtract(Vector4(value, value, value, value), left, &left);
	}

	Vector4 operator*(float value, Vector4 left)
	{
		return *Vector4::Multiply(Vector4(value, value, value, value), left, &left);
	}

	Vector4 operator/(float value, Vector4 left)
	{
		return *Vector4::Divide(Vector4(value, value, value, value), left, &left);
	}

	Vector4 &Vector4::operator+=(const Vector4 &other)
	{
		Vector4 result = Vector4();
		return *Vector4::Add(*this, other, &result);
	}

	Vector4 &Vector4::operator-=(const Vector4 &other)
	{
		Vector4 result = Vector4();
		return *Vector4::Subtract(*this, other, &result);
	}

	Vector4 &Vector4::operator*=(const Vector4 &other)
	{
		Vector4 result = Vector4();
		return *Vector4::Multiply(*this, other, &result);
	}

	Vector4 &Vector4::operator/=(const Vector4 &other)
	{
		Vector4 result = Vector4();
		return *Vector4::Divide(*this, other, &result);
	}

	Vector4 &Vector4::operator+=(float value)
	{
		return *Vector4::Add(*this, Vector4(value, value, value, value), this);
	}

	Vector4 &Vector4::operator-=(float value)
	{
		return *Vector4::Subtract(*this, Vector4(value, value, value, value), this);
	}

	Vector4 &Vector4::operator*=(float value)
	{
		return *Vector4::Multiply(*this, Vector4(value, value, value, value), this);
	}

	Vector4 &Vector4::operator/=(float value)
	{
		return *Vector4::Divide(*this, Vector4(value, value, value, value), this);
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
