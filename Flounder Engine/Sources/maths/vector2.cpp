#include "vector2.h"

namespace flounder {
	vector2::vector2()
	{
		this->set(0.0f, 0.0f);
	}

	vector2::vector2(float x, float y)
	{
		this->set(x, y);
	}

	vector2::vector2(vector2 *source)
	{
		this->set(source);
	}

	vector2::~vector2()
	{
	}

	vector2 *vector2::set(float x, float y)
	{
		this->x = x;
		this->y = y;
		return this;
	}

	vector2 *vector2::set(vector2 *source)
	{
		this->x = source->x;
		this->y = source->y;
		return this;
	}

	vector2 *vector2::add(vector2 *left, vector2 *right, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(left->x + right->x, left->y + right->y);
	}

	vector2 *vector2::subtract(vector2 *left, vector2 *right, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(left->x - right->x, left->y - right->y);
	}

	vector2 *vector2::multiply(vector2 *left, vector2 *right, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(left->x * right->x, left->y * right->y);
	}

	vector2 *vector2::divide(vector2 *left, vector2 *right, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(left->x / right->x, left->y / right->y);
	}

	float vector2::angle(vector2 *left, vector2 *right)
	{
		float dls = dot(left, right) / (left->length() * right->length());

		if (dls < -1.0f)
		{
			dls = -1.0f;
		}
		else if (dls > 1.0f)
		{
			dls = 1.0f;
		}

		return static_cast<float>(acos(dls));
	}

	float vector2::dot(vector2 *left, vector2 *right)
	{
		return left->x * right->x + left->y * right->y;
	}

	vector2 *vector2::scale(vector2 *source, float scalar, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(source->x * scalar, source->y * scalar);
	}

	vector2 *vector2::rotate(vector2 *source, float angle, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		double theta = __radians(angle);
		return destination->set(static_cast<float>(source->x * cos(theta) - source->y * sin(theta)), static_cast<float>(source->x * sin(theta) + source->y * cos(theta)));
	}

	vector2 *vector2::rotate(vector2 *source, float angle, vector2 *rotationAxis, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		double theta = __radians(angle);
		return destination->set(static_cast<float>(((source->x - rotationAxis->x) * cos(theta)) - ((source->y - rotationAxis->y) * sin(theta) + rotationAxis->x)), static_cast<float>(((source->x - rotationAxis->x) * sin(theta)) + ((source->y - rotationAxis->y) * cos(theta) + rotationAxis->y)));
	}

	vector2 *vector2::negate(vector2 *source, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(-source->x, -source->y);
	}

	vector2 *vector2::normalize(vector2 *source, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		float length = source->length();

		if (length != 0.0f)
		{
			return destination->set(source->x / length, source->y / length);
		}
		else
		{
			throw std::invalid_argument("Zero length vector");
		}
	}

	vector2 *vector2::maxVector(vector2 *a, vector2 *b)
	{
		return new vector2(__max(a->x, b->x), __max(a->y, b->y));
	}

	vector2 *vector2::minVector(vector2 *a, vector2 *b)
	{
		return new vector2(__min(a->x, b->x), __min(a->y, b->y));
	}

	float vector2::maxComponent(vector2 *vector)
	{
		return __max(vector->x, vector->y);
	}

	float vector2::minComponent(vector2 *vector)
	{
		return __min(vector->x, vector->y);
	}

	float vector2::getDistanceSquared(vector2 *point1, vector2 *point2)
	{
		float dx = point1->x - point2->x;
		float dy = point1->y - point2->y;
		return dx * dx + dy * dy;
	}

	float vector2::getDistance(vector2 *point1, vector2 *point2)
	{
		return static_cast<float>(sqrt(pow(point2->x - point1->x, 2) + pow(point2->y - point1->y, 2)));
	}

	vector2 *vector2::getVectorDistance(vector2 *point1, vector2 *point2, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		return destination->set(static_cast<float>(pow(point2->x - point1->x, 2)), static_cast<float>(pow(point2->y - point1->y, 2)));
	}

	bool vector2::pointInTriangle(vector2 * point, vector2 * v1, vector2 * v2, vector2 * v3)
	{
		bool b1 = (point->x - v2->x) * (v1->y - v2->y) - (v1->x - v2->x) * (point->y - v2->y);
		bool b2 = (point->x - v3->x) * (v2->y - v3->y) - (v2->x - v3->x) * (point->y - v3->y);
		bool b3 = (point->x - v1->x) * (v3->y - v1->y) - (v3->x - v1->x) * (point->y - v1->y);
		return ((b1 == b2) && (b2 == b3));
	}

	vector2 *vector2::translate(float x, float y)
	{
		this->x += x;
		this->y += y;
		return this;
	}

	vector2 *vector2::negate()
	{
		return negate(this, this);
	}

	vector2 *vector2::normalize()
	{
		return normalize(this, this);
	}

	vector2 *vector2::scale(float scalar)
	{
		return scale(this, scalar, this);
	}

	bool vector2::isZero()
	{
		return x == 0.0f && y == 0.0f;
	}

	float vector2::length()
	{
		return sqrt(lengthSquared());
	}

	float vector2::lengthSquared()
	{
		return x * x + y * y;
	}
}
