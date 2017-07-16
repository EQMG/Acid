#pragma once

namespace flounder {
	/// <summary>
	/// Classic Perlin noise in 3D.
	/// </summary>
	class noiseclassic
	{
	private:
		static const int GRAD_3[12][3];
		static const int P[256];
		static const int PERM[512]; // To remove the need for index wrapping, float the permutation table length.

		int m_seed;
	public:
		noiseclassic(const int seed);

		~noiseclassic();

		float noise(float x);

		float noise(float x, float y);

		// Classic Perlin noise, 3D version.
		float noise(float x, float y, float z);

		inline int getSeed() { return m_seed; }

		inline void setSeed(const int seed) { m_seed = seed; }
	private:
		// This method is a *lot* faster than using (int)Math.floor(x)
		int fastfloor(const float x);

		float dot(const int *g, const float x, const float y, const float z);

		float mix(const float a, const float b, const float t);

		float fade(const float t);
	};
}
