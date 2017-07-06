#include "matrix3x3.h"

namespace flounder {
	matrix3x3::matrix3x3()
	{
		this->setIdentity();
	}

	matrix3x3::matrix3x3(matrix3x3 *source)
	{
		source->set(source);
	}

	matrix3x3::matrix3x3(float source[9])
	{
		this->set(source);
	}

	matrix3x3::~matrix3x3()
	{
	}

	matrix3x3 *matrix3x3::set(matrix3x3 *source)
	{
		this->m00 = source->m00;
		this->m01 = source->m01;
		this->m02 = source->m02;
		this->m10 = source->m10;
		this->m11 = source->m11;
		this->m12 = source->m12;
		this->m20 = source->m20;
		this->m21 = source->m21;
		this->m22 = source->m22;
		return this;
	}

	matrix3x3 * matrix3x3::set(float source[9])
	{
		this->m00 = source[0];
		this->m01 = source[1];
		this->m02 = source[2];
		this->m10 = source[3];
		this->m11 = source[4];
		this->m12 = source[5];
		this->m20 = source[6];
		this->m21 = source[7];
		this->m22 = source[8];
		return this;
	}

	matrix3x3 *matrix3x3::setIdentity(matrix3x3 *source)
	{
		source->m00 = 1.0f;
		source->m01 = 0.0f;
		source->m02 = 0.0f;
		source->m10 = 0.0f;
		source->m11 = 1.0f;
		source->m12 = 0.0f;
		source->m20 = 0.0f;
		source->m21 = 0.0f;
		source->m22 = 1.0f;
		return source;
	}
	matrix3x3 *matrix3x3::add(matrix3x3 *left, matrix3x3 *right, matrix3x3 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix3x3();
		}

		destination->m00 = left->m00 + right->m00;
		destination->m01 = left->m01 + right->m01;
		destination->m02 = left->m02 + right->m02;
		destination->m10 = left->m10 + right->m10;
		destination->m11 = left->m11 + right->m11;
		destination->m12 = left->m12 + right->m12;
		destination->m20 = left->m20 + right->m20;
		destination->m21 = left->m21 + right->m21;
		destination->m22 = left->m22 + right->m22;
		return destination;
	}

	matrix3x3 *matrix3x3::subtract(matrix3x3 *left, matrix3x3 *right, matrix3x3 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix3x3();
		}

		destination->m00 = left->m00 - right->m00;
		destination->m01 = left->m01 - right->m01;
		destination->m02 = left->m02 - right->m02;
		destination->m10 = left->m10 - right->m10;
		destination->m11 = left->m11 - right->m11;
		destination->m12 = left->m12 - right->m12;
		destination->m20 = left->m20 - right->m20;
		destination->m21 = left->m21 - right->m21;
		destination->m22 = left->m22 - right->m22;
		return destination;
	}

	matrix3x3 *matrix3x3::multiply(matrix3x3 *left, matrix3x3 *right, matrix3x3 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix3x3();
		}

		float m00 = left->m00 * right->m00 + left->m10 * right->m01 + left->m20 * right->m02;
		float m01 = left->m01 * right->m00 + left->m11 * right->m01 + left->m21 * right->m02;
		float m02 = left->m02 * right->m00 + left->m12 * right->m01 + left->m22 * right->m02;
		float m10 = left->m00 * right->m10 + left->m10 * right->m11 + left->m20 * right->m12;
		float m11 = left->m01 * right->m10 + left->m11 * right->m11 + left->m21 * right->m12;
		float m12 = left->m02 * right->m10 + left->m12 * right->m11 + left->m22 * right->m12;
		float m20 = left->m00 * right->m20 + left->m10 * right->m21 + left->m20 * right->m22;
		float m21 = left->m01 * right->m20 + left->m11 * right->m21 + left->m21 * right->m22;
		float m22 = left->m02 * right->m20 + left->m12 * right->m21 + left->m22 * right->m22;

		destination->m00 = m00;
		destination->m01 = m01;
		destination->m02 = m02;
		destination->m10 = m10;
		destination->m11 = m11;
		destination->m12 = m12;
		destination->m20 = m20;
		destination->m21 = m21;
		destination->m22 = m22;
		return destination;
	}

	vector3 *matrix3x3::transform(matrix3x3 *left, vector3 *right, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}


		float x = left->m00 * right->x + left->m10 * right->y + left->m20 * right->z;
		float y = left->m01 * right->x + left->m11 * right->y + left->m21 * right->z;
		float z = left->m02 * right->x + left->m12 * right->y + left->m22 * right->z;

		destination->x = x;
		destination->y = y;
		destination->z = z;
		return destination;
	}

	matrix3x3 * matrix3x3::scale(matrix3x3 * left, vector3 * right, matrix3x3 * destination)
	{
		if (destination == NULL)
		{
			destination = new matrix3x3();
		}

		destination->m00 = left->m00 * right->x;
		destination->m01 = left->m01 * right->x;
		destination->m02 = left->m02 * right->x;
		destination->m10 = left->m10 * right->y;
		destination->m11 = left->m11 * right->y;
		destination->m12 = left->m12 * right->y;
		destination->m20 = left->m20 * right->z;
		destination->m21 = left->m21 * right->z;
		destination->m22 = left->m22 * right->z;
		return destination;
	}

	matrix3x3 *matrix3x3::invert(matrix3x3 *source, matrix3x3 *destination)
	{
		float determinant = source->determinant();

		if (determinant != 0.0f)
		{
			if (destination == NULL)
			{
				destination = new matrix3x3();
			}

			/*
			* Does it the ordinary way. inv(A) = 1/det(A) * adj(T), where adj(T) = transpose(Conjugate Matrix) m00 m01 m02 m10 m11 m12 m20 m21 m22
			*/
			float determinant_inv = 1.0f / determinant;

			// Get the conjugate matrix.
			float t00 = source->m11 * source->m22 - source->m12 * source->m21;
			float t01 = -source->m10 * source->m22 + source->m12 * source->m20;
			float t02 = source->m10 * source->m21 - source->m11 * source->m20;
			float t10 = -source->m01 * source->m22 + source->m02 * source->m21;
			float t11 = source->m00 * source->m22 - source->m02 * source->m20;
			float t12 = -source->m00 * source->m21 + source->m01 * source->m20;
			float t20 = source->m01 * source->m12 - source->m02 * source->m11;
			float t21 = -source->m00 * source->m12 + source->m02 * source->m10;
			float t22 = source->m00 * source->m11 - source->m01 * source->m10;

			destination->m00 = t00 * determinant_inv;
			destination->m11 = t11 * determinant_inv;
			destination->m22 = t22 * determinant_inv;
			destination->m01 = t10 * determinant_inv;
			destination->m10 = t01 * determinant_inv;
			destination->m20 = t02 * determinant_inv;
			destination->m02 = t20 * determinant_inv;
			destination->m12 = t21 * determinant_inv;
			destination->m21 = t12 * determinant_inv;
			return destination;
		}
		else
		{
			return NULL;
		}
	}

	matrix3x3 *matrix3x3::negate(matrix3x3 *source, matrix3x3 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix3x3();
		}

		destination->m00 = -source->m00;
		destination->m01 = -source->m02;
		destination->m02 = -source->m01;
		destination->m10 = -source->m10;
		destination->m11 = -source->m12;
		destination->m12 = -source->m11;
		destination->m20 = -source->m20;
		destination->m21 = -source->m22;
		destination->m22 = -source->m21;
		return destination;
	}

	matrix3x3 *matrix3x3::transpose(matrix3x3 *source, matrix3x3 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix3x3();
		}

		float m00 = source->m00;
		float m01 = source->m10;
		float m02 = source->m20;
		float m10 = source->m01;
		float m11 = source->m11;
		float m12 = source->m21;
		float m20 = source->m02;
		float m21 = source->m12;
		float m22 = source->m22;

		destination->m00 = m00;
		destination->m01 = m01;
		destination->m02 = m02;
		destination->m10 = m10;
		destination->m11 = m11;
		destination->m12 = m12;
		destination->m20 = m20;
		destination->m21 = m21;
		destination->m22 = m22;
		return destination;
	}

	float *matrix3x3::toArray(matrix3x3 *matrix)
	{
		float *result = new float[9];
		result[0] = matrix->m00;
		result[1] = matrix->m01;
		result[2] = matrix->m02;
		result[3] = matrix->m10;
		result[4] = matrix->m11;
		result[5] = matrix->m12;
		result[6] = matrix->m20;
		result[7] = matrix->m21;
		result[8] = matrix->m22;
		return result;
	}

	matrix3x3 *matrix3x3::setZero(matrix3x3 *source)
	{
		source->m00 = 0.0f;
		source->m01 = 0.0f;
		source->m02 = 0.0f;
		source->m10 = 0.0f;
		source->m11 = 0.0f;
		source->m12 = 0.0f;
		source->m20 = 0.0f;
		source->m21 = 0.0f;
		source->m22 = 0.0f;
		return source;
	}

	matrix3x3 *matrix3x3::setIdentity()
	{
		return setIdentity(this);
	}

	float matrix3x3::determinant()
	{
		return m00 * (m11 * m22 - m12 * m21) + m01 * (m12 * m20 - m10 * m22) + m02 * (m10 * m21 - m11 * m20);
	}

	matrix3x3 *matrix3x3::invert()
	{
		return invert(this, this);
	}

	matrix3x3 *matrix3x3::negate()
	{
		return negate(this, this);
	}

	matrix3x3 *matrix3x3::transpose()
	{
		return transpose(this, this);
	}

	matrix3x3 *matrix3x3::setZero()
	{
		return setZero(this);
	}
}
