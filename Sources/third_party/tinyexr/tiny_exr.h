
/*
Copyright (c) 2014 - 2019, Syoyo Fujita and many contributors.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Syoyo Fujita nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// TinyEXR contains some OpenEXR code, which is licensed under ------------

///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

// End of OpenEXR license -------------------------------------------------

#ifndef TINYEXR_H_
#define TINYEXR_H_

//
//
//   Do this:
//    #define TINYEXR_IMPLEMENTATION
//   before you include this file in *one* C or C++ file to create the
//   implementation.
//
//   // i.e. it should look like this:
//   #include ...
//   #include ...
//   #include ...
//   #define TINYEXR_IMPLEMENTATION
//   #include "tinyexr.h"
//
//

#include <stddef.h>  // for size_t
#include <stdint.h>  // guess stdint.h is available(C99)

#ifdef __cplusplus
extern "C" {
#endif

// Use embedded miniz or not to decode ZIP format pixel. Linking with zlib
// required if this flas is 0.
#ifndef TINYEXR_USE_MINIZ
#define TINYEXR_USE_MINIZ (1)
#endif

// Disable PIZ comporession when applying cpplint.
#ifndef TINYEXR_USE_PIZ
#define TINYEXR_USE_PIZ (1)
#endif

#ifndef TINYEXR_USE_ZFP
#define TINYEXR_USE_ZFP (0)  // TinyEXR extension.
// http://computation.llnl.gov/projects/floating-point-compression
#endif

#ifndef TINYEXR_USE_THREAD
#define TINYEXR_USE_THREAD (1)
// http://computation.llnl.gov/projects/floating-point-compression
#endif

#ifndef TINYEXR_USE_OPENMP
#ifdef _OPENMP
#define TINYEXR_USE_OPENMP (1)
#else
#define TINYEXR_USE_OPENMP (0)
#endif
#endif

#define TINYEXR_SUCCESS (0)
#define TINYEXR_ERROR_INVALID_MAGIC_NUMBER (-1)
#define TINYEXR_ERROR_INVALID_EXR_VERSION (-2)
#define TINYEXR_ERROR_INVALID_ARGUMENT (-3)
#define TINYEXR_ERROR_INVALID_DATA (-4)
#define TINYEXR_ERROR_INVALID_FILE (-5)
#define TINYEXR_ERROR_INVALID_PARAMETER (-6)
#define TINYEXR_ERROR_CANT_OPEN_FILE (-7)
#define TINYEXR_ERROR_UNSUPPORTED_FORMAT (-8)
#define TINYEXR_ERROR_INVALID_HEADER (-9)
#define TINYEXR_ERROR_UNSUPPORTED_FEATURE (-10)
#define TINYEXR_ERROR_CANT_WRITE_FILE (-11)
#define TINYEXR_ERROR_SERIALZATION_FAILED (-12)
#define TINYEXR_ERROR_LAYER_NOT_FOUND (-13)

// @note { OpenEXR file format: http://www.openexr.com/openexrfilelayout.pdf }

// pixel type: possible values are: UINT = 0 HALF = 1 FLOAT = 2
#define TINYEXR_PIXELTYPE_UINT (0)
#define TINYEXR_PIXELTYPE_HALF (1)
#define TINYEXR_PIXELTYPE_FLOAT (2)

#define TINYEXR_MAX_HEADER_ATTRIBUTES (1024)
#define TINYEXR_MAX_CUSTOM_ATTRIBUTES (128)

#define TINYEXR_COMPRESSIONTYPE_NONE (0)
#define TINYEXR_COMPRESSIONTYPE_RLE (1)
#define TINYEXR_COMPRESSIONTYPE_ZIPS (2)
#define TINYEXR_COMPRESSIONTYPE_ZIP (3)
#define TINYEXR_COMPRESSIONTYPE_PIZ (4)
#define TINYEXR_COMPRESSIONTYPE_ZFP (128)  // TinyEXR extension

#define TINYEXR_ZFP_COMPRESSIONTYPE_RATE (0)
#define TINYEXR_ZFP_COMPRESSIONTYPE_PRECISION (1)
#define TINYEXR_ZFP_COMPRESSIONTYPE_ACCURACY (2)

#define TINYEXR_TILE_ONE_LEVEL (0)
#define TINYEXR_TILE_MIPMAP_LEVELS (1)
#define TINYEXR_TILE_RIPMAP_LEVELS (2)

#define TINYEXR_TILE_ROUND_DOWN (0)
#define TINYEXR_TILE_ROUND_UP (1)

typedef struct _EXRVersion {
  int version;    // this must be 2
  int tiled;      // tile format image
  int long_name;  // long name attribute
  int non_image;  // deep image(EXR 2.0)
  int multipart;  // multi-part(EXR 2.0)
} EXRVersion;

typedef struct _EXRAttribute {
  char name[256];  // name and type are up to 255 chars long.
  char type[256];
  unsigned char *value;  // uint8_t*
  int size;
  int pad0;
} EXRAttribute;

typedef struct _EXRChannelInfo {
  char name[256];  // less than 255 bytes long
  int pixel_type;
  int x_sampling;
  int y_sampling;
  unsigned char p_linear;
  unsigned char pad[3];
} EXRChannelInfo;

typedef struct _EXRTile {
  int offset_x;
  int offset_y;
  int level_x;
  int level_y;

  int width;   // actual width in a tile.
  int height;  // actual height int a tile.

  unsigned char **images;  // image[channels][pixels]
} EXRTile;

typedef struct _EXRHeader {
  float pixel_aspect_ratio;
  int line_order;
  int data_window[4];
  int display_window[4];
  float screen_window_center[2];
  float screen_window_width;

  int chunk_count;

  // Properties for tiled format(`tiledesc`).
  int tiled;
  int tile_size_x;
  int tile_size_y;
  int tile_level_mode;
  int tile_rounding_mode;

  int long_name;
  int non_image;
  int multipart;
  unsigned int header_len;

  // Custom attributes(exludes required attributes(e.g. `channels`,
  // `compression`, etc)
  int num_custom_attributes;
  EXRAttribute *custom_attributes;  // array of EXRAttribute. size =
                                    // `num_custom_attributes`.

  EXRChannelInfo *channels;  // [num_channels]

  int *pixel_types;  // Loaded pixel type(TINYEXR_PIXELTYPE_*) of `images` for
  // each channel. This is overwritten with `requested_pixel_types` when
  // loading.
  int num_channels;

  int compression_type;        // compression type(TINYEXR_COMPRESSIONTYPE_*)
  int *requested_pixel_types;  // Filled initially by
                               // ParseEXRHeaderFrom(Meomory|File), then users
                               // can edit it(only valid for HALF pixel type
                               // channel)

} EXRHeader;

typedef struct _EXRMultiPartHeader {
  int num_headers;
  EXRHeader *headers;

} EXRMultiPartHeader;

typedef struct _EXRImage {
  EXRTile *tiles;  // Tiled pixel data. The application must reconstruct image
                   // from tiles manually. NULL if scanline format.
  unsigned char **images;  // image[channels][pixels]. NULL if tiled format.

  int width;
  int height;
  int num_channels;

  // Properties for tile format.
  int num_tiles;

} EXRImage;

typedef struct _EXRMultiPartImage {
  int num_images;
  EXRImage *images;

} EXRMultiPartImage;

typedef struct _DeepImage {
  const char **channel_names;
  float ***image;      // image[channels][scanlines][samples]
  int **offset_table;  // offset_table[scanline][offsets]
  int num_channels;
  int width;
  int height;
  int pad0;
} DeepImage;

// @deprecated { For backward compatibility. Not recommended to use. }
// Loads single-frame OpenEXR image. Assume EXR image contains A(single channel
// alpha) or RGB(A) channels.
// Application must free image data as returned by `out_rgba`
// Result image format is: float x RGBA x width x hight
// Returns negative value and may set error string in `err` when there's an
// error
extern int LoadEXR(float **out_rgba, int *width, int *height,
                   const char *filename, const char **err);

// Loads single-frame OpenEXR image by specifing layer name. Assume EXR image contains A(single channel
// alpha) or RGB(A) channels.
// Application must free image data as returned by `out_rgba`
// Result image format is: float x RGBA x width x hight
// Returns negative value and may set error string in `err` when there's an
// error
// When the specified layer name is not found in the EXR file, the function will return `TINYEXR_ERROR_LAYER_NOT_FOUND`.
extern int LoadEXRWithLayer(float **out_rgba, int *width, int *height,
                   const char *filename, const char *layer_name, const char **err);

//
// Get layer infos from EXR file.
//
// @param[out] layer_names List of layer names. Application must free memory after using this.
// @param[out] num_layers The number of layers
// @param[out] err Error string(wll be filled when the function returns error code). Free it using FreeEXRErrorMessage after using this value.
//
// @return TINYEXR_SUCCEES upon success.
//
extern int EXRLayers(const char *filename, const char **layer_names[], int *num_layers, const char **err);

// @deprecated { to be removed. }
// Simple wrapper API for ParseEXRHeaderFromFile.
// checking given file is a EXR file(by just look up header)
// @return TINYEXR_SUCCEES for EXR image, TINYEXR_ERROR_INVALID_HEADER for
// others
extern int IsEXR(const char *filename);

// @deprecated { to be removed. }
// Saves single-frame OpenEXR image. Assume EXR image contains RGB(A) channels.
// components must be 1(Grayscale), 3(RGB) or 4(RGBA).
// Input image format is: `float x width x height`, or `float x RGB(A) x width x
// hight`
// Save image as fp16(HALF) format when `save_as_fp16` is positive non-zero
// value.
// Save image as fp32(FLOAT) format when `save_as_fp16` is 0.
// Use ZIP compression by default.
// Returns negative value and may set error string in `err` when there's an
// error
extern int SaveEXR(const float *data, const int width, const int height,
                   const int components, const int save_as_fp16,
                   const char *filename, const char **err);

// Initialize EXRHeader struct
extern void InitEXRHeader(EXRHeader *exr_header);

// Initialize EXRImage struct
extern void InitEXRImage(EXRImage *exr_image);

// Free's internal data of EXRHeader struct
extern int FreeEXRHeader(EXRHeader *exr_header);

// Free's internal data of EXRImage struct
extern int FreeEXRImage(EXRImage *exr_image);

// Free's error message
extern void FreeEXRErrorMessage(const char *msg);

// Parse EXR version header of a file.
extern int ParseEXRVersionFromFile(EXRVersion *version, const char *filename);

// Parse EXR version header from memory-mapped EXR data.
extern int ParseEXRVersionFromMemory(EXRVersion *version,
                                     const unsigned char *memory, size_t size);

// Parse single-part OpenEXR header from a file and initialize `EXRHeader`.
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int ParseEXRHeaderFromFile(EXRHeader *header, const EXRVersion *version,
                                  const char *filename, const char **err);

// Parse single-part OpenEXR header from a memory and initialize `EXRHeader`.
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int ParseEXRHeaderFromMemory(EXRHeader *header,
                                    const EXRVersion *version,
                                    const unsigned char *memory, size_t size,
                                    const char **err);

// Parse multi-part OpenEXR headers from a file and initialize `EXRHeader*`
// array.
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int ParseEXRMultipartHeaderFromFile(EXRHeader ***headers,
                                           int *num_headers,
                                           const EXRVersion *version,
                                           const char *filename,
                                           const char **err);

// Parse multi-part OpenEXR headers from a memory and initialize `EXRHeader*`
// array
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int ParseEXRMultipartHeaderFromMemory(EXRHeader ***headers,
                                             int *num_headers,
                                             const EXRVersion *version,
                                             const unsigned char *memory,
                                             size_t size, const char **err);

// Loads single-part OpenEXR image from a file.
// Application must setup `ParseEXRHeaderFromFile` before calling this function.
// Application can free EXRImage using `FreeEXRImage`
// Returns negative value and may set error string in `err` when there's an
// error
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int LoadEXRImageFromFile(EXRImage *image, const EXRHeader *header,
                                const char *filename, const char **err);

// Loads single-part OpenEXR image from a memory.
// Application must setup `EXRHeader` with
// `ParseEXRHeaderFromMemory` before calling this function.
// Application can free EXRImage using `FreeEXRImage`
// Returns negative value and may set error string in `err` when there's an
// error
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int LoadEXRImageFromMemory(EXRImage *image, const EXRHeader *header,
                                  const unsigned char *memory,
                                  const size_t size, const char **err);

// Loads multi-part OpenEXR image from a file.
// Application must setup `ParseEXRMultipartHeaderFromFile` before calling this
// function.
// Application can free EXRImage using `FreeEXRImage`
// Returns negative value and may set error string in `err` when there's an
// error
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int LoadEXRMultipartImageFromFile(EXRImage *images,
                                         const EXRHeader **headers,
                                         unsigned int num_parts,
                                         const char *filename,
                                         const char **err);

// Loads multi-part OpenEXR image from a memory.
// Application must setup `EXRHeader*` array with
// `ParseEXRMultipartHeaderFromMemory` before calling this function.
// Application can free EXRImage using `FreeEXRImage`
// Returns negative value and may set error string in `err` when there's an
// error
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int LoadEXRMultipartImageFromMemory(EXRImage *images,
                                           const EXRHeader **headers,
                                           unsigned int num_parts,
                                           const unsigned char *memory,
                                           const size_t size, const char **err);

// Saves multi-channel, single-frame OpenEXR image to a file.
// Returns negative value and may set error string in `err` when there's an
// error
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int SaveEXRImageToFile(const EXRImage *image,
                              const EXRHeader *exr_header, const char *filename,
                              const char **err);

// Saves multi-channel, single-frame OpenEXR image to a memory.
// Image is compressed using EXRImage.compression value.
// Return the number of bytes if success.
// Return zero and will set error string in `err` when there's an
// error.
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern size_t SaveEXRImageToMemory(const EXRImage *image,
                                   const EXRHeader *exr_header,
                                   unsigned char **memory, const char **err);

// Loads single-frame OpenEXR deep image.
// Application must free memory of variables in DeepImage(image, offset_table)
// Returns negative value and may set error string in `err` when there's an
// error
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int LoadDeepEXR(DeepImage *out_image, const char *filename,
                       const char **err);

// NOT YET IMPLEMENTED:
// Saves single-frame OpenEXR deep image.
// Returns negative value and may set error string in `err` when there's an
// error
// extern int SaveDeepEXR(const DeepImage *in_image, const char *filename,
//                       const char **err);

// NOT YET IMPLEMENTED:
// Loads multi-part OpenEXR deep image.
// Application must free memory of variables in DeepImage(image, offset_table)
// extern int LoadMultiPartDeepEXR(DeepImage **out_image, int num_parts, const
// char *filename,
//                       const char **err);

// For emscripten.
// Loads single-frame OpenEXR image from memory. Assume EXR image contains
// RGB(A) channels.
// Returns negative value and may set error string in `err` when there's an
// error
// When there was an error message, Application must free `err` with
// FreeEXRErrorMessage()
extern int LoadEXRFromMemory(float **out_rgba, int *width, int *height,
                             const unsigned char *memory, size_t size,
                             const char **err);

#ifdef __cplusplus
}
#endif

#endif  // TINYEXR_H_
