#include "colour.h"

namespace flounder {
	colour::colour()
	{
		this->set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	colour::colour(colour *source)
	{
		this->set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	colour::colour(float r, float g, float b)
	{
		this->set(r, g, b);
	}

	colour::colour(float r, float g, float b, bool convert)
	{
		this->set(r, g, b, convert);
	}

	colour::colour(float r, float g, float b, float a)
	{
		this->set(r, g, b, a);
	}

	colour::colour(float r, float g, float b, float a, bool convert)
	{
		this->set(r, g, b, a, convert);
	}

	colour::~colour()
	{
	}

	colour *colour::set(colour *source)
	{
		return this->set(source->r, source->g, source->b, source->a);
	}

	colour *colour::set(float r, float g, float b)
	{
		return set(r, g, b, false);
	}

	colour *colour::set(float r, float g, float b, bool convert)
	{
		if (convert)
		{
			this->r = r / 255.0f;
			this->g = g / 255.0f;
			this->b = b / 255.0f;
		}
		else
		{
			this->r = r;
			this->g = g;
			this->b = b;
		}

		return this;
	}

	colour *colour::set(float r, float g, float b, float a)
	{
		return set(r, g, b, a, false);
	}

	colour *colour::set(float r, float g, float b, float a, bool convert)
	{
		if (convert)
		{
			this->r = r / 255.0f;
			this->g = g / 255.0f;
			this->b = b / 255.0f;
			this->a = a / 255.0f;
		}
		else
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		return this;
	}

	colour *colour::add(colour *left, colour *right, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		return destination->set(left->r + right->r, left->g + right->g, left->b + right->b, left->a + right->a);
	}

	colour *colour::subtract(colour *left, colour *right, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		return destination->set(left->r - right->r, left->g - right->g, left->b - right->b, left->a - right->a);
	}

	colour *colour::multiply(colour *left, colour *right, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		return destination->set(left->r * right->r, left->g * right->g, left->b * right->b, left->a * right->a);
	}

	colour *colour::divide(colour *left, colour *right, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		return destination->set(left->r / right->r, left->g / right->g, left->b / right->b, left->a / right->a);
	}

	colour *colour::interpolate(colour *left, colour *right, float blend, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		float r = ((1.0f - blend) * left->r) + (blend * right->r);
		float g = ((1.0f - blend) * left->g) + (blend * right->g);
		float b = ((1.0f - blend) * left->b) + (blend * right->b);
		float a = ((1.0f - blend) * left->a) + (blend * right->a);
		return destination->set(r, g, b, a);
	}

	colour *colour::getUnit()
	{
		colour *result = new colour(this);
		result->scale(1.0f / length());
		return result;
	}

	colour *colour::scale(float scalar)
	{
		this->r *= scalar;
		this->g *= scalar;
		this->b *= scalar;
		return this;
	}

	float colour::length()
	{
		return sqrt(lengthSquared());
	}

	float colour::lengthSquared()
	{
		return pow(r, 2) + pow(g, 2) + pow(b, 2) + pow(a, 2);
	}
}
