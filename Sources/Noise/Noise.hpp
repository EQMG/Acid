//
// MIT License
//
// Copyright(c) 2017 Jordan Peck
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#pragma once

#include <cstdint>
#include <memory>
#include "Engine/Exports.hpp"

namespace acid
{
	enum NoiseType
	{
		NOISE_TYPE_VALUE = 0,
		NOISE_TYPE_VALUE_FRACTAL = 1,
		NOISE_TYPE_PERLIN = 2,
		NOISE_TYPE_PERLIN_FRACTAL = 3,
		NOISE_TYPE_SIMPLEX = 4,
		NOISE_TYPE_SIMPLEX_FRACTAL = 5,
		NOISE_TYPE_CELLULAR = 6,
		NOISE_TYPE_STATIC = 7,
		NOISE_TYPE_CUBIC = 8,
		NOISE_TYPE_CUBIC_FRACTAL = 9
	};

	enum NoiseInterp
	{
		NOISE_INTERP_LINEAR = 0,
		NOISE_INTERP_HERMITE = 1,
		NOISE_INTERP_QUINTIC = 2
	};

	enum NoiseFractal
	{
		NOISE_FRACTAL_FBM = 0,
		NOISE_FRACTAL_BILLOW = 1,
		NOISE_FRACTAL_RIGIDMULTI = 2
	};

	enum NoiseCellularFunc
	{
		NOISE_CELLULAR_EUCLIDEAN = 0,
		NOISE_CELLULAR_MANHATTAN = 1,
		NOISE_CELLULAR_NATURAL = 2
	};

	enum NoiseCellularReturn
	{
		NOISE_CELLULAR_CELLVALUE = 0,
		NOISE_CELLULAR_NOISELOOKUP = 1,
		NOISE_CELLULAR_DISTANCE = 2,
		NOISE_CELLULAR_DISTANCE2 = 3,
		NOISE_CELLULAR_DISTANCE2ADD = 4,
		NOISE_CELLULAR_DISTANCE2SUB = 5,
		NOISE_CELLULAR_DISTANCE2MUL = 6,
		NOISE_CELLULAR_DISTANCE2DIV = 7
	};

	class ACID_EXPORT Noise
	{
	private:
		int32_t m_seed;
		std::unique_ptr<uint8_t[]> m_perm;
		std::unique_ptr<uint8_t[]> m_perm12;

		float m_frequency;
		NoiseInterp m_interp;
		NoiseType m_noiseType;

		int32_t m_octaves;
		float m_lacunarity;
		float m_gain;
		NoiseFractal m_fractalType;
		float m_fractalBounding;

		NoiseCellularFunc m_cellularDistanceFunction;
		NoiseCellularReturn m_cellularReturnType;
		std::unique_ptr<Noise> m_cellularNoiseLookup;
		int32_t m_cellularDistanceIndex0;
		int32_t m_cellularDistanceIndex1;
		float m_cellularJitter;

		float m_gradientPerturbAmp;
	public:
		/// <summary>
		/// Creates a new multi-type noise object.
		/// </summary>
		/// <param name="seed"> The seed. </param>
		/// <param name="frequency"> The noise frequency. </param>
		/// <param name="interp"> The interp type. </param>
		/// <param name="noiseType"> The noise type. </param>
		/// <param name="octaves"> The amount of octaves to use. </param>
		/// <param name="lacunarity"> The lacunarity between octaves. </param>
		/// <param name="fractalType"> The fractal noise type. </param>
		/// <param name="fractalBounding"> The fractal bounding. </param>
		explicit Noise(const int32_t &seed = 1337, const float &frequency = 0.01f, const NoiseInterp &interp = NOISE_INTERP_QUINTIC, const NoiseType &noiseType = NOISE_TYPE_SIMPLEX, const int32_t &octaves = 3,
		      const float &lacunarity = 2.0f, const float &gain = 0.5f, const NoiseFractal &fractalType = NOISE_FRACTAL_FBM, const float &fractalBounding = 0.0f);

		/// <summary>
		/// Gets the seed used for all noise types.
		/// </summary>
		/// <returns> The seed. </returns>
		int32_t GetSeed() const { return m_seed; }

		/// <summary>
		/// Sets seed used for all noise types.
		/// </summary>
		/// <param name="seed"> The new seed (Default: 1337). </param>
		void SetSeed(const int32_t &seed);

		/// <summary>
		/// Gets the frequency used for all noise types.
		/// </summary>
		/// <returns> The frequency. </returns>
		float GetFrequency() const { return m_frequency; }

		/// <summary>
		/// Sets the frequency used for all noise types.
		/// </summary>
		/// <param name="frequency"> The new frequency (Default: 0.01). </param>
		void SetFrequency(const float &frequency) { m_frequency = frequency; }

		/// <summary>
		/// Gets the interpolation method used for supported noise types.
		/// </summary>
		/// <returns> The interpolation method. </returns>
		NoiseInterp GetInterp() const { return m_interp; }

		/// <summary>
		/// Sets the interpolation method used to smooth between noise values.
		/// </summary>
		/// <param name="interp"> The new interpolation method (Default: Quintic). </param>
		void SetInterp(const NoiseInterp &interp) { m_interp = interp; }

		/// <summary>
		/// Gets the type of noise.
		/// </summary>
		/// <returns> The type of noise (Default: Simplex). </returns>
		NoiseType GetNoiseType() const { return m_noiseType; }

		/// <summary>
		/// Sets the type of noise.
		/// </summary>
		/// <param name="noiseType"> The new noise type. </param>
		void SetNoiseType(const NoiseType &noiseType) { m_noiseType = noiseType; }

		/// <summary>
		/// Gets the octave count for all fractal noise types.
		/// </summary>
		/// <returns> The octave count (Default: 3). </returns>
		int32_t GetFractalOctaves() const { return m_octaves; }

		/// <summary>
		/// Sets the octave count for all fractal noise types.
		/// </summary>
		/// <param name="octaves"> The new octaves count (Default: 2.0). </param>
		void SetFractalOctaves(const int32_t &octaves);

		/// <summary>
		/// Gets the octave lacunarity for all fractal noise types.
		/// </summary>
		/// <returns> The octave lacunarity. </returns>
		float GetFractalLacunarity() const { return m_lacunarity; }

		/// <summary>
		/// Sets the octave octave lacunarity for all fractal noise types.
		/// </summary>
		/// <param name="lacunarity"> The new octave lacunarity (Default: 0.5). </param>
		void SetFractalLacunarity(const float &lacunarity) { m_lacunarity = lacunarity; }

		/// <summary>
		/// Gets the octave gain for all fractal noise types.
		/// </summary>
		/// <returns> The octave gain. </returns>
		float GetFractalGain() const { return m_gain; }

		/// <summary>
		/// Sets the octave gain for all fractal noise types.
		/// </summary>
		/// <param name="gain"> The new octave gain. </param>
		void SetFractalGain(const float &gain);

		/// <summary>
		/// Gets the method for combining octaves in all fractal noise types.
		/// </summary>
		/// <returns> The method for combining octaves. </returns>
		NoiseFractal GetFractalType() const { return m_fractalType; }

		/// <summary>
		/// Sets the method for combining octaves in all fractal noise types.
		/// </summary>
		/// <param name="fractalType"> The new method for combining octaves (Default: FBM). </param>
		void SetFractalType(const NoiseFractal &fractalType) { m_fractalType = fractalType; }

		/// <summary>
		/// Gets the distance function used in cellular noise calculations.
		/// </summary>
		/// <returns> The cellular distance function. </returns>
		NoiseCellularFunc GetCellularDistanceFunction() const { return m_cellularDistanceFunction; }

		/// <summary>
		/// Sets the distance function used in cellular noise calculations.
		/// </summary>
		/// <param name="cellularDistanceFunction"> The new cellular distance function (Default: Euclidean). </param>
		void SetCellularDistanceFunction(const NoiseCellularFunc &cellularDistanceFunction) { m_cellularDistanceFunction = cellularDistanceFunction; }

		/// <summary>
		/// Gets the return type from cellular noise calculations.
		/// </summary>
		/// <returns> The cellular return type. </returns>
		NoiseCellularReturn GetCellularReturnType() const { return m_cellularReturnType; }

		/// <summary>
		/// Sets the return type from cellular noise calculations.
		/// Note: NoiseLookup requires another FastNoise object be set with SetCellularNoiseLookup() to function.
		/// </summary>
		/// <param name="cellularReturnType"> The new cellular return type (Default: CellValue). </param>
		void SetCellularReturnType(const NoiseCellularReturn &cellularReturnType) { m_cellularReturnType = cellularReturnType; }

		/// <summary>
		/// Gets the noise used to calculate a cell value if the cellular return type is NoiseLookup.
		/// </summary>
		/// <returns> The cellular noise lookup. </returns>
		Noise *GetCellularNoiseLookup() const { return m_cellularNoiseLookup.get(); }

		/// <summary>
		/// Sets the noise used to calculate a cell value if cellular return type is NoiseLookup.
		/// The lookup value is acquired through GetNoise() so ensure you SetNoiseType() on the noise lookup, value, Perlin or simplex is recommended.
		/// </summary>
		/// <param name="noise"> The new cellular noise lookup. </param>
		void SetCellularNoiseLookup(Noise *noise) { m_cellularNoiseLookup.reset(noise); }

		/// <summary>
		/// Gets the 2 distance indices used for distance2 return types.
		/// </summary>
		/// <param name="cellularDistanceIndex0"> The cellular distance at index 0. </param>
		/// <param name="cellularDistanceIndex1"> The cellular distance at index 1. </param>
		void GetCellularDistance2Indices(int32_t &cellularDistanceIndex0, int32_t &cellularDistanceIndex1) const;

		/// <summary>
		/// Sets the 2 distance indices used for distance2 return types.
		/// Both indices must be >= 0, index1 must be < 4.
		/// </summary>
		/// <param name="cellularDistanceIndex0"> The new cellular distance at index 0 (Default: 0). </param>
		/// <param name="cellularDistanceIndex1"> The new cellular distance at index 1 (Default: 1). </param>
		void SetCellularDistance2Indices(const int32_t &cellularDistanceIndex0, const int32_t &cellularDistanceIndex1);

		/// <summary>
		/// Gets the maximum distance a cellular point can move from its grid position.
		/// </summary>
		/// <returns> The cellular maximum distance. </returns>
		float GetCellularJitter() const { return m_cellularJitter; }

		/// <summary>
		/// Sets the maximum distance a cellular point can move from its grid position.
		/// Setting this high will make artifacts more common.
		/// </summary>
		/// <param name="cellularJitter"> The new cellular maximum distance (Default: 0.45). </param>
		void SetCellularJitter(const float &cellularJitter) { m_cellularJitter = cellularJitter; }

		/// <summary>
		/// Gets the maximum warp distance from original location when using GradientPerturb{Fractal}(...).
		/// </summary>
		/// <returns> The cellular maximum warp distance. </returns>
		float GetGradientPerturbAmp() const { return m_gradientPerturbAmp; }

		/// <summary>
		/// Sets the maximum warp distance from original location when using GradientPerturb{Fractal}(...).
		/// </summary>
		/// <param name="gradientPerturbAmp"> The new cellular maximum warp distance (Default: 1.0). </param>
		void SetGradientPerturbAmp(const float &gradientPerturbAmp) { m_gradientPerturbAmp = gradientPerturbAmp; }

		//2D
		float GetValue(float x, float y) const;

		float GetValueFractal(float x, float y) const;

		float GetPerlin(float x, float y) const;

		float GetPerlinFractal(float x, float y) const;

		float GetSimplex(float x, float y) const;

		float GetSimplexFractal(float x, float y) const;

		float GetCellular(float x, float y) const;

		float GetWhiteNoise(float x, float y) const;

		float GetWhiteNoiseInt(int32_t x, int32_t y) const;

		float GetCubic(float x, float y) const;

		float GetCubicFractal(float x, float y) const;

		float GetNoise(float x, float y) const;

		void GradientPerturb(float &x, float &y) const;

		void GradientPerturbFractal(float &x, float &y) const;

		//3D
		float GetValue(float x, float y, float z) const;

		float GetValueFractal(float x, float y, float z) const;

		float GetPerlin(float x, float y, float z) const;

		float GetPerlinFractal(float x, float y, float z) const;

		float GetSimplex(float x, float y, float z) const;

		float GetSimplexFractal(float x, float y, float z) const;

		float GetCellular(float x, float y, float z) const;

		float GetWhiteNoise(float x, float y, float z) const;

		float GetWhiteNoiseInt(int32_t x, int32_t y, int32_t z) const;

		float GetCubic(float x, float y, float z) const;

		float GetCubicFractal(float x, float y, float z) const;

		float GetNoise(float x, float y, float z) const;

		void GradientPerturb(float &x, float &y, float &z) const;

		void GradientPerturbFractal(float &x, float &y, float &z) const;

		//4D
		float GetSimplex(float x, float y, float z, float w) const;

		float GetWhiteNoise(float x, float y, float z, float w) const;

		float GetWhiteNoiseInt(int32_t x, int32_t y, int32_t z, int32_t w) const;

	private:
		void CalculateFractalBounding();

		// Helpers
		static int32_t FastFloor(const float &f);

		static int32_t FastRound(const float &f);

		static float Lerp(const float &a, const float &b, const float &t);

		static float InterpHermite(const float &t);

		static float InterpQuintic(const float &t);

		static float CubicLerp(const float &a, const float &b, const float &c, const float &d, const float &t);

		uint8_t Index2d12(const uint8_t &offset, const int32_t &x, const int32_t &y) const;

		uint8_t Index3d12(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z) const;

		uint8_t Index4d32(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z, const int32_t &w) const;

		uint8_t Index2d256(const uint8_t &offset, const int32_t &x, const int32_t &y) const;

		uint8_t Index3d256(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z) const;

		uint8_t Index4d256(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z, const int32_t &w) const;

		static float ValueCoord2d(const int32_t &seed, const int32_t &x, const int32_t &y);

		static float ValueCoord3d(const int32_t &seed, const int32_t &x, const int32_t &y, const int32_t &z);

		static float ValueCoord4d(const int32_t &seed, const int32_t &x, const int32_t &y, const int32_t &z, const int32_t &w);

		float ValueCoord2dFast(const uint8_t &offset, const int32_t &x, const int32_t &y) const;

		float ValueCoord3dFast(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z) const;

		float GradCoord2d(const uint8_t &offset, const int32_t &x, const int32_t &y, const float &xd, const float &yd) const;

		float GradCoord3d(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z, const float &xd, const float &yd, const float &zd) const;

		float GradCoord4d(const uint8_t &offset, const int32_t &x, const int32_t &y, const int32_t &z, const int32_t &w, const float &xd, const float &yd, const float &zd, const float &wd) const;

		// 2D
		float SingleValueFractalFbm(float x, float y) const;

		float SingleValueFractalBillow(float x, float y) const;

		float SingleValueFractalRigidMulti(float x, float y) const;

		float SingleValue(const uint8_t &offset, const float &x, const float &y) const;

		float SinglePerlinFractalFbm(float x, float y) const;

		float SinglePerlinFractalBillow(float x, float y) const;

		float SinglePerlinFractalRigidMulti(float x, float y) const;

		float SinglePerlin(const uint8_t &offset, const float &x, const float &y) const;

		float SingleSimplexFractalFbm(float x, float y) const;

		float SingleSimplexFractalBillow(float x, float y) const;

		float SingleSimplexFractalRigidMulti(float x, float y) const;

		float singleSimplexFractalBlend(float x, float y) const;

		float SingleSimplex(const uint8_t &offset, const float &x, const float &y) const;

		float SingleCubicFractalFbm(float x, float y) const;

		float SingleCubicFractalBillow(float x, float y) const;

		float SingleCubicFractalRigidMulti(float x, float y) const;

		float SingleCubic(const uint8_t &offset, const float &x, const float &y) const;

		float SingleCellular(const float &x, const float &y) const;

		float SingleCellular2Edge(const float &x, const float &y) const;

		void SingleGradientPerturb(const uint8_t &offset, const float &warpAmp, const float &frequency, float x, float y) const;

		//3D
		float SingleValueFractalFbm(float x, float y, float z) const;

		float SingleValueFractalBillow(float x, float y, float z) const;

		float SingleValueFractalRigidMulti(float x, float y, float z) const;

		float SingleValue(const uint8_t &offset, const float &x, const float &y, const float &z) const;

		float SinglePerlinFractalFbm(float x, float y, float z) const;

		float SinglePerlinFractalBillow(float x, float y, float z) const;

		float SinglePerlinFractalRigidMulti(float x, float y, float z) const;

		float SinglePerlin(const uint8_t &offset, const float &x, const float &y, const float &z) const;

		float SingleSimplexFractalFbm(float x, float y, float z) const;

		float SingleSimplexFractalBillow(float x, float y, float z) const;

		float SingleSimplexFractalRigidMulti(float x, float y, float z) const;

		float SingleSimplex(const uint8_t &offset, const float &x, const float &y, const float &z) const;

		float SingleCubicFractalFbm(float x, float y, float z) const;

		float SingleCubicFractalBillow(float x, float y, float z) const;

		float SingleCubicFractalRigidMulti(float x, float y, float z) const;

		float SingleCubic(const uint8_t &offset, const float &x, const float &y, const float &z) const;

		float SingleCellular(const float &x, const float &y, const float &z) const;

		float SingleCellular2Edge(const float &x, const float &y, const float &z) const;

		void SingleGradientPerturb(const uint8_t &offset, const float &warpAmp, const float &frequency, float x, float y, float z) const;

		//4D
		float SingleSimplex(const uint8_t &offset, const float &x, const float &y, const float &z, const float &w) const;
	};
}
