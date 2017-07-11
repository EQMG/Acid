#include "vector4.h"

namespace flounder {
	vector4::vector4()
	{
		this->set(0.0f, 0.0f, 0.0f, 0.0f);
	}

	vector4::vector4(const float x, const float y, const float z, const float w)
	{
		this->set(x, y, z, w);
	}

	vector4::vector4(vector4 *source)
	{
		this->set(source);
	}

	vector4::~vector4()
	{
	}

	vector4 *vector4::set(const float x, const float y, const float z, const float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		return this;
	}

	vector4 *vector4::set(vector4 *source)
	{
		if (source == NULL)
		{
			return this;
		}

		this->x = source->x;
		this->y = source->y;
		this->z = source->z;
		this->w = source->w;
		return this;
	}

	vector4 *vector4::add(vector4 *left, vector4 *right, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(left->x + right->x, left->y + right->y, left->z + right->z, left->w + right->w);
	}

	vector4 *vector4::subtract(vector4 *left, vector4 *right, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(left->x - right->x, left->y - right->y, left->z - right->z, left->w - right->w);
	}

	vector4 *vector4::multiply(vector4 *left, vector4 *right, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(left->x * right->x, left->y * right->y, left->z * right->z, left->w * right->w);
	}

	vector4 *vector4::divide(vector4 *left, vector4 *right, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(left->x / right->x, left->y / right->y, left->z / right->z, left->w / right->w);
	}

	float vector4::angle(vector4 *left, vector4 *right)
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

		return (float) acos(dls);
	}

	float vector4::dot(vector4 *left, vector4 *right)
	{
		return left->x * right->x + left->y * right->y + left->z * right->z + left->w * right->w;
	}

	vector4 *vector4::scale(vector4 *source, const float scalar, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(source->x * scalar, source->y * scalar, source->z * scalar, source->w * scalar);
	}

	vector4 *vector4::negate(vector4 *source, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		return destination->set(-source->x, -source->y, -source->z, -source->w);
	}

	vector4 *vector4::normalize(vector4 *source, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		float length = source->length();

		if (length != 0.0f)
		{
			return destination->set(source->x / length, source->y / length, source->z / length, source->w / length);
		}
		else
		{
			throw std::invalid_argument("Zero length vector");
		}
	}

	vector4 *vector4::translate(const float x, const float y, const float z, const float w)
	{
		this->x += x;
		this->y += y;
		this->z += z;
		this->w += w;
		return this;
	}

	vector4 *vector4::negate()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
		return this;
	}

	vector4 *vector4::normalize()
	{
		return normalize(this, this);
	}

	vector4 *vector4::scale(const float scalar)
	{
		return scale(this, scalar, this);
	}

	bool vector4::isZero()
	{
		return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
	}

	float vector4::length()
	{
		return sqrt(lengthSquared());
	}

	float vector4::lengthSquared()
	{
		return x * x + y * y + z * z + w * w;
	}
}
