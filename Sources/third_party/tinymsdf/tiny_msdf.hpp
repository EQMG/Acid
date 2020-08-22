/**
 * Based on the C++ implementation by Viktor Chlumský.
 * https://github.com/Chlumsky/msdfgen
 */

#pragma once

typedef struct FT_LibraryRec_ *FT_Library;
typedef struct FT_FaceRec_ *FT_Face;

namespace tinymsdf {
using unicode_t = unsigned;

/// A 2D image bitmap with N channels of type T.
template<typename T, int N>
class Bitmap {
public:
	Bitmap() = default;
	Bitmap(T *pixels, int width, int height) :
		pixels(pixels), width(width), height(height), owned(false) {
	}
	Bitmap(int width, int height) :
		pixels(new T[N * width * height]), width(width), height(height) {
	}
	~Bitmap() {
		if (owned) delete[] pixels;
	}

	T *operator()(int x, int y) const {
		return pixels + N * (width * y + x);
	}

    T *pixels;
    int width, height;
	bool owned = true;
};

constexpr double DEFAULT_ERROR_CORRECTION_THRESHOLD = 1.001;

/// Generates a conventional single-channel signed distance field.
bool GenerateSDF(Bitmap<float, 1> &output, FT_Face face, unicode_t unicode);
/// Generates a single-channel signed pseudo-distance field.
bool GeneratePseudoSDF(Bitmap<float, 1> &output, FT_Face face, unicode_t unicode);
/// Generates a multi-channel signed distance field.
bool GenerateMSDF(Bitmap<float, 3> &output, FT_Face face, unicode_t unicode, double edgeThreshold = DEFAULT_ERROR_CORRECTION_THRESHOLD);
/// Generates a multi-channel signed distance field with true distance in the alpha channel.
bool GenerateMTSDF(Bitmap<float, 4> &output, FT_Face face, unicode_t unicode, double edgeThreshold = DEFAULT_ERROR_CORRECTION_THRESHOLD);
}
