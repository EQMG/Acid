#include "Matrix4.hpp"

#include "Matrix2.hpp"
#include "Matrix3.hpp"

namespace acid
{
const Matrix4 Matrix4::Identity = Matrix4(1.0f);
const Matrix4 Matrix4::Zero = Matrix4(0.0f);

Matrix4::Matrix4(const float &diagonal)
{
	std::memset(m_rows, 0, 4 * sizeof(Vector4f));
	m_rows[0][0] = diagonal;
	m_rows[1][1] = diagonal;
	m_rows[2][2] = diagonal;
	m_rows[3][3] = diagonal;
}

Matrix4::Matrix4(const Matrix2 &source)
{
	std::memset(m_rows, 0, 4 * sizeof(Vector4f));
	m_rows[2][2] = 1.0f;
	m_rows[3][3] = 1.0f;
	std::memcpy(m_rows, source.m_rows, 2 * sizeof(Vector2f));
}

Matrix4::Matrix4(const Matrix3 &source)
{
	std::memset(m_rows, 0, 4 * sizeof(Vector4f));
	m_rows[3][3] = 1.0f;
	std::memcpy(m_rows, source.m_rows, 3 * sizeof(Vector3f));
}

Matrix4::Matrix4(const float *source)
{
	std::memcpy(m_rows, source, 4 * 4 * sizeof(float));
}

Matrix4::Matrix4(const Vector4f *source)
{
	std::memcpy(m_rows, source, 4 * sizeof(Vector4f));
}

Matrix4 Matrix4::Add(const Matrix4 &other) const
{
	Matrix4 result = Matrix4();

	for (int32_t row = 0; row < 4; row++)
	{
		for (int32_t col = 0; col < 4; col++)
		{
			result[row][col] = m_rows[row][col] + other[row][col];
		}
	}

	return result;
}

Matrix4 Matrix4::Subtract(const Matrix4 &other) const
{
	Matrix4 result = Matrix4();

	for (int32_t row = 0; row < 4; row++)
	{
		for (int32_t col = 0; col < 4; col++)
		{
			result[row][col] = m_rows[row][col] - other[row][col];
		}
	}

	return result;
}

Matrix4 Matrix4::Multiply(const Matrix4 &other) const
{
	Matrix4 result = Matrix4();

	for (int32_t row = 0; row < 4; row++)
	{
		for (int32_t col = 0; col < 4; col++)
		{
			result[row][col] = m_rows[0][col] * other[row][0] + m_rows[1][col] * other[row][1] + m_rows[2][col] * other[row][2] + m_rows[3][col] * other[row][3];
		}
	}

	return result;
}

Vector4f Matrix4::Multiply(const Vector4f &other) const
{
	Vector4f result = Vector4f();

	for (int32_t row = 0; row < 4; row++)
	{
		result[row] = m_rows[0][row] * other.m_x + m_rows[1][row] * other.m_y + m_rows[2][row] * other.m_z + m_rows[3][row] * other.m_w;
	}

	return result;
}

Matrix4 Matrix4::Divide(const Matrix4 &other) const
{
	Matrix4 result = Matrix4();

	for (int32_t row = 0; row < 4; row++)
	{
		for (int32_t col = 0; col < 4; col++)
		{
			result[row][col] = m_rows[0][col] / other[row][0] + m_rows[1][col] / other[row][1] + m_rows[2][col] / other[row][2] + m_rows[3][col] / other[row][3];
		}
	}

	return result;
}

Vector4f Matrix4::Transform(const Vector4f &other) const
{
	Vector4f result = Vector4f();

	for (int32_t row = 0; row < 4; row++)
	{
		result[row] = m_rows[0][row] * other.m_x + m_rows[1][row] * other.m_y + m_rows[2][row] * other.m_z + m_rows[3][row] * other.m_w;
	}

	return result;
}

Matrix4 Matrix4::Translate(const Vector2f &other) const
{
	Matrix4 result = Matrix4(*this);

	for (int32_t col = 0; col < 4; col++)
	{
		result[3][col] += m_rows[0][col] * other.m_x + m_rows[1][col] * other.m_y;
	}

	return result;
}

Matrix4 Matrix4::Translate(const Vector3f &other) const
{
	Matrix4 result = Matrix4(*this);

	for (int32_t col = 0; col < 4; col++)
	{
		result[3][col] += m_rows[0][col] * other.m_x + m_rows[1][col] * other.m_y + m_rows[2][col] * other.m_z;
	}

	return result;
}

Matrix4 Matrix4::Scale(const Vector3f &other) const
{
	Matrix4 result = Matrix4(*this);

	for (int32_t row = 0; row < 3; row++)
	{
		for (int32_t col = 0; col < 4; col++)
		{
			result[row][col] *= other[row];
		}
	}

	return result;
}

Matrix4 Matrix4::Scale(const Vector4f &other) const
{
	Matrix4 result = Matrix4(*this);

	for (int32_t row = 0; row < 4; row++)
	{
		for (int32_t col = 0; col < 4; col++)
		{
			result[row][col] *= other[row];
		}
	}

	return result;
}

Matrix4 Matrix4::Rotate(const float &angle, const Vector3f &axis) const
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

	for (int32_t row = 0; row < 3; row++)
	{
		for (int32_t col = 0; col < 4; col++)
		{
			result[row][col] = m_rows[0][col] * f[row][0] + m_rows[1][col] * f[row][1] + m_rows[2][col] * f[row][2];
		}
	}

	return result;
}

Matrix4 Matrix4::Negate() const
{
	Matrix4 result = Matrix4();

	for (int32_t row = 0; row < 4; row++)
	{
		for (int32_t col = 0; col < 4; col++)
		{
			result[row][col] = -m_rows[row][col];
		}
	}

	return result;
}

Matrix4 Matrix4::Inverse() const
{
	Matrix4 result = Matrix4();

	float det = Determinant();

	if (det == 0.0f)
	{
		throw std::runtime_error("Can't invert a matrix with a determinant of zero");
	}

	for (int32_t j = 0; j < 4; j++)
	{
		for (int32_t i = 0; i < 4; i++)
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

	for (int32_t row = 0; row < 4; row++)
	{
		for (int32_t col = 0; col < 4; col++)
		{
			result[row][col] = m_rows[col][row];
		}
	}

	return result;
}

float Matrix4::Determinant() const
{
	float result = 0.0f;

	for (int32_t i = 0; i < 4; i++)
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

Matrix3 Matrix4::GetSubmatrix(const int32_t &row, const int32_t &col) const
{
	Matrix3 result = Matrix3();
	int32_t colCount = 0;
	int32_t rowCount = 0;

	for (int32_t i = 0; i < 4; i++)
	{
		if (i != row)
		{
			colCount = 0;

			for (int32_t j = 0; j < 4; j++)
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

Matrix4 Matrix4::TransformationMatrix(const Vector3f &translation, const Vector3f &rotation, const Vector3f &scale)
{
	Matrix4 result = Matrix4();
	result = result.Translate(translation);
	result = result.Rotate(rotation.m_x, Vector3f::Right);
	result = result.Rotate(rotation.m_y, Vector3f::Up);
	result = result.Rotate(rotation.m_z, Vector3f::Front);
	result = result.Scale(scale);
	return result;
}

Matrix4 Matrix4::PerspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear, const float &zFar)
{
	Matrix4 result = Matrix4(0.0f);

	float f = std::tan(0.5f * fov);

	result[0][0] = 1.0f / (aspectRatio * f);
	result[1][1] = 1.0f / f;
	result[2][2] = zFar / (zNear - zFar);
	result[2][3] = -1.0f;
	result[3][2] = -(zFar * zNear) / (zFar - zNear);
	return result;
}

Matrix4 Matrix4::PerspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear)
{
	Matrix4 result = Matrix4(0.0f);

	float range = std::tan(0.5f * fov) * zNear;
	float left = -range * aspectRatio;
	float right = range * aspectRatio;
	float bottom = -range;
	float top = range;

	result[0][0] = (2.0f * zNear) / (right - left);
	result[1][1] = (2.0f * zNear) / (top - bottom);
	result[2][2] = -1.0f;
	result[2][3] = -1.0f;
	result[3][2] = -2.0f * zNear;
	return result;
}

Matrix4 Matrix4::OrthographicMatrix(const float &left, const float &right, const float &bottom, const float &top, const float &zNear, const float &zFar)
{
	Matrix4 result = Matrix4();

	result[0][0] = 2.0f / (right - left);
	result[1][1] = 2.0f / (top - bottom);
	result[3][0] = -(right + left) / (right - left);
	result[3][1] = -(top + bottom) / (top - bottom);
	result[2][2] = -1.0f / (zFar - zNear);
	result[2][3] = zNear / (zFar - zNear);
	return result;
}

Matrix4 Matrix4::FrustumMatrix(const float &left, const float &right, const float &bottom, const float &top, const float &zNear, const float &zFar)
{
	Matrix4 result = Matrix4(0.0f);

	result[0][0] = (2.0f * zNear) / (right - left);
	result[1][1] = (2.0f * zNear) / (top - bottom);
	result[2][0] = (right + left) / (right - left);
	result[2][1] = (top + bottom) / (top - bottom);
	result[2][3] = -1.0f;
	result[2][2] = zFar / (zNear - zFar);
	result[3][2] = -(zFar * zNear) / (zFar - zNear);
	return result;
}

Matrix4 Matrix4::ViewMatrix(const Vector3f &position, const Vector3f &rotation)
{
	Matrix4 result = Matrix4();
	result = result.Rotate(rotation.m_x, Vector3f::Right);
	result = result.Rotate(rotation.m_y, Vector3f::Up);
	result = result.Rotate(rotation.m_z, Vector3f::Front);
	result = result.Translate(-position);
	return result;
}

Vector3f Matrix4::Project(const Vector3f &worldSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix)
{
	Vector4f point4 = Vector4f(worldSpace.m_x, worldSpace.m_y, worldSpace.m_z, 1.0f);
	point4 = viewMatrix.Transform(point4);
	point4 = projectionMatrix.Transform(point4);

	Vector3f result = Vector3f(point4);
	result.m_x /= result.m_z;
	result.m_y /= result.m_z;
	return result;
}

Vector3f Matrix4::Unproject(const Vector3f &screenSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix)
{
	Vector3f result = Vector3f();
	// TODO: Create
	return result;
}

Matrix4 Matrix4::LookAt(const Vector3f &eye, const Vector3f &centre, const Vector3f &up)
{
	Vector3f f = (centre - eye).Normalize();
	Vector3f s = f.Cross(up).Normalize();
	Vector3f u = s.Cross(f);

	Matrix4 result = Matrix4();
	result[0][0] = s.m_x;
	result[1][0] = s.m_y;
	result[2][0] = s.m_z;
	result[0][1] = u.m_x;
	result[1][1] = u.m_y;
	result[2][1] = u.m_z;
	result[0][2] = -f.m_x;
	result[1][2] = -f.m_y;
	result[2][2] = -f.m_z;
	result[3][0] = -s.Dot(eye);
	result[3][1] = -u.Dot(eye);
	result[3][2] = f.Dot(eye);
	return result;
}

void Matrix4::Decode(const Metadata &metadata)
{
	metadata.GetChild("m0", m_rows[0]);
	metadata.GetChild("m1", m_rows[1]);
	metadata.GetChild("m2", m_rows[2]);
	metadata.GetChild("m3", m_rows[3]);
}

void Matrix4::Encode(Metadata &metadata) const
{
	metadata.SetChild("m0", m_rows[0]);
	metadata.SetChild("m1", m_rows[1]);
	metadata.SetChild("m2", m_rows[2]);
	metadata.SetChild("m3", m_rows[3]);
}

bool Matrix4::operator==(const Matrix4 &other) const
{
	return m_rows[0] == other[0] && m_rows[1] == other[1] && m_rows[2] == other[2] && m_rows[3] == other[3];
}

bool Matrix4::operator!=(const Matrix4 &other) const
{
	return !(*this == other);
}

Matrix4 Matrix4::operator-() const
{
	return Negate();
}

const Vector4f &Matrix4::operator[](const uint32_t &index) const
{
	assert(index < 4);
	return m_rows[index];
}

Vector4f &Matrix4::operator[](const uint32_t &index)
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

Matrix4 operator*(const Vector4f &left, const Matrix4 &right)
{
	return right.Scale(left);
}

Matrix4 operator/(const Vector4f &left, const Matrix4 &right)
{
	return right.Scale(1.0f / left);
}

Matrix4 operator*(const Matrix4 &left, const Vector4f &right)
{
	return left.Scale(right);
}

Matrix4 operator/(const Matrix4 &left, const Vector4f &right)
{
	return left.Scale(1.0f / right);
}

Matrix4 operator*(const float &left, const Matrix4 &right)
{
	return right.Scale(Vector4f(left, left, left, left));
}

Matrix4 operator/(const float &left, const Matrix4 &right)
{
	return right.Scale(1.0f / Vector4f(left, left, left, left));
}

Matrix4 operator*(const Matrix4 &left, const float &right)
{
	return left.Scale(Vector4f(right, right, right, right));
}

Matrix4 operator/(const Matrix4 &left, const float &right)
{
	return left.Scale(1.0f / Vector4f(right, right, right, right));
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

Matrix4 &Matrix4::operator*=(const Vector4f &other)
{
	return *this = Scale(other);
}

Matrix4 &Matrix4::operator/=(const Vector4f &other)
{
	return *this = Scale(1.0f / other);
}

Matrix4 &Matrix4::operator*=(const float &other)
{
	return *this = Scale(Vector4f(other, other, other, other));
}

Matrix4 &Matrix4::operator/=(const float &other)
{
	return *this = Scale(1.0f / Vector4f(other, other, other, other));
}

std::ostream &operator<<(std::ostream &stream, const Matrix4 &matrix)
{
	stream << matrix.ToString();
	return stream;
}

std::string Matrix4::ToString() const
{
	std::stringstream stream;
	stream.precision(10);
	stream << "Matrix4(" << m_rows[0][0] << ", " << m_rows[0][1] << ", " << m_rows[0][2] << ", " << m_rows[0][3] << ", \n" << m_rows[1][0] << ", " << m_rows[1][1] << ", "
	       << m_rows[1][2] << ", " << m_rows[1][3] << ", \n" << m_rows[2][0] << ", " << m_rows[2][1] << ", " << m_rows[2][2] << ", " << m_rows[2][3] << ", \n" << m_rows[3][0]
	       << ", " << m_rows[3][1] << ", " << m_rows[3][2] << ", " << m_rows[3][3] << ")";
	return stream.str();
}
}
