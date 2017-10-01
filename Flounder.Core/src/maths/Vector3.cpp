#include "Vector3.hpp"

#include <assert.h>
#include "Maths.hpp"
#include "Vector2.hpp"
#include "Vector4.hpp"

namespace Flounder
{
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

	Vector3::Vector3(const float &x, const float &y, const float &z) :
		m_x(x),
		m_y(y),
		m_z(z)
	{
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

		return acos(dls);
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

		return destination->Set(left.m_y * right.m_z - left.m_z * right.m_y, right.m_x * left.m_z - right.m_z * left.m_x, left.m_x * right.m_y - left.m_y * right.m_x);
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
		assert(false && "Instead of calling Vector3::Rotate, call Matrix4::Rotate!");
		return nullptr;
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
		return sqrt(LengthSquared(source));
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
		float dx = point1.m_x - point2.m_x;
		float dy = point1.m_y - point2.m_y;
		float dz = point1.m_z - point2.m_z;
		return dx * dx + dy * dy + dz * dz;
	}

	float Vector3::GetDistance(const Vector3 &point1, const Vector3 &point2)
	{
		return sqrt(pow(point2.m_x - point1.m_x, 2) + pow(point2.m_y - point1.m_y, 2) + pow(point2.m_z - point1.m_z, 2));
	}

	Vector3 *Vector3::GetVectorDistance(const Vector3 &point1, const Vector3 &point2, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return destination->Set(pow(point2.m_x - point1.m_x, 2), pow(point2.m_y - point1.m_y, 2), pow(point2.m_z - point1.m_z, 2));
	}

	Vector3 *Vector3::RandomUnitVector(Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		float theta = Maths::RandomInRange(0.0f, 1.0f) * 2.0f * PI;
		float z = Maths::RandomInRange(0.0f, 1.0f) * 2.0f - 1.0f;
		float rootOneMinusZSquared = sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);
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
		}
		while (Length(*destination) == 0.0f);

		destination->Normalize();
		destination->Scale(radius);
		float a = Maths::RandomInRange(0.0f, 1.0f);
		float b = Maths::RandomInRange(0.0f, 1.0f);

		if (a > b)
		{
			float temp = a;
			a = b;
			b = temp;
		}

		float randX = b * cos(2.0f * PI * (a / b));
		float randY = b * sin(2.0f * PI * (a / b));
		float distance = Vector3(randX, randY, 0.0f).Length();
		destination->Scale(distance);
		return destination;
	}

	float Vector3::BaryCentric(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &pos)
	{
		float det = (p2.m_z - p3.m_z) * (p1.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (p1.m_z - p3.m_z);
		float l1 = ((p2.m_z - p3.m_z) * (pos.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (pos.m_y - p3.m_z)) / det;
		float l2 = ((p3.m_z - p1.m_z) * (pos.m_x - p3.m_x) + (p1.m_x - p3.m_x) * (pos.m_y - p3.m_z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.m_y + l2 * p2.m_y + l3 * p3.m_y;
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

	bool Vector3::IsZero() const
	{
		return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f;
	}

	float Vector3::Length() const
	{
		return Length(*this);
	}

	float Vector3::LengthSquared() const
	{
		return LengthSquared(*this);
	}
}
