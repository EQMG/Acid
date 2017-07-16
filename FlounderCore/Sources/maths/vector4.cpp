#include "vector4.h"

namespace flounder {
	vector4::vector4()
	{
		this->set(0.0f, 0.0f, 0.0f, 0.0f);
	}

	vector4::vector4(const float &x, const float &y, const float &z, const float &w)
	{
		this->set(x, y, z, w);
	}

	vector4::vector4(const vector4 &source)
	{
		this->set(source);
	}

	vector4::~vector4()
	{
	}

	vector4 *vector4::set(const float &x, const float &y, const float &z, const float &w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		return this;
	}

	vector4 *vector4::set(const vector4 &source)
	{
		this->x = source.x;
		this->y = source.y;
		this->z = source.z;
		this->w = source.w;
		return this;
	}

	vector4 *vector4::add(const vector4 &left, const vector4 &right, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	}

	vector4 *vector4::subtract(const vector4 &left, const vector4 &right, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	}

	vector4 *vector4::multiply(const vector4 &left, const vector4 &right, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
	}

	vector4 *vector4::divide(const vector4 &left, const vector4 &right, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
	}

	float vector4::angle(const vector4 &left, const vector4 &right)
	{
		float dls = dot(left, right) / (length(left) * length(right));

		if (dls < -1.0f)
		{
			dls = -1.0f;
		}
		else if (dls > 1.0f)
		{
			dls = 1.0f;
		}

		return (float) acos(dls);
	}

	float vector4::dot(const vector4 &left, const vector4 &right)
	{
		return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
	}

	vector4 *vector4::scale(const vector4 &source, const float &scalar, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(source.x * scalar, source.y * scalar, source.z * scalar, source.w * scalar);
	}

	vector4 *vector4::negate(const vector4 &source, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(-source.x, -source.y, -source.z, -source.w);
	}

	vector4 *vector4::normalize(const vector4 &source, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		float l = length(source);

		if (l != 0.0f)
		{
			return destination->set(source.x / l, source.y / l, source.z / l, source.w / l);
		}
		else
		{
			throw std::invalid_argument("Zero length vector");
		}
	}

	float vector4::length(const vector4 &source)
	{
		return sqrt(lengthSquared(source));
	}

	float vector4::lengthSquared(const vector4 &source)
	{
		return source.x * source.x + source.y * source.y + source.z * source.z + source.w * source.w;
	}

	vector4 *vector4::maxVector(const vector4 &a, const vector4 &b, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(__max(a.x, b.x), __max(a.y, b.y), __max(a.z, b.z), __max(a.w, b.w));
	}

	vector4 *vector4::minVector(const vector4 &a, const vector4 &b, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(__min(a.x, b.x), __min(a.y, b.y), __min(a.z, b.z), __min(a.w, b.w));
	}

	float vector4::maxComponent(const vector4 &vector)
	{
		return __max(vector.x, __max(vector.y, __max(vector.z, vector.w)));
	}

	float vector4::minComponent(const vector4 &vector)
	{
		return __min(vector.x, __min(vector.y, __min(vector.z, vector.w)));
	}

	float vector4::getDistanceSquared(const vector4 &point1, const vector4 &point2)
	{
		float dx = point1.x - point2.x;
		float dy = point1.y - point2.y;
		float dz = point1.z - point2.z;
		float dw = point1.w - point2.w;
		return dx * dx + dy * dy + dz * dz + dw * dw;
	}

	float vector4::getDistance(const vector4 &point1, const vector4 &point2)
	{
		return (float) sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2) + pow(point2.z - point1.z, 2) + pow(point2.w - point1.w, 2));
	}

	vector4 *vector4::getVectorDistance(const vector4 &point1, const vector4 &point2, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(pow(point2.x - point1.x, 2), pow(point2.y - point1.y, 2), pow(point2.z - point1.z, 2), pow(point2.w - point1.w, 2));
	}

	vector4 *vector4::translate(const float &x, const float &y, const float &z, const float &w)
	{
		this->x += x;
		this->y += y;
		this->z += z;
		this->w += w;
		return this;
	}

	vector4 *vector4::negate()
	{
		this->x = -x;
		this->y = -y;
		this->z = -z;
		this->w = -w;
		return this;
	}

	vector4 *vector4::normalize()
	{
		return normalize(*this, this);
	}

	vector4 *vector4::scale(const float &scalar)
	{
		return scale(*this, scalar, this);
	}

	bool vector4::isZero()
	{
		return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
	}

	float vector4::length()
	{
		return length(*this);
	}

	float vector4::lengthSquared()
	{
		return lengthSquared(*this);
	}
}
