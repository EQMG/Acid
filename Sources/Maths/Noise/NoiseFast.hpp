#pragma once

#include "Engine/Exports.hpp"

namespace fl
{
	class FL_EXPORT NoiseFast
	{
	public:
// Hashing
#define X_PRIME 1619
#define Y_PRIME 31337
#define Z_PRIME 6971
#define W_PRIME 1013

// Cellular
#define FN_CELLULAR_INDEX_MAX 3

		enum TypeNoise
		{
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

		enum TypeInterp
		{
			Linear,
			Hermite,
			Quintic
		};

		enum TypeFractal
		{
			Fbm,
			Billow,
			RigidMulti
		};

		enum TypeCellularFunction
		{
			Euclidean,
			Manhattan,
			Natural
		};

		enum TypeCellularReturn
		{
			CellValue,
			NoiseLookup,
			Distance,
			Distance2,
			Distance2Add,
			Distance2Sub,
			Distance2Mul,
			Distance2Div
		};

	private:
		static const float GRAD_X[];
		static const float GRAD_Y[];
		static const float GRAD_Z[];

		static const float GRAD_4D[];

		static const float VAL_LUT[];

		static const float F3;
		static const float G3;

		static const float F2;
		static const float G2;

		static const unsigned char SIMPLEX_4D[];
		static const float F4;
		static const float G4;

		static const float CUBIC_2D_BOUNDING;
		static const float CUBIC_3D_BOUNDING;

		static const float CELL_2D_X[];
		static const float CELL_2D_Y[];
		static const float CELL_3D_X[];
		static const float CELL_3D_Y[];
		static const float CELL_3D_Z[];

		int m_seed;
		unsigned char *m_perm;
		unsigned char *m_perm12;

		float m_frequency;
		TypeInterp m_interp;
		TypeNoise m_noiseType;

		int m_octaves;
		float m_lacunarity;
		float m_gain;
		TypeFractal m_fractalType;
		float m_fractalBounding;

		TypeCellularFunction m_cellularDistanceFunction;
		TypeCellularReturn m_cellularReturnType;
		NoiseFast *m_cellularNoiseLookup;
		int m_cellularDistanceIndex0;
		int m_cellularDistanceIndex1;
		float m_cellularJitter;

		float m_gradientPerturbAmp;
	public:
		NoiseFast(const int &seed);

		~NoiseFast();

		// Returns seed used for all noise types
		int GetSeed() const { return m_seed; }

		// Sets seed used for all noise types
		// Default: 1337
		void SetSeed(const int &seed);

		// Returns frequency used for all noise types
		float GetFrequency() const { return m_frequency; }

		// Sets frequency for all noise types
		// Default: 0.01
		void SetFrequency(const float &frequency) { m_frequency = frequency; }

		// Returns interpolation method used for supported noise types
		TypeInterp GetInterp() const { return m_interp; }

		// Changes the interpolation method used to smooth between noise values
		// Possible interpolation methods (lowest to highest quality) :
		// - Linear
		// - Hermite
		// - Quintic
		// Used in Value, Perlin Noise and Position Warping
		// Default: Quintic
		void SetInterp(const TypeInterp &interp) { m_interp = interp; }

		// Returns the noise type used by GetNoise
		TypeNoise GetNoiseType() const { return m_noiseType; }

		// Sets noise return type of GetNoise(...)
		// Default: Simplex
		void SetNoiseType(const TypeNoise &noiseType) { m_noiseType = noiseType; }

		// Returns octave count for all fractal noise types
		int GetFractalOctaves() const { return m_octaves; }

		// Sets octave count for all fractal noise types
		// Default: 3
		void SetFractalOctaves(const int &octaves);

		// Returns octave lacunarity for all fractal noise types
		float GetFractalLacunarity() const { return m_lacunarity; }

		// Sets octave lacunarity for all fractal noise types
		// Default: 2.0
		void SetFractalLacunarity(const float &lacunarity) { m_lacunarity = lacunarity; }

		// Returns octave gain for all fractal noise types
		float GetFractalGain() const { return m_gain; }

		// Sets octave gain for all fractal noise types
		// Default: 0.5
		void SetFractalGain(const float &gain);

		// Returns method for combining octaves in all fractal noise types
		TypeFractal GetFractalType() const { return m_fractalType; }

		// Sets method for combining octaves in all fractal noise types
		// Default: FBM
		void SetFractalType(const TypeFractal &fractalType) { m_fractalType = fractalType; }

		// Returns the distance function used in cellular noise calculations
		TypeCellularFunction GetCellularDistanceFunction() const { return m_cellularDistanceFunction; }

		// Sets distance function used in cellular noise calculations
		// Default: Euclidean
		void SetCellularDistanceFunction(const TypeCellularFunction &cellularDistanceFunction) { m_cellularDistanceFunction = cellularDistanceFunction; }

		// Returns the return type from cellular noise calculations
		TypeCellularReturn GetCellularReturnType() const { return m_cellularReturnType; }

		// Sets return type from cellular noise calculations
		// Note: NoiseLookup requires another FastNoise object be set with SetCellularNoiseLookup() to function
		// Default: CellValue
		void SetCellularReturnType(const TypeCellularReturn &cellularReturnType) { m_cellularReturnType = cellularReturnType; }

		// Returns the noise used to calculate a cell value if the cellular return type is NoiseLookup
		NoiseFast *GetCellularNoiseLookup() const { return m_cellularNoiseLookup; }

		// Noise used to calculate a cell value if cellular return type is NoiseLookup
		// The lookup value is acquired through GetNoise() so ensure you SetNoiseType() on the noise lookup, value, Perlin or simplex is recommended
		void SetCellularNoiseLookup(NoiseFast *noise) { m_cellularNoiseLookup = noise; }

		// Returns the 2 distance indices used for distance2 return types
		void GetCellularDistance2Indices(int &cellularDistanceIndex0, int &cellularDistanceIndex1) const;

		// Sets the 2 distance indices used for distance2 return types
		// Default: 0, 1
		// Note: index0 should be lower than index1
		// Both indices must be >= 0, index1 must be < 4
		void SetCellularDistance2Indices(const int &cellularDistanceIndex0, const int &cellularDistanceIndex1);

		// Returns the maximum distance a cellular point can move from its grid position
		float GetCellularJitter() const { return m_cellularJitter; }

		// Sets the maximum distance a cellular point can move from its grid position
		// Setting this high will make artifacts more common
		// Default: 0.45
		void SetCellularJitter(const float &cellularJitter) { m_cellularJitter = cellularJitter; }

		// Returns the maximum warp distance from original location when using GradientPerturb{Fractal}(...)
		float GetGradientPerturbAmp() const { return m_gradientPerturbAmp; }

		// Sets the maximum warp distance from original location when using GradientPerturb{Fractal}(...)
		// Default: 1.0
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

		float GetWhiteNoiseInt(int x, int y) const;

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

		float GetWhiteNoiseInt(int x, int y, int z) const;

		float GetCubic(float x, float y, float z) const;

		float GetCubicFractal(float x, float y, float z) const;

		float GetNoise(float x, float y, float z) const;

		void GradientPerturb(float &x, float &y, float &z) const;

		void GradientPerturbFractal(float &x, float &y, float &z) const;

		//4D
		float GetSimplex(float x, float y, float z, float w) const;

		float GetWhiteNoise(float x, float y, float z, float w) const;

		float GetWhiteNoiseInt(int x, int y, int z, int w) const;

	private:
		void CalculateFractalBounding();

		// Helpers
		static int FastFloor(const float &f);

		static int FastRound(const float &f);

		static float Lerp(const float &a, const float &b, const float &t);

		static float InterpHermite(const float &t);

		static float InterpQuintic(const float &t);

		static float CubicLerp(const float &a, const float &b, const float &c, const float &d, const float &t);

		unsigned char Index2d12(const unsigned char &offset, const int &x, const int &y) const;

		unsigned char Index3d12(const unsigned char &offset, const int &x, const int &y, const int &z) const;

		unsigned char Index4d32(const unsigned char &offset, const int &x, const int &y, const int &z, const int &w) const;

		unsigned char Index2d256(const unsigned char &offset, const int &x, const int &y) const;

		unsigned char Index3d256(const unsigned char &offset, const int &x, const int &y, const int &z) const;

		unsigned char Index4d256(const unsigned char &offset, const int &x, const int &y, const int &z, const int &w) const;

		static float ValueCoord2d(const int &seed, const int &x, const int &y);

		static float ValueCoord3d(const int &seed, const int &x, const int &y, const int &z);

		static float ValueCoord4d(const int &seed, const int &x, const int &y, const int &z, const int &w);

		float ValueCoord2dFast(const unsigned char &offset, const int &x, const int &y) const;

		float ValueCoord3dFast(const unsigned char &offset, const int &x, const int &y, const int &z) const;

		float GradCoord2d(const unsigned char &offset, const int &x, const int &y, const float &xd, const float &yd) const;

		float GradCoord3d(const unsigned char &offset, const int &x, const int &y, const int &z, const float &xd, const float &yd, const float &zd) const;

		float GradCoord4d(const unsigned char &offset, const int &x, const int &y, const int &z, const int &w, const float &xd, const float &yd, const float &zd, const float &wd) const;

		// 2D
		float SingleValueFractalFbm(float x, float y) const;

		float SingleValueFractalBillow(float x, float y) const;

		float SingleValueFractalRigidMulti(float x, float y) const;

		float SingleValue(const unsigned char &offset, const float &x, const float &y) const;

		float SinglePerlinFractalFbm(float x, float y) const;

		float SinglePerlinFractalBillow(float x, float y) const;

		float SinglePerlinFractalRigidMulti(float x, float y) const;

		float SinglePerlin(const unsigned char &offset, const float &x, const float &y) const;

		float SingleSimplexFractalFbm(float x, float y) const;

		float SingleSimplexFractalBillow(float x, float y) const;

		float SingleSimplexFractalRigidMulti(float x, float y) const;

		float singleSimplexFractalBlend(float x, float y) const;

		float SingleSimplex(const unsigned char &offset, const float &x, const float &y) const;

		float SingleCubicFractalFbm(float x, float y) const;

		float SingleCubicFractalBillow(float x, float y) const;

		float SingleCubicFractalRigidMulti(float x, float y) const;

		float SingleCubic(const unsigned char &offset, const float &x, const float &y) const;

		float SingleCellular(const float &x, const float &y) const;

		float SingleCellular2Edge(const float &x, const float &y) const;

		void SingleGradientPerturb(const unsigned char &offset, const float &warpAmp, const float &frequency, float x, float y) const;

		//3D
		float SingleValueFractalFbm(float x, float y, float z) const;

		float SingleValueFractalBillow(float x, float y, float z) const;

		float SingleValueFractalRigidMulti(float x, float y, float z) const;

		float SingleValue(const unsigned char &offset, const float &x, const float &y, const float &z) const;

		float SinglePerlinFractalFbm(float x, float y, float z) const;

		float SinglePerlinFractalBillow(float x, float y, float z) const;

		float SinglePerlinFractalRigidMulti(float x, float y, float z) const;

		float SinglePerlin(const unsigned char &offset, const float &x, const float &y, const float &z) const;

		float SingleSimplexFractalFbm(float x, float y, float z) const;

		float SingleSimplexFractalBillow(float x, float y, float z) const;

		float SingleSimplexFractalRigidMulti(float x, float y, float z) const;

		float SingleSimplex(const unsigned char &offset, const float &x, const float &y, const float &z) const;

		float SingleCubicFractalFbm(float x, float y, float z) const;

		float SingleCubicFractalBillow(float x, float y, float z) const;

		float SingleCubicFractalRigidMulti(float x, float y, float z) const;

		float SingleCubic(const unsigned char &offset, const float &x, const float &y, const float &z) const;

		float SingleCellular(const float &x, const float &y, const float &z) const;

		float SingleCellular2Edge(const float &x, const float &y, const float &z) const;

		void SingleGradientPerturb(const unsigned char &offset, const float &warpAmp, const float &frequency, float x, float y, float z) const;

		//4D
		float SingleSimplex(const unsigned char &offset, const float &x, const float &y, const float &z, const float &w) const;
	};
}
