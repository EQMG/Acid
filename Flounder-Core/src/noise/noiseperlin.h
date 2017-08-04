#pragma once

#include "../helpers/helperarray.h"
#include "../maths/maths.h"

namespace flounder
{
	/// <summary>
	/// Computes Perlin Noise for three dimensions.
	/// <p>
	/// The result is a continuous function that interpolates a smooth path along a series random points. The function is consistent, so given the same parameters, it will always return the same value.
	/// <p>
	/// Computing noise for one and two dimensions can make use of the 3D problem flounder.space by just setting the un-needed dimensions to a fixed value.
	/// </summary>
	class noiseperlin
	{
	private:
		static const int B;
		static const int BM;
		static const int N;
		static const int SAMPLE_SIZE;

		int *m_pimp;
		int *m_p;
		float *m_g1;
		float **m_g2;
		float **m_g3;

		int m_seed;
	public:
		noiseperlin(const int &seed);

		~noiseperlin();

		/// <summary>
		/// Create noise in a 1D space using the original noise noise algorithm.
		/// </summary>
		/// <param name="x"> The X coordinate of the location to sample.
		/// </param>
		/// <returns> A noisy value at the given position. </returns>
		float noise(const float &x);

		/// <summary>
		/// Create noise in a 2D space using the original noise noise algorithm.
		/// </summary>
		/// <param name="x"> The X coordinate of the location to sample. </param>
		/// <param name="y"> The Y coordinate of the location to sample.
		/// </param>
		/// <returns> A noisy value at the given position. </returns>
		float noise(const float &x, const float &y);

		/// <summary>
		/// Create noise in a 3D space using the original noise noise algorithm.
		/// </summary>
		/// <param name="x"> The X coordinate of the location to sample. </param>
		/// <param name="y"> The Y coordinate of the location to sample. </param>
		/// <param name="z"> The Z coordinate of the location to sample.
		/// </param>
		/// <returns> A noisy value at the given position. </returns>
		float noise(const float &x, const float &y, const float &z);

		/// <summary>
		/// Computes noise function for three dimensions at the point (x,y,z).
		/// </summary>
		/// <param name="x"> x dimension parameter. </param>
		/// <param name="y"> y dimension parameter. </param>
		/// <param name="z"> z dimension parameter.
		/// </param>
		/// <returns> the noise value at the point (x, y, z). </returns>
		float improvedNoise(const float &x, const float &y, const float &z);

		/// <summary>
		/// Create a 1D tileable noise function for the given width.
		/// </summary>
		/// <param name="x"> The X coordinate to generate the noise for. </param>
		/// <param name="w"> The width of the tiled block.
		/// </param>
		/// <returns> The value of the noise at the given coordinate. </returns>
		float tileableNoise(const float &x, const float &w);

		/// <summary>
		/// Create a 2D tileable noise function for the given width and height.
		/// </summary>
		/// <param name="x"> The X coordinate to generate the noise for. </param>
		/// <param name="y"> The Y coordinate to generate the noise for. </param>
		/// <param name="w"> The width of the tiled block. </param>
		/// <param name="h"> The height of the tiled block.
		/// </param>
		/// <returns> The value of the noise at the given coordinate. </returns>
		float tileableNoise(const float &x, const float &y, const float &w, const float &h);

		/// <summary>
		/// Create a 3D tileable noise function for the given width, height and depth.
		/// </summary>
		/// <param name="x"> The X coordinate to generate the noise for. </param>
		/// <param name="y"> The Y coordinate to generate the noise for. </param>
		/// <param name="z"> The Z coordinate to generate the noise for. </param>
		/// <param name="w"> The width of the tiled block. </param>
		/// <param name="h"> The height of the tiled block. </param>
		/// <param name="d"> The depth of the tiled block.
		/// </param>
		/// <returns> The value of the noise at the given coordinate. </returns>
		float tileableNoise(const float &x, const float &y, const float &z, const float &w, const float &h, const float &d);

		/// <summary>
		/// Create a turbulence function in 1D using the original noise noise function.
		/// </summary>
		/// <param name="x"> The X coordinate of the location to sample. </param>
		/// <param name="freq"> The frequency of the turbulence to create.
		/// </param>
		/// <returns> The value at the given coordinates. </returns>
		float turbulence(const float &x, const float &freq);

		/// <summary>
		/// Create a turbulence function in 2D using the original noise noise function.
		/// </summary>
		/// <param name="x"> The X coordinate of the location to sample. </param>
		/// <param name="y"> The Y coordinate of the location to sample. </param>
		/// <param name="freq"> The frequency of the turbulence to create.
		/// </param>
		/// <returns> The value at the given coordinates. </returns>
		float turbulence(const float &x, const float &y, const float &freq);

		/// <summary>
		/// Create a turbulence function in 3D using the original noise noise function.
		/// </summary>
		/// <param name="x"> The X coordinate of the location to sample. </param>
		/// <param name="y"> The Y coordinate of the location to sample. </param>
		/// <param name="z"> The Z coordinate of the location to sample. </param>
		/// <param name="freq"> The frequency of the turbulence to create.
		/// </param>
		/// <returns> The value at the given coordinates. </returns>
		float turbulence(const float &x, const float &y, const float &z, const float &freq);

		/// <summary>
		/// Create a turbulent noise output based on the core noise function. This uses the noise as a base function and is suitable for creating clouds, marble and explosion effects. For example, a typical marble effect would set the colour to be:
		/// <pre>
		/// sin(point + turbulence(point) * point.x);
		/// </pre>
		/// </summary>
		/// <param name="x"> The X coordinate of the location to sample. </param>
		/// <param name="y"> The Y coordinate of the location to sample. </param>
		/// <param name="z"> The Z coordinate of the location to sample. </param>
		/// <param name="loF"> The lower location. </param>
		/// <param name="hiF"> The upper location.
		/// </param>
		/// <returns> The value at the given coordinates. </returns>
		float improvedTurbulence(const float &x, const float &y, const float &z, const float &loF, const float &hiF);

		/// <summary>
		/// Create a turbulence function that can be tiled across a surface in 2D.
		/// </summary>
		/// <param name="x"> The X coordinate of the location to sample. </param>
		/// <param name="y"> The Y coordinate of the location to sample. </param>
		/// <param name="w"> The width to tile over. </param>
		/// <param name="h"> The height to tile over. </param>
		/// <param name="freq"> The frequency of the turbulence to create.
		/// </param>
		/// <returns> The value at the given coordinates. </returns>
		float tileableTurbulence(const float &x, const float &y, const float &w, const float &h, const float &freq);

		/// <summary>
		/// Create a turbulence function that can be tiled across a surface in 3D.
		/// </summary>
		/// <param name="x"> The X coordinate of the location to sample. </param>
		/// <param name="y"> The Y coordinate of the location to sample. </param>
		/// <param name="z"> The Z coordinate of the location to sample. </param>
		/// <param name="w"> The width to tile over. </param>
		/// <param name="h"> The height to tile over. </param>
		/// <param name="d"> The depth to tile over. </param>
		/// <param name="freq"> The frequency of the turbulence to create.
		/// </param>
		/// <returns> The value at the given coordinates. </returns>
		float tileableTurbulence(const float &x, const float &y, const float &z, const float &w, const float &h, const float &d, const float &freq);
	private:
		/// <summary>
		/// Fade curve calculation which is 6t^5 - 15t^4 + 10t^3. This is the new algorithm, where the old one used to be 3t^2 - 2t^3.
		/// </summary>
		/// <param name="t"> The t parameter to calculate the fade for.
		/// </param>
		/// <returns> the drop-off amount. </returns>
		float fade(const float &t);

		/// <summary>
		/// Calculate the gradient function based on the hash code.
		/// </summary>
		float grad(const int &hash, const float &x, const float &y, const float &z);

		/// <summary>
		/// S-curve function for value distribution for Perlin-1 noise function.
		/// </summary>
		float sCurve(const float &t);

		/// <summary>
		/// Simple lerp function using floats.
		/// </summary>
		float lerp(const float &t, const float &a, const float &b);

		/// <summary>
		/// Simple bias generator using exponents.
		/// </summary>
		float bias(const float &a, const float &b);

		/// <summary>
		/// Gain generator that caps to the range of [0, 1].
		/// </summary>
		float gain(const float &a, const float &b);
	public:
		inline int getSeed() const { return m_seed; }
	};
}
