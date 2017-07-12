#include "vector2.h"

namespace flounder {
	vector2::vector2()
	{
		this->set(0.0f, 0.0f);
	}

	vector2::vector2(const float &x, const float &y)
	{
		this->set(x, y);
	}

	vector2::vector2(const vector2 &source)
	{
		this->set(source);
	}

	vector2::~vector2()
	{
	}

	vector2 *vector2::set(const float &x, const float &y)
	{
		this->x = x;
		this->y = y;
		return this;
	}

	vector2 *vector2::set(const vector2 &source)
	{
		this->x = source.x;
		this->y = source.y;
		return this;
	}

	vector2 *vector2::add(const vector2 &left, const vector2 &right, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(left.x + right.x, left.y + right.y);
	}

	vector2 *vector2::subtract(const vector2 &left, const vector2 &right, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(left.x - right.x, left.y - right.y);
	}

	vector2 *vector2::multiply(const vector2 &left, const vector2 &right, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(left.x * right.x, left.y * right.y);
	}

	vector2 *vector2::divide(const vector2 &left, const vector2 &right, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(left.x / right.x, left.y / right.y);
	}

	float vector2::angle(const vector2 &left, const vector2 &right)
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

	float vector2::dot(const vector2 &left, const vector2 &right)
	{
		return left.x * right.x + left.y * right.y;
	}

	vector2 *vector2::scale(const vector2 &source, const float &scalar, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(source.x * scalar, source.y * scalar);
	}

	vector2 *vector2::rotate(const vector2 &source, const float &angle, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		double theta = __radians(angle);
		return destination->set((float) (source.x * cos(theta) - source.y * sin(theta)), (float) (source.x * sin(theta) + source.y * cos(theta)));
	}

	vector2 *vector2::rotate(const vector2 &source, const float &angle, const vector2 &rotationAxis, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		double theta = __radians(angle);
		return destination->set((float)(((source.x - rotationAxis.x) * cos(theta)) - ((source.y - rotationAxis.y) * sin(theta) + rotationAxis.x)), (float)(((source.x - rotationAxis.x) * sin(theta)) + ((source.y - rotationAxis.y) * cos(theta) + rotationAxis.y)));
	}

	vector2 *vector2::negate(const vector2 &source, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(-source.x, -source.y);
	}

	vector2 *vector2::normalize(const vector2 &source, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		float l = length(source);

		if (l != 0.0f)
		{
			return destination->set(source.x / l, source.y / l);
		}
		else
		{
			throw std::invalid_argument("Zero length vector");
		}
	}

	float vector2::length(const vector2 &source)
	{
		return sqrt(lengthSquared(source));
	}

	float vector2::lengthSquared(const vector2 &source)
	{
		return source.x * source.x + source.y * source.y;
	}

	vector2 *vector2::maxVector(const vector2 &a, const vector2 &b, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(__max(a.x, b.x), __max(a.y, b.y));
	}

	vector2 *vector2::minVector(const vector2 &a, const vector2 &b, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(__min(a.x, b.x), __min(a.y, b.y));
	}

	float vector2::maxComponent(const vector2 &vector)
	{
		return __max(vector.x, vector.y);
	}

	float vector2::minComponent(const vector2 &vector)
	{
		return __min(vector.x, vector.y);
	}

	float vector2::getDistanceSquared(const vector2 &point1, const vector2 &point2)
	{
		float dx = point1.x - point2.x;
		float dy = point1.y - point2.y;
		return dx * dx + dy * dy;
	}

	float vector2::getDistance(const vector2 &point1, const vector2 &point2)
	{
		return sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));
	}

	vector2 *vector2::getVectorDistance(const vector2 &point1, const vector2 &point2, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(pow(point2.x - point1.x, 2), pow(point2.y - point1.y, 2));
	}

	bool vector2::pointInTriangle(const vector2 &point, const vector2 &v1, const vector2 &v2, const vector2 &v3)
	{
		bool b1 = (point.x - v2.x) * (v1.y - v2.y) - (v1.x - v2.x) * (point.y - v2.y);
		bool b2 = (point.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (point.y - v3.y);
		bool b3 = (point.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (point.y - v1.y);
		return ((b1 == b2) & (b2 == b3));
	}

	vector2 *vector2::translate(const float &x, const float &y)
	{
		this->x += x;
		this->y += y;
		return this;
	}

	vector2 *vector2::negate()
	{
		return negate(*this, this);
	}

	vector2 *vector2::normalize()
	{
		return normalize(*this, this);
	}

	vector2 *vector2::scale(const float &scalar)
	{
		return scale(*this, scalar, this);
	}

	bool vector2::isZero()
	{
		return x == 0.0f & y == 0.0f;
	}

	float vector2::length()
	{
		return length(*this);
	}

	float vector2::lengthSquared()
	{
		return lengthSquared(*this);
	}
}
