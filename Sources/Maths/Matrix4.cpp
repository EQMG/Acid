#include "Matrix4.hpp"

#ifdef ACID_SSE
#include <emmintrin.h>
#endif
#include <cassert>
#include "Maths.hpp"
#include "Matrix3.hpp"
#include "Quaternion.hpp"
#include "Vector2.hpp"

namespace acid
{
	const Matrix4 Matrix4::IDENTITY = Matrix4(new float[16]{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f});
	const Matrix4 Matrix4::ZERO = Matrix4();

	Matrix4::Matrix4(const float &diagonal)
	{
#ifdef ACID_SSE
		_mm_storeu_ps(&m_rows[0][0], _mm_set_ps(0.0f, 0.0f, 0.0f, diagonal));
		_mm_storeu_ps(&m_rows[1][0], _mm_set_ps(0.0f, 0.0f, diagonal, 0.0f));
		_mm_storeu_ps(&m_rows[2][0], _mm_set_ps(0.0f, diagonal, 0.0f, 0.0f));
		_mm_storeu_ps(&m_rows[3][0], _mm_set_ps(diagonal, 0.0f, 0.0f, 0.0f));
#else
		memset(m_rows, 0, 4 * sizeof(Vector4));
		m_rows[0][0] = diagonal;
		m_rows[1][1] = diagonal;
		m_rows[2][2] = diagonal;
		m_rows[3][3] = diagonal;
#endif
	}

	Matrix4::Matrix4(const Matrix4 &source)
	{
#ifdef ACID_SSE
		_mm_storeu_ps(&m_rows[0][0], _mm_loadu_ps(source[0].m_elements));
		_mm_storeu_ps(&m_rows[1][0], _mm_loadu_ps(source[1].m_elements));
		_mm_storeu_ps(&m_rows[2][0], _mm_loadu_ps(source[2].m_elements));
		_mm_storeu_ps(&m_rows[3][0], _mm_loadu_ps(source[3].m_elements));
#else
		memcpy(m_rows, source.m_rows, 4 * sizeof(Vector4));
#endif
	}

	Matrix4::Matrix4(const float *source)
	{
#ifdef ACID_SSE
		_mm_storeu_ps(&m_rows[0][0], _mm_loadu_ps(source));
		_mm_storeu_ps(&m_rows[1][0], _mm_loadu_ps(source + 4));
		_mm_storeu_ps(&m_rows[2][0], _mm_loadu_ps(source + 8));
		_mm_storeu_ps(&m_rows[3][0], _mm_loadu_ps(source + 12));
#else
		memcpy(m_rows, source, 4 * 4 * sizeof(float));
#endif
	}

	Matrix4::Matrix4(const Vector4 *source)
	{
#ifdef ACID_SSE
		_mm_storeu_ps(&m_rows[0][0], _mm_loadu_ps(source[0].m_elements));
		_mm_storeu_ps(&m_rows[1][0], _mm_loadu_ps(source[1].m_elements));
		_mm_storeu_ps(&m_rows[2][0], _mm_loadu_ps(source[2].m_elements));
		_mm_storeu_ps(&m_rows[3][0], _mm_loadu_ps(source[3].m_elements));
#else
		memcpy(m_rows, source, 4 * sizeof(Vector4));
#endif
	}

	Matrix4::~Matrix4()
	{
	}

	Matrix4 Matrix4::Add(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();

#ifdef ACID_SSE
		_mm_storeu_ps(&result[0][0], _mm_add_ps(_mm_loadu_ps(&m_rows[0][0]), _mm_loadu_ps(&other[0][0])));
		_mm_storeu_ps(&result[1][0], _mm_add_ps(_mm_loadu_ps(&m_rows[1][0]), _mm_loadu_ps(&other[1][0])));
		_mm_storeu_ps(&result[2][0], _mm_add_ps(_mm_loadu_ps(&m_rows[2][0]), _mm_loadu_ps(&other[2][0])));
		_mm_storeu_ps(&result[3][0], _mm_add_ps(_mm_loadu_ps(&m_rows[3][0]), _mm_loadu_ps(&other[3][0])));
#else
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result[row][col] = m_rows[row][col] + other[row][col];
			}
		}
#endif

		return result;
	}

	Matrix4 Matrix4::Subtract(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();

#ifdef ACID_SSE
		_mm_storeu_ps(&result[0][0], _mm_sub_ps(_mm_loadu_ps(&m_rows[0][0]), _mm_loadu_ps(&other[0][0])));
		_mm_storeu_ps(&result[1][0], _mm_sub_ps(_mm_loadu_ps(&m_rows[1][0]), _mm_loadu_ps(&other[1][0])));
		_mm_storeu_ps(&result[2][0], _mm_sub_ps(_mm_loadu_ps(&m_rows[2][0]), _mm_loadu_ps(&other[2][0])));
		_mm_storeu_ps(&result[3][0], _mm_sub_ps(_mm_loadu_ps(&m_rows[3][0]), _mm_loadu_ps(&other[3][0])));
#else
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result[row][col] = m_rows[row][col] - other[row][col];
			}
		}
#endif

		return result;
	}

	Matrix4 Matrix4::Multiply(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();

#ifdef ACID_SSE
		__m128 r0 = _mm_loadu_ps(&other[0][0]);
		__m128 r1 = _mm_loadu_ps(&other[1][0]);
		__m128 r2 = _mm_loadu_ps(&other[2][0]);
		__m128 r3 = _mm_loadu_ps(&other[3][0]);

		__m128 l = _mm_loadu_ps(&m_rows[0][0]);
		__m128 t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
		__m128 t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
		__m128 t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
		__m128 t3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
		_mm_storeu_ps(&result[0][0], _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));

		l = _mm_loadu_ps(&m_rows[1][0]);
		t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
		t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
		t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
		t3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
		_mm_storeu_ps(&result[1][0], _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));

		l = _mm_loadu_ps(&m_rows[2][0]);
		t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
		t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
		t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
		t3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
		_mm_storeu_ps(&result[2][0], _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));

		l = _mm_loadu_ps(&m_rows[3][0]);
		t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
		t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
		t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
		t3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
		_mm_storeu_ps(&result[3][0], _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));
#else
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result[row][col] = m_rows[0][col] * other[row][0] + m_rows[1][col] * other[row][1] + m_rows[2][col] * other[row][2] + m_rows[3][col] * other[row][3];
			}
		}
#endif

		return result;
	}

	Vector4 Matrix4::Multiply(const Vector4 &other) const
	{
		Vector4 result = Vector4();

		for (int row = 0; row < 4; row++)
		{
			result[row] = m_rows[0][row] * other.m_x + m_rows[1][row] * other.m_y + m_rows[2][row] * other.m_z + m_rows[3][row] * other.m_w;
		}

		return result;
	}

	Matrix4 Matrix4::Divide(const Matrix4 &other) const
	{
		Matrix4 result = Matrix4();

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result[row][col] = m_rows[0][col] / other[row][0] + m_rows[1][col] / other[row][1] + m_rows[2][col] / other[row][2] + m_rows[3][col] / other[row][3];
			}
		}

		return result;
	}

	Vector4 Matrix4::Transform(const Vector4 &other) const
	{
		Vector4 result = Vector4();

		for (int row = 0; row < 4; row++)
		{
			result[row] = m_rows[0][row] * other.m_x + m_rows[1][row] * other.m_y + m_rows[2][row] * other.m_z + m_rows[3][row] * other.m_w;
		}

		return result;
	}

	Matrix4 Matrix4::Translate(const Vector2 &other) const
	{
		Matrix4 result = Matrix4(*this);

		for (int col = 0; col < 4; col++)
		{
			result[3][col] += m_rows[0][col] * other.m_x + m_rows[1][col] * other.m_y;
		}

		return result;
	}

	Matrix4 Matrix4::Translate(const Vector3 &other) const
	{
		Matrix4 result = Matrix4(*this);

		for (int col = 0; col < 4; col++)
		{
			result[3][col] += m_rows[0][col] * other.m_x + m_rows[1][col] * other.m_y + m_rows[2][col] * other.m_z;
		}

		return result;
	}

	Matrix4 Matrix4::Scale(const Vector3 &other) const
	{
		Matrix4 result = Matrix4(*this);

		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result[row][col] *= other[row];
			}
		}

		return result;
	}

	Matrix4 Matrix4::Scale(const Vector4 &other) const
	{
		Matrix4 result = Matrix4(*this);

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result[row][col] *= other[row];
			}
		}

		return result;
	}

	Matrix4 Matrix4::Rotate(const float &angle, const Vector3 &axis) const
	{
		Matrix4 result = Matrix4(*this);

		float c = std::cos(angle);
		float s = std::sin(angle);
		float o = 1.0f - c;
		float xy = axis.m_x * axis.m_y;
		float yz = axis.m_y * axis.m_z;
		float xz = axis.m_x * axis.m_z;
		float xs = axis.m_x * s;
		float ys = axis.m_y * s;
		float zs = axis.m_z * s;

		Matrix3 f = Matrix3();
		f[0][0] = axis.m_x * axis.m_x * o + c;
		f[0][1] = xy * o + zs;
		f[0][2] = xz * o - ys;
		f[1][0] = xy * o - zs;
		f[1][1] = axis.m_y * axis.m_y * o + c;
		f[1][2] = yz * o + xs;
		f[2][0] = xz * o + ys;
		f[2][1] = yz * o - xs;
		f[2][2] = axis.m_z * axis.m_z * o + c;

		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result[row][col] = m_rows[0][col] * f[row][0] + m_rows[1][col] * f[row][1] + m_rows[2][col] * f[row][2];
			}
		}

		return result;
	}

	Matrix4 Matrix4::Negate() const
	{
		Matrix4 result = Matrix4();

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result[row][col] = -m_rows[row][col];
			}
		}

		return result;
	}

	Matrix4 Matrix4::Invert() const
	{
		Matrix4 result = Matrix4();

		float det = Determinant();
		assert(det != 0.0f && "Determinant cannot be zero!");

		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				// Get minor of element [j][i] - not [i][j], this is where the transpose happens.
				Matrix3 minorSubmatrix = GetSubmatrix(j, i);
				float minor = minorSubmatrix.Determinant();

				// Multiply by (−1)^{i+j}.
				float factor = ((i + j) % 2 == 1) ? -1.0f : 1.0f;
				float cofactor = minor * factor;

				result[i][j] = cofactor / det;
			}
		}

		return result;
	}

	Matrix4 Matrix4::Transpose() const
	{
		Matrix4 result = Matrix4();

#ifdef ACID_SSE
		__m128 m0 = _mm_loadu_ps(&m_rows[0][0]);
		__m128 m1 = _mm_loadu_ps(&m_rows[1][0]);
		__m128 m2 = _mm_loadu_ps(&m_rows[2][0]);
		__m128 m3 = _mm_loadu_ps(&m_rows[3][0]);
		_MM_TRANSPOSE4_PS(m0, m1, m2, m3);

		_mm_storeu_ps(&result[0][0], m0);
		_mm_storeu_ps(&result[1][0], m1);
		_mm_storeu_ps(&result[2][0], m2);
		_mm_storeu_ps(&result[3][0], m3);
#else
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result[row][col] = m_rows[col][row];
			}
		}
#endif

		return result;
	}

	float Matrix4::Determinant() const
	{
		float result = 0.0f;

		for (int i = 0; i < 4; i++)
		{
			// Get minor of element [0][i].
			Matrix3 minorSubmatrix = GetSubmatrix(0, i);
			float minor = minorSubmatrix.Determinant();

			// If this is an odd-numbered row, negate the value.
			float factor = (i % 2 == 1) ? -1.0f : 1.0f;

			result += factor * m_rows[0][i] * minor;
		}

		return result;
	}

	Matrix3 Matrix4::GetSubmatrix(const int &row, const int &col) const
	{
		Matrix3 result = Matrix3();
		int colCount = 0;
		int rowCount = 0;

		for (int i = 0; i < 4; i++)
		{
			if (i != row)
			{
				colCount = 0;

				for (int j = 0; j < 4; j++)
				{
					if (j != col)
					{
						result[rowCount][colCount] = m_rows[i][j];
						colCount++;
					}
				}

				rowCount++;
			}
		}

		return result;
	}

	Matrix4 Matrix4::TransformationMatrix(const Vector3 &translation, const Vector3 &rotation, const Vector3 &scale)
	{
		Matrix4 result = Matrix4();

		if (translation.LengthSquared() != 0.0f)
		{
			result = result.Translate(translation);
		}

		if (rotation.LengthSquared() != 0.0f)
		{
			result = result.Rotate(Maths::Radians(rotation.m_x), Vector3::RIGHT); // Rotate the X component.
			result = result.Rotate(Maths::Radians(rotation.m_y), Vector3::UP); // Rotate the Y component.
			result = result.Rotate(Maths::Radians(rotation.m_z), Vector3::FRONT); // Rotate the Z component.
		}

		if (scale.m_x != 1.0f || scale.m_y != 1.0f || scale.m_z != 1.0f)
		{
			result = result.Scale(scale);
		}

		return result;
	}

	Matrix4 Matrix4::TransformationMatrix(const Vector3 &translation, const Quaternion &rotation, const Vector3 &scale)
	{
		Matrix4 result = Matrix4();

		if (translation.LengthSquared() != 0.0f)
		{
			result = result.Translate(translation);
		}

		if (rotation != Quaternion::W_ONE)
		{
			Vector3 euler = rotation.ToEuler(); // TODO: Use Quaternion!
			// result *= rotation.ToRotationMatrix();
			result = result.Rotate(Maths::Radians(euler.m_x), Vector3::RIGHT); // Rotate the X component.
			result = result.Rotate(Maths::Radians(euler.m_y), Vector3::UP); // Rotate the Y component.
			result = result.Rotate(Maths::Radians(euler.m_z), Vector3::FRONT); // Rotate the Z component.
		}

		if (scale.m_x != 1.0f || scale.m_y != 1.0f || scale.m_z != 1.0f)
		{
			result = result.Scale(scale);
		}

		return result;
	}

	Matrix4 Matrix4::PerspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear, const float &zFar)
	{
		Matrix4 result = Matrix4();

		float yScale = 1.0f / std::tan(Maths::Radians(fov / 2.0f));
		float xScale = yScale / aspectRatio;
		float length = zFar - zNear;

		result[0][0] = xScale;
		result[1][1] = -yScale;
		result[2][2] = -((zFar + zNear) / length);
		result[2][3] = -1.0f;
		result[3][2] = -((2.0f * zNear * zFar) / length);
		result[3][3] = 0.0f;
		return result;
	}

	Matrix4 Matrix4::OrthographicMatrix(const float &left, const float &right, const float &bottom, const float &top, const float &near, const float &far)
	{
		Matrix4 result = Matrix4();

		float ox = 2.0f / (right - left);
		float oy = 2.0f / (top - bottom);
		float oz = -2.0f / (far - near);

		float tx = -(right + left) / (right - left);
		float ty = -(top + bottom) / (top - bottom);
		float tz = -(far + near) / (far - near);

		result[0][0] = ox;
		result[1][1] = oy;
		result[2][2] = oz;
		result[0][3] = tx;
		result[1][3] = ty;
		result[2][3] = tz;
		result[3][3] = 1.0f;
		return result;
	}

	Matrix4 Matrix4::ViewMatrix(const Vector3 &position, const Vector3 &rotation)
	{
		Matrix4 result = Matrix4();

		if (rotation != 0.0f)
		{
			result = result.Rotate(Maths::Radians(rotation.m_x), Vector3::RIGHT); // Rotate the X component.
			result = result.Rotate(Maths::Radians(-rotation.m_y), Vector3::UP); // Rotate the Y component.
			result = result.Rotate(Maths::Radians(rotation.m_z), Vector3::FRONT); // Rotate the Z component.
		}

		if (position != 0.0f)
		{
			result = result.Translate(position.Negate());
		}

		return result;
	}

	Matrix4 Matrix4::ViewMatrix(const Vector3 &position, const Quaternion &rotation)
	{
		Matrix4 result = Matrix4();

		if (rotation != 0.0f)
		{
			Vector3 euler = rotation.ToEuler(); // TODO: Use Quaternion!
			// result *= rotation.ToRotationMatrix();
			result = result.Rotate(Maths::Radians(euler.m_x), Vector3::RIGHT); // Rotate the X component.
			result = result.Rotate(Maths::Radians(-euler.m_y), Vector3::UP); // Rotate the Y component.
			result = result.Rotate(Maths::Radians(euler.m_z), Vector3::FRONT); // Rotate the Z component.
		}

		if (position != 0.0f)
		{
			result = result.Translate(position.Negate());
		}

		return result;
	}

	Vector3 Matrix4::WorldToScreenSpace(const Vector3 &worldSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix)
	{
		Vector4 point4 = Vector4(worldSpace.m_x, worldSpace.m_y, worldSpace.m_z, 1.0f);
		point4 = viewMatrix.Transform(point4);
		point4 = projectionMatrix.Transform(point4);

		Vector3 result = Vector3(point4);
		result.m_x /= result.m_z;
		result.m_y /= result.m_z;
		return result;
	}

	Matrix4 Matrix4::LookAt(const Vector3 &camera, const Vector3 &object, const Vector3 &up)
	{
		Vector3 f = (object - camera).Normalize();
		Vector3 s = f.Cross(up.Normalize());
		Vector3 u = s.Cross(f);

		Matrix4 result = Matrix4();
		result[0][0] = s.m_x;
		result[0][1] = s.m_y;
		result[0][2] = s.m_z;
		result[1][0] = u.m_x;
		result[1][1] = u.m_y;
		result[1][2] = u.m_z;
		result[2][0] = -f.m_x;
		result[2][1] = -f.m_y;
		result[2][2] = -f.m_z;

		result *= IDENTITY.Translate(Vector3(-camera.m_x, -camera.m_y, -camera.m_z));

		return result;
	}

	void Matrix4::Write(LoadedValue *destination)
	{
		m_rows[0].Write(destination->GetChild("m0", true));
		m_rows[1].Write(destination->GetChild("m1", true));
		m_rows[2].Write(destination->GetChild("m2", true));
		m_rows[3].Write(destination->GetChild("m3", true));
	}

	Matrix4 &Matrix4::operator=(const Matrix4 &other)
	{
#ifdef ACID_SSE
		_mm_storeu_ps(&m_rows[0][0], _mm_loadu_ps(&other[0][0]));
		_mm_storeu_ps(&m_rows[1][0], _mm_loadu_ps(&other[1][0]));
		_mm_storeu_ps(&m_rows[2][0], _mm_loadu_ps(&other[2][0]));
		_mm_storeu_ps(&m_rows[3][0], _mm_loadu_ps(&other[3][0]));
#else
		for (int i = 0; i < 4; i++)
		{
			m_rows[i] = other[i];
		}
#endif

		return *this;
	}

	Matrix4 &Matrix4::operator=(const float *array)
	{
#ifdef ACID_SSE
		_mm_storeu_ps(&m_rows[0][0], _mm_loadu_ps(array));
		_mm_storeu_ps(&m_rows[1][0], _mm_loadu_ps(array + 4));
		_mm_storeu_ps(&m_rows[2][0], _mm_loadu_ps(array + 8));
		_mm_storeu_ps(&m_rows[3][0], _mm_loadu_ps(array + 12));
#else
		memcpy(m_rows, array, 4 * 4 * sizeof(float));
#endif
		return *this;
	}

	Matrix4 &Matrix4::operator=(LoadedValue *source)
	{
		m_rows[0] = source->GetChild("m0");
		m_rows[1] = source->GetChild("m1");
		m_rows[2] = source->GetChild("m2");
		m_rows[3] = source->GetChild("m3");
		return *this;
	}

	bool Matrix4::operator==(const Matrix4 &other) const
	{
#ifdef ACID_SSE
		__m128 c0 = _mm_cmpeq_ps(_mm_loadu_ps(&m_rows[0][0]), _mm_loadu_ps(&other[0][0]));
		__m128 c1 = _mm_cmpeq_ps(_mm_loadu_ps(&m_rows[1][0]), _mm_loadu_ps(&other[1][0]));
		c0 = _mm_and_ps(c0, c1);
		__m128 c2 = _mm_cmpeq_ps(_mm_loadu_ps(&m_rows[2][0]), _mm_loadu_ps(&other[2][0]));
		__m128 c3 = _mm_cmpeq_ps(_mm_loadu_ps(&m_rows[3][0]), _mm_loadu_ps(&other[3][0]));
		c2 = _mm_and_ps(c2, c3);
		c0 = _mm_and_ps(c0, c2);
		__m128 hi = _mm_movehl_ps(c0, c0);
		c0 = _mm_and_ps(c0, hi);
		hi = _mm_shuffle_ps(c0, c0, _MM_SHUFFLE(1, 1, 1, 1));
		c0 = _mm_and_ps(c0, hi);
		return _mm_cvtsi128_si32(_mm_castps_si128(c0)) == -1;
#else
		return m_rows[0] == other[0] && m_rows[1] == other[1] && m_rows[2] == other[2] && m_rows[3] == other[3];
#endif
	}

	bool Matrix4::operator!=(const Matrix4 &other) const
	{
		return !(*this == other);
	}

	Matrix4 Matrix4::operator-() const
	{
		return Negate();
	}

	const Vector4 &Matrix4::operator[](const uint32_t &index) const
	{
		assert(index < 4);
		return m_rows[index];
	}

	Vector4 &Matrix4::operator[](const uint32_t &index)
	{
		assert(index < 4);
		return m_rows[index];
	}

	Matrix4 operator+(const Matrix4 &left, const Matrix4 &right)
	{
		return left.Add(right);
	}

	Matrix4 operator-(const Matrix4 &left, const Matrix4 &right)
	{
		return left.Subtract(right);
	}

	Matrix4 operator*(const Matrix4 &left, const Matrix4 &right)
	{
		return left.Multiply(right);
	}

	Matrix4 operator/(const Matrix4 &left, const Matrix4 &right)
	{
		return left.Divide(right);
	}

	Matrix4 operator*(const Vector4 &left, const Matrix4 &right)
	{
		return right.Scale(left);
	}

	Matrix4 operator/(const Vector4 &left, const Matrix4 &right)
	{
		return right.Scale(1.0f / left);
	}

	Matrix4 operator*(const Matrix4 &left, const Vector4 &right)
	{
		return left.Scale(right);
	}

	Matrix4 operator/(const Matrix4 &left, const Vector4 &right)
	{
		return left.Scale(1.0f / right);
	}

	Matrix4 operator*(const float &left, const Matrix4 &right)
	{
		return right.Scale(Vector4(left, left, left, left));
	}

	Matrix4 operator/(const float &left, const Matrix4 &right)
	{
		return right.Scale(1.0f / Vector4(left, left, left, left));
	}

	Matrix4 operator*(const Matrix4 &left, const float &right)
	{
		return left.Scale(Vector4(right, right, right, right));
	}

	Matrix4 operator/(const Matrix4 &left, const float &right)
	{
		return left.Scale(1.0f / Vector4(right, right, right, right));
	}

	Matrix4 &Matrix4::operator+=(const Matrix4 &other)
	{
		return *this = Add(other);
	}

	Matrix4 &Matrix4::operator-=(const Matrix4 &other)
	{
		return *this = Subtract(other);
	}

	Matrix4 &Matrix4::operator*=(const Matrix4 &other)
	{
		return *this = Multiply(other);
	}

	Matrix4 &Matrix4::operator/=(const Matrix4 &other)
	{
		return *this = Divide(other);
	}

	Matrix4 &Matrix4::operator*=(const Vector4 &other)
	{
		return *this = Scale(other);
	}

	Matrix4 &Matrix4::operator/=(const Vector4 &other)
	{
		return *this = Scale(1.0f / other);
	}

	Matrix4 &Matrix4::operator*=(const float &other)
	{
		return *this = Scale(Vector4(other, other, other, other));
	}

	Matrix4 &Matrix4::operator/=(const float &other)
	{
		return *this = Scale(1.0f / Vector4(other, other, other, other));
	}

	std::ostream &operator<<(std::ostream &stream, const Matrix4 &matrix)
	{
		stream << matrix.ToString();
		return stream;
	}

	std::string Matrix4::ToString() const
	{
		std::stringstream result;
		result << "Matrix4(" << m_rows[0][0] << ", " << m_rows[0][1] << ", " << m_rows[0][2] << ", " << m_rows[0][3] << ", " <<
			m_rows[1][0] << ", " << m_rows[1][1] << ", " << m_rows[1][2] << ", " << m_rows[1][3] << ", " <<
			m_rows[2][0] << ", " << m_rows[2][1] << ", " << m_rows[2][2] << ", " << m_rows[2][3] << ", " <<
			m_rows[3][0] << ", " << m_rows[3][1] << ", " << m_rows[3][2] << ", " << m_rows[3][3] << ")";
		return result.str();
	}
}
