#include "Frustum.hpp"

#include <cmath>
#include "Helpers/SquareArray.hpp"

namespace fl
{
	Frustum::Frustum() :
		m_frustum(SquareArray::FloatArray(6, 4))
	{
	}

	Frustum::~Frustum()
	{
		for (int i = 0; i < 6; i++)
		{
			delete[] m_frustum[i];
		}

		delete[] m_frustum;
	}

	void Frustum::Update(const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix) const
	{
		float *view = viewMatrix.ToArray();
		float *proj = projectionMatrix.ToArray();
		float *clip = new float[16];

		clip[0] = view[0] * proj[0] + view[1] * proj[4] + view[2] * proj[8] + view[3] * proj[12];
		clip[1] = view[0] * proj[1] + view[1] * proj[5] + view[2] * proj[9] + view[3] * proj[13];
		clip[2] = view[0] * proj[2] + view[1] * proj[6] + view[2] * proj[10] + view[3] * proj[14];
		clip[3] = view[0] * proj[3] + view[1] * proj[7] + view[2] * proj[11] + view[3] * proj[15];

		clip[4] = view[4] * proj[0] + view[5] * proj[4] + view[6] * proj[8] + view[7] * proj[12];
		clip[5] = view[4] * proj[1] + view[5] * proj[5] + view[6] * proj[9] + view[7] * proj[13];
		clip[6] = view[4] * proj[2] + view[5] * proj[6] + view[6] * proj[10] + view[7] * proj[14];
		clip[7] = view[4] * proj[3] + view[5] * proj[7] + view[6] * proj[11] + view[7] * proj[15];

		clip[8] = view[8] * proj[0] + view[9] * proj[4] + view[10] * proj[8] + view[11] * proj[12];
		clip[9] = view[8] * proj[1] + view[9] * proj[5] + view[10] * proj[9] + view[11] * proj[13];
		clip[10] = view[8] * proj[2] + view[9] * proj[6] + view[10] * proj[10] + view[11] * proj[14];
		clip[11] = view[8] * proj[3] + view[9] * proj[7] + view[10] * proj[11] + view[11] * proj[15];

		clip[12] = view[12] * proj[0] + view[13] * proj[4] + view[14] * proj[8] + view[15] * proj[12];
		clip[13] = view[12] * proj[1] + view[13] * proj[5] + view[14] * proj[9] + view[15] * proj[13];
		clip[14] = view[12] * proj[2] + view[13] * proj[6] + view[14] * proj[10] + view[15] * proj[14];
		clip[15] = view[12] * proj[3] + view[13] * proj[7] + view[14] * proj[11] + view[15] * proj[15];

		// This will extract the LEFT side of the frustum.
		m_frustum[FrustumLeft][FrustumA] = clip[3] - clip[0];
		m_frustum[FrustumLeft][FrustumB] = clip[7] - clip[4];
		m_frustum[FrustumLeft][FrustumC] = clip[11] - clip[8];
		m_frustum[FrustumLeft][FrustumD] = clip[15] - clip[12];

		NormalizePlane(FrustumLeft);

		// This will extract the RIGHT side of the frustum.
		m_frustum[FrustumRight][FrustumA] = clip[3] + clip[0];
		m_frustum[FrustumRight][FrustumB] = clip[7] + clip[4];
		m_frustum[FrustumRight][FrustumC] = clip[11] + clip[8];
		m_frustum[FrustumRight][FrustumD] = clip[15] + clip[12];

		NormalizePlane(FrustumRight);

		// This will extract the BOTTOM side of the frustum.
		m_frustum[FrustumBottom][FrustumA] = clip[3] + clip[1];
		m_frustum[FrustumBottom][FrustumB] = clip[7] + clip[5];
		m_frustum[FrustumBottom][FrustumC] = clip[11] + clip[9];
		m_frustum[FrustumBottom][FrustumD] = clip[15] + clip[13];

		NormalizePlane(FrustumBottom);

		// This will extract the TOP side of the frustum.
		m_frustum[FrustumTop][FrustumA] = clip[3] - clip[1];
		m_frustum[FrustumTop][FrustumB] = clip[7] - clip[5];
		m_frustum[FrustumTop][FrustumC] = clip[11] - clip[9];
		m_frustum[FrustumTop][FrustumD] = clip[15] - clip[13];

		NormalizePlane(FrustumTop);

		// This will extract the BACK side of the frustum.
		m_frustum[FrustumBack][FrustumA] = clip[3] + clip[2];
		m_frustum[FrustumBack][FrustumB] = clip[7] + clip[6];
		m_frustum[FrustumBack][FrustumC] = clip[11] + clip[10];
		m_frustum[FrustumBack][FrustumD] = clip[15] + clip[14];

		NormalizePlane(FrustumBack);

		// This will extract the FRONT side of the frustum.
		m_frustum[FrustumFront][FrustumA] = clip[3] - clip[2];
		m_frustum[FrustumFront][FrustumB] = clip[7] - clip[6];
		m_frustum[FrustumFront][FrustumC] = clip[11] - clip[10];
		m_frustum[FrustumFront][FrustumD] = clip[15] - clip[14];

		NormalizePlane(FrustumFront);

		// Deletes the arrays used to update the frustum.
		delete[] view;
		delete[] proj;
		delete[] clip;
	}

	bool Frustum::PointInFrustum(const Vector3 &position) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_frustum[i][FrustumA] * position.m_x + m_frustum[i][FrustumB] * position.m_y + m_frustum[i][FrustumC] * position.m_z +
				m_frustum[i][FrustumD] <= 0.0f)
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
			if (m_frustum[i][FrustumA] * position.m_x + m_frustum[i][FrustumB] * position.m_y + m_frustum[i][FrustumC] * position.m_z +
				m_frustum[i][FrustumD] <= -radius)
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
			if (m_frustum[i][FrustumA] * min.m_x + m_frustum[i][FrustumB] * min.m_y + m_frustum[i][FrustumC] * min.m_z + m_frustum[i][FrustumD] <= 0.0f &&
				m_frustum[i][FrustumA] * max.m_x + m_frustum[i][FrustumB] * min.m_y + m_frustum[i][FrustumC] * min.m_z + m_frustum[i][FrustumD] <= 0.0f &&
				m_frustum[i][FrustumA] * min.m_x + m_frustum[i][FrustumB] * max.m_y + m_frustum[i][FrustumC] * min.m_z + m_frustum[i][FrustumD] <= 0.0f &&
				m_frustum[i][FrustumA] * max.m_x + m_frustum[i][FrustumB] * max.m_y + m_frustum[i][FrustumC] * min.m_z + m_frustum[i][FrustumD] <= 0.0f &&
				m_frustum[i][FrustumA] * min.m_x + m_frustum[i][FrustumB] * min.m_y + m_frustum[i][FrustumC] * max.m_z + m_frustum[i][FrustumD] <= 0.0f &&
				m_frustum[i][FrustumA] * max.m_x + m_frustum[i][FrustumB] * min.m_y + m_frustum[i][FrustumC] * max.m_z + m_frustum[i][FrustumD] <= 0.0f &&
				m_frustum[i][FrustumA] * min.m_x + m_frustum[i][FrustumB] * max.m_y + m_frustum[i][FrustumC] * max.m_z + m_frustum[i][FrustumD] <= 0.0f &&
				m_frustum[i][FrustumA] * max.m_x + m_frustum[i][FrustumB] * max.m_y + m_frustum[i][FrustumC] * max.m_z + m_frustum[i][FrustumD] <= 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	void Frustum::NormalizePlane(const int &side) const
	{
		const float magnitude = std::sqrt(m_frustum[side][FrustumA] * m_frustum[side][FrustumA] +
			m_frustum[side][FrustumB] * m_frustum[side][FrustumB] +
			m_frustum[side][FrustumC] * m_frustum[side][FrustumC]);
		m_frustum[side][FrustumA] /= magnitude;
		m_frustum[side][FrustumB] /= magnitude;
		m_frustum[side][FrustumC] /= magnitude;
		m_frustum[side][FrustumD] /= magnitude;
	}
}
