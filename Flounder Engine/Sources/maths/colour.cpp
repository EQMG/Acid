#include "colour.h"

namespace flounder {
	colour::colour()
	{
		this->set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	colour::colour(const float &r, const float &g, const float &b)
	{
		this->set(r, g, b);
	}

	colour::colour(const float &r, const float &g, const float &b, const bool &convert)
	{
		this->set(r, g, b, convert);
	}

	colour::colour(const float &r, const float &g, const float &b, const float &a)
	{
		this->set(r, g, b, a);
	}

	colour::colour(const float &r, const float &g, const float &b, const float &a, const bool &convert)
	{
		this->set(r, g, b, a, convert);
	}

	colour::colour(const colour &source)
	{
		this->set(source);
	}

	colour::~colour()
	{
	}

	colour *colour::set(const float &r, const float &g, const float &b)
	{
		return set(r, g, b, false);
	}

	colour *colour::set(const float &r, const float &g, const float &b, const bool &convert)
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

	colour *colour::set(const float &r, const float &g, const float &b, const float &a)
	{
		return set(r, g, b, a, false);
	}

	colour *colour::set(const float &r, const float &g, const float &b, const float &a, const bool &convert)
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

	colour *colour::set(const colour &source)
	{
		return this->set(source.r, source.g, source.b, source.a);
	}

	colour *colour::add(const colour &left, const colour &right, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		return destination->set(left.r + right.r, left.g + right.g, left.b + right.b, left.a + right.a);
	}

	colour *colour::subtract(const colour &left, const colour &right, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		return destination->set(left.r - right.r, left.g - right.g, left.b - right.b, left.a - right.a);
	}

	colour *colour::multiply(const colour &left, const colour &right, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		return destination->set(left.r * right.r, left.g * right.g, left.b * right.b, left.a * right.a);
	}

	colour *colour::divide(const colour &left, const colour &right, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		return destination->set(left.r / right.r, left.g / right.g, left.b / right.b, left.a / right.a);
	}

	colour *colour::interpolate(const colour &left, const colour &right, const float blend, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		float r = ((1.0f - blend) * left.r) + (blend * right.r);
		float g = ((1.0f - blend) * left.g) + (blend * right.g);
		float b = ((1.0f - blend) * left.b) + (blend * right.b);
		float a = ((1.0f - blend) * left.a) + (blend * right.a);
		return destination->set(r, g, b, a);
	}

	colour *colour::getUnit(const colour &source, colour *destination)
	{
		if (destination == NULL)
		{
			destination = new colour();
		}

		float l = length(source);
		return destination->set(source)->scale(1.0f / l);
	}

	float colour::length(const colour &source)
	{
		return sqrt(lengthSquared(source));
	}

	float colour::lengthSquared(const colour &source)
	{
		return pow(source.r, 2) + pow(source.g, 2) + pow(source.b, 2) + pow(source.a, 2);
	}

	colour *colour::scale(const float scalar)
	{
		this->r *= scalar;
		this->g *= scalar;
		this->b *= scalar;
		return this;
	}

	float colour::length()
	{
		return length(*this);
	}

	float colour::lengthSquared()
	{
		return lengthSquared(*this);
	}
}
