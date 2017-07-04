#include "vector3.h"

namespace flounder {
	vector3::vector3()
	{
		this->set(0.0f, 0.0f, 0.0f);
	}

	vector3::vector3(float x, float y, float z)
	{
		this->set(x, y, z);
	}

	vector3::vector3(vector3 *source)
	{
		this->set(source);
	}

	vector3::vector3(vector4 *source)
	{
		this->set(source);
	}

	vector3::~vector3()
	{
		delete &x;
		delete &y;
		delete &z;
	}

	vector3 *vector3::set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		return this;
	}

	vector3 *vector3::set(vector3 *source)
	{
		if (source == NULL)
		{
			return this;
		}

		this->x = source->x;
		this->y = source->y;
		this->z = source->z;
		return this;
	}

	vector3 *vector3::set(vector4 *source)
	{
		if (source == NULL)
		{
			return this;
		}

		this->x = source->x;
		this->y = source->y;
		this->z = source->z;
		return this;
	}

	vector3 *vector3::add(vector3 *left, vector3 *right, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(left->x + right->x, left->y + right->y, left->z + right->z);
	}

	vector3 *vector3::subtract(vector3 *left, vector3 *right, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(left->x - right->x, left->y - right->y, left->z - right->z);
	}

	vector3 *vector3::multiply(vector3 *left, vector3 *right, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(left->x * right->x, left->y * right->y, left->z * right->z);
	}

	vector3 *vector3::divide(vector3 *left, vector3 *right, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(left->x / right->x, left->y / right->y, left->z / right->z);
	}

	float vector3::angle(vector3 *left, vector3 *right)
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

	float vector3::dot(vector3 *left, vector3 *right)
	{
		return left->x * right->x + left->y * right->y + left->z * right->z;
	}

	vector3 *vector3::cross(vector3 * left, vector3 * right, vector3 * destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(left->y * right->z - left->z * right->y, right->x * left->z - right->z * left->x, left->x * right->y - left->y * right->x);
	}

	vector3 *vector3::scale(vector3 * source, float scalar, vector3 * destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(source->x * scalar, source->y * scalar, source->z * scalar);
	}

	vector3 *vector3::rotate(vector3 * source, vector3 * rotation, vector3 * destination)
	{
		throw std::logic_error("Instead of calling vector3::rotate, call matrix4x4::rotate!");
	}

	vector3 *vector3::negate(vector3 * source, vector3 * destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(-source->x, -source->y, -source->z);
	}


	vector3 *vector3::normalize(vector3 * source, vector3 * destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		float length = source->length();
		return destination->set(source->x / length, source->y / length, source->z / length);
	}

	vector3 *vector3::maxVector(vector3 *a, vector3 *b)
	{
		return new vector3(__max(a->x, b->x), __max(a->y, b->y), __max(a->z, b->z));
	}

	vector3 *vector3::minVector(vector3 *a, vector3 *b)
	{
		return new vector3(__min(a->x, b->x), __min(a->y, b->y), __min(a->z, b->z));
	}

	float vector3::maxComponent(vector3 *vector)
	{
		return __max(__max(vector->x, vector->y), vector->z);
	}

	float vector3::minComponent(vector3 *vector)
	{
		return __min(__min(vector->x, vector->y), vector->z);
	}

	float vector3::getDistanceSquared(vector3 *point1, vector3 *point2)
	{
		float dx = point1->x - point2->x;
		float dy = point1->y - point2->y;
		float dz = point1->z - point2->z;
		return dx * dx + dy * dy + dz * dz;
	}

	float vector3::getDistance(vector3 *point1, vector3 *point2)
	{
		return sqrt(pow(point2->x - point1->x, 2) + pow(point2->y - point1->y, 2) + pow(point2->z - point1->z, 2));
	}

	vector3 *vector3::getVectorDistance(vector3 *point1, vector3 *point2, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return destination->set(pow(point2->x - point1->x, 2), pow(point2->y - point1->y, 2), pow(point2->z - point1->z, 2));
	}

	vector3 * vector3::generateRandomUnitVector(vector3 * destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		float theta = rand() * 2.0f * PI;
		float z = rand() * 2.0f - 1.0f;
		float rootOneMinusZSquared = sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);
		return destination->set(x, y, z);
	}

	vector3 * vector3::randomPointOnCircle(vector3 * destination, vector3 * normal, float radius)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		do
		{
			vector3 *randomVector = generateRandomUnitVector(NULL);
			vector3::cross(randomVector, normal, destination);
		} while (destination->length() == 0.0f);

		destination->normalize();
		destination->scale(radius);
		float a = rand();
		float b = rand();

		if (a > b)
		{
			float temp = a;
			a = b;
			b = temp;
		}

		float randX = b * cos(2.0f * PI * (a / b));
		float randY = b * sin(2.0f * PI * (a / b));
		float distance = (new vector3(randX, randY, 0.0f))->length();
		destination->scale(distance);
		return destination;
	}

	float vector3::baryCentric(vector3 * p1, vector3 * p2, vector3 * p3, vector2 * pos)
	{
		float det = (p2->z - p3->z) * (p1->x - p3->x) + (p3->x - p2->x) * (p1->z - p3->z);
		float l1 = ((p2->z - p3->z) * (pos->x - p3->x) + (p3->x - p2->x) * (pos->y - p3->z)) / det;
		float l2 = ((p3->z - p1->z) * (pos->x - p3->x) + (p1->x - p3->x) * (pos->y - p3->z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1->y + l2 * p2->y + l3 * p3->y;
	}

	vector3 *vector3::translate(float x, float y, float z)
	{
		this->x += x;
		this->y += y;
		this->z += z;
		return this;
	}

	vector3 *vector3::negate()
	{
		this->x = -x;
		this->y = -y;
		this->z = -z;
		return this;
	}

	vector3 *vector3::normalize()
	{
		float length = this->length();

		if (length != 0.0f)
		{
			float l = 1.0f / length;
			return scale(l);
		}
		else
		{
			throw std::invalid_argument("Zero length vector");
		}
	}

	vector3 *vector3::scale(float scale)
	{
		this->x *= scale;
		this->y *= scale;
		this->z *= scale;
		return this;
	}

	bool vector3::isZero()
	{
		return x == 0.0f && y == 0.0f && z == 0.0f;
	}

	float vector3::length()
	{
		return sqrt(lengthSquared());
	}

	float vector3::lengthSquared()
	{
		return x * x + y * y + z * z;
	}
}
