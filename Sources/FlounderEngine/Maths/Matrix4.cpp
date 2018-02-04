#include "Matrix4.hpp"

#include <sstream>
#include "Maths.hpp"

namespace Flounder
{
	Matrix4::Matrix4() :
		m_00(1.0f),
		m_01(0.0f),
		m_02(0.0f),
		m_03(0.0f),
		m_10(0.0f),
		m_11(1.0f),
		m_12(0.0f),
		m_13(0.0f),
		m_20(0.0f),
		m_21(0.0f),
		m_22(1.0f),
		m_23(0.0f),
		m_30(0.0f),
		m_31(0.0f),
		m_32(0.0f),
		m_33(1.0f)
	{
	}

	Matrix4::Matrix4(const Matrix4 &source) :
		m_00(source.m_00),
		m_01(source.m_01),
		m_02(source.m_02),
		m_03(source.m_03),
		m_10(source.m_10),
		m_11(source.m_11),
		m_12(source.m_12),
		m_13(source.m_13),
		m_20(source.m_20),
		m_21(source.m_21),
		m_22(source.m_22),
		m_23(source.m_23),
		m_30(source.m_30),
		m_31(source.m_31),
		m_32(source.m_32),
		m_33(source.m_33)
	{
	}

	Matrix4::Matrix4(const float source[16]) :
		m_00(source[0]),
		m_01(source[1]),
		m_02(source[2]),
		m_03(source[3]),
		m_10(source[4]),
		m_11(source[5]),
		m_12(source[6]),
		m_13(source[7]),
		m_20(source[8]),
		m_21(source[9]),
		m_22(source[10]),
		m_23(source[11]),
		m_30(source[12]),
		m_31(source[13]),
		m_32(source[14]),
		m_33(source[15])
	{
	}

	Matrix4::~Matrix4()
	{
	}

	Matrix4 *Matrix4::Set(const Matrix4 &source)
	{
		m_00 = source.m_00;
		m_01 = source.m_01;
		m_02 = source.m_02;
		m_03 = source.m_03;
		m_10 = source.m_10;
		m_11 = source.m_11;
		m_12 = source.m_12;
		m_13 = source.m_13;
		m_20 = source.m_20;
		m_21 = source.m_21;
		m_22 = source.m_22;
		m_23 = source.m_23;
		m_30 = source.m_30;
		m_31 = source.m_31;
		m_32 = source.m_32;
		m_33 = source.m_33;
		return this;
	}

	Matrix4 *Matrix4::Set(const float source[16])
	{
		m_00 = source[0];
		m_01 = source[1];
		m_02 = source[2];
		m_03 = source[3];
		m_10 = source[4];
		m_11 = source[5];
		m_12 = source[6];
		m_13 = source[7];
		m_20 = source[8];
		m_21 = source[9];
		m_22 = source[10];
		m_23 = source[11];
		m_30 = source[12];
		m_31 = source[13];
		m_32 = source[14];
		m_33 = source[15];
		return this;
	}

	Matrix4 *Matrix4::SetIdentity(Matrix4 *source)
	{
		source->m_00 = 1.0f;
		source->m_01 = 0.0f;
		source->m_02 = 0.0f;
		source->m_03 = 0.0f;
		source->m_10 = 0.0f;
		source->m_11 = 1.0f;
		source->m_12 = 0.0f;
		source->m_13 = 0.0f;
		source->m_20 = 0.0f;
		source->m_21 = 0.0f;
		source->m_22 = 1.0f;
		source->m_23 = 0.0f;
		source->m_30 = 0.0f;
		source->m_31 = 0.0f;
		source->m_32 = 0.0f;
		source->m_33 = 1.0f;
		return source;
	}

	float Matrix4::Determinant(const Matrix4 &source)
	{
		return (source.m_00 * (source.m_11 * source.m_22 * source.m_33 + source.m_12 * source.m_23 * source.m_31 +
			source.m_13 * source.m_21 * source.m_32 - source.m_13 * source.m_22 * source.m_31 -
			source.m_11 * source.m_23 * source.m_32 - source.m_12 * source.m_21 * source.m_33))
			- (source.m_01 * (source.m_10 * source.m_22 * source.m_33 + source.m_12 * source.m_23 * source.m_30 +
			source.m_13 * source.m_20 * source.m_32 - source.m_13 * source.m_22 * source.m_30 -
			source.m_10 * source.m_23 * source.m_32 - source.m_12 * source.m_20 * source.m_33))
			+ (source.m_02 * (source.m_10 * source.m_21 * source.m_33 + source.m_11 * source.m_23 * source.m_30 +
			source.m_13 * source.m_20 * source.m_31 - source.m_13 * source.m_21 * source.m_30 -
			source.m_10 * source.m_23 * source.m_31 - source.m_11 * source.m_20 * source.m_33))
			- (source.m_03 * (source.m_10 * source.m_21 * source.m_32 + source.m_11 * source.m_22 * source.m_30 +
			source.m_12 * source.m_20 * source.m_31 - source.m_12 * source.m_21 * source.m_30 -
			source.m_10 * source.m_22 * source.m_31 - source.m_11 * source.m_20 * source.m_32));
	}

	Matrix4 *Matrix4::Add(const Matrix4 &left, const Matrix4 &right, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		destination->m_00 = left.m_00 + right.m_00;
		destination->m_01 = left.m_01 + right.m_01;
		destination->m_02 = left.m_02 + right.m_02;
		destination->m_03 = left.m_03 + right.m_03;
		destination->m_10 = left.m_10 + right.m_10;
		destination->m_11 = left.m_11 + right.m_11;
		destination->m_12 = left.m_12 + right.m_12;
		destination->m_13 = left.m_13 + right.m_13;
		destination->m_20 = left.m_20 + right.m_20;
		destination->m_21 = left.m_21 + right.m_21;
		destination->m_22 = left.m_22 + right.m_22;
		destination->m_23 = left.m_23 + right.m_23;
		destination->m_30 = left.m_30 + right.m_30;
		destination->m_31 = left.m_31 + right.m_31;
		destination->m_32 = left.m_32 + right.m_32;
		destination->m_33 = left.m_33 + right.m_33;
		return destination;
	}

	Matrix4 *Matrix4::Subtract(const Matrix4 &left, const Matrix4 &right, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		destination->m_00 = left.m_00 - right.m_00;
		destination->m_01 = left.m_01 - right.m_01;
		destination->m_02 = left.m_02 - right.m_02;
		destination->m_03 = left.m_03 - right.m_03;
		destination->m_10 = left.m_10 - right.m_10;
		destination->m_11 = left.m_11 - right.m_11;
		destination->m_12 = left.m_12 - right.m_12;
		destination->m_13 = left.m_13 - right.m_13;
		destination->m_20 = left.m_20 - right.m_20;
		destination->m_21 = left.m_21 - right.m_21;
		destination->m_22 = left.m_22 - right.m_22;
		destination->m_23 = left.m_23 - right.m_23;
		destination->m_30 = left.m_30 - right.m_30;
		destination->m_31 = left.m_31 - right.m_31;
		destination->m_32 = left.m_32 - right.m_32;
		destination->m_33 = left.m_33 - right.m_33;
		return destination;
	}

	Vector4 *Matrix4::Multiply(const Matrix4 &left, const Vector4 &right, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		const float x = left.m_00 * right.m_x + left.m_10 * right.m_y + left.m_20 * right.m_z + left.m_30 * right.m_w;
		const float y = left.m_01 * right.m_x + left.m_11 * right.m_y + left.m_21 * right.m_z + left.m_31 * right.m_w;
		const float z = left.m_02 * right.m_x + left.m_12 * right.m_y + left.m_22 * right.m_z + left.m_32 * right.m_w;
		const float w = left.m_03 * right.m_x + left.m_13 * right.m_y + left.m_23 * right.m_z + left.m_33 * right.m_w;
		return destination->Set(x, y, z, w);
	}

	Matrix4 *Matrix4::Multiply(const Matrix4 &left, const Matrix4 &right, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		const float m00 = left.m_00 * right.m_00 + left.m_10 * right.m_01 + left.m_20 * right.m_02 + left.m_30 * right.m_03;
		const float m01 = left.m_01 * right.m_00 + left.m_11 * right.m_01 + left.m_21 * right.m_02 + left.m_31 * right.m_03;
		const float m02 = left.m_02 * right.m_00 + left.m_12 * right.m_01 + left.m_22 * right.m_02 + left.m_32 * right.m_03;
		const float m03 = left.m_03 * right.m_00 + left.m_13 * right.m_01 + left.m_23 * right.m_02 + left.m_33 * right.m_03;
		const float m10 = left.m_00 * right.m_10 + left.m_10 * right.m_11 + left.m_20 * right.m_12 + left.m_30 * right.m_13;
		const float m11 = left.m_01 * right.m_10 + left.m_11 * right.m_11 + left.m_21 * right.m_12 + left.m_31 * right.m_13;
		const float m12 = left.m_02 * right.m_10 + left.m_12 * right.m_11 + left.m_22 * right.m_12 + left.m_32 * right.m_13;
		const float m13 = left.m_03 * right.m_10 + left.m_13 * right.m_11 + left.m_23 * right.m_12 + left.m_33 * right.m_13;
		const float m20 = left.m_00 * right.m_20 + left.m_10 * right.m_21 + left.m_20 * right.m_22 + left.m_30 * right.m_23;
		const float m21 = left.m_01 * right.m_20 + left.m_11 * right.m_21 + left.m_21 * right.m_22 + left.m_31 * right.m_23;
		const float m22 = left.m_02 * right.m_20 + left.m_12 * right.m_21 + left.m_22 * right.m_22 + left.m_32 * right.m_23;
		const float m23 = left.m_03 * right.m_20 + left.m_13 * right.m_21 + left.m_23 * right.m_22 + left.m_33 * right.m_23;
		const float m30 = left.m_00 * right.m_30 + left.m_10 * right.m_31 + left.m_20 * right.m_32 + left.m_30 * right.m_33;
		const float m31 = left.m_01 * right.m_30 + left.m_11 * right.m_31 + left.m_21 * right.m_32 + left.m_31 * right.m_33;
		const float m32 = left.m_02 * right.m_30 + left.m_12 * right.m_31 + left.m_22 * right.m_32 + left.m_32 * right.m_33;
		const float m33 = left.m_03 * right.m_30 + left.m_13 * right.m_31 + left.m_23 * right.m_32 + left.m_33 * right.m_33;

		destination->m_00 = m00;
		destination->m_01 = m01;
		destination->m_02 = m02;
		destination->m_03 = m03;
		destination->m_10 = m10;
		destination->m_11 = m11;
		destination->m_12 = m12;
		destination->m_13 = m13;
		destination->m_20 = m20;
		destination->m_21 = m21;
		destination->m_22 = m22;
		destination->m_23 = m23;
		destination->m_30 = m30;
		destination->m_31 = m31;
		destination->m_32 = m32;
		destination->m_33 = m33;
		return destination;
	}

	Matrix4 *Matrix4::Divide(const Matrix4 &left, const Matrix4 &right, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		const float m00 = left.m_00 / right.m_00 + left.m_10 / right.m_01 + left.m_20 / right.m_02 + left.m_30 / right.m_03;
		const float m01 = left.m_01 / right.m_00 + left.m_11 / right.m_01 + left.m_21 / right.m_02 + left.m_31 / right.m_03;
		const float m02 = left.m_02 / right.m_00 + left.m_12 / right.m_01 + left.m_22 / right.m_02 + left.m_32 / right.m_03;
		const float m03 = left.m_03 / right.m_00 + left.m_13 / right.m_01 + left.m_23 / right.m_02 + left.m_33 / right.m_03;
		const float m10 = left.m_00 / right.m_10 + left.m_10 / right.m_11 + left.m_20 / right.m_12 + left.m_30 / right.m_13;
		const float m11 = left.m_01 / right.m_10 + left.m_11 / right.m_11 + left.m_21 / right.m_12 + left.m_31 / right.m_13;
		const float m12 = left.m_02 / right.m_10 + left.m_12 / right.m_11 + left.m_22 / right.m_12 + left.m_32 / right.m_13;
		const float m13 = left.m_03 / right.m_10 + left.m_13 / right.m_11 + left.m_23 / right.m_12 + left.m_33 / right.m_13;
		const float m20 = left.m_00 / right.m_20 + left.m_10 / right.m_21 + left.m_20 / right.m_22 + left.m_30 / right.m_23;
		const float m21 = left.m_01 / right.m_20 + left.m_11 / right.m_21 + left.m_21 / right.m_22 + left.m_31 / right.m_23;
		const float m22 = left.m_02 / right.m_20 + left.m_12 / right.m_21 + left.m_22 / right.m_22 + left.m_32 / right.m_23;
		const float m23 = left.m_03 / right.m_20 + left.m_13 / right.m_21 + left.m_23 / right.m_22 + left.m_33 / right.m_23;
		const float m30 = left.m_00 / right.m_30 + left.m_10 / right.m_31 + left.m_20 / right.m_32 + left.m_30 / right.m_33;
		const float m31 = left.m_01 / right.m_30 + left.m_11 / right.m_31 + left.m_21 / right.m_32 + left.m_31 / right.m_33;
		const float m32 = left.m_02 / right.m_30 + left.m_12 / right.m_31 + left.m_22 / right.m_32 + left.m_32 / right.m_33;
		const float m33 = left.m_03 / right.m_30 + left.m_13 / right.m_31 + left.m_23 / right.m_32 + left.m_33 / right.m_33;

		destination->m_00 = m00;
		destination->m_01 = m01;
		destination->m_02 = m02;
		destination->m_03 = m03;
		destination->m_10 = m10;
		destination->m_11 = m11;
		destination->m_12 = m12;
		destination->m_13 = m13;
		destination->m_20 = m20;
		destination->m_21 = m21;
		destination->m_22 = m22;
		destination->m_23 = m23;
		destination->m_30 = m30;
		destination->m_31 = m31;
		destination->m_32 = m32;
		destination->m_33 = m33;
		return destination;
	}

	Vector4 *Matrix4::Transform(const Matrix4 &left, const Vector4 &right, Vector4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector4();
		}

		const float x = left.m_00 * right.m_x + left.m_10 * right.m_y + left.m_20 * right.m_z + left.m_30 * right.m_w;
		const float y = left.m_01 * right.m_x + left.m_11 * right.m_y + left.m_21 * right.m_z + left.m_31 * right.m_w;
		const float z = left.m_02 * right.m_x + left.m_12 * right.m_y + left.m_22 * right.m_z + left.m_32 * right.m_w;
		const float w = left.m_03 * right.m_x + left.m_13 * right.m_y + left.m_23 * right.m_z + left.m_33 * right.m_w;

		return destination->Set(x, y, z, w);
	}

	Matrix4 *Matrix4::Scale(const Matrix4 &left, const Vector3 &right, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		destination->m_00 = left.m_00 * right.m_x;
		destination->m_01 = left.m_01 * right.m_x;
		destination->m_02 = left.m_02 * right.m_x;
		destination->m_03 = left.m_03 * right.m_x;
		destination->m_10 = left.m_10 * right.m_y;
		destination->m_11 = left.m_11 * right.m_y;
		destination->m_12 = left.m_12 * right.m_y;
		destination->m_13 = left.m_13 * right.m_y;
		destination->m_20 = left.m_20 * right.m_z;
		destination->m_21 = left.m_21 * right.m_z;
		destination->m_22 = left.m_22 * right.m_z;
		destination->m_23 = left.m_23 * right.m_z;
		return destination;
	}

	Matrix4 *Matrix4::Scale(const Matrix4 &left, const Vector4 &right, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		destination->m_00 = left.m_00 * right.m_x;
		destination->m_01 = left.m_01 * right.m_x;
		destination->m_02 = left.m_02 * right.m_x;
		destination->m_03 = left.m_03 * right.m_x;
		destination->m_10 = left.m_10 * right.m_y;
		destination->m_11 = left.m_11 * right.m_y;
		destination->m_12 = left.m_12 * right.m_y;
		destination->m_13 = left.m_13 * right.m_y;
		destination->m_20 = left.m_20 * right.m_z;
		destination->m_21 = left.m_21 * right.m_z;
		destination->m_22 = left.m_22 * right.m_z;
		destination->m_23 = left.m_23 * right.m_z;
		destination->m_30 = left.m_30 * right.m_w;
		destination->m_31 = left.m_31 * right.m_w;
		destination->m_32 = left.m_32 * right.m_w;
		destination->m_33 = left.m_33 * right.m_w;
		return destination;
	}

	Matrix4 *Matrix4::Invert(const Matrix4 &source, Matrix4 *destination)
	{
		const float d = Determinant(source);

		if (d != 0.0f)
		{
			if (destination == nullptr)
			{
				destination = new Matrix4();
			}

			const float determinantInv = 1.0f / d;

			// First row.
			const float t00 = Determinant3x3(source.m_11, source.m_12, source.m_13, source.m_21, source.m_22, source.m_23, source.m_31, source.m_32, source.m_33);
			const float t01 = -Determinant3x3(source.m_10, source.m_12, source.m_13, source.m_20, source.m_22, source.m_23, source.m_30, source.m_32, source.m_33);
			const float t02 = Determinant3x3(source.m_10, source.m_11, source.m_13, source.m_20, source.m_21, source.m_23, source.m_30, source.m_31, source.m_33);
			const float t03 = -Determinant3x3(source.m_10, source.m_11, source.m_12, source.m_20, source.m_21, source.m_22, source.m_30, source.m_31, source.m_32);

			// Second row.
			const float t10 = -Determinant3x3(source.m_01, source.m_02, source.m_03, source.m_21, source.m_22, source.m_23, source.m_31, source.m_32, source.m_33);
			const float t11 = Determinant3x3(source.m_00, source.m_02, source.m_03, source.m_20, source.m_22, source.m_23, source.m_30, source.m_32, source.m_33);
			const float t12 = -Determinant3x3(source.m_00, source.m_01, source.m_03, source.m_20, source.m_21, source.m_23, source.m_30, source.m_31, source.m_33);
			const float t13 = Determinant3x3(source.m_00, source.m_01, source.m_02, source.m_20, source.m_21, source.m_22, source.m_30, source.m_31, source.m_32);

			// Third row.
			const float t20 = Determinant3x3(source.m_01, source.m_02, source.m_03, source.m_11, source.m_12, source.m_13, source.m_31, source.m_32, source.m_33);
			const float t21 = -Determinant3x3(source.m_00, source.m_02, source.m_03, source.m_10, source.m_12, source.m_13, source.m_30, source.m_32, source.m_33);
			const float t22 = Determinant3x3(source.m_00, source.m_01, source.m_03, source.m_10, source.m_11, source.m_13, source.m_30, source.m_31, source.m_33);
			const float t23 = -Determinant3x3(source.m_00, source.m_01, source.m_02, source.m_10, source.m_11, source.m_12, source.m_30, source.m_31, source.m_32);

			// Fourth row.
			const float t30 = -Determinant3x3(source.m_01, source.m_02, source.m_03, source.m_11, source.m_12, source.m_13, source.m_21, source.m_22, source.m_23);
			const float t31 = Determinant3x3(source.m_00, source.m_02, source.m_03, source.m_10, source.m_12, source.m_13, source.m_20, source.m_22, source.m_23);
			const float t32 = -Determinant3x3(source.m_00, source.m_01, source.m_03, source.m_10, source.m_11, source.m_13, source.m_20, source.m_21, source.m_23);
			const float t33 = Determinant3x3(source.m_00, source.m_01, source.m_02, source.m_10, source.m_11, source.m_12, source.m_20, source.m_21, source.m_22);

			// Transpose and divide by the determinant.
			destination->m_00 = t00 * determinantInv;
			destination->m_11 = t11 * determinantInv;
			destination->m_22 = t22 * determinantInv;
			destination->m_33 = t33 * determinantInv;
			destination->m_01 = t10 * determinantInv;
			destination->m_10 = t01 * determinantInv;
			destination->m_20 = t02 * determinantInv;
			destination->m_02 = t20 * determinantInv;
			destination->m_12 = t21 * determinantInv;
			destination->m_21 = t12 * determinantInv;
			destination->m_03 = t30 * determinantInv;
			destination->m_30 = t03 * determinantInv;
			destination->m_13 = t31 * determinantInv;
			destination->m_31 = t13 * determinantInv;
			destination->m_32 = t23 * determinantInv;
			destination->m_23 = t32 * determinantInv;
			return destination;
		}

		return nullptr;
	}

	Matrix4 *Matrix4::Negate(const Matrix4 &source, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		destination->m_00 = -source.m_00;
		destination->m_01 = -source.m_01;
		destination->m_02 = -source.m_02;
		destination->m_03 = -source.m_03;
		destination->m_10 = -source.m_10;
		destination->m_11 = -source.m_11;
		destination->m_12 = -source.m_12;
		destination->m_13 = -source.m_13;
		destination->m_20 = -source.m_20;
		destination->m_21 = -source.m_21;
		destination->m_22 = -source.m_22;
		destination->m_23 = -source.m_23;
		destination->m_30 = -source.m_30;
		destination->m_31 = -source.m_31;
		destination->m_32 = -source.m_32;
		destination->m_33 = -source.m_33;
		return destination;
	}

	Matrix4 *Matrix4::Transpose(const Matrix4 &source, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		const float m00 = source.m_00;
		const float m01 = source.m_10;
		const float m02 = source.m_20;
		const float m03 = source.m_30;
		const float m10 = source.m_01;
		const float m11 = source.m_11;
		const float m12 = source.m_21;
		const float m13 = source.m_31;
		const float m20 = source.m_02;
		const float m21 = source.m_12;
		const float m22 = source.m_22;
		const float m23 = source.m_32;
		const float m30 = source.m_03;
		const float m31 = source.m_13;
		const float m32 = source.m_23;
		const float m33 = source.m_33;

		destination->m_00 = m00;
		destination->m_01 = m01;
		destination->m_02 = m02;
		destination->m_03 = m03;
		destination->m_10 = m10;
		destination->m_11 = m11;
		destination->m_12 = m12;
		destination->m_13 = m13;
		destination->m_20 = m20;
		destination->m_21 = m21;
		destination->m_22 = m22;
		destination->m_23 = m23;
		destination->m_30 = m30;
		destination->m_31 = m31;
		destination->m_32 = m32;
		destination->m_33 = m33;
		return destination;
	}

	Matrix4 *Matrix4::Translate(const Matrix4 &left, const Vector2 &right, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		destination->m_30 += left.m_00 * right.m_x + left.m_10 * right.m_y;
		destination->m_31 += left.m_01 * right.m_x + left.m_11 * right.m_y;
		destination->m_32 += left.m_02 * right.m_x + left.m_12 * right.m_y;
		destination->m_33 += left.m_03 * right.m_x + left.m_13 * right.m_y;
		return destination;
	}

	Matrix4 *Matrix4::Translate(const Matrix4 &left, const Vector3 &right, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		destination->m_30 += left.m_00 * right.m_x + left.m_10 * right.m_y + left.m_20 * right.m_z;
		destination->m_31 += left.m_01 * right.m_x + left.m_11 * right.m_y + left.m_21 * right.m_z;
		destination->m_32 += left.m_02 * right.m_x + left.m_12 * right.m_y + left.m_22 * right.m_z;
		destination->m_33 += left.m_03 * right.m_x + left.m_13 * right.m_y + left.m_23 * right.m_z;
		return destination;
	}

	Matrix4 *Matrix4::Rotate(const Matrix4 &source, const Vector3 &axis, const float &angle, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		const float c = std::cos(angle);
		const float s = std::sin(angle);
		const float o = 1.0f - c;
		const float xy = axis.m_x * axis.m_y;
		const float yz = axis.m_y * axis.m_z;
		const float xz = axis.m_x * axis.m_z;
		const float xs = axis.m_x * s;
		const float ys = axis.m_y * s;
		const float zs = axis.m_z * s;

		const float f00 = axis.m_x * axis.m_x * o + c;
		const float f01 = xy * o + zs;
		const float f02 = xz * o - ys;
		const float f10 = xy * o - zs;
		const float f11 = axis.m_y * axis.m_y * o + c;
		const float f12 = yz * o + xs;
		const float f20 = xz * o + ys;
		const float f21 = yz * o - xs;
		const float f22 = axis.m_z * axis.m_z * o + c;

		const float t00 = source.m_00 * f00 + source.m_10 * f01 + source.m_20 * f02;
		const float t01 = source.m_01 * f00 + source.m_11 * f01 + source.m_21 * f02;
		const float t02 = source.m_02 * f00 + source.m_12 * f01 + source.m_22 * f02;
		const float t03 = source.m_03 * f00 + source.m_13 * f01 + source.m_23 * f02;
		const float t10 = source.m_00 * f10 + source.m_10 * f11 + source.m_20 * f12;
		const float t11 = source.m_01 * f10 + source.m_11 * f11 + source.m_21 * f12;
		const float t12 = source.m_02 * f10 + source.m_12 * f11 + source.m_22 * f12;
		const float t13 = source.m_03 * f10 + source.m_13 * f11 + source.m_23 * f12;
		const float t20 = source.m_00 * f20 + source.m_10 * f21 + source.m_20 * f22;
		const float t21 = source.m_01 * f20 + source.m_11 * f21 + source.m_21 * f22;
		const float t22 = source.m_02 * f20 + source.m_12 * f21 + source.m_22 * f22;
		const float t23 = source.m_03 * f20 + source.m_13 * f21 + source.m_23 * f22;

		destination->m_00 = t00;
		destination->m_01 = t01;
		destination->m_02 = t02;
		destination->m_03 = t03;
		destination->m_10 = t10;
		destination->m_11 = t11;
		destination->m_12 = t12;
		destination->m_13 = t13;
		destination->m_20 = t20;
		destination->m_21 = t21;
		destination->m_22 = t22;
		destination->m_23 = t23;
		return destination;
	}

	float *Matrix4::ToArray(const Matrix4 &matrix)
	{
		float *result = new float[16];
		result[0] = matrix.m_00;
		result[1] = matrix.m_01;
		result[2] = matrix.m_02;
		result[3] = matrix.m_03;
		result[4] = matrix.m_10;
		result[5] = matrix.m_11;
		result[6] = matrix.m_12;
		result[7] = matrix.m_13;
		result[8] = matrix.m_20;
		result[9] = matrix.m_21;
		result[10] = matrix.m_22;
		result[11] = matrix.m_23;
		result[12] = matrix.m_30;
		result[13] = matrix.m_31;
		result[14] = matrix.m_32;
		result[15] = matrix.m_33;
		return result;
	}

	Matrix4 *Matrix4::SetZero(Matrix4 *source)
	{
		source->m_00 = 0.0f;
		source->m_01 = 0.0f;
		source->m_02 = 0.0f;
		source->m_03 = 0.0f;
		source->m_10 = 0.0f;
		source->m_11 = 0.0f;
		source->m_12 = 0.0f;
		source->m_13 = 0.0f;
		source->m_20 = 0.0f;
		source->m_21 = 0.0f;
		source->m_22 = 0.0f;
		source->m_23 = 0.0f;
		source->m_30 = 0.0f;
		source->m_31 = 0.0f;
		source->m_32 = 0.0f;
		source->m_33 = 0.0f;
		return source;
	}

	Matrix4 *Matrix4::TransformationMatrix(const Vector2 &translation, const float &scale, Matrix4 *destination)
	{
		return TransformationMatrix(Vector3(translation.m_x, translation.m_y, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(scale, scale, scale), destination);
	}

	Matrix4 *Matrix4::TransformationMatrix(const Vector2 &translation, const Vector3 &scale, Matrix4 *destination)
	{
		return TransformationMatrix(Vector3(translation.m_x, translation.m_y, 0.0f), Vector3(0.0f, 0.0f, 0.0f), scale, destination);
	}

	Matrix4 *Matrix4::TransformationMatrix(const Vector3 &translation, const Vector3 &rotation, const float &scale, Matrix4 *destination)
	{
		return TransformationMatrix(translation, rotation, Vector3(scale, scale, scale), destination);
	}

	Matrix4 *Matrix4::TransformationMatrix(const Vector3 &translation, const Vector3 &rotation, const Vector3 &scale, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		destination->SetIdentity();

		if (Vector3::LengthSquared(translation) != 0.0f)
		{
			Translate(*destination, translation, destination);
		}

		if (Vector3::LengthSquared(rotation) != 0.0f)
		{
			Rotate(*destination, Vector3(1.0f, 0.0f, 0.0f), Maths::Radians(rotation.m_x), destination); // Rotate the X component.
			Rotate(*destination, Vector3(0.0f, 1.0f, 0.0f), Maths::Radians(rotation.m_y), destination); // Rotate the Y component.
			Rotate(*destination, Vector3(0.0f, 0.0f, 1.0f), Maths::Radians(rotation.m_z), destination); // Rotate the Z component.
		}

		// Only scales if there is a scale.
		if (scale.m_x != 1.0f || scale.m_y != 1.0f || scale.m_z != 1.0f)
		{
			Matrix4::Scale(*destination, scale, destination);
		}

		return destination;
	}

	Matrix4 *Matrix4::PerspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear, const float &zFar, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		destination->SetIdentity();
		const float yScale = 1.0f / std::tan(Maths::Radians(fov / 2.0f));
		const float xScale = yScale / aspectRatio;
		const float length = zFar - zNear;

		destination->m_00 = xScale;
		destination->m_11 = -yScale;
		destination->m_22 = -((zFar + zNear) / length);
		destination->m_23 = -1.0f;
		destination->m_32 = -((2.0f * zNear * zFar) / length);
		destination->m_33 = 0.0f;
		return destination;
	}

	Matrix4 *Matrix4::OrthographicMatrix(const float &left, const float &right, const float &bottom, const float &top, const float &near, const float &far, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		const float ox = 2.0f / (right - left);
		const float oy = 2.0f / (top - bottom);
		const float oz = -2.0f / (far - near);

		const float tx = -(right + left) / (right - left);
		const float ty = -(top + bottom) / (top - bottom);
		const float tz = -(far + near) / (far - near);

		destination->SetIdentity();
		destination->m_00 = ox;
		destination->m_11 = oy;
		destination->m_22 = oz;
		destination->m_03 = tx;
		destination->m_13 = ty;
		destination->m_23 = tz;
		destination->m_33 = 1.0f;
		return destination;
	}

	Matrix4 *Matrix4::ViewMatrix(const Vector3 &position, const Vector3 &rotation, Matrix4 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Matrix4();
		}

		destination->SetIdentity();

		if (rotation != 0.0f)
		{
			Rotate(*destination, Vector3(1.0f, 0.0f, 0.0f), Maths::Radians(rotation.m_x), destination);
			Rotate(*destination, Vector3(0.0f, 1.0f, 0.0f), Maths::Radians(-rotation.m_y), destination);
			Rotate(*destination, Vector3(0.0f, 0.0f, 1.0f), Maths::Radians(rotation.m_z), destination);
		}

		if (position != 0.0f)
		{
			Translate(*destination, *Vector3(position).Negate(), destination);
		}

		return destination;
	}

	Vector3 *Matrix4::WorldToScreenSpace(const Vector3 &worldSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix, Vector3 *destination)
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		Vector4 point4 = Vector4(worldSpace.m_x, worldSpace.m_y, worldSpace.m_z, 1.0f);
		Transform(viewMatrix, point4, &point4);
		Transform(projectionMatrix, point4, &point4);

		Vector3 point = Vector3(point4);
		point.m_x /= point.m_z;
		point.m_y /= point.m_z;
		return destination->Set(point.m_x, point.m_y, point.m_z);
	}

	Matrix4 *Matrix4::SetIdentity()
	{
		return SetIdentity(this);
	}

	float Matrix4::Determinant()
	{
		return Determinant(*this);
	}

	Matrix4 *Matrix4::Invert()
	{
		return Invert(*this, this);
	}

	Matrix4 *Matrix4::Negate()
	{
		return Negate(*this, this);
	}

	Matrix4 *Matrix4::Transpose()
	{
		return Transpose(*this, this);
	}

	Matrix4 *Matrix4::SetZero()
	{
		return SetZero(this);
	}

	bool Matrix4::operator==(const Matrix4 &other) const
	{
		return m_00 == other.m_00 && m_01 == other.m_01 && m_02 == other.m_02 && m_03 == other.m_03 &&
			m_10 == other.m_10 && m_11 == other.m_11 && m_12 == other.m_12 && m_13 == other.m_13 &&
			m_20 == other.m_20 && m_21 == other.m_21 && m_22 == other.m_22 && m_23 == other.m_23 &&
			m_30 == other.m_30 && m_31 == other.m_31 && m_32 == other.m_32 && m_33 == other.m_33;
	}

	bool Matrix4::operator!=(const Matrix4 &other) const
	{
		return !(*this == other);
	}

	Matrix4 &Matrix4::operator-()
	{
		return *Matrix4(*this).Negate();
	}

	Matrix4 operator+(Matrix4 left, const Matrix4 &right)
	{
		return *Matrix4::Add(left, right, &left);
	}

	Matrix4 operator-(Matrix4 left, const Matrix4 &right)
	{
		return *Matrix4::Subtract(left, right, &left);
	}

	Matrix4 operator*(Matrix4 left, const Matrix4 &right)
	{
		return *Matrix4::Multiply(left, right, &left);
	}

	Matrix4 operator/(Matrix4 left, const Matrix4 &right)
	{
		return *Matrix4::Divide(left, right, &left);
	}

	Matrix4 operator*(Matrix4 left, Vector4 value)
	{
		return *Matrix4::Scale(left, value, &left);
	}

	Matrix4 operator/(Matrix4 left, Vector4 value)
	{
		return *Matrix4::Scale(left, 1.0f / value, &left);
	}

	Matrix4 &Matrix4::operator+=(const Matrix4 &other)
	{
		Matrix4 result = Matrix4();
		return *Matrix4::Add(*this, other, &result);
	}

	Matrix4 &Matrix4::operator-=(const Matrix4 &other)
	{
		Matrix4 result = Matrix4();
		return *Matrix4::Subtract(*this, other, &result);
	}

	Matrix4 &Matrix4::operator*=(const Matrix4 &other)
	{
		Matrix4 result = Matrix4();
		return *Matrix4::Multiply(*this, other, &result);
	}

	Matrix4 &Matrix4::operator/=(const Matrix4 &other)
	{
		Matrix4 result = Matrix4();
		return *Matrix4::Divide(*this, other, &result);
	}

	std::ostream &operator<<(std::ostream &stream, const Matrix4 &matrix)
	{
		stream << matrix.ToString();
		return stream;
	}

	std::string Matrix4::ToString() const
	{
		std::stringstream result;
		result << "Matrix4(" << m_00 << ", " << m_01 << ", " << m_02 << ", " << m_03 << ", " <<
			m_10 << ", " << m_11 << ", " << m_12 << ", " << m_13 << ", " <<
			m_20 << ", " << m_21 << ", " << m_22 << ", " << m_23 << ", " <<
			m_30 << ", " << m_31 << ", " << m_32 << ", " << m_33 << ")";
		return result.str();
	}

	float Matrix4::Determinant3x3(const float &t00, const float &t01, const float &t02, const float &t10, const float &t11, const float &t12, const float &t20, const float &t21, const float &t22)
	{
		return t00 * (t11 * t22 - t12 * t21) + t01 * (t12 * t20 - t10 * t22) + t02 * (t10 * t21 - t11 * t20);
	}
}
