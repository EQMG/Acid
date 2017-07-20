#pragma once

#include <cmath>

namespace flounder 
{
	/// <summary>
	/// Simplex noise in 2D, 3D and 4D.
	/// </summary>
	class noisesimplex
	{
	private:
		static const int GRAD_3[12][3];
		static const int GRAD_4[32][4];
		static const int P[256]; // The data used to create the permutations below, for(int i=0; i<512; i++) PERM[i]=P[i & 255];
		static const int PERM[512]; // To remove the need for index wrapping, float the permutation table length.
		static const int SIMPLEX[64][4]; // A lookup table to traverse the simplex around a given point in 4D. Details can be found where this table is used, in the 4D noise method.

		int m_seed;
	public:
		noisesimplex(const int seed);

		~noisesimplex();

		// 2D SIMPLEX noise
		float noise(float xin, float yin);

		// 3D SIMPLEX noise
		float noise(float xin, float yin, float zin);

		// 4D SIMPLEX noise
		float noise(float x, float y, float z, float w);

		inline int getSeed() { return m_seed; }

		inline void setSeed(const int seed) { m_seed = seed; }
	private:
		// This method is a *lot* faster than using (int)Math.floor(x)
		static int fastfloor(float x);

		static float dot(const int *g, const float x, const float y);

		static float dot(const int *g, const float x, const float y, const float z);

		static float dot(const int *g, const float x, const float y, const float z, const float w);
	};
}
