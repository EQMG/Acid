#pragma once

#include <math.h>
#include <assert.h>

#include <algorithm>
#include <random>

#include "../maths/maths.hpp"

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
	float getValue(float x, float y) const;
	float getValueFractal(float x, float y) const;

	float getPerlin(float x, float y) const;
	float getPerlinFractal(float x, float y) const;

	float getSimplex(float x, float y) const;
	float getSimplexFractal(float x, float y) const;

	float getCellular(float x, float y) const;

	float getWhiteNoise(float x, float y) const;
	float getWhiteNoiseInt(int x, int y) const;

	float getCubic(float x, float y) const;
	float getCubicFractal(float x, float y) const;

	float getNoise(float x, float y) const;

	void gradientPerturb(float &x, float &y) const;
	void gradientPerturbFractal(float &x, float &y) const;

	//3D
	float getValue(float x, float y, float z) const;
	float getValueFractal(float x, float y, float z) const;

	float getPerlin(float x, float y, float z) const;
	float getPerlinFractal(float x, float y, float z) const;

	float getSimplex(float x, float y, float z) const;
	float getSimplexFractal(float x, float y, float z) const;

	float getCellular(float x, float y, float z) const;

	float getWhiteNoise(float x, float y, float z) const;
	float getWhiteNoiseInt(int x, int y, int z) const;

	float getCubic(float x, float y, float z) const;
	float getCubicFractal(float x, float y, float z) const;

	float getNoise(float x, float y, float z) const;

	void gradientPerturb(float &x, float &y, float &z) const;
	void gradientPerturbFractal(float &x, float &y, float &z) const;

	//4D
	float getSimplex(float x, float y, float z, float w) const;

	float getWhiteNoise(float x, float y, float z, float w) const;
	float getWhiteNoiseInt(int x, int y, int z, int w) const;
private:
	void calculateFractalBounding();

	// Helpers
	inline static int fastFloor(const float &f);
	inline static int fastRound(const float &f);

	inline static float lerp(const float &a, const float &b, const float &t);
	inline static float interpHermite(const float &t);
	inline static float interpQuintic(const float &t);
	inline static float cubicLerp(const float &a, const float &b, const float &c, const float &d, const float &t);

	inline unsigned char index2D_12(const unsigned char &offset, const int &x, const int &y) const;
	inline unsigned char index3D_12(const unsigned char &offset, const int &x, const int &y, const int &z) const;
	inline unsigned char index4D_32(const unsigned char &offset, const int &x, const int &y, const int &z, const int &w) const;
	inline unsigned char index2D_256(const unsigned char &offset, const int &x, const int &y) const;
	inline unsigned char index3D_256(const unsigned char &offset, const int &x, const int &y, const int &z) const;
	inline unsigned char index4D_256(const unsigned char &offset, const int &x, const int &y, const int &z, const int &w) const;

	static float valCoord2D(const int &seed, const int &x, const int &y);
	static float valCoord3D(const int &seed, const int &x, const int &y, const int &z);
	static float valCoord4D(const int &seed, const int &x, const int &y, const int &z, const int &w);

	inline float valCoord2DFast(const unsigned char &offset, const int &x, const int &y) const;
	inline float valCoord3DFast(const unsigned char &offset, const int &x, const int &y, const int &z) const;

	inline float gradCoord2D(const unsigned char &offset, const int &x, const int &y, const float &xd, const float &yd) const;
	inline float gradCoord3D(const unsigned char &offset, const int &x, const int &y, const int &z, const float &xd, const float &yd, const float &zd) const;
	inline float gradCoord4D(const unsigned char &offset, const int &x, const int &y, const int &z, const int &w, const float &xd, const float &yd, const float &zd, const float &wd) const;

	// 2D
	float singleValueFractalFBM(float x, float y) const;
	float singleValueFractalBillow(float x, float y) const;
	float singleValueFractalRigidMulti(float x, float y) const;
	float singleValue(const unsigned char &offset, const float &x, const float &y) const;

	float singlePerlinFractalFBM(float x, float y) const;
	float singlePerlinFractalBillow(float x, float y) const;
	float singlePerlinFractalRigidMulti(float x, float y) const;
	float singlePerlin(const unsigned char &offset, const float &x, const float &y) const;

	float singleSimplexFractalFBM(float x, float y) const;
	float singleSimplexFractalBillow(float x, float y) const;
	float singleSimplexFractalRigidMulti(float x, float y) const;
	float singleSimplexFractalBlend(float x, float y) const;
	float singleSimplex(const unsigned char &offset, const float &x, const float &y) const;

	float singleCubicFractalFBM(float x, float y) const;
	float singleCubicFractalBillow(float x, float y) const;
	float singleCubicFractalRigidMulti(float x, float y) const;
	float singleCubic(const unsigned char &offset, const float &x, const float &y) const;

	float singleCellular(const float &x, const float &y) const;
	float singleCellular2Edge(const float &x, const float &y) const;

	void singleGradientPerturb(const unsigned char &offset, const float &warpAmp, const float &frequency, float x, float y) const;

	//3D
	float singleValueFractalFBM(float x, float y, float z) const;
	float singleValueFractalBillow(float x, float y, float z) const;
	float singleValueFractalRigidMulti(float x, float y, float z) const;
	float singleValue(const unsigned char &offset, const float &x, const float &y, const float &z) const;

	float singlePerlinFractalFBM(float x, float y, float z) const;
	float singlePerlinFractalBillow(float x, float y, float z) const;
	float singlePerlinFractalRigidMulti(float x, float y, float z) const;
	float singlePerlin(const unsigned char &offset, const float &x, const float &y, const float &z) const;

	float singleSimplexFractalFBM(float x, float y, float z) const;
	float singleSimplexFractalBillow(float x, float y, float z) const;
	float singleSimplexFractalRigidMulti(float x, float y, float z) const;
	float singleSimplex(const unsigned char &offset, const float &x, const float &y, const float &z) const;

	float singleCubicFractalFBM(float x, float y, float z) const;
	float singleCubicFractalBillow(float x, float y, float z) const;
	float singleCubicFractalRigidMulti(float x, float y, float z) const;
	float singleCubic(const unsigned char &offset, const float &x, const float &y, const float &z) const;

	float singleCellular(const float &x, const float &y, const float &z) const;
	float singleCellular2Edge(const float &x, const float &y, const float &z) const;

	void singleGradientPerturb(const unsigned char &offset, const float &warpAmp, const float &frequency, float x, float y, float z) const;

	//4D
	float singleSimplex(const unsigned char &offset, const float &x, const float &y, const float &z, const float &w) const;
};
