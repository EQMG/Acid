//
// TinyDNGLoader, single header only DNG/TIFF loader.
//

/*
The MIT License (MIT)

Copyright (c) 2016 - 2018 Syoyo Fujita and many contributors.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef TINY_DNG_LOADER_H_
#define TINY_DNG_LOADER_H_

// @note {
// https://www.adobe.com/content/dam/Adobe/en/products/photoshop/pdfs/dng_spec_1.4.0.0.pdf
// }

#include <string>
#include <vector>

#if !defined(TINY_DNG_NO_EXCEPTION)
#include <stdexcept>
#endif

namespace tinydng {

typedef enum {
  LIGHTSOURCE_UNKNOWN = 0,
  LIGHTSOURCE_DAYLIGHT = 1,
  LIGHTSOURCE_FLUORESCENT = 2,
  LIGHTSOURCE_TUNGSTEN = 3,
  LIGHTSOURCE_FLASH = 4,
  LIGHTSOURCE_FINE_WEATHER = 9,
  LIGHTSOURCE_CLOUDY_WEATHER = 10,
  LIGHTSOURCE_SHADE = 11,
  LIGHTSOURCE_DAYLIGHT_FLUORESCENT = 12,
  LIGHTSOURCE_DAY_WHITE_FLUORESCENT = 13,
  LIGHTSOURCE_COOL_WHITE_FLUORESCENT = 14,
  LIGHTSOURCE_WHITE_FLUORESCENT = 15,
  LIGHTSOURCE_STANDARD_LIGHT_A = 17,
  LIGHTSOURCE_STANDARD_LIGHT_B = 18,
  LIGHTSOURCE_STANDARD_LIGHT_C = 19,
  LIGHTSOURCE_D55 = 20,
  LIGHTSOURCE_D65 = 21,
  LIGHTSOURCE_D75 = 22,
  LIGHTSOURCE_D50 = 23,
  LIGHTSOURCE_ISO_STUDIO_TUNGSTEN = 24,
  LIGHTSOURCE_OTHER_LIGHT_SOURCE = 255
} LightSource;

typedef enum {
  COMPRESSION_NONE = 1,
  COMPRESSION_LZW = 5,        // LZW
  COMPRESSION_OLD_JPEG = 6,   // JPEG or lossless JPEG
  COMPRESSION_NEW_JPEG = 7,   // Usually lossles JPEG, may be JPEG
  COMPRESSION_LOSSY = 34892,  // Lossy JPEGG
  COMPRESSION_NEF = 34713     // NIKON RAW
} Compression;

typedef enum {
  TYPE_NOTYPE = 0,
  TYPE_BYTE = 1,
  TYPE_ASCII = 2,  // null-terminated string
  TYPE_SHORT = 3,
  TYPE_LONG = 4,
  TYPE_RATIONAL = 5,  // 64-bit unsigned fraction
  TYPE_SBYTE = 6,
  TYPE_UNDEFINED = 7,  // 8-bit untyped data */
  TYPE_SSHORT = 8,
  TYPE_SLONG = 9,
  TYPE_SRATIONAL = 10,  // 64-bit signed fraction
  TYPE_FLOAT = 11,
  TYPE_DOUBLE = 12,
  TYPE_IFD = 13,     // 32-bit unsigned integer (offset)
  TYPE_LONG8 = 16,   // BigTIFF 64-bit unsigned
  TYPE_SLONG8 = 17,  // BigTIFF 64-bit signed
  TYPE_IFD8 = 18     // BigTIFF 64-bit unsigned integer (offset)
} DataType;

typedef enum {
  SAMPLEFORMAT_UINT = 1,
  SAMPLEFORMAT_INT = 2,
  SAMPLEFORMAT_IEEEFP = 3,  // floating point
  SAMPLEFORMAT_VOID = 4,
  SAMPLEFORMAT_COMPLEXINT = 5,
  SAMPLEFORMAT_COMPLEXIEEEFP = 6
} SampleFormat;

struct FieldInfo {
  int tag;
  short read_count;
  short write_count;
  DataType type;
  unsigned short bit;
  unsigned char ok_to_change;
  unsigned char pass_count;
  std::string name;

  FieldInfo()
      : tag(0),
        read_count(-1),
        write_count(-1),
        type(TYPE_NOTYPE),
        bit(0),
        ok_to_change(0),
        pass_count(0) {}
};

struct FieldData {
  int tag;
  DataType type;
  std::string name;
  std::vector<unsigned char> data;

  FieldData() : tag(0), type(TYPE_NOTYPE) {}
};

struct DNGImage {
  int black_level[4];  // for each spp(up to 4)
  int white_level[4];  // for each spp(up to 4)
  int version;         // DNG version

  int samples_per_pixel;
  int rows_per_strip;

  int bits_per_sample_original;  // BitsPerSample in stored file.
  int bits_per_sample;  // Bits per sample after reading(decoding) DNG image.

  char cfa_plane_color[4];  // 0:red, 1:green, 2:blue, 3:cyan, 4:magenta,
                            // 5:yellow, 6:white
  int cfa_pattern[2][2];    // @fixme { Support non 2x2 CFA pattern. }
  short cfa_pattern_dim;
  short _pad_cfa_patern_dim;
  int cfa_layout;
  int active_area[4];  // top, left, bottom, right
  bool has_active_area;
  unsigned char pad_has_active_area[3];

  int tile_width;
  int tile_length;
  unsigned int tile_offset;
  unsigned int tile_byte_count;  // (compressed) size

  int pad0;
  double analog_balance[3];
  bool has_analog_balance;
  unsigned char pad1[7];

  double as_shot_neutral[3];
  int pad3;
  bool has_as_shot_neutral;
  unsigned char pad4[7];

  int pad5;
  double color_matrix1[3][3];
  double color_matrix2[3][3];

  double forward_matrix1[3][3];
  double forward_matrix2[3][3];

  double camera_calibration1[3][3];
  double camera_calibration2[3][3];

  LightSource calibration_illuminant1;
  LightSource calibration_illuminant2;

  int width;
  int height;
  int compression;
  unsigned int offset;
  short orientation;
  short _pad0;
  int strip_byte_count;
  int jpeg_byte_count;
  short planar_configuration;  // 1: chunky, 2: planar
  short predictor;  // tag 317. 1 = no prediction, 2 = horizontal differencing,
                    // 3 = floating point horizontal differencing

  SampleFormat sample_format;

  // For an image with multiple strips.
  int strips_per_image;
  std::vector<unsigned int> strip_byte_counts;
  std::vector<unsigned int> strip_offsets;

  // CR2(Canon RAW) specific
  unsigned short cr2_slices[3];
  unsigned short pad_c;

  std::vector<unsigned char>
      data;  // Decoded pixel data(len = spp * width * height * bps / 8)

  // Custom fields
  std::vector<FieldData> custom_fields;
};

///
/// Loads DNG image and store it to `images`
///
/// If DNG contains multiple images(e.g. full-res image + thumnail image),
/// The function creates `DNGImage` data strucure for each images.
///
/// C++ exception would be trigerred inside the function unless
/// TINY_DNG_NO_EXCEPTION macro is defined.
///
/// @param[in] filename DNG filename.
/// @param[in] custom_fields List of custom fields to parse(optional. can pass
/// empty array).
/// @param[out] images Loaded DNG images.
/// @param[out] warn Warning message.
/// @param[out] err Error message.
///
/// @return true upon success.
/// @return false upon failure and store error message into `err`.
///
bool LoadDNG(const char* filename, std::vector<FieldInfo>& custom_fields,
             std::vector<DNGImage>* images, std::string* warn,
             std::string* err);

///
/// Check if a file is DNG(TIFF) or not.
/// Extra message will be stored `msg`.
///
bool IsDNG(const char* filename, std::string* msg);

///
/// A variant of `LoadDNG` which loads DNG image from memory.
/// Up to 2GB DNG data.
///
bool LoadDNGFromMemory(const char* mem, unsigned int size,
                       std::vector<FieldInfo>& custom_fields,
                       std::vector<DNGImage>* images, std::string* warn,
                       std::string* err);

///
/// A variant of `IsDNG` which checks if a data is DNG image.
///
bool IsDNGFromMemory(const char* mem, unsigned int size, std::string* msg);

}  // namespace tinydng

#endif  // TINY_DNG_LOADER_H_
