#include "Frustum.hpp"

#include <array>
#include <cmath>
#include "Helpers/SquareArray.hpp"

namespace acid
{
	Frustum::Frustum() :
		m_frustumArray(SquareArray::FloatArray(6, 4))
	{
	}

	Frustum::~Frustum()
	{
		for (int i = 0; i < 6; i++)
		{
			delete[] m_frustumArray[i];
		}

		delete[] m_frustumArray;
	}

	void Frustum::Update(const Matrix4 &view, const Matrix4 &projection)
	{
		auto clip = std::array<float, 16>();

		clip[0] = view[0][0] * projection[0][0] + view[0][1] * projection[1][0] + view[0][2] * projection[2][0] + view[0][3] * projection[3][0];
		clip[1] = view[0][0] * projection[0][1] + view[0][1] * projection[1][1] + view[0][2] * projection[2][1] + view[0][3] * projection[3][1];
		clip[2] = view[0][0] * projection[0][2] + view[0][1] * projection[1][2] + view[0][2] * projection[2][2] + view[0][3] * projection[3][2];
		clip[3] = view[0][0] * projection[0][3] + view[0][1] * projection[1][3] + view[0][2] * projection[2][3] + view[0][3] * projection[3][3];

		clip[4] = view[1][0] * projection[0][0] + view[1][1] * projection[1][0] + view[1][2] * projection[2][0] + view[1][3] * projection[3][0];
		clip[5] = view[1][0] * projection[0][1] + view[1][1] * projection[1][1] + view[1][2] * projection[2][1] + view[1][3] * projection[3][1];
		clip[6] = view[1][0] * projection[0][2] + view[1][1] * projection[1][2] + view[1][2] * projection[2][2] + view[1][3] * projection[3][2];
		clip[7] = view[1][0] * projection[0][3] + view[1][1] * projection[1][3] + view[1][2] * projection[2][3] + view[1][3] * projection[3][3];

		clip[8] = view[2][0] * projection[0][0] + view[2][1] * projection[1][0] + view[2][2] * projection[2][0] + view[2][3] * projection[3][0];
		clip[9] = view[2][0] * projection[0][1] + view[2][1] * projection[1][1] + view[2][2] * projection[2][1] + view[2][3] * projection[3][1];
		clip[10] = view[2][0] * projection[0][2] + view[2][1] * projection[1][2] + view[2][2] * projection[2][2] + view[2][3] * projection[3][2];
		clip[11] = view[2][0] * projection[0][3] + view[2][1] * projection[1][3] + view[2][2] * projection[2][3] + view[2][3] * projection[3][3];

		clip[12] = view[3][0] * projection[0][0] + view[3][1] * projection[1][0] + view[3][2] * projection[2][0] + view[3][3] * projection[3][0];
		clip[13] = view[3][0] * projection[0][1] + view[3][1] * projection[1][1] + view[3][2] * projection[2][1] + view[3][3] * projection[3][1];
		clip[14] = view[3][0] * projection[0][2] + view[3][1] * projection[1][2] + view[3][2] * projection[2][2] + view[3][3] * projection[3][2];
		clip[15] = view[3][0] * projection[0][3] + view[3][1] * projection[1][3] + view[3][2] * projection[2][3] + view[3][3] * projection[3][3];

		// This will extract the LEFT side of the frustum.
		m_frustumArray[1][0] = clip[3] - clip[0];
		m_frustumArray[1][1] = clip[7] - clip[4];
		m_frustumArray[1][2] = clip[11] - clip[8];
		m_frustumArray[1][3] = clip[15] - clip[12];

		NormalizePlane(1);

		// This will extract the RIGHT side of the frustum.
		m_frustumArray[0][0] = clip[3] + clip[0];
		m_frustumArray[0][1] = clip[7] + clip[4];
		m_frustumArray[0][2] = clip[11] + clip[8];
		m_frustumArray[0][3] = clip[15] + clip[12];

		NormalizePlane(0);

		// This will extract the BOTTOM side of the frustum.
		m_frustumArray[2][0] = clip[3] + clip[1];
		m_frustumArray[2][1] = clip[7] + clip[5];
		m_frustumArray[2][2] = clip[11] + clip[9];
		m_frustumArray[2][3] = clip[15] + clip[13];

		NormalizePlane(2);

		// This will extract the TOP side of the frustum.
		m_frustumArray[3][0] = clip[3] - clip[1];
		m_frustumArray[3][1] = clip[7] - clip[5];
		m_frustumArray[3][2] = clip[11] - clip[9];
		m_frustumArray[3][3] = clip[15] - clip[13];

		NormalizePlane(3);

		// This will extract the BACK side of the frustum.
		m_frustumArray[4][0] = clip[3] + clip[2];
		m_frustumArray[4][1] = clip[7] + clip[6];
		m_frustumArray[4][2] = clip[11] + clip[10];
		m_frustumArray[4][3] = clip[15] + clip[14];

		NormalizePlane(4);

		// This will extract the FRONT side of the frustum.
		m_frustumArray[5][0] = clip[3] - clip[2];
		m_frustumArray[5][1] = clip[7] - clip[6];
		m_frustumArray[5][2] = clip[11] - clip[10];
		m_frustumArray[5][3] = clip[15] - clip[14];

		NormalizePlane(5);
	}

	bool Frustum::PointInFrustum(const Vector3 &position) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_frustumArray[i][0] * position.m_x + m_frustumArray[i][1] * position.m_y + m_frustumArray[i][2] * position.m_z +
				m_frustumArray[i][3] <= 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	bool Frustum::SphereInFrustum(const Vector3 &position, const float &radius) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_frustumArray[i][0] * position.m_x + m_frustumArray[i][1] * position.m_y + m_frustumArray[i][2] * position.m_z +
				m_frustumArray[i][3] <= -radius)
			{
				return false;
			}
		}

		return true;
	}

	bool Frustum::CubeInFrustum(const Vector3 &min, const Vector3 &max) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_frustumArray[i][0] * min.m_x + m_frustumArray[i][1] * min.m_y + m_frustumArray[i][2] * min.m_z + m_frustumArray[i][3] <= 0.0f &&
				m_frustumArray[i][0] * max.m_x + m_frustumArray[i][1] * min.m_y + m_frustumArray[i][2] * min.m_z + m_frustumArray[i][3] <= 0.0f &&
				m_frustumArray[i][0] * min.m_x + m_frustumArray[i][1] * max.m_y + m_frustumArray[i][2] * min.m_z + m_frustumArray[i][3] <= 0.0f &&
				m_frustumArray[i][0] * max.m_x + m_frustumArray[i][1] * max.m_y + m_frustumArray[i][2] * min.m_z + m_frustumArray[i][3] <= 0.0f &&
				m_frustumArray[i][0] * min.m_x + m_frustumArray[i][1] * min.m_y + m_frustumArray[i][2] * max.m_z + m_frustumArray[i][3] <= 0.0f &&
				m_frustumArray[i][0] * max.m_x + m_frustumArray[i][1] * min.m_y + m_frustumArray[i][2] * max.m_z + m_frustumArray[i][3] <= 0.0f &&
				m_frustumArray[i][0] * min.m_x + m_frustumArray[i][1] * max.m_y + m_frustumArray[i][2] * max.m_z + m_frustumArray[i][3] <= 0.0f &&
				m_frustumArray[i][0] * max.m_x + m_frustumArray[i][1] * max.m_y + m_frustumArray[i][2] * max.m_z + m_frustumArray[i][3] <= 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	void Frustum::NormalizePlane(const int &side)
	{
		const float magnitude = std::sqrt(m_frustumArray[side][0] * m_frustumArray[side][0] +
			m_frustumArray[side][1] * m_frustumArray[side][1] +
			m_frustumArray[side][2] * m_frustumArray[side][2]);
		m_frustumArray[side][0] /= magnitude;
		m_frustumArray[side][1] /= magnitude;
		m_frustumArray[side][2] /= magnitude;
		m_frustumArray[side][3] /= magnitude;
	}
}
