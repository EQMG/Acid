#pragma once

#include <math.h>
#include <assert.h>

#include <algorithm>
#include <random>

// Hashing
#define X_PRIME 1619
#define Y_PRIME 31337
#define Z_PRIME 6971
#define W_PRIME 1013

// Cellular
#define FN_CELLULAR_INDEX_MAX 3

class noisefast
{
public:
	enum typenoise
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

	enum typeinterp
	{
		Linear,
		Hermite,
		Quintic
	};

	enum typefractal
	{
		FBM,
		Billow,
		RigidMulti
	};

	enum typecellularfunction
	{
		Euclidean,
		Manhattan,
		Natural
	};

	enum typecellularreturn
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

	static const float CELL_2D_X[];
	static const float CELL_2D_Y[];
	static const float CELL_3D_X[];
	static const float CELL_3D_Y[];
	static const float CELL_3D_Z[];

	static const float F3;
	static const float G3;

	static const float F2;
	static const float G2;

	static const unsigned char SIMPLEX_4D[];
	static const float F4;
	static const float G4;

	static const float CUBIC_2D_BOUNDING;
	static const float CUBIC_3D_BOUNDING;

	int m_seed;
	unsigned char m_perm[512];
	unsigned char m_perm12[512];

	float m_frequency;
	typeinterp m_interp;
	typenoise m_noiseType;

	int m_octaves;
	float m_lacunarity;
	float m_gain;
	typefractal m_fractalType;

	float m_fractalBounding;

	typecellularfunction m_cellularDistanceFunction;
	typecellularreturn m_cellularReturnType;
	noisefast *m_cellularNoiseLookup;
	int m_cellularDistanceIndex0;
	int m_cellularDistanceIndex1;
	float m_cellularJitter;

	float m_gradientPerturbAmp;
public:
	noisefast(int seed);

	~noisefast();

	// Returns seed used for all noise types
	int getSeed() const { return m_seed; }

	// Sets seed used for all noise types
	// Default: 1337
	void setSeed(int seed);

	// Returns frequency used for all noise types
	float getFrequency() const { return m_frequency; }

	// Sets frequency for all noise types
	// Default: 0.01
	void setFrequency(float frequency) { m_frequency = frequency; }

	// Returns interpolation method used for supported noise types
	typeinterp getInterp() const { return m_interp; }

	// Changes the interpolation method used to smooth between noise values
	// Possible interpolation methods (lowest to highest quality) :
	// - Linear
	// - Hermite
	// - Quintic
	// Used in Value, Perlin Noise and Position Warping
	// Default: Quintic
	void setInterp(typeinterp interp) { m_interp = interp; }

	// Returns the noise type used by GetNoise
	typenoise getNoiseType() const { return m_noiseType; }

	// Sets noise return type of GetNoise(...)
	// Default: Simplex
	void setNoiseType(typenoise noiseType) { m_noiseType = noiseType; }

	// Returns octave count for all fractal noise types
	int getFractalOctaves() const { return m_octaves; }

	// Sets octave count for all fractal noise types
	// Default: 3
	void setFractalOctaves(int octaves);

	// Returns octave lacunarity for all fractal noise types
	float getFractalLacunarity() const { return m_lacunarity; }

	// Sets octave lacunarity for all fractal noise types
	// Default: 2.0
	void setFractalLacunarity(float lacunarity) { m_lacunarity = lacunarity; }

	// Returns octave gain for all fractal noise types
	float getFractalGain() const { return m_gain; }

	// Sets octave gain for all fractal noise types
	// Default: 0.5
	void setFractalGain(float gain);

	// Returns method for combining octaves in all fractal noise types
	typefractal getFractalType() const { return m_fractalType; }

	// Sets method for combining octaves in all fractal noise types
	// Default: FBM
	void setFractalType(typefractal fractalType) { m_fractalType = fractalType; }

	// Returns the distance function used in cellular noise calculations
	typecellularfunction getCellularDistanceFunction() const { return m_cellularDistanceFunction; }

	// Sets distance function used in cellular noise calculations
	// Default: Euclidean
	void setCellularDistanceFunction(typecellularfunction cellularDistanceFunction) { m_cellularDistanceFunction = cellularDistanceFunction; }

	// Returns the return type from cellular noise calculations
	typecellularreturn getCellularReturnType() const { return m_cellularReturnType; }

	// Sets return type from cellular noise calculations
	// Note: NoiseLookup requires another FastNoise object be set with SetCellularNoiseLookup() to function
	// Default: CellValue
	void setCellularReturnType(typecellularreturn cellularReturnType) { m_cellularReturnType = cellularReturnType; }

	// Returns the noise used to calculate a cell value if the cellular return type is NoiseLookup
	noisefast *getCellularNoiseLookup() const { return m_cellularNoiseLookup; }

	// Noise used to calculate a cell value if cellular return type is NoiseLookup
	// The lookup value is acquired through GetNoise() so ensure you SetNoiseType() on the noise lookup, value, Perlin or simplex is recommended
	void setCellularNoiseLookup(noisefast *noise) { m_cellularNoiseLookup = noise; }

	// Returns the 2 distance indices used for distance2 return types
	void getCellularDistance2Indices(int &cellularDistanceIndex0, int &cellularDistanceIndex1) const;

	// Sets the 2 distance indices used for distance2 return types
	// Default: 0, 1
	// Note: index0 should be lower than index1
	// Both indices must be >= 0, index1 must be < 4
	void setCellularDistance2Indices(int cellularDistanceIndex0, int cellularDistanceIndex1);

	// Returns the maximum distance a cellular point can move from its grid position
	float getCellularJitter() const { return m_cellularJitter; }

	// Sets the maximum distance a cellular point can move from its grid position
	// Setting this high will make artifacts more common
	// Default: 0.45
	void setCellularJitter(float cellularJitter) { m_cellularJitter = cellularJitter; }

	// Returns the maximum warp distance from original location when using GradientPerturb{Fractal}(...)
	float getGradientPerturbAmp() const { return m_gradientPerturbAmp; }

	// Sets the maximum warp distance from original location when using GradientPerturb{Fractal}(...)
	// Default: 1.0
	void setGradientPerturbAmp(float gradientPerturbAmp) { m_gradientPerturbAmp = gradientPerturbAmp; }

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
	void calculateFractalBounding();

	// Helpers
	inline static int FastFloor(float f);
	inline static int FastRound(float f);
	inline static int FastAbs(int i);
	inline static float FastAbs(float f);
	inline static float Lerp(float a, float b, float t);
	inline static float InterpHermiteFunc(float t);
	inline static float InterpQuinticFunc(float t);
	inline static float CubicLerp(float a, float b, float c, float d, float t);

	inline unsigned char Index2D_12(unsigned char offset, int x, int y) const;
	inline unsigned char Index3D_12(unsigned char offset, int x, int y, int z) const;
	inline unsigned char Index4D_32(unsigned char offset, int x, int y, int z, int w) const;
	inline unsigned char Index2D_256(unsigned char offset, int x, int y) const;
	inline unsigned char Index3D_256(unsigned char offset, int x, int y, int z) const;
	inline unsigned char Index4D_256(unsigned char offset, int x, int y, int z, int w) const;

	static float ValCoord2D(int seed, int x, int y);
	static float ValCoord3D(int seed, int x, int y, int z);
	static float ValCoord4D(int seed, int x, int y, int z, int w);

	inline float ValCoord2DFast(unsigned char offset, int x, int y) const;
	inline float ValCoord3DFast(unsigned char offset, int x, int y, int z) const;
	inline float GradCoord2D(unsigned char offset, int x, int y, float xd, float yd) const;
	inline float GradCoord3D(unsigned char offset, int x, int y, int z, float xd, float yd, float zd) const;
	inline float GradCoord4D(unsigned char offset, int x, int y, int z, int w, float xd, float yd, float zd, float wd) const;

	//2D
	float SingleValueFractalFBM(float x, float y) const;
	float SingleValueFractalBillow(float x, float y) const;
	float SingleValueFractalRigidMulti(float x, float y) const;
	float SingleValue(unsigned char offset, float x, float y) const;

	float SinglePerlinFractalFBM(float x, float y) const;
	float SinglePerlinFractalBillow(float x, float y) const;
	float SinglePerlinFractalRigidMulti(float x, float y) const;
	float SinglePerlin(unsigned char offset, float x, float y) const;

	float SingleSimplexFractalFBM(float x, float y) const;
	float SingleSimplexFractalBillow(float x, float y) const;
	float SingleSimplexFractalRigidMulti(float x, float y) const;
	float SingleSimplexFractalBlend(float x, float y) const;
	float SingleSimplex(unsigned char offset, float x, float y) const;

	float SingleCubicFractalFBM(float x, float y) const;
	float SingleCubicFractalBillow(float x, float y) const;
	float SingleCubicFractalRigidMulti(float x, float y) const;
	float SingleCubic(unsigned char offset, float x, float y) const;

	float SingleCellular(float x, float y) const;
	float SingleCellular2Edge(float x, float y) const;

	void SingleGradientPerturb(unsigned char offset, float warpAmp, float frequency, float &x, float &y) const;

	//3D
	float SingleValueFractalFBM(float x, float y, float z) const;
	float SingleValueFractalBillow(float x, float y, float z) const;
	float SingleValueFractalRigidMulti(float x, float y, float z) const;
	float SingleValue(unsigned char offset, float x, float y, float z) const;

	float SinglePerlinFractalFBM(float x, float y, float z) const;
	float SinglePerlinFractalBillow(float x, float y, float z) const;
	float SinglePerlinFractalRigidMulti(float x, float y, float z) const;
	float SinglePerlin(unsigned char offset, float x, float y, float z) const;

	float SingleSimplexFractalFBM(float x, float y, float z) const;
	float SingleSimplexFractalBillow(float x, float y, float z) const;
	float SingleSimplexFractalRigidMulti(float x, float y, float z) const;
	float SingleSimplex(unsigned char offset, float x, float y, float z) const;

	float SingleCubicFractalFBM(float x, float y, float z) const;
	float SingleCubicFractalBillow(float x, float y, float z) const;
	float SingleCubicFractalRigidMulti(float x, float y, float z) const;
	float SingleCubic(unsigned char offset, float x, float y, float z) const;

	float SingleCellular(float x, float y, float z) const;
	float SingleCellular2Edge(float x, float y, float z) const;

	void SingleGradientPerturb(unsigned char offset, float warpAmp, float frequency, float &x, float &y, float &z) const;

	//4D
	float SingleSimplex(unsigned char offset, float x, float y, float z, float w) const;
};
