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
		m_frustum[FRUSTUM_LEFT][FRUSTUM_A] = clip[3] - clip[0];
		m_frustum[FRUSTUM_LEFT][FRUSTUM_B] = clip[7] - clip[4];
		m_frustum[FRUSTUM_LEFT][FRUSTUM_C] = clip[11] - clip[8];
		m_frustum[FRUSTUM_LEFT][FRUSTUM_D] = clip[15] - clip[12];

		NormalizePlane(FRUSTUM_LEFT);

		// This will extract the RIGHT side of the frustum.
		m_frustum[FRUSTUM_RIGHT][FRUSTUM_A] = clip[3] + clip[0];
		m_frustum[FRUSTUM_RIGHT][FRUSTUM_B] = clip[7] + clip[4];
		m_frustum[FRUSTUM_RIGHT][FRUSTUM_C] = clip[11] + clip[8];
		m_frustum[FRUSTUM_RIGHT][FRUSTUM_D] = clip[15] + clip[12];

		NormalizePlane(FRUSTUM_RIGHT);

		// This will extract the BOTTOM side of the frustum.
		m_frustum[FRUSTUM_BOTTOM][FRUSTUM_A] = clip[3] + clip[1];
		m_frustum[FRUSTUM_BOTTOM][FRUSTUM_B] = clip[7] + clip[5];
		m_frustum[FRUSTUM_BOTTOM][FRUSTUM_C] = clip[11] + clip[9];
		m_frustum[FRUSTUM_BOTTOM][FRUSTUM_D] = clip[15] + clip[13];

		NormalizePlane(FRUSTUM_BOTTOM);

		// This will extract the TOP side of the frustum.
		m_frustum[FRUSTUM_TOP][FRUSTUM_A] = clip[3] - clip[1];
		m_frustum[FRUSTUM_TOP][FRUSTUM_B] = clip[7] - clip[5];
		m_frustum[FRUSTUM_TOP][FRUSTUM_C] = clip[11] - clip[9];
		m_frustum[FRUSTUM_TOP][FRUSTUM_D] = clip[15] - clip[13];

		NormalizePlane(FRUSTUM_TOP);

		// This will extract the BACK side of the frustum.
		m_frustum[FRUSTUM_BACK][FRUSTUM_A] = clip[3] + clip[2];
		m_frustum[FRUSTUM_BACK][FRUSTUM_B] = clip[7] + clip[6];
		m_frustum[FRUSTUM_BACK][FRUSTUM_C] = clip[11] + clip[10];
		m_frustum[FRUSTUM_BACK][FRUSTUM_D] = clip[15] + clip[14];

		NormalizePlane(FRUSTUM_BACK);

		// This will extract the FRONT side of the frustum.
		m_frustum[FRUSTUM_FRONT][FRUSTUM_A] = clip[3] - clip[2];
		m_frustum[FRUSTUM_FRONT][FRUSTUM_B] = clip[7] - clip[6];
		m_frustum[FRUSTUM_FRONT][FRUSTUM_C] = clip[11] - clip[10];
		m_frustum[FRUSTUM_FRONT][FRUSTUM_D] = clip[15] - clip[14];

		NormalizePlane(FRUSTUM_FRONT);

		// Deletes the arrays used to update the frustum.
		delete[] view;
		delete[] proj;
		delete[] clip;
	}

	bool Frustum::PointInFrustum(const Vector3 &position) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_frustum[i][FRUSTUM_A] * position.m_x + m_frustum[i][FRUSTUM_B] * position.m_y + m_frustum[i][FRUSTUM_C] * position.m_z +
				m_frustum[i][FRUSTUM_D] <= 0.0f)
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
			if (m_frustum[i][FRUSTUM_A] * position.m_x + m_frustum[i][FRUSTUM_B] * position.m_y + m_frustum[i][FRUSTUM_C] * position.m_z +
				m_frustum[i][FRUSTUM_D] <= -radius)
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
			if (m_frustum[i][FRUSTUM_A] * min.m_x + m_frustum[i][FRUSTUM_B] * min.m_y + m_frustum[i][FRUSTUM_C] * min.m_z + m_frustum[i][FRUSTUM_D] <= 0.0f &&
				m_frustum[i][FRUSTUM_A] * max.m_x + m_frustum[i][FRUSTUM_B] * min.m_y + m_frustum[i][FRUSTUM_C] * min.m_z + m_frustum[i][FRUSTUM_D] <= 0.0f &&
				m_frustum[i][FRUSTUM_A] * min.m_x + m_frustum[i][FRUSTUM_B] * max.m_y + m_frustum[i][FRUSTUM_C] * min.m_z + m_frustum[i][FRUSTUM_D] <= 0.0f &&
				m_frustum[i][FRUSTUM_A] * max.m_x + m_frustum[i][FRUSTUM_B] * max.m_y + m_frustum[i][FRUSTUM_C] * min.m_z + m_frustum[i][FRUSTUM_D] <= 0.0f &&
				m_frustum[i][FRUSTUM_A] * min.m_x + m_frustum[i][FRUSTUM_B] * min.m_y + m_frustum[i][FRUSTUM_C] * max.m_z + m_frustum[i][FRUSTUM_D] <= 0.0f &&
				m_frustum[i][FRUSTUM_A] * max.m_x + m_frustum[i][FRUSTUM_B] * min.m_y + m_frustum[i][FRUSTUM_C] * max.m_z + m_frustum[i][FRUSTUM_D] <= 0.0f &&
				m_frustum[i][FRUSTUM_A] * min.m_x + m_frustum[i][FRUSTUM_B] * max.m_y + m_frustum[i][FRUSTUM_C] * max.m_z + m_frustum[i][FRUSTUM_D] <= 0.0f &&
				m_frustum[i][FRUSTUM_A] * max.m_x + m_frustum[i][FRUSTUM_B] * max.m_y + m_frustum[i][FRUSTUM_C] * max.m_z + m_frustum[i][FRUSTUM_D] <= 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	void Frustum::NormalizePlane(const int &side) const
	{
		const float magnitude = std::sqrt(m_frustum[side][FRUSTUM_A] * m_frustum[side][FRUSTUM_A] +
			m_frustum[side][FRUSTUM_B] * m_frustum[side][FRUSTUM_B] +
			m_frustum[side][FRUSTUM_C] * m_frustum[side][FRUSTUM_C]);
		m_frustum[side][FRUSTUM_A] /= magnitude;
		m_frustum[side][FRUSTUM_B] /= magnitude;
		m_frustum[side][FRUSTUM_C] /= magnitude;
		m_frustum[side][FRUSTUM_D] /= magnitude;
	}
}
