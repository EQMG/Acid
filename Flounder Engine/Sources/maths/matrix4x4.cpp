#include "matrix4x4.h"

namespace flounder {
	matrix4x4::matrix4x4()
	{
		this->setIdentity();
	}

	matrix4x4::matrix4x4(const matrix4x4 &source)
	{
		this->set(source);
	}

	matrix4x4::matrix4x4(const float source[16])
	{
		this->set(source);
	}

	matrix4x4::~matrix4x4()
	{
	}

	matrix4x4 *matrix4x4::set(const matrix4x4 &source)
	{
		this->m00 = source.m00;
		this->m01 = source.m01;
		this->m02 = source.m02;
		this->m03 = source.m03;
		this->m10 = source.m10;
		this->m11 = source.m11;
		this->m12 = source.m12;
		this->m13 = source.m13;
		this->m20 = source.m20;
		this->m21 = source.m21;
		this->m22 = source.m22;
		this->m23 = source.m23;
		this->m30 = source.m30;
		this->m31 = source.m31;
		this->m32 = source.m32;
		this->m33 = source.m33;
		return this;
	}

	matrix4x4 *matrix4x4::set(const float source[16])
	{
		this->m00 = source[0];
		this->m01 = source[1];
		this->m02 = source[2];
		this->m03 = source[3];
		this->m10 = source[4];
		this->m11 = source[5];
		this->m12 = source[6];
		this->m13 = source[7];
		this->m20 = source[8];
		this->m21 = source[9];
		this->m22 = source[10];
		this->m23 = source[11];
		this->m30 = source[12];
		this->m31 = source[13];
		this->m32 = source[14];
		this->m33 = source[15];
		return this;
	}

	matrix4x4 *matrix4x4::setIdentity(matrix4x4 *source)
	{
		source->m00 = 1.0f;
		source->m01 = 0.0f;
		source->m02 = 0.0f;
		source->m03 = 0.0f;
		source->m10 = 0.0f;
		source->m11 = 1.0f;
		source->m12 = 0.0f;
		source->m13 = 0.0f;
		source->m20 = 0.0f;
		source->m21 = 0.0f;
		source->m22 = 1.0f;
		source->m23 = 0.0f;
		source->m30 = 0.0f;
		source->m31 = 0.0f;
		source->m32 = 0.0f;
		source->m33 = 1.0f;
		return source;
	}

	float matrix4x4::determinant(const matrix4x4 &source)
	{
		return (source.m00 * (source.m11 * source.m22 * source.m33 + source.m12 * source.m23 * source.m31 + source.m13 * source.m21 * source.m32 - source.m13 * source.m22 * source.m31 - source.m11 * source.m23 * source.m32 - source.m12 * source.m21 * source.m33))
			- (source.m01 * (source.m10 * source.m22 * source.m33 + source.m12 * source.m23 * source.m30 + source.m13 * source.m20 * source.m32 - source.m13 * source.m22 * source.m30 - source.m10 * source.m23 * source.m32 - source.m12 * source.m20 * source.m33))
			+ (source.m02 * (source.m10 * source.m21 * source.m33 + source.m11 * source.m23 * source.m30 + source.m13 * source.m20 * source.m31 - source.m13 * source.m21 * source.m30 - source.m10 * source.m23 * source.m31 - source.m11 * source.m20 * source.m33))
			- (source.m03 * (source.m10 * source.m21 * source.m32 + source.m11 * source.m22 * source.m30 + source.m12 * source.m20 * source.m31 - source.m12 * source.m21 * source.m30 - source.m10 * source.m22 * source.m31 - source.m11 * source.m20 * source.m32));
	}

	matrix4x4 *matrix4x4::add(const matrix4x4 &left, const matrix4x4 &right, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		destination->m00 = left.m00 + right.m00;
		destination->m01 = left.m01 + right.m01;
		destination->m02 = left.m02 + right.m02;
		destination->m03 = left.m03 + right.m03;
		destination->m10 = left.m10 + right.m10;
		destination->m11 = left.m11 + right.m11;
		destination->m12 = left.m12 + right.m12;
		destination->m13 = left.m13 + right.m13;
		destination->m20 = left.m20 + right.m20;
		destination->m21 = left.m21 + right.m21;
		destination->m22 = left.m22 + right.m22;
		destination->m23 = left.m23 + right.m23;
		destination->m30 = left.m30 + right.m30;
		destination->m31 = left.m31 + right.m31;
		destination->m32 = left.m32 + right.m32;
		destination->m33 = left.m33 + right.m33;
		return destination;
	}

	matrix4x4 *matrix4x4::subtract(const matrix4x4 &left, const matrix4x4 &right, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		destination->m00 = left.m00 - right.m00;
		destination->m01 = left.m01 - right.m01;
		destination->m02 = left.m02 - right.m02;
		destination->m03 = left.m03 - right.m03;
		destination->m10 = left.m10 - right.m10;
		destination->m11 = left.m11 - right.m11;
		destination->m12 = left.m12 - right.m12;
		destination->m13 = left.m13 - right.m13;
		destination->m20 = left.m20 - right.m20;
		destination->m21 = left.m21 - right.m21;
		destination->m22 = left.m22 - right.m22;
		destination->m23 = left.m23 - right.m23;
		destination->m30 = left.m30 - right.m30;
		destination->m31 = left.m31 - right.m31;
		destination->m32 = left.m32 - right.m32;
		destination->m33 = left.m33 - right.m33;
		return destination;
	}

	vector4 *matrix4x4::multiply(const matrix4x4 &left, const vector4 &right, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		float x = left.m00 * right.x + left.m10 * right.y + left.m20 * right.z + left.m30 * right.w;
		float y = left.m01 * right.x + left.m11 * right.y + left.m21 * right.z + left.m31 * right.w;
		float z = left.m02 * right.x + left.m12 * right.y + left.m22 * right.z + left.m32 * right.w;
		float w = left.m03 * right.x + left.m13 * right.y + left.m23 * right.z + left.m33 * right.w;
		return destination->set(x, y, z, w);
	}

	matrix4x4 *matrix4x4::multiply(const matrix4x4 &left, const matrix4x4 &right, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		float m00 = left.m00 * right.m00 + left.m10 * right.m01 + left.m20 * right.m02 + left.m30 * right.m03;
		float m01 = left.m01 * right.m00 + left.m11 * right.m01 + left.m21 * right.m02 + left.m31 * right.m03;
		float m02 = left.m02 * right.m00 + left.m12 * right.m01 + left.m22 * right.m02 + left.m32 * right.m03;
		float m03 = left.m03 * right.m00 + left.m13 * right.m01 + left.m23 * right.m02 + left.m33 * right.m03;
		float m10 = left.m00 * right.m10 + left.m10 * right.m11 + left.m20 * right.m12 + left.m30 * right.m13;
		float m11 = left.m01 * right.m10 + left.m11 * right.m11 + left.m21 * right.m12 + left.m31 * right.m13;
		float m12 = left.m02 * right.m10 + left.m12 * right.m11 + left.m22 * right.m12 + left.m32 * right.m13;
		float m13 = left.m03 * right.m10 + left.m13 * right.m11 + left.m23 * right.m12 + left.m33 * right.m13;
		float m20 = left.m00 * right.m20 + left.m10 * right.m21 + left.m20 * right.m22 + left.m30 * right.m23;
		float m21 = left.m01 * right.m20 + left.m11 * right.m21 + left.m21 * right.m22 + left.m31 * right.m23;
		float m22 = left.m02 * right.m20 + left.m12 * right.m21 + left.m22 * right.m22 + left.m32 * right.m23;
		float m23 = left.m03 * right.m20 + left.m13 * right.m21 + left.m23 * right.m22 + left.m33 * right.m23;
		float m30 = left.m00 * right.m30 + left.m10 * right.m31 + left.m20 * right.m32 + left.m30 * right.m33;
		float m31 = left.m01 * right.m30 + left.m11 * right.m31 + left.m21 * right.m32 + left.m31 * right.m33;
		float m32 = left.m02 * right.m30 + left.m12 * right.m31 + left.m22 * right.m32 + left.m32 * right.m33;
		float m33 = left.m03 * right.m30 + left.m13 * right.m31 + left.m23 * right.m32 + left.m33 * right.m33;

		destination->m00 = m00;
		destination->m01 = m01;
		destination->m02 = m02;
		destination->m03 = m03;
		destination->m10 = m10;
		destination->m11 = m11;
		destination->m12 = m12;
		destination->m13 = m13;
		destination->m20 = m20;
		destination->m21 = m21;
		destination->m22 = m22;
		destination->m23 = m23;
		destination->m30 = m30;
		destination->m31 = m31;
		destination->m32 = m32;
		destination->m33 = m33;
		return destination;
	}

	vector4 *matrix4x4::transform(const matrix4x4 &left, const vector4 &right, vector4 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector4();
		}

		float x = left.m00 * right.x + left.m10 * right.y + left.m20 * right.z + left.m30 * right.w;
		float y = left.m01 * right.x + left.m11 * right.y + left.m21 * right.z + left.m31 * right.w;
		float z = left.m02 * right.x + left.m12 * right.y + left.m22 * right.z + left.m32 * right.w;
		float w = left.m03 * right.x + left.m13 * right.y + left.m23 * right.z + left.m33 * right.w;

		return destination->set(x, y, z, w);
	}

	matrix4x4 *matrix4x4::scale(const matrix4x4 &left, const vector4 &right, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		destination->m00 = left.m00 * right.x;
		destination->m01 = left.m01 * right.x;
		destination->m02 = left.m02 * right.x;
		destination->m03 = left.m03 * right.x;
		destination->m10 = left.m10 * right.y;
		destination->m11 = left.m11 * right.y;
		destination->m12 = left.m12 * right.y;
		destination->m13 = left.m13 * right.y;
		destination->m20 = left.m20 * right.z;
		destination->m21 = left.m21 * right.z;
		destination->m22 = left.m22 * right.z;
		destination->m23 = left.m23 * right.z;
		destination->m30 = left.m30 * right.w;
		destination->m31 = left.m31 * right.w;
		destination->m32 = left.m32 * right.w;
		destination->m33 = left.m33 * right.w;
		return destination;
	}

	matrix4x4 *matrix4x4::invert(const matrix4x4 &source, matrix4x4 *destination)
	{
		float d = determinant(source);

		if (d != NULL)
		{
			if (destination == NULL)
			{
				destination = new matrix4x4();
			}

			float determinant_inv = 1.0f / d;

			// First row.
			float t00 = determinant3x3(source.m11, source.m12, source.m13, source.m21, source.m22, source.m23, source.m31, source.m32, source.m33);
			float t01 = -determinant3x3(source.m10, source.m12, source.m13, source.m20, source.m22, source.m23, source.m30, source.m32, source.m33);
			float t02 = determinant3x3(source.m10, source.m11, source.m13, source.m20, source.m21, source.m23, source.m30, source.m31, source.m33);
			float t03 = -determinant3x3(source.m10, source.m11, source.m12, source.m20, source.m21, source.m22, source.m30, source.m31, source.m32);
			// Second row.
			float t10 = -determinant3x3(source.m01, source.m02, source.m03, source.m21, source.m22, source.m23, source.m31, source.m32, source.m33);
			float t11 = determinant3x3(source.m00, source.m02, source.m03, source.m20, source.m22, source.m23, source.m30, source.m32, source.m33);
			float t12 = -determinant3x3(source.m00, source.m01, source.m03, source.m20, source.m21, source.m23, source.m30, source.m31, source.m33);
			float t13 = determinant3x3(source.m00, source.m01, source.m02, source.m20, source.m21, source.m22, source.m30, source.m31, source.m32);
			// Third row.
			float t20 = determinant3x3(source.m01, source.m02, source.m03, source.m11, source.m12, source.m13, source.m31, source.m32, source.m33);
			float t21 = -determinant3x3(source.m00, source.m02, source.m03, source.m10, source.m12, source.m13, source.m30, source.m32, source.m33);
			float t22 = determinant3x3(source.m00, source.m01, source.m03, source.m10, source.m11, source.m13, source.m30, source.m31, source.m33);
			float t23 = -determinant3x3(source.m00, source.m01, source.m02, source.m10, source.m11, source.m12, source.m30, source.m31, source.m32);
			// Fourth row.
			float t30 = -determinant3x3(source.m01, source.m02, source.m03, source.m11, source.m12, source.m13, source.m21, source.m22, source.m23);
			float t31 = determinant3x3(source.m00, source.m02, source.m03, source.m10, source.m12, source.m13, source.m20, source.m22, source.m23);
			float t32 = -determinant3x3(source.m00, source.m01, source.m03, source.m10, source.m11, source.m13, source.m20, source.m21, source.m23);
			float t33 = determinant3x3(source.m00, source.m01, source.m02, source.m10, source.m11, source.m12, source.m20, source.m21, source.m22);

			// Transpose and divide by the determinant.
			destination->m00 = t00 * determinant_inv;
			destination->m11 = t11 * determinant_inv;
			destination->m22 = t22 * determinant_inv;
			destination->m33 = t33 * determinant_inv;
			destination->m01 = t10 * determinant_inv;
			destination->m10 = t01 * determinant_inv;
			destination->m20 = t02 * determinant_inv;
			destination->m02 = t20 * determinant_inv;
			destination->m12 = t21 * determinant_inv;
			destination->m21 = t12 * determinant_inv;
			destination->m03 = t30 * determinant_inv;
			destination->m30 = t03 * determinant_inv;
			destination->m13 = t31 * determinant_inv;
			destination->m31 = t13 * determinant_inv;
			destination->m32 = t23 * determinant_inv;
			destination->m23 = t32 * determinant_inv;
			return destination;
		}
		else
		{
			return NULL;
		}
	}

	matrix4x4 *matrix4x4::negate(const matrix4x4 &source, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		destination->m00 = -source.m00;
		destination->m01 = -source.m01;
		destination->m02 = -source.m02;
		destination->m03 = -source.m03;
		destination->m10 = -source.m10;
		destination->m11 = -source.m11;
		destination->m12 = -source.m12;
		destination->m13 = -source.m13;
		destination->m20 = -source.m20;
		destination->m21 = -source.m21;
		destination->m22 = -source.m22;
		destination->m23 = -source.m23;
		destination->m30 = -source.m30;
		destination->m31 = -source.m31;
		destination->m32 = -source.m32;
		destination->m33 = -source.m33;
		return destination;
	}

	matrix4x4 *matrix4x4::transpose(const matrix4x4 &source, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		float m00 = source.m00;
		float m01 = source.m10;
		float m02 = source.m20;
		float m03 = source.m30;
		float m10 = source.m01;
		float m11 = source.m11;
		float m12 = source.m21;
		float m13 = source.m31;
		float m20 = source.m02;
		float m21 = source.m12;
		float m22 = source.m22;
		float m23 = source.m32;
		float m30 = source.m03;
		float m31 = source.m13;
		float m32 = source.m23;
		float m33 = source.m33;

		destination->m00 = m00;
		destination->m01 = m01;
		destination->m02 = m02;
		destination->m03 = m03;
		destination->m10 = m10;
		destination->m11 = m11;
		destination->m12 = m12;
		destination->m13 = m13;
		destination->m20 = m20;
		destination->m21 = m21;
		destination->m22 = m22;
		destination->m23 = m23;
		destination->m30 = m30;
		destination->m31 = m31;
		destination->m32 = m32;
		destination->m33 = m33;
		return destination;
	}

	matrix4x4 *matrix4x4::translate(const matrix4x4 &left, const vector2 &right, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		destination->m30 += left.m00 * right.x + left.m10 * right.y;
		destination->m31 += left.m01 * right.x + left.m11 * right.y;
		destination->m32 += left.m02 * right.x + left.m12 * right.y;
		destination->m33 += left.m03 * right.x + left.m13 * right.y;
		return destination;
	}

	matrix4x4 *matrix4x4::translate(const matrix4x4 &left, const vector3 &right, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		destination->m30 += left.m00 * right.x + left.m10 * right.y + left.m20 * right.z;
		destination->m31 += left.m01 * right.x + left.m11 * right.y + left.m21 * right.z;
		destination->m32 += left.m02 * right.x + left.m12 * right.y + left.m22 * right.z;
		destination->m33 += left.m03 * right.x + left.m13 * right.y + left.m23 * right.z;
		return destination;
	}

	matrix4x4 *matrix4x4::rotate(const matrix4x4 &source, const vector3 &axis, const float &angle, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		float c = (float) cos(angle);
		float s = (float) sin(angle);
		float o = 1.0f - c;
		float xy = axis.x * axis.y;
		float yz = axis.y * axis.z;
		float xz = axis.x * axis.z;
		float xs = axis.x * s;
		float ys = axis.y * s;
		float zs = axis.z * s;

		float f00 = axis.x * axis.x * o + c;
		float f01 = xy * o + zs;
		float f02 = xz * o - ys;
		float f10 = xy * o - zs;
		float f11 = axis.y * axis.y * o + c;
		float f12 = yz * o + xs;
		float f20 = xz * o + ys;
		float f21 = yz * o - xs;
		float f22 = axis.z * axis.z * o + c;

		float t00 = source.m00 * f00 + source.m10 * f01 + source.m20 * f02;
		float t01 = source.m01 * f00 + source.m11 * f01 + source.m21 * f02;
		float t02 = source.m02 * f00 + source.m12 * f01 + source.m22 * f02;
		float t03 = source.m03 * f00 + source.m13 * f01 + source.m23 * f02;
		float t10 = source.m00 * f10 + source.m10 * f11 + source.m20 * f12;
		float t11 = source.m01 * f10 + source.m11 * f11 + source.m21 * f12;
		float t12 = source.m02 * f10 + source.m12 * f11 + source.m22 * f12;
		float t13 = source.m03 * f10 + source.m13 * f11 + source.m23 * f12;
		destination->m20 = source.m00 * f20 + source.m10 * f21 + source.m20 * f22;
		destination->m21 = source.m01 * f20 + source.m11 * f21 + source.m21 * f22;
		destination->m22 = source.m02 * f20 + source.m12 * f21 + source.m22 * f22;
		destination->m23 = source.m03 * f20 + source.m13 * f21 + source.m23 * f22;
		destination->m00 = t00;
		destination->m01 = t01;
		destination->m02 = t02;
		destination->m03 = t03;
		destination->m10 = t10;
		destination->m11 = t11;
		destination->m12 = t12;
		destination->m13 = t13;
		return destination;
	}

	float *matrix4x4::toArray(const matrix4x4 &matrix)
	{
		float *result = new float[16];
		result[0] = matrix.m00;
		result[1] = matrix.m01;
		result[2] = matrix.m02;
		result[3] = matrix.m03;
		result[4] = matrix.m10;
		result[5] = matrix.m11;
		result[6] = matrix.m12;
		result[7] = matrix.m13;
		result[8] = matrix.m20;
		result[9] = matrix.m21;
		result[10] = matrix.m22;
		result[11] = matrix.m23;
		result[12] = matrix.m30;
		result[13] = matrix.m31;
		result[14] = matrix.m32;
		result[15] = matrix.m33;
		return result;
	}

	matrix4x4 *matrix4x4::setZero(matrix4x4 *source)
	{
		source->m00 = 0.0f;
		source->m01 = 0.0f;
		source->m02 = 0.0f;
		source->m03 = 0.0f;
		source->m10 = 0.0f;
		source->m11 = 0.0f;
		source->m12 = 0.0f;
		source->m13 = 0.0f;
		source->m20 = 0.0f;
		source->m21 = 0.0f;
		source->m22 = 0.0f;
		source->m23 = 0.0f;
		source->m30 = 0.0f;
		source->m31 = 0.0f;
		source->m32 = 0.0f;
		source->m33 = 0.0f;
		return source;
	}

	vector3 *matrix4x4::rotate(const vector3 &source, const vector3 &rotation, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		matrix4x4 *matrix = matrix4x4::transformationMatrix(vector3(0.0f, 0.0f, 0.0f), rotation, vector3(1.0f, 1.0f, 1.0f), NULL);
		vector4 direction4 = vector4(source.x, source.y, source.z, 1.0f);
		matrix4x4::transform(*matrix, direction4, &direction4);
		delete matrix;
		return destination->set(direction4.x, direction4.y, direction4.z);
	}

	matrix4x4 *matrix4x4::transformationMatrix(const vector2 &translation, const float &scale, matrix4x4 *destination)
	{
		return transformationMatrix(vector3(translation.x, translation.y, 0.0f), vector3(0.0f, 0.0f, 0.0f), vector3(scale, scale, scale), destination);
	}

	matrix4x4 *matrix4x4::transformationMatrix(const vector2 &translation, const vector3 &scale, matrix4x4 *destination)
	{
		return transformationMatrix(vector3(translation.x, translation.y, 0.0f), vector3(0.0f, 0.0f, 0.0f), scale, destination);
	}

	matrix4x4 *matrix4x4::transformationMatrix(const vector3 &translation, const vector3 &rotation, const float &scale, matrix4x4 *destination)
	{
		return transformationMatrix(translation, rotation, vector3(scale, scale, scale), destination);
	}

	matrix4x4 *matrix4x4::transformationMatrix(const vector3 &translation, const vector3 &rotation, const vector3 &scale, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		destination->setIdentity();

		if (vector3::lengthSquared(translation) != 0.0f)
		{
			matrix4x4::translate(*destination, translation, destination);
		}

		if (vector3::lengthSquared(rotation) != 0.0f)
		{
			matrix4x4::rotate(*destination, vector3(1.0f, 0.0f, 0.0f), __radians(rotation.x), destination); // Rotate the X component.
			matrix4x4::rotate(*destination, vector3(0.0f, 1.0f, 0.0f), __radians(rotation.y), destination); // Rotate the Y component.
			matrix4x4::rotate(*destination, vector3(0.0f, 0.0f, 1.0f), __radians(rotation.z), destination); // Rotate the Z component.
		}

		// Only scales if there is a scale.
		if (scale.x != 1.0f && scale.y != 1.0f && scale.z != 1.0f)
		{
			matrix4x4::scale(*destination, vector4(scale.x, scale.y, scale.z, 1.0f), destination);
		}

		return destination;
	}

	matrix4x4 *matrix4x4::perspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear, const float &zFar, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		destination->setIdentity();
		float yScale = 1.0f / tan(__radians(fov / 2.0f));
		float xScale = yScale / aspectRatio;
		float length = zFar - zNear;

		destination->m00 = xScale;
		destination->m11 = yScale;
		destination->m22 = -((zFar + zNear) / length);
		destination->m23 = -1.0f;
		destination->m32 = -((2.0f * zNear * zFar) / length);
		destination->m33 = 0.0f;
		return destination;
	}

	matrix4x4 *matrix4x4::orthographicMatrix(const float &left, const float &right, const float &bottom, const float &top, const float &near, const float &far, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		float ox = 2.0f / (right - left);
		float oy = 2.0f / (top - bottom);
		float oz = -2.0f / (far - near);

		float tx = -(right + left) / (right - left);
		float ty = -(top + bottom) / (top - bottom);
		float tz = -(far + near) / (far - near);

		destination->setIdentity();
		destination->m00 = ox;
		destination->m11 = oy;
		destination->m22 = oz;
		destination->m03 = tx;
		destination->m13 = ty;
		destination->m23 = tz;
		destination->m33 = 1.0f;
		return destination;
	}

	matrix4x4 *matrix4x4::viewMatrix(const vector3 &position, const vector3 &rotation, matrix4x4 *destination)
	{
		if (destination == NULL)
		{
			destination = new matrix4x4();
		}

		destination->setIdentity();
		vector3 point = vector3(position);
		point.negate();
		matrix4x4::rotate(*destination, vector3(1.0f, 0.0f, 0.0f), (float) __radians(rotation.x), destination);
		matrix4x4::rotate(*destination, vector3(0.0f, 1.0f, 0.0f), (float) __radians(-rotation.y), destination);
		matrix4x4::rotate(*destination, vector3(0.0f, 0.0f, 1.0f), (float) __radians(rotation.z), destination);
		matrix4x4::translate(*destination, point, destination);
		return destination;
	}

	vector3 *matrix4x4::worldToScreenSpace(const vector3 &worldSpace, const matrix4x4 &viewMatrix, const matrix4x4 &projectionMatrix, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		vector4 point4 = vector4(worldSpace.x, worldSpace.y, worldSpace.z, 1.0f);
		matrix4x4::transform(viewMatrix, point4, &point4);
		matrix4x4::transform(projectionMatrix, point4, &point4);
		vector3 point = vector3(point4);

		point.x /= point.z;
		point.y /= point.z;
		return destination->set(point.x, point.y, point.z);
	}

	vector3 *matrix4x4::generateRandomUnitVectorWithinCone(const vector3 &coneDirection, const float &angle, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		float cosAngle = cos(angle);
		float theta = (float) __random();
		theta *= 2.0f * PI;
		float z = cosAngle + (float)__random();
		z *= (1.0f - cosAngle);
		float rootOneMinusZSquared = sqrt(1.0f - z * z);
		float x = rootOneMinusZSquared * cos(theta);
		float y = rootOneMinusZSquared * sin(theta);

		vector4 direction = vector4(x, y, z, 1.0f);

		if ((coneDirection.x != 0.0F) || (coneDirection.y != 0.0F) || ((coneDirection.z != 1.0f) && (coneDirection.z != -1.0f)))
		{
			vector3 *rotateAxis = vector3::cross(coneDirection, vector3(0.0f, 0.0f, 1.0f), NULL);
			rotateAxis->normalize();
			float rotateAngle = acos(vector3::dot(coneDirection, vector3(0.0f, 0.0f, 1.0f)));
			matrix4x4 rotationMatrix = matrix4x4();
			rotationMatrix.setIdentity();
			matrix4x4::rotate(rotationMatrix, *rotateAxis, -rotateAngle, &rotationMatrix);
			matrix4x4::transform(rotationMatrix, direction, &direction);
			delete rotateAxis;
		}
		else if (coneDirection.z == -1.0f)
		{
			direction.z *= -1.0f;
		}

		return destination->set(direction);
	}

	matrix4x4 *matrix4x4::setIdentity()
	{
		return setIdentity(this);
	}

	float matrix4x4::determinant()
	{
		return determinant(*this);
	}

	matrix4x4 *matrix4x4::invert()
	{
		return invert(*this, this);
	}

	matrix4x4 *matrix4x4::negate()
	{
		return negate(*this, this);
	}

	matrix4x4 *matrix4x4::transpose()
	{
		return transpose(*this, this);
	}

	matrix4x4 *matrix4x4::setZero()
	{
		return setZero(this);
	}

	float matrix4x4::determinant3x3(const float &t00, const float &t01, const float &t02, const float &t10, const float &t11, const float &t12, const float &t20, const float &t21, const float &t22)
	{
		return t00 * (t11 * t22 - t12 * t21) + t01 * (t12 * t20 - t10 * t22) + t02 * (t10 * t21 - t11 * t20);
	}
}
