#include "noiseperlin.h"

namespace flounder
{
	const int noiseperlin::B = 0x1000; // 4096
	const int noiseperlin::BM = 0xff; // 255
	const int noiseperlin::N = 0x1000; // 4096
	const int noiseperlin::SAMPLE_SIZE = 256;

	noiseperlin::noiseperlin(const int &seed)
	{
		m_seed = seed;

		m_pimp = new int[SAMPLE_SIZE << 1];

		// Local variables for setting the noise.
		int i, j, k;

		// Calculate the table of psuedo-random coefficients
		for (i = 0; i < SAMPLE_SIZE; i++)
		{
			m_pimp[i] = i;
		}

		// Generate the psuedo-random permutation table
		while (--i > 0)
		{
			k = m_pimp[i];
			j = static_cast<int>(maths::randomInRange(0l, static_cast<long>(sizeof(long))) & SAMPLE_SIZE);
			m_pimp[i] = m_pimp[j];
			m_pimp[j] = k;
		}

		m_p = new int[B + B + 2];
		m_g1 = new float[B + B + 2];
		m_g3 = helperarray::recFloatArray(B + B + 2, 3);
		m_g2 = helperarray::recFloatArray(B + B + 2, 2);

		for (i = 0; i < B; i++)
		{
			m_p[i] = i;

			m_g1[i] = static_cast<float>(maths::randomInRange(0, B + B + 2) % (B + B) - B) / B;

			for (j = 0; j < 2; j++)
			{
				m_g2[i][j] = static_cast<float>(maths::randomInRange(0, B + B + 2) % (B + B) - B) / B;
			}

			// Normalize 2.
			float s2 = static_cast<float>(1 / sqrt(m_g2[i][0] * m_g2[i][0] + m_g2[i][1] * m_g2[i][1]));
			m_g2[i][0] *= s2;
			m_g2[i][1] *= s2;

			for (j = 0; j < 3; j++)
			{
				m_g3[i][j] = static_cast<float>(maths::randomInRange(0, B + B + 2) % (B + B) - B) / B;
			}

			// Normalize 3.
			float s3 = static_cast<float>(1 / sqrt(m_g3[i][0] * m_g3[i][0] + m_g3[i][1] * m_g3[i][1] + m_g3[i][2] * m_g3[i][2]));
			m_g3[i][0] *= s3;
			m_g3[i][1] *= s3;
			m_g3[i][2] *= s3;
		}

		while (--i > 0)
		{
			k = m_p[i];
			j = maths::randomInRange(0, B + B + 2) % B;
			m_p[i] = m_p[j];
			m_p[j] = k;
		}

		for (i = 0; i < B + 2; i++)
		{
			m_p[B + i] = m_p[i];
			m_g1[B + i] = m_g1[i];

			for (j = 0; j < 2; j++)
			{
				m_g2[B + i][j] = m_g2[i][j];
			}

			for (j = 0; j < 3; j++)
			{
				m_g3[B + i][j] = m_g3[i][j];
			}
		}
	}

	noiseperlin::~noiseperlin()
	{
		delete[] m_pimp;
		delete[] m_p;
		delete[] m_g1;
		delete[] m_g2;
		delete[] m_g3;
	}

	float noiseperlin::noise(const float &x)
	{
		float t = x + N;
		int bx0 = static_cast<int>(t) & BM;
		int bx1 = bx0 + 1 & BM;
		float rx0 = t - static_cast<int>(t);
		float rx1 = rx0 - 1.0f;

		float sx = sCurve(rx0);

		float u = rx0 * m_g1[m_p[bx0]];
		float v = rx1 * m_g1[m_p[bx1]];

		return lerp(sx, u, v);
	}

	float noiseperlin::noise(const float &x, const float &y)
	{
		float t = x + N;
		int bx0 = static_cast<int>(t) & BM;
		int bx1 = bx0 + 1 & BM;
		float rx0 = t - static_cast<int>(t);
		float rx1 = rx0 - 1.0f;

		t = y + N;
		int by0 = static_cast<int>(t) & BM;
		int by1 = by0 + 1 & BM;
		float ry0 = t - static_cast<int>(t);
		float ry1 = ry0 - 1.0f;

		int i = m_p[bx0];
		int j = m_p[bx1];

		int b00 = m_p[i + by0];
		int b10 = m_p[j + by0];
		int b01 = m_p[i + by1];
		int b11 = m_p[j + by1];

		float sx = sCurve(rx0);
		float sy = sCurve(ry0);

		float *q = m_g2[b00];
		float u = rx0 * q[0] + ry0 * q[1];
		q = m_g2[b10];
		float v = rx1 * q[0] + ry0 * q[1];
		float a = lerp(sx, u, v);

		q = m_g2[b01];
		u = rx0 * q[0] + ry1 * q[1];
		q = m_g2[b11];
		v = rx1 * q[0] + ry1 * q[1];
		float b = lerp(sx, u, v);

		return lerp(sy, a, b);
	}

	float noiseperlin::noise(const float &x, const float &y, const float &z)
	{
		float t = x + N;
		int bx0 = static_cast<int>(t) & BM;
		int bx1 = bx0 + 1 & BM;
		float rx0 = t - static_cast<int>(t);
		float rx1 = rx0 - 1.0f;

		t = y + N;
		int by0 = static_cast<int>(t) & BM;
		int by1 = by0 + 1 & BM;
		float ry0 = t - static_cast<int>(t);
		float ry1 = ry0 - 1.0f;

		t = z + N;
		int bz0 = static_cast<int>(t) & BM;
		int bz1 = bz0 + 1 & BM;
		float rz0 = t - static_cast<int>(t);
		float rz1 = rz0 - 1.0f;

		int i = m_p[bx0];
		int j = m_p[bx1];

		int b00 = m_p[i + by0];
		int b10 = m_p[j + by0];
		int b01 = m_p[i + by1];
		int b11 = m_p[j + by1];

		t = sCurve(rx0);
		float sy = sCurve(ry0);
		float sz = sCurve(rz0);

		float *q = m_g3[b00 + bz0];
		float u = rx0 * q[0] + ry0 * q[1] + rz0 * q[2];
		q = m_g3[b10 + bz0];
		float v = rx1 * q[0] + ry0 * q[1] + rz0 * q[2];
		float a = lerp(t, u, v);

		q = m_g3[b01 + bz0];
		u = rx0 * q[0] + ry1 * q[1] + rz0 * q[2];
		q = m_g3[b11 + bz0];
		v = rx1 * q[0] + ry1 * q[1] + rz0 * q[2];
		float b = lerp(t, u, v);

		float c = lerp(sy, a, b);

		q = m_g3[b00 + bz1];
		u = rx0 * q[0] + ry0 * q[1] + rz1 * q[2];
		q = m_g3[b10 + bz1];
		v = rx1 * q[0] + ry0 * q[1] + rz1 * q[2];
		a = lerp(t, u, v);

		q = m_g3[b01 + bz1];
		u = rx0 * q[0] + ry1 * q[1] + rz1 * q[2];
		q = m_g3[b11 + bz1];
		v = rx1 * q[0] + ry1 * q[1] + rz1 * q[2];
		b = lerp(t, u, v);

		float d = lerp(sy, a, b);

		return lerp(sz, c, d);
	}

	float noiseperlin::improvedNoise(const float &x, const float &y, const float &z)
	{
		// Constraint the point to a unit cube
		int uc_x = static_cast<int>(floor(x)) & 255;
		int uc_y = static_cast<int>(floor(y)) & 255;
		int uc_z = static_cast<int>(floor(z)) & 255;

		// Relative location of the point in the unit cube
		float xo = x - static_cast<float>(floor(x));
		float yo = y - static_cast<float>(floor(y));
		float zo = z - static_cast<float>(floor(z));

		// Fade curves for x, y and z
		float u = fade(xo);
		float v = fade(yo);
		float w = fade(zo);

		// Generate a hash for each coordinate to find out where in the cube it lies
		int a = m_pimp[uc_x] + uc_y;
		int aa = m_pimp[a] + uc_z;
		int ab = m_pimp[a + 1] + uc_z;

		int b = m_pimp[uc_x + 1] + uc_y;
		int ba = m_pimp[b] + uc_z;
		int bb = m_pimp[b + 1] + uc_z;

		// Blend results from the 8 corners based on the noise function
		float c1 = grad(m_pimp[aa], xo, yo, zo);
		float c2 = grad(m_pimp[ba], xo - 1.0f, yo, zo);
		float c3 = grad(m_pimp[ab], xo, yo - 1.0f, zo);
		float c4 = grad(m_pimp[bb], xo - 1.0f, yo - 1.0f, zo);
		float c5 = grad(m_pimp[aa + 1], xo, yo, zo - 1.0f);
		float c6 = grad(m_pimp[ba + 1], xo - 1.0f, yo, zo - 1.0f);
		float c7 = grad(m_pimp[ab + 1], xo, yo - 1.0f, zo - 1.0f);
		float c8 = grad(m_pimp[bb + 1], xo - 1.0f, yo - 1.0f, zo - 1.0f);

		return lerp(w, lerp(v, lerp(u, c1, c2), lerp(u, c3, c4)), lerp(v, lerp(u, c5, c6), lerp(u, c7, c8)));
	}

	float noiseperlin::tileableNoise(const float &x, const float &w)
	{
		return (noise(x) * (w - x) + noise(x - w) * x) / w;
	}

	float noiseperlin::tileableNoise(const float &x, const float &y, const float &w, const float &h)
	{
		return (noise(x, y) * (w - x) * (h - y) + 
			noise(x - w, y) * x * (h - y) + 
			noise(x, y - h) * (w - x) * y + 
			noise(x - w, y - h) * x * y) 
			/ (w * h);
	}

	float noiseperlin::tileableNoise(const float &x, const float &y, const float &z, const float &w, const float &h, const float &d)
	{
		return (noise(x, y, z) * (w - x) * (h - y) * (d - z) + 
			noise(x - w, y, z) * x * (h - y) * (d - z) + 
			noise(x, y - h, z) * (w - x) * y * (d - z) + 
			noise(x - w, y - h, z) * x * y * (d - z) + 
			noise(x, y, z - d) * (w - x) * (h - y) * z + 
			noise(x - w, y, z - d) * x * (h - y) * z + 
			noise(x, y - h, z - d) * (w - x) * y * z + 
			noise(x - w, y - h, z - d) * x * y * z ) 
			/ (w * h * d);
	}

	float noiseperlin::turbulence(const float &x, const float &freq)
	{
		float t = 0;
		float f = freq;

		do
		{
			t += noise(freq * x) / freq;
			f *= 0.5f;
		} while (f >= 1.0f);

		return t;
	}

	float noiseperlin::turbulence(const float &x, const float &y, const float &freq)
	{
		float t = 0.0f;
		float f = freq;

		do
		{
			t += noise(freq * x, freq * y) / freq;
			f *= 0.5f;
		} while (f >= 1.0f);

		return t;
	}

	float noiseperlin::turbulence(const float &x, const float &y, const float &z, const float &freq)
	{
		float t = 0.0f;
		float f = freq;

		do
		{
			t += noise(freq * x, freq * y, freq * z) / freq;
			f *= 0.5f;
		} while (f >= 1.0f);

		return t;
	}

	float noiseperlin::improvedTurbulence(const float &x, const float &y, const float &z, const float &loF, const float &hiF)
	{
		float px = x + 123.456f;
		float py = y;
		float pz = z;
		float t = 0.0f;

		for (float f = loF; f < hiF; f *= 2)
		{
			t += fabs(improvedNoise(px, py, pz)) / f;

			px *= 2.0f;
			py *= 2.0f;
			pz *= 2.0f;
		}

		return t - 0.3f;
	}

	float noiseperlin::tileableTurbulence(const float &x, const float &y, const float &w, const float &h, const float &freq)
	{
		float t = 0.0f;
		float f = freq;

		do
		{
			t += tileableNoise(freq * x, freq * y, w * freq, h * freq) / freq;
			f *= 0.5f;
		} while (f >= 1.0f);

		return t;
	}

	float noiseperlin::tileableTurbulence(const float &x, const float &y, const float &z, const float &w, const float &h, const float &d, const float &freq)
	{
		float t = 0.0f;
		float f = freq;

		do
		{
			t += tileableNoise(freq * x, freq * y, freq * z, w * freq, h * freq, d * freq) / freq;
			f *= 0.5f;
		} while (f >= 1.0f);

		return t;
	}

	float noiseperlin::fade(const float &t)
	{
		return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
	}

	float noiseperlin::grad(const int &hash, const float &x, const float &y, const float &z)
	{
		// Convert low 4 bits of hash code into 12 gradient directions
		int h = hash & 15;
		float u = h < 8.0f || h == 12.0f || h == 13.0f ? x : y;
		float v = h < 4.0f || h == 12.0f || h == 13.0f ? y : z;

		return ((h & 1) == 0.0f ? u : -u) + ((h & 2) == 0.0f ? v : -v);
	}

	float noiseperlin::sCurve(const float &t)
	{
		return t * t * (3.0f - 2.0f * t);
	}

	float noiseperlin::lerp(const float &t, const float &a, const float &b)
	{
		return a + t * (b - a);
	}

	float noiseperlin::bias(const float &a, const float &b)
	{
		return static_cast<float>(pow(a, log(b) / LOG_HALF));
	}

	float noiseperlin::gain(const float &a, const float &b)
	{
		if (a < 0.001f)
		{
			return 0;
		}
		else if (a > 0.999f)
		{
			return 1.0f;
		}

		float p = static_cast<float>(log(1.0f - b)) / LOG_HALF;

		if (a < 0.5f)
		{
			return static_cast<float>(pow(2.0f * a, p) / 2.0f);
		}
		else
		{
			return 1 - static_cast<float>(pow(2.0f * (1.0f - a), p) / 2.0f);
		}
	}
}
