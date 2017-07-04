#include "matrix2x2.h"

namespace flounder {
	matrix2x2::matrix2x2()
	{
		this->setIdentity();
	}

	matrix2x2::matrix2x2(matrix2x2 *source)
	{
		source->set(source);
	}

	matrix2x2::matrix2x2(float source[4])
	{
		this->set(source);
	}

	matrix2x2::~matrix2x2()
	{
		delete &m00;
		delete &m01;
		delete &m10;
		delete &m11;
	}

	matrix2x2 *matrix2x2::set(matrix2x2 *source)
	{
		this->m00 = source->m00;
		this->m01 = source->m01;
		this->m10 = source->m10;
		this->m11 = source->m11;
		return this;
	}

	matrix2x2 * matrix2x2::set(float source[4])
	{
		this->m00 = source[0];
		this->m01 = source[1];
		this->m10 = source[2];
		this->m11 = source[3];
		return this;
	}

	matrix2x2 *matrix2x2::setIdentity(matrix2x2 *source)
	{
		source->m00 = 1.0f;
		source->m01 = 0.0f;
		source->m10 = 0.0f;
		source->m11 = 1.0f;
		return source;
	}

	matrix2x2 *matrix2x2::add(matrix2x2 *left, matrix2x2 *right, matrix2x2 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix2x2();
		}

		destination->m00 = left->m00 + right->m00;
		destination->m01 = left->m01 + right->m01;
		destination->m10 = left->m10 + right->m10;
		destination->m11 = left->m11 + right->m11;
		return destination;
	}

	matrix2x2 *matrix2x2::subtract(matrix2x2 *left, matrix2x2 *right, matrix2x2 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix2x2();
		}

		destination->m00 = left->m00 - right->m00;
		destination->m01 = left->m01 - right->m01;
		destination->m10 = left->m10 - right->m10;
		destination->m11 = left->m11 - right->m11;
		return destination;
	}

	matrix2x2 *matrix2x2::multiply(matrix2x2 *left, matrix2x2 *right, matrix2x2 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix2x2();
		}

		float m00 = left->m00 * right->m00 + left->m10 * right->m01;
		float m01 = left->m01 * right->m00 + left->m11 * right->m01;
		float m10 = left->m00 * right->m10 + left->m10 * right->m11;
		float m11 = left->m01 * right->m10 + left->m11 * right->m11;

		destination->m00 = m00;
		destination->m01 = m01;
		destination->m10 = m10;
		destination->m11 = m11;
		return destination;
	}

	vector2 *matrix2x2::transform(matrix2x2 *left, vector2 *right, vector2 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector2();
		}

		float x = left->m00 * right->x + left->m10 * right->y;
		float y = left->m01 * right->x + left->m11 * right->y;

		destination->x = x;
		destination->y = y;
		return destination;
	}

	matrix2x2 * matrix2x2::scale(matrix2x2 * left, vector2 * right, matrix2x2 * destination)
	{
		if (destination == NULL)
		{
			destination = new matrix2x2();
		}

		destination->m00 = left->m00 * right->x;
		destination->m01 = left->m01 * right->x;
		destination->m10 = left->m10 * right->y;
		destination->m11 = left->m11 * right->y;
		return destination;
	}

	matrix2x2 *matrix2x2::invert(matrix2x2 *source, matrix2x2 *destination)
	{
		float determinant = source->determinant();

		if (determinant != 0.0f)
		{
			if (destination == NULL)
			{
				destination = new matrix2x2();
			}

			float determinant_inv = 1 / determinant;
			float t00 = source->m11 * determinant_inv;
			float t01 = -source->m01 * determinant_inv;
			float t11 = source->m00 * determinant_inv;
			float t10 = -source->m10 * determinant_inv;

			destination->m00 = t00;
			destination->m01 = t01;
			destination->m10 = t10;
			destination->m11 = t11;
			return destination;
		}
		else
		{
			return NULL;
		}
	}

	matrix2x2 *matrix2x2::negate(matrix2x2 *source, matrix2x2 *destination)
	{
		if (destination == 0)
		{
			destination = new matrix2x2();
		}

		destination->m00 = -source->m00;
		destination->m01 = -source->m01;
		destination->m10 = -source->m10;
		destination->m11 = -source->m11;
		return destination;
	}

	matrix2x2 *matrix2x2::transpose(matrix2x2 *source, matrix2x2 *destination)
	{
		if (destination == 0)
		{
			destination = new matrix2x2();
		}

		float m01 = source->m10;
		float m10 = source->m01;

		destination->m01 = m01;
		destination->m10 = m10;
		return destination;
	}

	float *matrix2x2::toArray(matrix2x2 *matrix)
	{
		float result[16];
		result[0] = matrix->m00;
		result[1] = matrix->m01;
		result[2] = matrix->m10;
		result[3] = matrix->m11;
		return result;
	}

	matrix2x2 *matrix2x2::setZero(matrix2x2 *source)
	{
		source->m00 = 0.0f;
		source->m01 = 0.0f;
		source->m10 = 0.0f;
		source->m11 = 0.0f;
		return source;
	}

	matrix2x2 *matrix2x2::setIdentity()
	{
		return setIdentity(this);
	}

	float matrix2x2::determinant()
	{
		return m00 * m11 - m01 * m10;
	}

	matrix2x2 *matrix2x2::invert()
	{
		return invert(this, this);
	}

	matrix2x2 *matrix2x2::negate()
	{
		return negate(this, this);
	}

	matrix2x2 *matrix2x2::transpose()
	{
		return transpose(this, this);
	}

	matrix2x2 *matrix2x2::setZero()
	{
		return setZero(this);
	}
}
