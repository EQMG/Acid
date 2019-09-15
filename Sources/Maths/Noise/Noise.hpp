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

#include "StdAfx.hpp"

namespace acid {
/**
 * @brief Class that can generate 2D, 3D and 4D noise values.
 */
class ACID_EXPORT
	Noise {
public:
	enum class Type {
		Value,
		ValueFractal,
		Perlin,
		PerlinFractal,
		Simplex,
		SimplexFractal,
		Cellular,
		WhiteNoise,
		Cubic,
		CubicFractal
	};

	enum class Interp {
		Linear,
		Hermite,
		Quintic
	};

	enum class Fractal {
		FBM,
		Billow,
		RigidMulti
	};

	enum class CellularDistance {
		Euclidean,
		Manhattan,
		Natural
	};

	enum class CellularReturn {
		CellValue,
		NoiseLookup,
		Distance,
		Distance2,
		Distance2Add,
		Distance2Sub,
		Distance2Mul,
		Distance2Div
	};

	/**
	 * Creates a new multi-type noise object.
	 * @param seed The seed.
	 * @param frequency The noise frequency.
	 * @param interp The interp type.
	 * @param type The noise type.
	 * @param octaves The amount of octaves to use.
	 * @param lacunarity The lacunarity between octaves.
	 * @param gain The octave gain.
	 * @param fractal The fractal noise type.
	 * @param fractalBounding The fractal bounding.
	 **/
	explicit Noise(uint64_t seed = 1337, float frequency = 0.01f, Interp interp = Interp::Quintic, Type type = Type::Simplex, int32_t octaves = 3,
		float lacunarity = 2.0f, float gain = 0.5f, Fractal fractal = Fractal::FBM, float fractalBounding = 0.0f);

	/**
	 * Gets the seed used for all noise types.
	 * @return The seed.
	 **/
	uint64_t GetSeed() const { return m_seed; }

	/**
	 * Sets seed used for all noise types.
	 * @param seed The new seed (Default: 1337).
	 **/
	void SetSeed(uint64_t seed);

	/**
	 * Gets the frequency used for all noise types.
	 * @return The frequency.
	 **/
	float GetFrequency() const { return m_frequency; }

	/**
	 * Sets the frequency used for all noise types.
	 * @param frequency The new frequency (Default: 0.01).
	 **/
	void SetFrequency(float frequency) { m_frequency = frequency; }

	/**
	 * Gets the interpolation method used for supported noise types.
	 * @return The interpolation method.
	 **/
	Interp GetInterp() const { return m_interp; }

	/**
	 * Sets the interpolation method used to smooth between noise values.
	 * @param interp The new interpolation method (Default: Quintic).
	 **/
	void SetInterp(Interp interp) { m_interp = interp; }

	/**
	 * Gets the type of noise.
	 * @return The type of noise (Default: Simplex).
	 **/
	Type GetType() const { return m_type; }

	/**
	 * Sets the type of noise.
	 * @param type The new noise type.
	 **/
	void SetType(Type type) { m_type = type; }

	/**
	 * Gets the octave count for all fractal noise types.
	 * @return The octave count (Default: 3).
	 **/
	int32_t GetFractalOctaves() const { return m_octaves; }

	/**
	 * Sets the octave count for all fractal noise types.
	 * @param octaves The new octaves count (Default: 2.0).
	 **/
	void SetFractalOctaves(int32_t octaves);

	/**
	 * Gets the octave lacunarity for all fractal noise types.
	 * @return The octave lacunarity.
	 **/
	float GetFractalLacunarity() const { return m_lacunarity; }

	/**
	 * Sets the octave octave lacunarity for all fractal noise types.
	 * @param lacunarity The new octave lacunarity (Default: 0.5).
	 **/
	void SetFractalLacunarity(float lacunarity) { m_lacunarity = lacunarity; }

	/**
	 * Gets the octave gain for all fractal noise types.
	 * @return The octave gain.
	 **/
	float GetFractalGain() const { return m_gain; }

	/**
	 * Sets the octave gain for all fractal noise types.
	 * @param gain The new octave gain.
	 **/
	void SetFractalGain(float gain);

	/**
	 * Gets the method for combining octaves in all fractal noise types.
	 * @return The method for combining octaves.
	 **/
	Fractal GetFractal() const { return m_fractal; }

	/**
	 * Sets the method for combining octaves in all fractal noise types.
	 * @param fractal The new method for combining octaves (Default: FBM).
	 **/
	void SetFractal(Fractal fractal) { m_fractal = fractal; }

	/**
	 * Gets the distance function used in cellular noise calculations.
	 * @return The cellular distance function.
	 **/
	CellularDistance GetCellularDistance() const { return m_cellularDistance; }

	/**
	 * Sets the distance function used in cellular noise calculations.
	 * @param cellularDistance'> The new cellular distance function (Default: Euclidean).
	 **/
	void SetCellularDistance(CellularDistance cellularDistance) { m_cellularDistance = cellularDistance; }

	/**
	 * Gets the return type from cellular noise calculations.
	 * @return The cellular return type.
	 **/
	CellularReturn GetCellularReturn() const { return m_cellularReturn; }

	/**
	 * Sets the return type from cellular noise calculations.
	 * Note: NoiseLookup requires another FastNoise object be set with SetCellularNoiseLookup() to function.
	 * @param cellularReturn> The new cellular return type (Default: CellValue).
	 **/
	void SetCellularReturn(CellularReturn cellularReturn) { m_cellularReturn = cellularReturn; }

	/**
	 * Gets the noise used to calculate a cell value if the cellular return type is NoiseLookup.
	 * @return The cellular noise lookup.
	 **/
	Noise *GetCellularNoiseLookup() const { return m_cellularNoiseLookup.get(); }

	/**
	 * Sets the noise used to calculate a cell value if cellular return type is NoiseLookup.
	 * The lookup value is acquired through GetNoise() so ensure you SetType() on the noise lookup, value, Perlin or simplex is recommended.
	 * @param noise The new cellular noise lookup.
	 **/
	void SetCellularNoiseLookup(std::unique_ptr<Noise> &&noise) { m_cellularNoiseLookup = std::move(noise); }

	/**
	 * Gets the 2 distance indices used for distance2 return types.
	 * @param cellularDistanceIndex0 The cellular distance at index 0.
	 * @param cellularDistanceIndex1 The cellular distance at index 1.
	 **/
	void GetCellularDistance2Indices(int32_t &cellularDistanceIndex0, int32_t &cellularDistanceIndex1) const;

	/**
	 * Sets the 2 distance indices used for distance2 return types.
	 * Both indices must be >= 0, index1 must be < 4.
	 * @param cellularDistanceIndex0 The new cellular distance at index 0 (Default: 0).
	 * @param cellularDistanceIndex1 The new cellular distance at index 1 (Default: 1).
	 **/
	void SetCellularDistance2Indices(int32_t cellularDistanceIndex0, int32_t cellularDistanceIndex1);

	/**
	 * Gets the maximum distance a cellular point can move from its grid position.
	 * @return The cellular maximum distance.
	 **/
	float GetCellularJitter() const { return m_cellularJitter; }

	/**
	 * Sets the maximum distance a cellular point can move from its grid position.
	 * Setting this high will make artifacts more common.
	 * @param cellularJitter The new cellular maximum distance (Default: 0.45).
	 **/
	void SetCellularJitter(float cellularJitter) { m_cellularJitter = cellularJitter; }

	/**
	 * Gets the maximum warp distance from original location when using GradientPerturb(Fractal}(...).
	 * @return The cellular maximum warp distance.
	 **/
	float GetGradientPerturbAmp() const { return m_gradientPerturbAmp; }

	/**
	 * Sets the maximum warp distance from original location when using GradientPerturb(Fractal}(...).
	 * @param gradientPerturbAmp The new cellular maximum warp distance (Default: 1.0).
	 **/
	void SetGradientPerturbAmp(float gradientPerturbAmp) { m_gradientPerturbAmp = gradientPerturbAmp; }

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
	static int32_t FastFloor(float f);
	static int32_t FastRound(float f);
	static float Lerp(float a, float b, float t);
	static float InterpHermite(float t);
	static float InterpQuintic(float t);
	static float CubicLerp(float a, float b, float c, float d, float t);

	uint8_t Index2d12(uint8_t offset, int32_t x, int32_t y) const;
	uint8_t Index3d12(uint8_t offset, int32_t x, int32_t y, int32_t z) const;
	uint8_t Index4d32(uint8_t offset, int32_t x, int32_t y, int32_t z, int32_t w) const;
	uint8_t Index2d256(uint8_t offset, int32_t x, int32_t y) const;
	uint8_t Index3d256(uint8_t offset, int32_t x, int32_t y, int32_t z) const;
	uint8_t Index4d256(uint8_t offset, int32_t x, int32_t y, int32_t z, int32_t w) const;

	static float ValueCoord2d(uint64_t seed, int32_t x, int32_t y);
	static float ValueCoord3d(uint64_t seed, int32_t x, int32_t y, int32_t z);
	static float ValueCoord4d(uint64_t seed, int32_t x, int32_t y, int32_t z, int32_t w);
	float ValueCoord2dFast(uint8_t offset, int32_t x, int32_t y) const;
	float ValueCoord3dFast(uint8_t offset, int32_t x, int32_t y, int32_t z) const;
	float GradCoord2d(uint8_t offset, int32_t x, int32_t y, float xd, float yd) const;
	float GradCoord3d(uint8_t offset, int32_t x, int32_t y, int32_t z, float xd, float yd, float zd) const;
	float GradCoord4d(uint8_t offset, int32_t x, int32_t y, int32_t z, int32_t w, float xd, float yd, float zd,
		float wd) const;

	// 2D
	float SingleValueFractalFbm(float x, float y) const;
	float SingleValueFractalBillow(float x, float y) const;
	float SingleValueFractalRigidMulti(float x, float y) const;
	float SingleValue(uint8_t offset, float x, float y) const;
	float SinglePerlinFractalFbm(float x, float y) const;
	float SinglePerlinFractalBillow(float x, float y) const;
	float SinglePerlinFractalRigidMulti(float x, float y) const;
	float SinglePerlin(uint8_t offset, float x, float y) const;
	float SingleSimplexFractalFbm(float x, float y) const;
	float SingleSimplexFractalBillow(float x, float y) const;
	float SingleSimplexFractalRigidMulti(float x, float y) const;
	float SingleSimplexFractalBlend(float x, float y) const;
	float SingleSimplex(uint8_t offset, float x, float y) const;
	float SingleCubicFractalFbm(float x, float y) const;
	float SingleCubicFractalBillow(float x, float y) const;
	float SingleCubicFractalRigidMulti(float x, float y) const;
	float SingleCubic(uint8_t offset, float x, float y) const;
	float SingleCellular(float x, float y) const;
	float SingleCellular2Edge(float x, float y) const;
	void SingleGradientPerturb(uint8_t offset, float warpAmp, float frequency, float x, float y) const;

	//3D
	float SingleValueFractalFbm(float x, float y, float z) const;
	float SingleValueFractalBillow(float x, float y, float z) const;
	float SingleValueFractalRigidMulti(float x, float y, float z) const;
	float SingleValue(uint8_t offset, float x, float y, float z) const;
	float SinglePerlinFractalFbm(float x, float y, float z) const;
	float SinglePerlinFractalBillow(float x, float y, float z) const;
	float SinglePerlinFractalRigidMulti(float x, float y, float z) const;
	float SinglePerlin(uint8_t offset, float x, float y, float z) const;
	float SingleSimplexFractalFbm(float x, float y, float z) const;
	float SingleSimplexFractalBillow(float x, float y, float z) const;
	float SingleSimplexFractalRigidMulti(float x, float y, float z) const;
	float SingleSimplex(uint8_t offset, float x, float y, float z) const;
	float SingleCubicFractalFbm(float x, float y, float z) const;
	float SingleCubicFractalBillow(float x, float y, float z) const;
	float SingleCubicFractalRigidMulti(float x, float y, float z) const;
	float SingleCubic(uint8_t offset, float x, float y, float z) const;
	float SingleCellular(float x, float y, float z) const;
	float SingleCellular2Edge(float x, float y, float z) const;
	void SingleGradientPerturb(uint8_t offset, float warpAmp, float frequency, float x, float y, float z) const;

	//4D
	float SingleSimplex(uint8_t offset, float x, float y, float z, float w) const;

	uint64_t m_seed;
	std::unique_ptr<uint8_t[]> m_perm;
	std::unique_ptr<uint8_t[]> m_perm12;

	float m_frequency;
	Interp m_interp;
	Type m_type;

	int32_t m_octaves;
	float m_lacunarity;
	float m_gain;
	Fractal m_fractal;
	float m_fractalBounding;

	CellularDistance m_cellularDistance;
	CellularReturn m_cellularReturn;
	std::unique_ptr<Noise> m_cellularNoiseLookup;
	int32_t m_cellularDistanceIndex0;
	int32_t m_cellularDistanceIndex1;
	float m_cellularJitter;

	float m_gradientPerturbAmp;
};
}
