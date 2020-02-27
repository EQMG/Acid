#define TINYEXR_IMPLEMENTATION
#include "tiny_exr.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

// #include <iostream> // debug

#include <limits>
#include <string>
#include <vector>

// C++11
#include <cstdint>

#if TINYEXR_USE_THREAD
#include <atomic>
#include <thread>
#endif

#if TINYEXR_USE_OPENMP
#include <omp.h>
#endif

#if TINYEXR_USE_ZFP
#include "zfp.h"
#endif

#if TINYEXR_USE_MINIZ
#include "miniz/miniz.h"
#endif

namespace tinyexr {

// C++11
typedef uint64_t tinyexr_uint64;
typedef int64_t tinyexr_int64;

// Reuse MINIZ_LITTE_ENDIAN macro

#if defined(__sparcv9)
// Big endian
#else
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || MINIZ_X86_OR_X64_CPU
// Set MINIZ_LITTLE_ENDIAN to 1 if the processor is little endian.
#define MINIZ_LITTLE_ENDIAN 1
#endif
#endif


// static bool IsBigEndian(void) {
//  union {
//    unsigned int i;
//    char c[4];
//  } bint = {0x01020304};
//
//  return bint.c[0] == 1;
//}

static void SetErrorMessage(const std::string &msg, const char **err) {
  if (err) {
#ifdef _WIN32
    (*err) = _strdup(msg.c_str());
#else
    (*err) = strdup(msg.c_str());
#endif
  }
}

static const int kEXRVersionSize = 8;

static void cpy2(unsigned short *dst_val, const unsigned short *src_val) {
  unsigned char *dst = reinterpret_cast<unsigned char *>(dst_val);
  const unsigned char *src = reinterpret_cast<const unsigned char *>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
}

static void swap2(unsigned short *val) {
#ifdef MINIZ_LITTLE_ENDIAN
  (void)val;
#else
  unsigned short tmp = *val;
  unsigned char *dst = reinterpret_cast<unsigned char *>(val);
  unsigned char *src = reinterpret_cast<unsigned char *>(&tmp);

  dst[0] = src[1];
  dst[1] = src[0];
#endif
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
static void cpy4(int *dst_val, const int *src_val) {
  unsigned char *dst = reinterpret_cast<unsigned char *>(dst_val);
  const unsigned char *src = reinterpret_cast<const unsigned char *>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

static void cpy4(unsigned int *dst_val, const unsigned int *src_val) {
  unsigned char *dst = reinterpret_cast<unsigned char *>(dst_val);
  const unsigned char *src = reinterpret_cast<const unsigned char *>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

static void cpy4(float *dst_val, const float *src_val) {
  unsigned char *dst = reinterpret_cast<unsigned char *>(dst_val);
  const unsigned char *src = reinterpret_cast<const unsigned char *>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

static void swap4(unsigned int *val) {
#ifdef MINIZ_LITTLE_ENDIAN
  (void)val;
#else
  unsigned int tmp = *val;
  unsigned char *dst = reinterpret_cast<unsigned char *>(val);
  unsigned char *src = reinterpret_cast<unsigned char *>(&tmp);

  dst[0] = src[3];
  dst[1] = src[2];
  dst[2] = src[1];
  dst[3] = src[0];
#endif
}

#if 0
static void cpy8(tinyexr::tinyexr_uint64 *dst_val, const tinyexr::tinyexr_uint64 *src_val) {
  unsigned char *dst = reinterpret_cast<unsigned char *>(dst_val);
  const unsigned char *src = reinterpret_cast<const unsigned char *>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
  dst[4] = src[4];
  dst[5] = src[5];
  dst[6] = src[6];
  dst[7] = src[7];
}
#endif

static void swap8(tinyexr::tinyexr_uint64 *val) {
#ifdef MINIZ_LITTLE_ENDIAN
  (void)val;
#else
  tinyexr::tinyexr_uint64 tmp = (*val);
  unsigned char *dst = reinterpret_cast<unsigned char *>(val);
  unsigned char *src = reinterpret_cast<unsigned char *>(&tmp);

  dst[0] = src[7];
  dst[1] = src[6];
  dst[2] = src[5];
  dst[3] = src[4];
  dst[4] = src[3];
  dst[5] = src[2];
  dst[6] = src[1];
  dst[7] = src[0];
#endif
}

// https://gist.github.com/rygorous/2156668
// Reuse MINIZ_LITTLE_ENDIAN flag from miniz.
union FP32 {
  unsigned int u;
  float f;
  struct {
#if MINIZ_LITTLE_ENDIAN
    unsigned int Mantissa : 23;
    unsigned int Exponent : 8;
    unsigned int Sign : 1;
#else
    unsigned int Sign : 1;
    unsigned int Exponent : 8;
    unsigned int Mantissa : 23;
#endif
  } s;
};

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

union FP16 {
  unsigned short u;
  struct {
#if MINIZ_LITTLE_ENDIAN
    unsigned int Mantissa : 10;
    unsigned int Exponent : 5;
    unsigned int Sign : 1;
#else
    unsigned int Sign : 1;
    unsigned int Exponent : 5;
    unsigned int Mantissa : 10;
#endif
  } s;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

static FP32 half_to_float(FP16 h) {
  static const FP32 magic = {113 << 23};
  static const unsigned int shifted_exp = 0x7c00
                                          << 13;  // exponent mask after shift
  FP32 o;

  o.u = (h.u & 0x7fffU) << 13U;           // exponent/mantissa bits
  unsigned int exp_ = shifted_exp & o.u;  // just the exponent
  o.u += (127 - 15) << 23;                // exponent adjust

  // handle exponent special cases
  if (exp_ == shifted_exp)    // Inf/NaN?
    o.u += (128 - 16) << 23;  // extra exp adjust
  else if (exp_ == 0)         // Zero/Denormal?
  {
    o.u += 1 << 23;  // extra exp adjust
    o.f -= magic.f;  // renormalize
  }

  o.u |= (h.u & 0x8000U) << 16U;  // sign bit
  return o;
}

static FP16 float_to_half_full(FP32 f) {
  FP16 o = {0};

  // Based on ISPC reference code (with minor modifications)
  if (f.s.Exponent == 0)  // Signed zero/denormal (which will underflow)
    o.s.Exponent = 0;
  else if (f.s.Exponent == 255)  // Inf or NaN (all exponent bits set)
  {
    o.s.Exponent = 31;
    o.s.Mantissa = f.s.Mantissa ? 0x200 : 0;  // NaN->qNaN and Inf->Inf
  } else                                      // Normalized number
  {
    // Exponent unbias the single, then bias the halfp
    int newexp = f.s.Exponent - 127 + 15;
    if (newexp >= 31)  // Overflow, return signed infinity
      o.s.Exponent = 31;
    else if (newexp <= 0)  // Underflow
    {
      if ((14 - newexp) <= 24)  // Mantissa might be non-zero
      {
        unsigned int mant = f.s.Mantissa | 0x800000;  // Hidden 1 bit
        o.s.Mantissa = mant >> (14 - newexp);
        if ((mant >> (13 - newexp)) & 1)  // Check for rounding
          o.u++;  // Round, might overflow into exp bit, but this is OK
      }
    } else {
      o.s.Exponent = static_cast<unsigned int>(newexp);
      o.s.Mantissa = f.s.Mantissa >> 13;
      if (f.s.Mantissa & 0x1000)  // Check for rounding
        o.u++;                    // Round, might overflow to inf, this is OK
    }
  }

  o.s.Sign = f.s.Sign;
  return o;
}

// NOTE: From OpenEXR code
// #define IMF_INCREASING_Y  0
// #define IMF_DECREASING_Y  1
// #define IMF_RAMDOM_Y    2
//
// #define IMF_NO_COMPRESSION  0
// #define IMF_RLE_COMPRESSION 1
// #define IMF_ZIPS_COMPRESSION  2
// #define IMF_ZIP_COMPRESSION 3
// #define IMF_PIZ_COMPRESSION 4
// #define IMF_PXR24_COMPRESSION 5
// #define IMF_B44_COMPRESSION 6
// #define IMF_B44A_COMPRESSION  7

#ifdef __clang__
#pragma clang diagnostic push

#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

#endif

static const char *ReadString(std::string *s, const char *ptr, size_t len) {
  // Read untile NULL(\0).
  const char *p = ptr;
  const char *q = ptr;
  while ((size_t(q - ptr) < len) && (*q) != 0) {
    q++;
  }

  if (size_t(q - ptr) >= len) {
    (*s) = std::string();
    return NULL;
  }

  (*s) = std::string(p, q);

  return q + 1;  // skip '\0'
}

static bool ReadAttribute(std::string *name, std::string *type,
                          std::vector<unsigned char> *data, size_t *marker_size,
                          const char *marker, size_t size) {
  size_t name_len = strnlen(marker, size);
  if (name_len == size) {
    // String does not have a terminating character.
    return false;
  }
  *name = std::string(marker, name_len);

  marker += name_len + 1;
  size -= name_len + 1;

  size_t type_len = strnlen(marker, size);
  if (type_len == size) {
    return false;
  }
  *type = std::string(marker, type_len);

  marker += type_len + 1;
  size -= type_len + 1;

  if (size < sizeof(uint32_t)) {
    return false;
  }

  uint32_t data_len;
  memcpy(&data_len, marker, sizeof(uint32_t));
  tinyexr::swap4(reinterpret_cast<unsigned int *>(&data_len));

  if (data_len == 0) {
    if ((*type).compare("string") == 0) {
      // Accept empty string attribute.

      marker += sizeof(uint32_t);
      size -= sizeof(uint32_t);

      *marker_size = name_len + 1 + type_len + 1 + sizeof(uint32_t);

      data->resize(1);
      (*data)[0] = '\0';

      return true;
    } else {
      return false;
    }
  }

  marker += sizeof(uint32_t);
  size -= sizeof(uint32_t);

  if (size < data_len) {
    return false;
  }

  data->resize(static_cast<size_t>(data_len));
  memcpy(&data->at(0), marker, static_cast<size_t>(data_len));

  *marker_size = name_len + 1 + type_len + 1 + sizeof(uint32_t) + data_len;
  return true;
}

static void WriteAttributeToMemory(std::vector<unsigned char> *out,
                                   const char *name, const char *type,
                                   const unsigned char *data, int len) {
  out->insert(out->end(), name, name + strlen(name) + 1);
  out->insert(out->end(), type, type + strlen(type) + 1);

  int outLen = len;
  tinyexr::swap4(reinterpret_cast<unsigned int *>(&outLen));
  out->insert(out->end(), reinterpret_cast<unsigned char *>(&outLen),
              reinterpret_cast<unsigned char *>(&outLen) + sizeof(int));
  out->insert(out->end(), data, data + len);
}

typedef struct {
  std::string name;  // less than 255 bytes long
  int pixel_type;
  int x_sampling;
  int y_sampling;
  unsigned char p_linear;
  unsigned char pad[3];
} ChannelInfo;

typedef struct {
  std::vector<tinyexr::ChannelInfo> channels;
  std::vector<EXRAttribute> attributes;

  int data_window[4];
  int line_order;
  int display_window[4];
  float screen_window_center[2];
  float screen_window_width;
  float pixel_aspect_ratio;

  int chunk_count;

  // Tiled format
  int tile_size_x;
  int tile_size_y;
  int tile_level_mode;
  int tile_rounding_mode;

  unsigned int header_len;

  int compression_type;

  void clear() {
    channels.clear();
    attributes.clear();

    data_window[0] = 0;
    data_window[1] = 0;
    data_window[2] = 0;
    data_window[3] = 0;
    line_order = 0;
    display_window[0] = 0;
    display_window[1] = 0;
    display_window[2] = 0;
    display_window[3] = 0;
    screen_window_center[0] = 0.0f;
    screen_window_center[1] = 0.0f;
    screen_window_width = 0.0f;
    pixel_aspect_ratio = 0.0f;

    chunk_count = 0;

    // Tiled format
    tile_size_x = 0;
    tile_size_y = 0;
    tile_level_mode = 0;
    tile_rounding_mode = 0;

    header_len = 0;
    compression_type = 0;
  }
} HeaderInfo;

static bool ReadChannelInfo(std::vector<ChannelInfo> &channels,
                            const std::vector<unsigned char> &data) {
  const char *p = reinterpret_cast<const char *>(&data.at(0));

  for (;;) {
    if ((*p) == 0) {
      break;
    }
    ChannelInfo info;

    tinyexr_int64 data_len = static_cast<tinyexr_int64>(data.size()) -
                             (p - reinterpret_cast<const char *>(data.data()));
    if (data_len < 0) {
      return false;
    }

    p = ReadString(&info.name, p, size_t(data_len));
    if ((p == NULL) && (info.name.empty())) {
      // Buffer overrun. Issue #51.
      return false;
    }

    const unsigned char *data_end =
        reinterpret_cast<const unsigned char *>(p) + 16;
    if (data_end >= (data.data() + data.size())) {
      return false;
    }

    memcpy(&info.pixel_type, p, sizeof(int));
    p += 4;
    info.p_linear = static_cast<unsigned char>(p[0]);  // uchar
    p += 1 + 3;                                        // reserved: uchar[3]
    memcpy(&info.x_sampling, p, sizeof(int));          // int
    p += 4;
    memcpy(&info.y_sampling, p, sizeof(int));  // int
    p += 4;

    tinyexr::swap4(reinterpret_cast<unsigned int *>(&info.pixel_type));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&info.x_sampling));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&info.y_sampling));

    channels.push_back(info);
  }

  return true;
}

static void WriteChannelInfo(std::vector<unsigned char> &data,
                             const std::vector<ChannelInfo> &channels) {
  size_t sz = 0;

  // Calculate total size.
  for (size_t c = 0; c < channels.size(); c++) {
    sz += strlen(channels[c].name.c_str()) + 1;  // +1 for \0
    sz += 16;                                    // 4 * int
  }
  data.resize(sz + 1);

  unsigned char *p = &data.at(0);

  for (size_t c = 0; c < channels.size(); c++) {
    memcpy(p, channels[c].name.c_str(), strlen(channels[c].name.c_str()));
    p += strlen(channels[c].name.c_str());
    (*p) = '\0';
    p++;

    int pixel_type = channels[c].pixel_type;
    int x_sampling = channels[c].x_sampling;
    int y_sampling = channels[c].y_sampling;
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&pixel_type));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&x_sampling));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&y_sampling));

    memcpy(p, &pixel_type, sizeof(int));
    p += sizeof(int);

    (*p) = channels[c].p_linear;
    p += 4;

    memcpy(p, &x_sampling, sizeof(int));
    p += sizeof(int);

    memcpy(p, &y_sampling, sizeof(int));
    p += sizeof(int);
  }

  (*p) = '\0';
}

static void CompressZip(unsigned char *dst,
                        tinyexr::tinyexr_uint64 &compressedSize,
                        const unsigned char *src, unsigned long src_size) {
  std::vector<unsigned char> tmpBuf(src_size);

  //
  // Apply EXR-specific? postprocess. Grabbed from OpenEXR's
  // ImfZipCompressor.cpp
  //

  //
  // Reorder the pixel data.
  //

  const char *srcPtr = reinterpret_cast<const char *>(src);

  {
    char *t1 = reinterpret_cast<char *>(&tmpBuf.at(0));
    char *t2 = reinterpret_cast<char *>(&tmpBuf.at(0)) + (src_size + 1) / 2;
    const char *stop = srcPtr + src_size;

    for (;;) {
      if (srcPtr < stop)
        *(t1++) = *(srcPtr++);
      else
        break;

      if (srcPtr < stop)
        *(t2++) = *(srcPtr++);
      else
        break;
    }
  }

  //
  // Predictor.
  //

  {
    unsigned char *t = &tmpBuf.at(0) + 1;
    unsigned char *stop = &tmpBuf.at(0) + src_size;
    int p = t[-1];

    while (t < stop) {
      int d = int(t[0]) - p + (128 + 256);
      p = t[0];
      t[0] = static_cast<unsigned char>(d);
      ++t;
    }
  }

#if TINYEXR_USE_MINIZ
  //
  // Compress the data using miniz
  //

  mz_ulong outSize = mz_compressBound(src_size);
  int ret = mz_compress(
      dst, &outSize, static_cast<const unsigned char *>(&tmpBuf.at(0)),
      src_size);
  assert(ret == MZ_OK);
  (void)ret;

  compressedSize = outSize;
#else
  uLong outSize = compressBound(static_cast<uLong>(src_size));
  int ret = compress(dst, &outSize, static_cast<const Bytef *>(&tmpBuf.at(0)),
                     src_size);
  assert(ret == Z_OK);

  compressedSize = outSize;
#endif

  // Use uncompressed data when compressed data is larger than uncompressed.
  // (Issue 40)
  if (compressedSize >= src_size) {
    compressedSize = src_size;
    memcpy(dst, src, src_size);
  }
}

static bool DecompressZip(unsigned char *dst,
                          unsigned long *uncompressed_size /* inout */,
                          const unsigned char *src, unsigned long src_size) {
  if ((*uncompressed_size) == src_size) {
    // Data is not compressed(Issue 40).
    memcpy(dst, src, src_size);
    return true;
  }
  std::vector<unsigned char> tmpBuf(*uncompressed_size);

#if TINYEXR_USE_MINIZ
  int ret =
      mz_uncompress(&tmpBuf.at(0), uncompressed_size, src, src_size);
  if (MZ_OK != ret) {
    return false;
  }
#else
  int ret = uncompress(&tmpBuf.at(0), uncompressed_size, src, src_size);
  if (Z_OK != ret) {
    return false;
  }
#endif

  //
  // Apply EXR-specific? postprocess. Grabbed from OpenEXR's
  // ImfZipCompressor.cpp
  //

  // Predictor.
  {
    unsigned char *t = &tmpBuf.at(0) + 1;
    unsigned char *stop = &tmpBuf.at(0) + (*uncompressed_size);

    while (t < stop) {
      int d = int(t[-1]) + int(t[0]) - 128;
      t[0] = static_cast<unsigned char>(d);
      ++t;
    }
  }

  // Reorder the pixel data.
  {
    const char *t1 = reinterpret_cast<const char *>(&tmpBuf.at(0));
    const char *t2 = reinterpret_cast<const char *>(&tmpBuf.at(0)) +
                     (*uncompressed_size + 1) / 2;
    char *s = reinterpret_cast<char *>(dst);
    char *stop = s + (*uncompressed_size);

    for (;;) {
      if (s < stop)
        *(s++) = *(t1++);
      else
        break;

      if (s < stop)
        *(s++) = *(t2++);
      else
        break;
    }
  }

  return true;
}

// RLE code from OpenEXR --------------------------------------

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#if __has_warning("-Wextra-semi-stmt")
#pragma clang diagnostic ignored "-Wextra-semi-stmt"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4204)  // nonstandard extension used : non-constant
                                 // aggregate initializer (also supported by GNU
                                 // C and C99, so no big deal)
#pragma warning(disable : 4244)  // 'initializing': conversion from '__int64' to
                                 // 'int', possible loss of data
#pragma warning(disable : 4267)  // 'argument': conversion from '__int64' to
                                 // 'int', possible loss of data
#pragma warning(disable : 4996)  // 'strdup': The POSIX name for this item is
                                 // deprecated. Instead, use the ISO C and C++
                                 // conformant name: _strdup.
#endif

const int MIN_RUN_LENGTH = 3;
const int MAX_RUN_LENGTH = 127;

//
// Compress an array of bytes, using run-length encoding,
// and return the length of the compressed data.
//

static int rleCompress(int inLength, const char in[], signed char out[]) {
  const char *inEnd = in + inLength;
  const char *runStart = in;
  const char *runEnd = in + 1;
  signed char *outWrite = out;

  while (runStart < inEnd) {
    while (runEnd < inEnd && *runStart == *runEnd &&
           runEnd - runStart - 1 < MAX_RUN_LENGTH) {
      ++runEnd;
    }

    if (runEnd - runStart >= MIN_RUN_LENGTH) {
      //
      // Compressable run
      //

      *outWrite++ = static_cast<char>(runEnd - runStart) - 1;
      *outWrite++ = *(reinterpret_cast<const signed char *>(runStart));
      runStart = runEnd;
    } else {
      //
      // Uncompressable run
      //

      while (runEnd < inEnd &&
             ((runEnd + 1 >= inEnd || *runEnd != *(runEnd + 1)) ||
              (runEnd + 2 >= inEnd || *(runEnd + 1) != *(runEnd + 2))) &&
             runEnd - runStart < MAX_RUN_LENGTH) {
        ++runEnd;
      }

      *outWrite++ = static_cast<char>(runStart - runEnd);

      while (runStart < runEnd) {
        *outWrite++ = *(reinterpret_cast<const signed char *>(runStart++));
      }
    }

    ++runEnd;
  }

  return static_cast<int>(outWrite - out);
}

//
// Uncompress an array of bytes compressed with rleCompress().
// Returns the length of the oncompressed data, or 0 if the
// length of the uncompressed data would be more than maxLength.
//

static int rleUncompress(int inLength, int maxLength, const signed char in[],
                         char out[]) {
  char *outStart = out;

  while (inLength > 0) {
    if (*in < 0) {
      int count = -(static_cast<int>(*in++));
      inLength -= count + 1;

      // Fixes #116: Add bounds check to in buffer.
      if ((0 > (maxLength -= count)) || (inLength < 0)) return 0;

      memcpy(out, in, count);
      out += count;
      in += count;
    } else {
      int count = *in++;
      inLength -= 2;

      if (0 > (maxLength -= count + 1)) return 0;

      memset(out, *reinterpret_cast<const char *>(in), count + 1);
      out += count + 1;

      in++;
    }
  }

  return static_cast<int>(out - outStart);
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

// End of RLE code from OpenEXR -----------------------------------

static void CompressRle(unsigned char *dst,
                        tinyexr::tinyexr_uint64 &compressedSize,
                        const unsigned char *src, unsigned long src_size) {
  std::vector<unsigned char> tmpBuf(src_size);

  //
  // Apply EXR-specific? postprocess. Grabbed from OpenEXR's
  // ImfRleCompressor.cpp
  //

  //
  // Reorder the pixel data.
  //

  const char *srcPtr = reinterpret_cast<const char *>(src);

  {
    char *t1 = reinterpret_cast<char *>(&tmpBuf.at(0));
    char *t2 = reinterpret_cast<char *>(&tmpBuf.at(0)) + (src_size + 1) / 2;
    const char *stop = srcPtr + src_size;

    for (;;) {
      if (srcPtr < stop)
        *(t1++) = *(srcPtr++);
      else
        break;

      if (srcPtr < stop)
        *(t2++) = *(srcPtr++);
      else
        break;
    }
  }

  //
  // Predictor.
  //

  {
    unsigned char *t = &tmpBuf.at(0) + 1;
    unsigned char *stop = &tmpBuf.at(0) + src_size;
    int p = t[-1];

    while (t < stop) {
      int d = int(t[0]) - p + (128 + 256);
      p = t[0];
      t[0] = static_cast<unsigned char>(d);
      ++t;
    }
  }

  // outSize will be (srcSiz * 3) / 2 at max.
  int outSize = rleCompress(static_cast<int>(src_size),
                            reinterpret_cast<const char *>(&tmpBuf.at(0)),
                            reinterpret_cast<signed char *>(dst));
  assert(outSize > 0);

  compressedSize = static_cast<tinyexr::tinyexr_uint64>(outSize);

  // Use uncompressed data when compressed data is larger than uncompressed.
  // (Issue 40)
  if (compressedSize >= src_size) {
    compressedSize = src_size;
    memcpy(dst, src, src_size);
  }
}

static bool DecompressRle(unsigned char *dst,
                          const unsigned long uncompressed_size,
                          const unsigned char *src, unsigned long src_size) {
  if (uncompressed_size == src_size) {
    // Data is not compressed(Issue 40).
    memcpy(dst, src, src_size);
    return true;
  }

  // Workaround for issue #112.
  // TODO(syoyo): Add more robust out-of-bounds check in `rleUncompress`.
  if (src_size <= 2) {
    return false;
  }

  std::vector<unsigned char> tmpBuf(uncompressed_size);

  int ret = rleUncompress(static_cast<int>(src_size),
                          static_cast<int>(uncompressed_size),
                          reinterpret_cast<const signed char *>(src),
                          reinterpret_cast<char *>(&tmpBuf.at(0)));
  if (ret != static_cast<int>(uncompressed_size)) {
    return false;
  }

  //
  // Apply EXR-specific? postprocess. Grabbed from OpenEXR's
  // ImfRleCompressor.cpp
  //

  // Predictor.
  {
    unsigned char *t = &tmpBuf.at(0) + 1;
    unsigned char *stop = &tmpBuf.at(0) + uncompressed_size;

    while (t < stop) {
      int d = int(t[-1]) + int(t[0]) - 128;
      t[0] = static_cast<unsigned char>(d);
      ++t;
    }
  }

  // Reorder the pixel data.
  {
    const char *t1 = reinterpret_cast<const char *>(&tmpBuf.at(0));
    const char *t2 = reinterpret_cast<const char *>(&tmpBuf.at(0)) +
                     (uncompressed_size + 1) / 2;
    char *s = reinterpret_cast<char *>(dst);
    char *stop = s + uncompressed_size;

    for (;;) {
      if (s < stop)
        *(s++) = *(t1++);
      else
        break;

      if (s < stop)
        *(s++) = *(t2++);
      else
        break;
    }
  }

  return true;
}

#if TINYEXR_USE_PIZ

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-long-long"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wc++11-extensions"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

#if __has_warning("-Wcast-qual")
#pragma clang diagnostic ignored "-Wcast-qual"
#endif

#if __has_warning("-Wextra-semi-stmt")
#pragma clang diagnostic ignored "-Wextra-semi-stmt"
#endif

#endif

//
// PIZ compress/uncompress, based on OpenEXR's ImfPizCompressor.cpp
//
// -----------------------------------------------------------------
// Copyright (c) 2004, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC)
// (3 clause BSD license)
//

struct PIZChannelData {
  unsigned short *start;
  unsigned short *end;
  int nx;
  int ny;
  int ys;
  int size;
};

//-----------------------------------------------------------------------------
//
//  16-bit Haar Wavelet encoding and decoding
//
//  The source code in this file is derived from the encoding
//  and decoding routines written by Christian Rouet for his
//  PIZ image file format.
//
//-----------------------------------------------------------------------------

//
// Wavelet basis functions without modulo arithmetic; they produce
// the best compression ratios when the wavelet-transformed data are
// Huffman-encoded, but the wavelet transform works only for 14-bit
// data (untransformed data values must be less than (1 << 14)).
//

inline void wenc14(unsigned short a, unsigned short b, unsigned short &l,
                   unsigned short &h) {
  short as = static_cast<short>(a);
  short bs = static_cast<short>(b);

  short ms = (as + bs) >> 1;
  short ds = as - bs;

  l = static_cast<unsigned short>(ms);
  h = static_cast<unsigned short>(ds);
}

inline void wdec14(unsigned short l, unsigned short h, unsigned short &a,
                   unsigned short &b) {
  short ls = static_cast<short>(l);
  short hs = static_cast<short>(h);

  int hi = hs;
  int ai = ls + (hi & 1) + (hi >> 1);

  short as = static_cast<short>(ai);
  short bs = static_cast<short>(ai - hi);

  a = static_cast<unsigned short>(as);
  b = static_cast<unsigned short>(bs);
}

//
// Wavelet basis functions with modulo arithmetic; they work with full
// 16-bit data, but Huffman-encoding the wavelet-transformed data doesn't
// compress the data quite as well.
//

const int NBITS = 16;
const int A_OFFSET = 1 << (NBITS - 1);
const int M_OFFSET = 1 << (NBITS - 1);
const int MOD_MASK = (1 << NBITS) - 1;

inline void wenc16(unsigned short a, unsigned short b, unsigned short &l,
                   unsigned short &h) {
  int ao = (a + A_OFFSET) & MOD_MASK;
  int m = ((ao + b) >> 1);
  int d = ao - b;

  if (d < 0) m = (m + M_OFFSET) & MOD_MASK;

  d &= MOD_MASK;

  l = static_cast<unsigned short>(m);
  h = static_cast<unsigned short>(d);
}

inline void wdec16(unsigned short l, unsigned short h, unsigned short &a,
                   unsigned short &b) {
  int m = l;
  int d = h;
  int bb = (m - (d >> 1)) & MOD_MASK;
  int aa = (d + bb - A_OFFSET) & MOD_MASK;
  b = static_cast<unsigned short>(bb);
  a = static_cast<unsigned short>(aa);
}

//
// 2D Wavelet encoding:
//

static void wav2Encode(
    unsigned short *in,  // io: values are transformed in place
    int nx,              // i : x size
    int ox,              // i : x offset
    int ny,              // i : y size
    int oy,              // i : y offset
    unsigned short mx)   // i : maximum in[x][y] value
{
  bool w14 = (mx < (1 << 14));
  int n = (nx > ny) ? ny : nx;
  int p = 1;   // == 1 <<  level
  int p2 = 2;  // == 1 << (level+1)

  //
  // Hierachical loop on smaller dimension n
  //

  while (p2 <= n) {
    unsigned short *py = in;
    unsigned short *ey = in + oy * (ny - p2);
    int oy1 = oy * p;
    int oy2 = oy * p2;
    int ox1 = ox * p;
    int ox2 = ox * p2;
    unsigned short i00, i01, i10, i11;

    //
    // Y loop
    //

    for (; py <= ey; py += oy2) {
      unsigned short *px = py;
      unsigned short *ex = py + ox * (nx - p2);

      //
      // X loop
      //

      for (; px <= ex; px += ox2) {
        unsigned short *p01 = px + ox1;
        unsigned short *p10 = px + oy1;
        unsigned short *p11 = p10 + ox1;

        //
        // 2D wavelet encoding
        //

        if (w14) {
          wenc14(*px, *p01, i00, i01);
          wenc14(*p10, *p11, i10, i11);
          wenc14(i00, i10, *px, *p10);
          wenc14(i01, i11, *p01, *p11);
        } else {
          wenc16(*px, *p01, i00, i01);
          wenc16(*p10, *p11, i10, i11);
          wenc16(i00, i10, *px, *p10);
          wenc16(i01, i11, *p01, *p11);
        }
      }

      //
      // Encode (1D) odd column (still in Y loop)
      //

      if (nx & p) {
        unsigned short *p10 = px + oy1;

        if (w14)
          wenc14(*px, *p10, i00, *p10);
        else
          wenc16(*px, *p10, i00, *p10);

        *px = i00;
      }
    }

    //
    // Encode (1D) odd line (must loop in X)
    //

    if (ny & p) {
      unsigned short *px = py;
      unsigned short *ex = py + ox * (nx - p2);

      for (; px <= ex; px += ox2) {
        unsigned short *p01 = px + ox1;

        if (w14)
          wenc14(*px, *p01, i00, *p01);
        else
          wenc16(*px, *p01, i00, *p01);

        *px = i00;
      }
    }

    //
    // Next level
    //

    p = p2;
    p2 <<= 1;
  }
}

//
// 2D Wavelet decoding:
//

static void wav2Decode(
    unsigned short *in,  // io: values are transformed in place
    int nx,              // i : x size
    int ox,              // i : x offset
    int ny,              // i : y size
    int oy,              // i : y offset
    unsigned short mx)   // i : maximum in[x][y] value
{
  bool w14 = (mx < (1 << 14));
  int n = (nx > ny) ? ny : nx;
  int p = 1;
  int p2;

  //
  // Search max level
  //

  while (p <= n) p <<= 1;

  p >>= 1;
  p2 = p;
  p >>= 1;

  //
  // Hierarchical loop on smaller dimension n
  //

  while (p >= 1) {
    unsigned short *py = in;
    unsigned short *ey = in + oy * (ny - p2);
    int oy1 = oy * p;
    int oy2 = oy * p2;
    int ox1 = ox * p;
    int ox2 = ox * p2;
    unsigned short i00, i01, i10, i11;

    //
    // Y loop
    //

    for (; py <= ey; py += oy2) {
      unsigned short *px = py;
      unsigned short *ex = py + ox * (nx - p2);

      //
      // X loop
      //

      for (; px <= ex; px += ox2) {
        unsigned short *p01 = px + ox1;
        unsigned short *p10 = px + oy1;
        unsigned short *p11 = p10 + ox1;

        //
        // 2D wavelet decoding
        //

        if (w14) {
          wdec14(*px, *p10, i00, i10);
          wdec14(*p01, *p11, i01, i11);
          wdec14(i00, i01, *px, *p01);
          wdec14(i10, i11, *p10, *p11);
        } else {
          wdec16(*px, *p10, i00, i10);
          wdec16(*p01, *p11, i01, i11);
          wdec16(i00, i01, *px, *p01);
          wdec16(i10, i11, *p10, *p11);
        }
      }

      //
      // Decode (1D) odd column (still in Y loop)
      //

      if (nx & p) {
        unsigned short *p10 = px + oy1;

        if (w14)
          wdec14(*px, *p10, i00, *p10);
        else
          wdec16(*px, *p10, i00, *p10);

        *px = i00;
      }
    }

    //
    // Decode (1D) odd line (must loop in X)
    //

    if (ny & p) {
      unsigned short *px = py;
      unsigned short *ex = py + ox * (nx - p2);

      for (; px <= ex; px += ox2) {
        unsigned short *p01 = px + ox1;

        if (w14)
          wdec14(*px, *p01, i00, *p01);
        else
          wdec16(*px, *p01, i00, *p01);

        *px = i00;
      }
    }

    //
    // Next level
    //

    p2 = p;
    p >>= 1;
  }
}

//-----------------------------------------------------------------------------
//
//  16-bit Huffman compression and decompression.
//
//  The source code in this file is derived from the 8-bit
//  Huffman compression and decompression routines written
//  by Christian Rouet for his PIZ image file format.
//
//-----------------------------------------------------------------------------

// Adds some modification for tinyexr.

const int HUF_ENCBITS = 16;  // literal (value) bit length
const int HUF_DECBITS = 14;  // decoding bit size (>= 8)

const int HUF_ENCSIZE = (1 << HUF_ENCBITS) + 1;  // encoding table size
const int HUF_DECSIZE = 1 << HUF_DECBITS;        // decoding table size
const int HUF_DECMASK = HUF_DECSIZE - 1;

struct HufDec {  // short code    long code
  //-------------------------------
  int len : 8;   // code length    0
  int lit : 24;  // lit      p size
  int *p;        // 0      lits
};

inline long long hufLength(long long code) { return code & 63; }

inline long long hufCode(long long code) { return code >> 6; }

inline void outputBits(int nBits, long long bits, long long &c, int &lc,
                       char *&out) {
  c <<= nBits;
  lc += nBits;

  c |= bits;

  while (lc >= 8) *out++ = static_cast<char>((c >> (lc -= 8)));
}

inline long long getBits(int nBits, long long &c, int &lc, const char *&in) {
  while (lc < nBits) {
    c = (c << 8) | *(reinterpret_cast<const unsigned char *>(in++));
    lc += 8;
  }

  lc -= nBits;
  return (c >> lc) & ((1 << nBits) - 1);
}

//
// ENCODING TABLE BUILDING & (UN)PACKING
//

//
// Build a "canonical" Huffman code table:
//  - for each (uncompressed) symbol, hcode contains the length
//    of the corresponding code (in the compressed data)
//  - canonical codes are computed and stored in hcode
//  - the rules for constructing canonical codes are as follows:
//    * shorter codes (if filled with zeroes to the right)
//      have a numerically higher value than longer codes
//    * for codes with the same length, numerical values
//      increase with numerical symbol values
//  - because the canonical code table can be constructed from
//    symbol lengths alone, the code table can be transmitted
//    without sending the actual code values
//  - see http://www.compressconsult.com/huffman/
//

static void hufCanonicalCodeTable(long long hcode[HUF_ENCSIZE]) {
  long long n[59];

  //
  // For each i from 0 through 58, count the
  // number of different codes of length i, and
  // store the count in n[i].
  //

  for (int i = 0; i <= 58; ++i) n[i] = 0;

  for (int i = 0; i < HUF_ENCSIZE; ++i) n[hcode[i]] += 1;

  //
  // For each i from 58 through 1, compute the
  // numerically lowest code with length i, and
  // store that code in n[i].
  //

  long long c = 0;

  for (int i = 58; i > 0; --i) {
    long long nc = ((c + n[i]) >> 1);
    n[i] = c;
    c = nc;
  }

  //
  // hcode[i] contains the length, l, of the
  // code for symbol i.  Assign the next available
  // code of length l to the symbol and store both
  // l and the code in hcode[i].
  //

  for (int i = 0; i < HUF_ENCSIZE; ++i) {
    int l = static_cast<int>(hcode[i]);

    if (l > 0) hcode[i] = l | (n[l]++ << 6);
  }
}

//
// Compute Huffman codes (based on frq input) and store them in frq:
//  - code structure is : [63:lsb - 6:msb] | [5-0: bit length];
//  - max code length is 58 bits;
//  - codes outside the range [im-iM] have a null length (unused values);
//  - original frequencies are destroyed;
//  - encoding tables are used by hufEncode() and hufBuildDecTable();
//

struct FHeapCompare {
  bool operator()(long long *a, long long *b) { return *a > *b; }
};

static void hufBuildEncTable(
    long long *frq,  // io: input frequencies [HUF_ENCSIZE], output table
    int *im,         //  o: min frq index
    int *iM)         //  o: max frq index
{
  //
  // This function assumes that when it is called, array frq
  // indicates the frequency of all possible symbols in the data
  // that are to be Huffman-encoded.  (frq[i] contains the number
  // of occurrences of symbol i in the data.)
  //
  // The loop below does three things:
  //
  // 1) Finds the minimum and maximum indices that point
  //    to non-zero entries in frq:
  //
  //     frq[im] != 0, and frq[i] == 0 for all i < im
  //     frq[iM] != 0, and frq[i] == 0 for all i > iM
  //
  // 2) Fills array fHeap with pointers to all non-zero
  //    entries in frq.
  //
  // 3) Initializes array hlink such that hlink[i] == i
  //    for all array entries.
  //

  std::vector<int> hlink(HUF_ENCSIZE);
  std::vector<long long *> fHeap(HUF_ENCSIZE);

  *im = 0;

  while (!frq[*im]) (*im)++;

  int nf = 0;

  for (int i = *im; i < HUF_ENCSIZE; i++) {
    hlink[i] = i;

    if (frq[i]) {
      fHeap[nf] = &frq[i];
      nf++;
      *iM = i;
    }
  }

  //
  // Add a pseudo-symbol, with a frequency count of 1, to frq;
  // adjust the fHeap and hlink array accordingly.  Function
  // hufEncode() uses the pseudo-symbol for run-length encoding.
  //

  (*iM)++;
  frq[*iM] = 1;
  fHeap[nf] = &frq[*iM];
  nf++;

  //
  // Build an array, scode, such that scode[i] contains the number
  // of bits assigned to symbol i.  Conceptually this is done by
  // constructing a tree whose leaves are the symbols with non-zero
  // frequency:
  //
  //     Make a heap that contains all symbols with a non-zero frequency,
  //     with the least frequent symbol on top.
  //
  //     Repeat until only one symbol is left on the heap:
  //
  //         Take the two least frequent symbols off the top of the heap.
  //         Create a new node that has first two nodes as children, and
  //         whose frequency is the sum of the frequencies of the first
  //         two nodes.  Put the new node back into the heap.
  //
  // The last node left on the heap is the root of the tree.  For each
  // leaf node, the distance between the root and the leaf is the length
  // of the code for the corresponding symbol.
  //
  // The loop below doesn't actually build the tree; instead we compute
  // the distances of the leaves from the root on the fly.  When a new
  // node is added to the heap, then that node's descendants are linked
  // into a single linear list that starts at the new node, and the code
  // lengths of the descendants (that is, their distance from the root
  // of the tree) are incremented by one.
  //

  std::make_heap(&fHeap[0], &fHeap[nf], FHeapCompare());

  std::vector<long long> scode(HUF_ENCSIZE);
  memset(scode.data(), 0, sizeof(long long) * HUF_ENCSIZE);

  while (nf > 1) {
    //
    // Find the indices, mm and m, of the two smallest non-zero frq
    // values in fHeap, add the smallest frq to the second-smallest
    // frq, and remove the smallest frq value from fHeap.
    //

    int mm = fHeap[0] - frq;
    std::pop_heap(&fHeap[0], &fHeap[nf], FHeapCompare());
    --nf;

    int m = fHeap[0] - frq;
    std::pop_heap(&fHeap[0], &fHeap[nf], FHeapCompare());

    frq[m] += frq[mm];
    std::push_heap(&fHeap[0], &fHeap[nf], FHeapCompare());

    //
    // The entries in scode are linked into lists with the
    // entries in hlink serving as "next" pointers and with
    // the end of a list marked by hlink[j] == j.
    //
    // Traverse the lists that start at scode[m] and scode[mm].
    // For each element visited, increment the length of the
    // corresponding code by one bit. (If we visit scode[j]
    // during the traversal, then the code for symbol j becomes
    // one bit longer.)
    //
    // Merge the lists that start at scode[m] and scode[mm]
    // into a single list that starts at scode[m].
    //

    //
    // Add a bit to all codes in the first list.
    //

    for (int j = m;; j = hlink[j]) {
      scode[j]++;

      assert(scode[j] <= 58);

      if (hlink[j] == j) {
        //
        // Merge the two lists.
        //

        hlink[j] = mm;
        break;
      }
    }

    //
    // Add a bit to all codes in the second list
    //

    for (int j = mm;; j = hlink[j]) {
      scode[j]++;

      assert(scode[j] <= 58);

      if (hlink[j] == j) break;
    }
  }

  //
  // Build a canonical Huffman code table, replacing the code
  // lengths in scode with (code, code length) pairs.  Copy the
  // code table from scode into frq.
  //

  hufCanonicalCodeTable(scode.data());
  memcpy(frq, scode.data(), sizeof(long long) * HUF_ENCSIZE);
}

//
// Pack an encoding table:
//  - only code lengths, not actual codes, are stored
//  - runs of zeroes are compressed as follows:
//
//    unpacked    packed
//    --------------------------------
//    1 zero    0  (6 bits)
//    2 zeroes    59
//    3 zeroes    60
//    4 zeroes    61
//    5 zeroes    62
//    n zeroes (6 or more)  63 n-6  (6 + 8 bits)
//

const int SHORT_ZEROCODE_RUN = 59;
const int LONG_ZEROCODE_RUN = 63;
const int SHORTEST_LONG_RUN = 2 + LONG_ZEROCODE_RUN - SHORT_ZEROCODE_RUN;
const int LONGEST_LONG_RUN = 255 + SHORTEST_LONG_RUN;

static void hufPackEncTable(
    const long long *hcode,  // i : encoding table [HUF_ENCSIZE]
    int im,                  // i : min hcode index
    int iM,                  // i : max hcode index
    char **pcode)            //  o: ptr to packed table (updated)
{
  char *p = *pcode;
  long long c = 0;
  int lc = 0;

  for (; im <= iM; im++) {
    int l = hufLength(hcode[im]);

    if (l == 0) {
      int zerun = 1;

      while ((im < iM) && (zerun < LONGEST_LONG_RUN)) {
        if (hufLength(hcode[im + 1]) > 0) break;
        im++;
        zerun++;
      }

      if (zerun >= 2) {
        if (zerun >= SHORTEST_LONG_RUN) {
          outputBits(6, LONG_ZEROCODE_RUN, c, lc, p);
          outputBits(8, zerun - SHORTEST_LONG_RUN, c, lc, p);
        } else {
          outputBits(6, SHORT_ZEROCODE_RUN + zerun - 2, c, lc, p);
        }
        continue;
      }
    }

    outputBits(6, l, c, lc, p);
  }

  if (lc > 0) *p++ = (unsigned char)(c << (8 - lc));

  *pcode = p;
}

//
// Unpack an encoding table packed by hufPackEncTable():
//

static bool hufUnpackEncTable(
    const char **pcode,  // io: ptr to packed table (updated)
    int ni,              // i : input size (in bytes)
    int im,              // i : min hcode index
    int iM,              // i : max hcode index
    long long *hcode)    //  o: encoding table [HUF_ENCSIZE]
{
  memset(hcode, 0, sizeof(long long) * HUF_ENCSIZE);

  const char *p = *pcode;
  long long c = 0;
  int lc = 0;

  for (; im <= iM; im++) {
    if (p - *pcode >= ni) {
      return false;
    }

    long long l = hcode[im] = getBits(6, c, lc, p);  // code length

    if (l == (long long)LONG_ZEROCODE_RUN) {
      if (p - *pcode > ni) {
        return false;
      }

      int zerun = getBits(8, c, lc, p) + SHORTEST_LONG_RUN;

      if (im + zerun > iM + 1) {
        return false;
      }

      while (zerun--) hcode[im++] = 0;

      im--;
    } else if (l >= (long long)SHORT_ZEROCODE_RUN) {
      int zerun = l - SHORT_ZEROCODE_RUN + 2;

      if (im + zerun > iM + 1) {
        return false;
      }

      while (zerun--) hcode[im++] = 0;

      im--;
    }
  }

  *pcode = const_cast<char *>(p);

  hufCanonicalCodeTable(hcode);

  return true;
}

//
// DECODING TABLE BUILDING
//

//
// Clear a newly allocated decoding table so that it contains only zeroes.
//

static void hufClearDecTable(HufDec *hdecod)  // io: (allocated by caller)
//     decoding table [HUF_DECSIZE]
{
  for (int i = 0; i < HUF_DECSIZE; i++) {
    hdecod[i].len = 0;
    hdecod[i].lit = 0;
    hdecod[i].p = NULL;
  }
  // memset(hdecod, 0, sizeof(HufDec) * HUF_DECSIZE);
}

//
// Build a decoding hash table based on the encoding table hcode:
//  - short codes (<= HUF_DECBITS) are resolved with a single table access;
//  - long code entry allocations are not optimized, because long codes are
//    unfrequent;
//  - decoding tables are used by hufDecode();
//

static bool hufBuildDecTable(const long long *hcode,  // i : encoding table
                             int im,                  // i : min index in hcode
                             int iM,                  // i : max index in hcode
                             HufDec *hdecod)  //  o: (allocated by caller)
//     decoding table [HUF_DECSIZE]
{
  //
  // Init hashtable & loop on all codes.
  // Assumes that hufClearDecTable(hdecod) has already been called.
  //

  for (; im <= iM; im++) {
    long long c = hufCode(hcode[im]);
    int l = hufLength(hcode[im]);

    if (c >> l) {
      //
      // Error: c is supposed to be an l-bit code,
      // but c contains a value that is greater
      // than the largest l-bit number.
      //

      // invalidTableEntry();
      return false;
    }

    if (l > HUF_DECBITS) {
      //
      // Long code: add a secondary entry
      //

      HufDec *pl = hdecod + (c >> (l - HUF_DECBITS));

      if (pl->len) {
        //
        // Error: a short code has already
        // been stored in table entry *pl.
        //

        // invalidTableEntry();
        return false;
      }

      pl->lit++;

      if (pl->p) {
        int *p = pl->p;
        pl->p = new int[pl->lit];

        for (int i = 0; i < pl->lit - 1; ++i) pl->p[i] = p[i];

        delete[] p;
      } else {
        pl->p = new int[1];
      }

      pl->p[pl->lit - 1] = im;
    } else if (l) {
      //
      // Short code: init all primary entries
      //

      HufDec *pl = hdecod + (c << (HUF_DECBITS - l));

      for (long long i = 1ULL << (HUF_DECBITS - l); i > 0; i--, pl++) {
        if (pl->len || pl->p) {
          //
          // Error: a short code or a long code has
          // already been stored in table entry *pl.
          //

          // invalidTableEntry();
          return false;
        }

        pl->len = l;
        pl->lit = im;
      }
    }
  }

  return true;
}

//
// Free the long code entries of a decoding table built by hufBuildDecTable()
//

static void hufFreeDecTable(HufDec *hdecod)  // io: Decoding table
{
  for (int i = 0; i < HUF_DECSIZE; i++) {
    if (hdecod[i].p) {
      delete[] hdecod[i].p;
      hdecod[i].p = 0;
    }
  }
}

//
// ENCODING
//

inline void outputCode(long long code, long long &c, int &lc, char *&out) {
  outputBits(hufLength(code), hufCode(code), c, lc, out);
}

inline void sendCode(long long sCode, int runCount, long long runCode,
                     long long &c, int &lc, char *&out) {
  //
  // Output a run of runCount instances of the symbol sCount.
  // Output the symbols explicitly, or if that is shorter, output
  // the sCode symbol once followed by a runCode symbol and runCount
  // expressed as an 8-bit number.
  //

  if (hufLength(sCode) + hufLength(runCode) + 8 < hufLength(sCode) * runCount) {
    outputCode(sCode, c, lc, out);
    outputCode(runCode, c, lc, out);
    outputBits(8, runCount, c, lc, out);
  } else {
    while (runCount-- >= 0) outputCode(sCode, c, lc, out);
  }
}

//
// Encode (compress) ni values based on the Huffman encoding table hcode:
//

static int hufEncode            // return: output size (in bits)
    (const long long *hcode,    // i : encoding table
     const unsigned short *in,  // i : uncompressed input buffer
     const int ni,              // i : input buffer size (in bytes)
     int rlc,                   // i : rl code
     char *out)                 //  o: compressed output buffer
{
  char *outStart = out;
  long long c = 0;  // bits not yet written to out
  int lc = 0;       // number of valid bits in c (LSB)
  int s = in[0];
  int cs = 0;

  //
  // Loop on input values
  //

  for (int i = 1; i < ni; i++) {
    //
    // Count same values or send code
    //

    if (s == in[i] && cs < 255) {
      cs++;
    } else {
      sendCode(hcode[s], cs, hcode[rlc], c, lc, out);
      cs = 0;
    }

    s = in[i];
  }

  //
  // Send remaining code
  //

  sendCode(hcode[s], cs, hcode[rlc], c, lc, out);

  if (lc) *out = (c << (8 - lc)) & 0xff;

  return (out - outStart) * 8 + lc;
}

//
// DECODING
//

//
// In order to force the compiler to inline them,
// getChar() and getCode() are implemented as macros
// instead of "inline" functions.
//

#define getChar(c, lc, in)                   \
  {                                          \
    c = (c << 8) | *(unsigned char *)(in++); \
    lc += 8;                                 \
  }

#if 0
#define getCode(po, rlc, c, lc, in, out, ob, oe) \
  {                                              \
    if (po == rlc) {                             \
      if (lc < 8) getChar(c, lc, in);            \
                                                 \
      lc -= 8;                                   \
                                                 \
      unsigned char cs = (c >> lc);              \
                                                 \
      if (out + cs > oe) return false;           \
                                                 \
      /* TinyEXR issue 78 */                     \
      unsigned short s = out[-1];                \
                                                 \
      while (cs-- > 0) *out++ = s;               \
    } else if (out < oe) {                       \
      *out++ = po;                               \
    } else {                                     \
      return false;                              \
    }                                            \
  }
#else
static bool getCode(int po, int rlc, long long &c, int &lc, const char *&in,
                    const char *in_end, unsigned short *&out,
                    const unsigned short *ob, const unsigned short *oe) {
  (void)ob;
  if (po == rlc) {
    if (lc < 8) {
      /* TinyEXR issue 78 */
      if ((in + 1) >= in_end) {
        return false;
      }

      getChar(c, lc, in);
    }

    lc -= 8;

    unsigned char cs = (c >> lc);

    if (out + cs > oe) return false;

    // Bounds check for safety
    // Issue 100.
    if ((out - 1) < ob) return false;
    unsigned short s = out[-1];

    while (cs-- > 0) *out++ = s;
  } else if (out < oe) {
    *out++ = po;
  } else {
    return false;
  }
  return true;
}
#endif

//
// Decode (uncompress) ni bits based on encoding & decoding tables:
//

static bool hufDecode(const long long *hcode,  // i : encoding table
                      const HufDec *hdecod,    // i : decoding table
                      const char *in,          // i : compressed input buffer
                      int ni,                  // i : input size (in bits)
                      int rlc,                 // i : run-length code
                      int no,  // i : expected output size (in bytes)
                      unsigned short *out)  //  o: uncompressed output buffer
{
  long long c = 0;
  int lc = 0;
  unsigned short *outb = out;          // begin
  unsigned short *oe = out + no;       // end
  const char *ie = in + (ni + 7) / 8;  // input byte size

  //
  // Loop on input bytes
  //

  while (in < ie) {
    getChar(c, lc, in);

    //
    // Access decoding table
    //

    while (lc >= HUF_DECBITS) {
      const HufDec pl = hdecod[(c >> (lc - HUF_DECBITS)) & HUF_DECMASK];

      if (pl.len) {
        //
        // Get short code
        //

        lc -= pl.len;
        // std::cout << "lit = " << pl.lit << std::endl;
        // std::cout << "rlc = " << rlc << std::endl;
        // std::cout << "c = " << c << std::endl;
        // std::cout << "lc = " << lc << std::endl;
        // std::cout << "in = " << in << std::endl;
        // std::cout << "out = " << out << std::endl;
        // std::cout << "oe = " << oe << std::endl;
        if (!getCode(pl.lit, rlc, c, lc, in, ie, out, outb, oe)) {
          return false;
        }
      } else {
        if (!pl.p) {
          return false;
        }
        // invalidCode(); // wrong code

        //
        // Search long code
        //

        int j;

        for (j = 0; j < pl.lit; j++) {
          int l = hufLength(hcode[pl.p[j]]);

          while (lc < l && in < ie)  // get more bits
            getChar(c, lc, in);

          if (lc >= l) {
            if (hufCode(hcode[pl.p[j]]) ==
                ((c >> (lc - l)) & (((long long)(1) << l) - 1))) {
              //
              // Found : get long code
              //

              lc -= l;
              if (!getCode(pl.p[j], rlc, c, lc, in, ie, out, outb, oe)) {
                return false;
              }
              break;
            }
          }
        }

        if (j == pl.lit) {
          return false;
          // invalidCode(); // Not found
        }
      }
    }
  }

  //
  // Get remaining (short) codes
  //

  int i = (8 - ni) & 7;
  c >>= i;
  lc -= i;

  while (lc > 0) {
    const HufDec pl = hdecod[(c << (HUF_DECBITS - lc)) & HUF_DECMASK];

    if (pl.len) {
      lc -= pl.len;
      if (!getCode(pl.lit, rlc, c, lc, in, ie, out, outb, oe)) {
        return false;
      }
    } else {
      return false;
      // invalidCode(); // wrong (long) code
    }
  }

  if (out - outb != no) {
    return false;
  }
  // notEnoughData ();

  return true;
}

static void countFrequencies(std::vector<long long> &freq,
                             const unsigned short data[/*n*/], int n) {
  for (int i = 0; i < HUF_ENCSIZE; ++i) freq[i] = 0;

  for (int i = 0; i < n; ++i) ++freq[data[i]];
}

static void writeUInt(char buf[4], unsigned int i) {
  unsigned char *b = (unsigned char *)buf;

  b[0] = i;
  b[1] = i >> 8;
  b[2] = i >> 16;
  b[3] = i >> 24;
}

static unsigned int readUInt(const char buf[4]) {
  const unsigned char *b = (const unsigned char *)buf;

  return (b[0] & 0x000000ff) | ((b[1] << 8) & 0x0000ff00) |
         ((b[2] << 16) & 0x00ff0000) | ((b[3] << 24) & 0xff000000);
}

//
// EXTERNAL INTERFACE
//

static int hufCompress(const unsigned short raw[], int nRaw,
                       char compressed[]) {
  if (nRaw == 0) return 0;

  std::vector<long long> freq(HUF_ENCSIZE);

  countFrequencies(freq, raw, nRaw);

  int im = 0;
  int iM = 0;
  hufBuildEncTable(freq.data(), &im, &iM);

  char *tableStart = compressed + 20;
  char *tableEnd = tableStart;
  hufPackEncTable(freq.data(), im, iM, &tableEnd);
  int tableLength = tableEnd - tableStart;

  char *dataStart = tableEnd;
  int nBits = hufEncode(freq.data(), raw, nRaw, iM, dataStart);
  int data_length = (nBits + 7) / 8;

  writeUInt(compressed, im);
  writeUInt(compressed + 4, iM);
  writeUInt(compressed + 8, tableLength);
  writeUInt(compressed + 12, nBits);
  writeUInt(compressed + 16, 0);  // room for future extensions

  return dataStart + data_length - compressed;
}

static bool hufUncompress(const char compressed[], int nCompressed,
                          std::vector<unsigned short> *raw) {
  if (nCompressed == 0) {
    if (raw->size() != 0) return false;

    return false;
  }

  int im = readUInt(compressed);
  int iM = readUInt(compressed + 4);
  // int tableLength = readUInt (compressed + 8);
  int nBits = readUInt(compressed + 12);

  if (im < 0 || im >= HUF_ENCSIZE || iM < 0 || iM >= HUF_ENCSIZE) return false;

  const char *ptr = compressed + 20;

  //
  // Fast decoder needs at least 2x64-bits of compressed data, and
  // needs to be run-able on this platform. Otherwise, fall back
  // to the original decoder
  //

  // if (FastHufDecoder::enabled() && nBits > 128)
  //{
  //    FastHufDecoder fhd (ptr, nCompressed - (ptr - compressed), im, iM, iM);
  //    fhd.decode ((unsigned char*)ptr, nBits, raw, nRaw);
  //}
  // else
  {
    std::vector<long long> freq(HUF_ENCSIZE);
    std::vector<HufDec> hdec(HUF_DECSIZE);

    hufClearDecTable(&hdec.at(0));

    hufUnpackEncTable(&ptr, nCompressed - (ptr - compressed), im, iM,
                      &freq.at(0));

    {
      if (nBits > 8 * (nCompressed - (ptr - compressed))) {
        return false;
      }

      hufBuildDecTable(&freq.at(0), im, iM, &hdec.at(0));
      hufDecode(&freq.at(0), &hdec.at(0), ptr, nBits, iM, raw->size(),
                raw->data());
    }
    // catch (...)
    //{
    //    hufFreeDecTable (hdec);
    //    throw;
    //}

    hufFreeDecTable(&hdec.at(0));
  }

  return true;
}

//
// Functions to compress the range of values in the pixel data
//

const int USHORT_RANGE = (1 << 16);
const int BITMAP_SIZE = (USHORT_RANGE >> 3);

static void bitmapFromData(const unsigned short data[/*nData*/], int nData,
                           unsigned char bitmap[BITMAP_SIZE],
                           unsigned short &minNonZero,
                           unsigned short &maxNonZero) {
  for (int i = 0; i < BITMAP_SIZE; ++i) bitmap[i] = 0;

  for (int i = 0; i < nData; ++i) bitmap[data[i] >> 3] |= (1 << (data[i] & 7));

  bitmap[0] &= ~1;  // zero is not explicitly stored in
                    // the bitmap; we assume that the
                    // data always contain zeroes
  minNonZero = BITMAP_SIZE - 1;
  maxNonZero = 0;

  for (int i = 0; i < BITMAP_SIZE; ++i) {
    if (bitmap[i]) {
      if (minNonZero > i) minNonZero = i;
      if (maxNonZero < i) maxNonZero = i;
    }
  }
}

static unsigned short forwardLutFromBitmap(
    const unsigned char bitmap[BITMAP_SIZE], unsigned short lut[USHORT_RANGE]) {
  int k = 0;

  for (int i = 0; i < USHORT_RANGE; ++i) {
    if ((i == 0) || (bitmap[i >> 3] & (1 << (i & 7))))
      lut[i] = k++;
    else
      lut[i] = 0;
  }

  return k - 1;  // maximum value stored in lut[],
}  // i.e. number of ones in bitmap minus 1

static unsigned short reverseLutFromBitmap(
    const unsigned char bitmap[BITMAP_SIZE], unsigned short lut[USHORT_RANGE]) {
  int k = 0;

  for (int i = 0; i < USHORT_RANGE; ++i) {
    if ((i == 0) || (bitmap[i >> 3] & (1 << (i & 7)))) lut[k++] = i;
  }

  int n = k - 1;

  while (k < USHORT_RANGE) lut[k++] = 0;

  return n;  // maximum k where lut[k] is non-zero,
}  // i.e. number of ones in bitmap minus 1

static void applyLut(const unsigned short lut[USHORT_RANGE],
                     unsigned short data[/*nData*/], int nData) {
  for (int i = 0; i < nData; ++i) data[i] = lut[data[i]];
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif  // __clang__

#ifdef _MSC_VER
#pragma warning(pop)
#endif

static bool CompressPiz(unsigned char *outPtr, unsigned int *outSize,
                        const unsigned char *inPtr, size_t inSize,
                        const std::vector<ChannelInfo> &channelInfo,
                        int data_width, int num_lines) {
  std::vector<unsigned char> bitmap(BITMAP_SIZE);
  unsigned short minNonZero;
  unsigned short maxNonZero;

#if !MINIZ_LITTLE_ENDIAN
  // @todo { PIZ compression on BigEndian architecture. }
  assert(0);
  return false;
#endif

  // Assume `inSize` is multiple of 2 or 4.
  std::vector<unsigned short> tmpBuffer(inSize / sizeof(unsigned short));

  std::vector<PIZChannelData> channelData(channelInfo.size());
  unsigned short *tmpBufferEnd = &tmpBuffer.at(0);

  for (size_t c = 0; c < channelData.size(); c++) {
    PIZChannelData &cd = channelData[c];

    cd.start = tmpBufferEnd;
    cd.end = cd.start;

    cd.nx = data_width;
    cd.ny = num_lines;
    // cd.ys = c.channel().ySampling;

    size_t pixelSize = sizeof(int);  // UINT and FLOAT
    if (channelInfo[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
      pixelSize = sizeof(short);
    }

    cd.size = static_cast<int>(pixelSize / sizeof(short));

    tmpBufferEnd += cd.nx * cd.ny * cd.size;
  }

  const unsigned char *ptr = inPtr;
  for (int y = 0; y < num_lines; ++y) {
    for (size_t i = 0; i < channelData.size(); ++i) {
      PIZChannelData &cd = channelData[i];

      // if (modp (y, cd.ys) != 0)
      //    continue;

      size_t n = static_cast<size_t>(cd.nx * cd.size);
      memcpy(cd.end, ptr, n * sizeof(unsigned short));
      ptr += n * sizeof(unsigned short);
      cd.end += n;
    }
  }

  bitmapFromData(&tmpBuffer.at(0), static_cast<int>(tmpBuffer.size()),
                 bitmap.data(), minNonZero, maxNonZero);

  std::vector<unsigned short> lut(USHORT_RANGE);
  unsigned short maxValue = forwardLutFromBitmap(bitmap.data(), lut.data());
  applyLut(lut.data(), &tmpBuffer.at(0), static_cast<int>(tmpBuffer.size()));

  //
  // Store range compression info in _outBuffer
  //

  char *buf = reinterpret_cast<char *>(outPtr);

  memcpy(buf, &minNonZero, sizeof(unsigned short));
  buf += sizeof(unsigned short);
  memcpy(buf, &maxNonZero, sizeof(unsigned short));
  buf += sizeof(unsigned short);

  if (minNonZero <= maxNonZero) {
    memcpy(buf, reinterpret_cast<char *>(&bitmap[0] + minNonZero),
           maxNonZero - minNonZero + 1);
    buf += maxNonZero - minNonZero + 1;
  }

  //
  // Apply wavelet encoding
  //

  for (size_t i = 0; i < channelData.size(); ++i) {
    PIZChannelData &cd = channelData[i];

    for (int j = 0; j < cd.size; ++j) {
      wav2Encode(cd.start + j, cd.nx, cd.size, cd.ny, cd.nx * cd.size,
                 maxValue);
    }
  }

  //
  // Apply Huffman encoding; append the result to _outBuffer
  //

  // length header(4byte), then huff data. Initialize length header with zero,
  // then later fill it by `length`.
  char *lengthPtr = buf;
  int zero = 0;
  memcpy(buf, &zero, sizeof(int));
  buf += sizeof(int);

  int length =
      hufCompress(&tmpBuffer.at(0), static_cast<int>(tmpBuffer.size()), buf);
  memcpy(lengthPtr, &length, sizeof(int));

  (*outSize) = static_cast<unsigned int>(
      (reinterpret_cast<unsigned char *>(buf) - outPtr) +
      static_cast<unsigned int>(length));

  // Use uncompressed data when compressed data is larger than uncompressed.
  // (Issue 40)
  if ((*outSize) >= inSize) {
    (*outSize) = static_cast<unsigned int>(inSize);
    memcpy(outPtr, inPtr, inSize);
  }
  return true;
}

static bool DecompressPiz(unsigned char *outPtr, const unsigned char *inPtr,
                          size_t tmpBufSize, size_t inLen, int num_channels,
                          const EXRChannelInfo *channels, int data_width,
                          int num_lines) {
  if (inLen == tmpBufSize) {
    // Data is not compressed(Issue 40).
    memcpy(outPtr, inPtr, inLen);
    return true;
  }

  std::vector<unsigned char> bitmap(BITMAP_SIZE);
  unsigned short minNonZero;
  unsigned short maxNonZero;

#if !MINIZ_LITTLE_ENDIAN
  // @todo { PIZ compression on BigEndian architecture. }
  assert(0);
  return false;
#endif

  memset(bitmap.data(), 0, BITMAP_SIZE);

  const unsigned char *ptr = inPtr;
  // minNonZero = *(reinterpret_cast<const unsigned short *>(ptr));
  tinyexr::cpy2(&minNonZero, reinterpret_cast<const unsigned short *>(ptr));
  // maxNonZero = *(reinterpret_cast<const unsigned short *>(ptr + 2));
  tinyexr::cpy2(&maxNonZero, reinterpret_cast<const unsigned short *>(ptr + 2));
  ptr += 4;

  if (maxNonZero >= BITMAP_SIZE) {
    return false;
  }

  if (minNonZero <= maxNonZero) {
    memcpy(reinterpret_cast<char *>(&bitmap[0] + minNonZero), ptr,
           maxNonZero - minNonZero + 1);
    ptr += maxNonZero - minNonZero + 1;
  }

  std::vector<unsigned short> lut(USHORT_RANGE);
  memset(lut.data(), 0, sizeof(unsigned short) * USHORT_RANGE);
  unsigned short maxValue = reverseLutFromBitmap(bitmap.data(), lut.data());

  //
  // Huffman decoding
  //

  int length;

  // length = *(reinterpret_cast<const int *>(ptr));
  tinyexr::cpy4(&length, reinterpret_cast<const int *>(ptr));
  ptr += sizeof(int);

  if (size_t((ptr - inPtr) + length) > inLen) {
    return false;
  }

  std::vector<unsigned short> tmpBuffer(tmpBufSize);
  hufUncompress(reinterpret_cast<const char *>(ptr), length, &tmpBuffer);

  //
  // Wavelet decoding
  //

  std::vector<PIZChannelData> channelData(static_cast<size_t>(num_channels));

  unsigned short *tmpBufferEnd = &tmpBuffer.at(0);

  for (size_t i = 0; i < static_cast<size_t>(num_channels); ++i) {
    const EXRChannelInfo &chan = channels[i];

    size_t pixelSize = sizeof(int);  // UINT and FLOAT
    if (chan.pixel_type == TINYEXR_PIXELTYPE_HALF) {
      pixelSize = sizeof(short);
    }

    channelData[i].start = tmpBufferEnd;
    channelData[i].end = channelData[i].start;
    channelData[i].nx = data_width;
    channelData[i].ny = num_lines;
    // channelData[i].ys = 1;
    channelData[i].size = static_cast<int>(pixelSize / sizeof(short));

    tmpBufferEnd += channelData[i].nx * channelData[i].ny * channelData[i].size;
  }

  for (size_t i = 0; i < channelData.size(); ++i) {
    PIZChannelData &cd = channelData[i];

    for (int j = 0; j < cd.size; ++j) {
      wav2Decode(cd.start + j, cd.nx, cd.size, cd.ny, cd.nx * cd.size,
                 maxValue);
    }
  }

  //
  // Expand the pixel data to their original range
  //

  applyLut(lut.data(), &tmpBuffer.at(0), static_cast<int>(tmpBufSize));

  for (int y = 0; y < num_lines; y++) {
    for (size_t i = 0; i < channelData.size(); ++i) {
      PIZChannelData &cd = channelData[i];

      // if (modp (y, cd.ys) != 0)
      //    continue;

      size_t n = static_cast<size_t>(cd.nx * cd.size);
      memcpy(outPtr, cd.end, static_cast<size_t>(n * sizeof(unsigned short)));
      outPtr += n * sizeof(unsigned short);
      cd.end += n;
    }
  }

  return true;
}
#endif  // TINYEXR_USE_PIZ

#if TINYEXR_USE_ZFP
struct ZFPCompressionParam {
  double rate;
  int precision;
  double tolerance;
  int type;  // TINYEXR_ZFP_COMPRESSIONTYPE_*

  ZFPCompressionParam() {
    type = TINYEXR_ZFP_COMPRESSIONTYPE_RATE;
    rate = 2.0;
    precision = 0;
    tolerance = 0.0f;
  }
};

bool FindZFPCompressionParam(ZFPCompressionParam *param,
                             const EXRAttribute *attributes,
                             int num_attributes) {
  bool foundType = false;

  for (int i = 0; i < num_attributes; i++) {
    if ((strcmp(attributes[i].name, "zfpCompressionType") == 0) &&
        (attributes[i].size == 1)) {
      param->type = static_cast<int>(attributes[i].value[0]);

      foundType = true;
    }
  }

  if (!foundType) {
    return false;
  }

  if (param->type == TINYEXR_ZFP_COMPRESSIONTYPE_RATE) {
    for (int i = 0; i < num_attributes; i++) {
      if ((strcmp(attributes[i].name, "zfpCompressionRate") == 0) &&
          (attributes[i].size == 8)) {
        param->rate = *(reinterpret_cast<double *>(attributes[i].value));
        return true;
      }
    }
  } else if (param->type == TINYEXR_ZFP_COMPRESSIONTYPE_PRECISION) {
    for (int i = 0; i < num_attributes; i++) {
      if ((strcmp(attributes[i].name, "zfpCompressionPrecision") == 0) &&
          (attributes[i].size == 4)) {
        param->rate = *(reinterpret_cast<int *>(attributes[i].value));
        return true;
      }
    }
  } else if (param->type == TINYEXR_ZFP_COMPRESSIONTYPE_ACCURACY) {
    for (int i = 0; i < num_attributes; i++) {
      if ((strcmp(attributes[i].name, "zfpCompressionTolerance") == 0) &&
          (attributes[i].size == 8)) {
        param->tolerance = *(reinterpret_cast<double *>(attributes[i].value));
        return true;
      }
    }
  } else {
    assert(0);
  }

  return false;
}

// Assume pixel format is FLOAT for all channels.
static bool DecompressZfp(float *dst, int dst_width, int dst_num_lines,
                          int num_channels, const unsigned char *src,
                          unsigned long src_size,
                          const ZFPCompressionParam &param) {
  size_t uncompressed_size = dst_width * dst_num_lines * num_channels;

  if (uncompressed_size == src_size) {
    // Data is not compressed(Issue 40).
    memcpy(dst, src, src_size);
  }

  zfp_stream *zfp = NULL;
  zfp_field *field = NULL;

  assert((dst_width % 4) == 0);
  assert((dst_num_lines % 4) == 0);

  if ((dst_width & 3U) || (dst_num_lines & 3U)) {
    return false;
  }

  field =
      zfp_field_2d(reinterpret_cast<void *>(const_cast<unsigned char *>(src)),
                   zfp_type_float, dst_width, dst_num_lines * num_channels);
  zfp = zfp_stream_open(NULL);

  if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_RATE) {
    zfp_stream_set_rate(zfp, param.rate, zfp_type_float, /* dimention */ 2,
                        /* write random access */ 0);
  } else if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_PRECISION) {
    zfp_stream_set_precision(zfp, param.precision, zfp_type_float);
  } else if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_ACCURACY) {
    zfp_stream_set_accuracy(zfp, param.tolerance, zfp_type_float);
  } else {
    assert(0);
  }

  size_t buf_size = zfp_stream_maximum_size(zfp, field);
  std::vector<unsigned char> buf(buf_size);
  memcpy(&buf.at(0), src, src_size);

  bitstream *stream = stream_open(&buf.at(0), buf_size);
  zfp_stream_set_bit_stream(zfp, stream);
  zfp_stream_rewind(zfp);

  size_t image_size = dst_width * dst_num_lines;

  for (int c = 0; c < num_channels; c++) {
    // decompress 4x4 pixel block.
    for (int y = 0; y < dst_num_lines; y += 4) {
      for (int x = 0; x < dst_width; x += 4) {
        float fblock[16];
        zfp_decode_block_float_2(zfp, fblock);
        for (int j = 0; j < 4; j++) {
          for (int i = 0; i < 4; i++) {
            dst[c * image_size + ((y + j) * dst_width + (x + i))] =
                fblock[j * 4 + i];
          }
        }
      }
    }
  }

  zfp_field_free(field);
  zfp_stream_close(zfp);
  stream_close(stream);

  return true;
}

// Assume pixel format is FLOAT for all channels.
bool CompressZfp(std::vector<unsigned char> *outBuf, unsigned int *outSize,
                 const float *inPtr, int width, int num_lines, int num_channels,
                 const ZFPCompressionParam &param) {
  zfp_stream *zfp = NULL;
  zfp_field *field = NULL;

  assert((width % 4) == 0);
  assert((num_lines % 4) == 0);

  if ((width & 3U) || (num_lines & 3U)) {
    return false;
  }

  // create input array.
  field = zfp_field_2d(reinterpret_cast<void *>(const_cast<float *>(inPtr)),
                       zfp_type_float, width, num_lines * num_channels);

  zfp = zfp_stream_open(NULL);

  if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_RATE) {
    zfp_stream_set_rate(zfp, param.rate, zfp_type_float, 2, 0);
  } else if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_PRECISION) {
    zfp_stream_set_precision(zfp, param.precision, zfp_type_float);
  } else if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_ACCURACY) {
    zfp_stream_set_accuracy(zfp, param.tolerance, zfp_type_float);
  } else {
    assert(0);
  }

  size_t buf_size = zfp_stream_maximum_size(zfp, field);

  outBuf->resize(buf_size);

  bitstream *stream = stream_open(&outBuf->at(0), buf_size);
  zfp_stream_set_bit_stream(zfp, stream);
  zfp_field_free(field);

  size_t image_size = width * num_lines;

  for (int c = 0; c < num_channels; c++) {
    // compress 4x4 pixel block.
    for (int y = 0; y < num_lines; y += 4) {
      for (int x = 0; x < width; x += 4) {
        float fblock[16];
        for (int j = 0; j < 4; j++) {
          for (int i = 0; i < 4; i++) {
            fblock[j * 4 + i] =
                inPtr[c * image_size + ((y + j) * width + (x + i))];
          }
        }
        zfp_encode_block_float_2(zfp, fblock);
      }
    }
  }

  zfp_stream_flush(zfp);
  (*outSize) = zfp_stream_compressed_size(zfp);

  zfp_stream_close(zfp);

  return true;
}

#endif

//
// -----------------------------------------------------------------
//

// TODO(syoyo): Refactor function arguments.
static bool DecodePixelData(/* out */ unsigned char **out_images,
                            const int *requested_pixel_types,
                            const unsigned char *data_ptr, size_t data_len,
                            int compression_type, int line_order, int width,
                            int height, int x_stride, int y, int line_no,
                            int num_lines, size_t pixel_data_size,
                            size_t num_attributes,
                            const EXRAttribute *attributes, size_t num_channels,
                            const EXRChannelInfo *channels,
                            const std::vector<size_t> &channel_offset_list) {
  if (compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {  // PIZ
#if TINYEXR_USE_PIZ
    if ((width == 0) || (num_lines == 0) || (pixel_data_size == 0)) {
      // Invalid input #90
      return false;
    }

    // Allocate original data size.
    std::vector<unsigned char> outBuf(static_cast<size_t>(
        static_cast<size_t>(width * num_lines) * pixel_data_size));
    size_t tmpBufLen = outBuf.size();

    bool ret = tinyexr::DecompressPiz(
        reinterpret_cast<unsigned char *>(&outBuf.at(0)), data_ptr, tmpBufLen,
        data_len, static_cast<int>(num_channels), channels, width, num_lines);

    if (!ret) {
      return false;
    }

    // For PIZ_COMPRESSION:
    //   pixel sample data for channel 0 for scanline 0
    //   pixel sample data for channel 1 for scanline 0
    //   pixel sample data for channel ... for scanline 0
    //   pixel sample data for channel n for scanline 0
    //   pixel sample data for channel 0 for scanline 1
    //   pixel sample data for channel 1 for scanline 1
    //   pixel sample data for channel ... for scanline 1
    //   pixel sample data for channel n for scanline 1
    //   ...
    for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
      if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned short *line_ptr = reinterpret_cast<unsigned short *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            FP16 hf;

            // hf.u = line_ptr[u];
            // use `cpy` to avoid unaligned memory access when compiler's
            // optimization is on.
            tinyexr::cpy2(&(hf.u), line_ptr + u);

            tinyexr::swap2(reinterpret_cast<unsigned short *>(&hf.u));

            if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
              unsigned short *image =
                  reinterpret_cast<unsigned short **>(out_images)[c];
              if (line_order == 0) {
                image += (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                image += static_cast<size_t>(
                             (height - 1 - (line_no + static_cast<int>(v)))) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              *image = hf.u;
            } else {  // HALF -> FLOAT
              FP32 f32 = half_to_float(hf);
              float *image = reinterpret_cast<float **>(out_images)[c];
              size_t offset = 0;
              if (line_order == 0) {
                offset = (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                offset = static_cast<size_t>(
                             (height - 1 - (line_no + static_cast<int>(v)))) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              image += offset;
              *image = f32.f;
            }
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_UINT);

        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned int *line_ptr = reinterpret_cast<unsigned int *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            unsigned int val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(&val);

            unsigned int *image =
                reinterpret_cast<unsigned int **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += static_cast<size_t>(
                           (height - 1 - (line_no + static_cast<int>(v)))) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT);
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const float *line_ptr = reinterpret_cast<float *>(&outBuf.at(
              v * pixel_data_size * static_cast<size_t>(x_stride) +
              channel_offset_list[c] * static_cast<size_t>(x_stride)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            float val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            float *image = reinterpret_cast<float **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += static_cast<size_t>(
                           (height - 1 - (line_no + static_cast<int>(v)))) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else {
        assert(0);
      }
    }
#else
    assert(0 && "PIZ is enabled in this build");
    return false;
#endif

  } else if (compression_type == TINYEXR_COMPRESSIONTYPE_ZIPS ||
             compression_type == TINYEXR_COMPRESSIONTYPE_ZIP) {
    // Allocate original data size.
    std::vector<unsigned char> outBuf(static_cast<size_t>(width) *
                                      static_cast<size_t>(num_lines) *
                                      pixel_data_size);

    unsigned long dstLen = static_cast<unsigned long>(outBuf.size());
    assert(dstLen > 0);
    if (!tinyexr::DecompressZip(
            reinterpret_cast<unsigned char *>(&outBuf.at(0)), &dstLen, data_ptr,
            static_cast<unsigned long>(data_len))) {
      return false;
    }

    // For ZIP_COMPRESSION:
    //   pixel sample data for channel 0 for scanline 0
    //   pixel sample data for channel 1 for scanline 0
    //   pixel sample data for channel ... for scanline 0
    //   pixel sample data for channel n for scanline 0
    //   pixel sample data for channel 0 for scanline 1
    //   pixel sample data for channel 1 for scanline 1
    //   pixel sample data for channel ... for scanline 1
    //   pixel sample data for channel n for scanline 1
    //   ...
    for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
      if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned short *line_ptr = reinterpret_cast<unsigned short *>(
              &outBuf.at(v * static_cast<size_t>(pixel_data_size) *
                             static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            tinyexr::FP16 hf;

            // hf.u = line_ptr[u];
            tinyexr::cpy2(&(hf.u), line_ptr + u);

            tinyexr::swap2(reinterpret_cast<unsigned short *>(&hf.u));

            if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
              unsigned short *image =
                  reinterpret_cast<unsigned short **>(out_images)[c];
              if (line_order == 0) {
                image += (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                image += (static_cast<size_t>(height) - 1U -
                          (static_cast<size_t>(line_no) + v)) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              *image = hf.u;
            } else {  // HALF -> FLOAT
              tinyexr::FP32 f32 = half_to_float(hf);
              float *image = reinterpret_cast<float **>(out_images)[c];
              size_t offset = 0;
              if (line_order == 0) {
                offset = (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                offset = (static_cast<size_t>(height) - 1U -
                          (static_cast<size_t>(line_no) + v)) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              image += offset;

              *image = f32.f;
            }
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_UINT);

        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned int *line_ptr = reinterpret_cast<unsigned int *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            unsigned int val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(&val);

            unsigned int *image =
                reinterpret_cast<unsigned int **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += (static_cast<size_t>(height) - 1U -
                        (static_cast<size_t>(line_no) + v)) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT);
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const float *line_ptr = reinterpret_cast<float *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            float val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            float *image = reinterpret_cast<float **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += (static_cast<size_t>(height) - 1U -
                        (static_cast<size_t>(line_no) + v)) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else {
        assert(0);
        return false;
      }
    }
  } else if (compression_type == TINYEXR_COMPRESSIONTYPE_RLE) {
    // Allocate original data size.
    std::vector<unsigned char> outBuf(static_cast<size_t>(width) *
                                      static_cast<size_t>(num_lines) *
                                      pixel_data_size);

    unsigned long dstLen = static_cast<unsigned long>(outBuf.size());
    if (dstLen == 0) {
      return false;
    }

    if (!tinyexr::DecompressRle(
            reinterpret_cast<unsigned char *>(&outBuf.at(0)), dstLen, data_ptr,
            static_cast<unsigned long>(data_len))) {
      return false;
    }

    // For RLE_COMPRESSION:
    //   pixel sample data for channel 0 for scanline 0
    //   pixel sample data for channel 1 for scanline 0
    //   pixel sample data for channel ... for scanline 0
    //   pixel sample data for channel n for scanline 0
    //   pixel sample data for channel 0 for scanline 1
    //   pixel sample data for channel 1 for scanline 1
    //   pixel sample data for channel ... for scanline 1
    //   pixel sample data for channel n for scanline 1
    //   ...
    for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
      if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned short *line_ptr = reinterpret_cast<unsigned short *>(
              &outBuf.at(v * static_cast<size_t>(pixel_data_size) *
                             static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            tinyexr::FP16 hf;

            // hf.u = line_ptr[u];
            tinyexr::cpy2(&(hf.u), line_ptr + u);

            tinyexr::swap2(reinterpret_cast<unsigned short *>(&hf.u));

            if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
              unsigned short *image =
                  reinterpret_cast<unsigned short **>(out_images)[c];
              if (line_order == 0) {
                image += (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                image += (static_cast<size_t>(height) - 1U -
                          (static_cast<size_t>(line_no) + v)) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              *image = hf.u;
            } else {  // HALF -> FLOAT
              tinyexr::FP32 f32 = half_to_float(hf);
              float *image = reinterpret_cast<float **>(out_images)[c];
              if (line_order == 0) {
                image += (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                image += (static_cast<size_t>(height) - 1U -
                          (static_cast<size_t>(line_no) + v)) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              *image = f32.f;
            }
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_UINT);

        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned int *line_ptr = reinterpret_cast<unsigned int *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            unsigned int val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(&val);

            unsigned int *image =
                reinterpret_cast<unsigned int **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += (static_cast<size_t>(height) - 1U -
                        (static_cast<size_t>(line_no) + v)) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT);
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const float *line_ptr = reinterpret_cast<float *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            float val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            float *image = reinterpret_cast<float **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += (static_cast<size_t>(height) - 1U -
                        (static_cast<size_t>(line_no) + v)) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else {
        assert(0);
        return false;
      }
    }
  } else if (compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
#if TINYEXR_USE_ZFP
    tinyexr::ZFPCompressionParam zfp_compression_param;
    if (!FindZFPCompressionParam(&zfp_compression_param, attributes,
                                 num_attributes)) {
      assert(0);
      return false;
    }

    // Allocate original data size.
    std::vector<unsigned char> outBuf(static_cast<size_t>(width) *
                                      static_cast<size_t>(num_lines) *
                                      pixel_data_size);

    unsigned long dstLen = outBuf.size();
    assert(dstLen > 0);
    tinyexr::DecompressZfp(reinterpret_cast<float *>(&outBuf.at(0)), width,
                           num_lines, num_channels, data_ptr,
                           static_cast<unsigned long>(data_len),
                           zfp_compression_param);

    // For ZFP_COMPRESSION:
    //   pixel sample data for channel 0 for scanline 0
    //   pixel sample data for channel 1 for scanline 0
    //   pixel sample data for channel ... for scanline 0
    //   pixel sample data for channel n for scanline 0
    //   pixel sample data for channel 0 for scanline 1
    //   pixel sample data for channel 1 for scanline 1
    //   pixel sample data for channel ... for scanline 1
    //   pixel sample data for channel n for scanline 1
    //   ...
    for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
      assert(channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT);
      if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT);
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const float *line_ptr = reinterpret_cast<float *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            float val;
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            float *image = reinterpret_cast<float **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += (static_cast<size_t>(height) - 1U -
                        (static_cast<size_t>(line_no) + v)) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else {
        assert(0);
        return false;
      }
    }
#else
    (void)attributes;
    (void)num_attributes;
    (void)num_channels;
    assert(0);
    return false;
#endif
  } else if (compression_type == TINYEXR_COMPRESSIONTYPE_NONE) {
    for (size_t c = 0; c < num_channels; c++) {
      for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
        if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
          const unsigned short *line_ptr =
              reinterpret_cast<const unsigned short *>(
                  data_ptr + v * pixel_data_size * size_t(width) +
                  channel_offset_list[c] * static_cast<size_t>(width));

          if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
            unsigned short *outLine =
                reinterpret_cast<unsigned short *>(out_images[c]);
            if (line_order == 0) {
              outLine += (size_t(y) + v) * size_t(x_stride);
            } else {
              outLine +=
                  (size_t(height) - 1 - (size_t(y) + v)) * size_t(x_stride);
            }

            for (int u = 0; u < width; u++) {
              tinyexr::FP16 hf;

              // hf.u = line_ptr[u];
              tinyexr::cpy2(&(hf.u), line_ptr + u);

              tinyexr::swap2(reinterpret_cast<unsigned short *>(&hf.u));

              outLine[u] = hf.u;
            }
          } else if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT) {
            float *outLine = reinterpret_cast<float *>(out_images[c]);
            if (line_order == 0) {
              outLine += (size_t(y) + v) * size_t(x_stride);
            } else {
              outLine +=
                  (size_t(height) - 1 - (size_t(y) + v)) * size_t(x_stride);
            }

            if (reinterpret_cast<const unsigned char *>(line_ptr + width) >
                (data_ptr + data_len)) {
              // Insufficient data size
              return false;
            }

            for (int u = 0; u < width; u++) {
              tinyexr::FP16 hf;

              // address may not be aliged. use byte-wise copy for safety.#76
              // hf.u = line_ptr[u];
              tinyexr::cpy2(&(hf.u), line_ptr + u);

              tinyexr::swap2(reinterpret_cast<unsigned short *>(&hf.u));

              tinyexr::FP32 f32 = half_to_float(hf);

              outLine[u] = f32.f;
            }
          } else {
            assert(0);
            return false;
          }
        } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
          const float *line_ptr = reinterpret_cast<const float *>(
              data_ptr + v * pixel_data_size * size_t(width) +
              channel_offset_list[c] * static_cast<size_t>(width));

          float *outLine = reinterpret_cast<float *>(out_images[c]);
          if (line_order == 0) {
            outLine += (size_t(y) + v) * size_t(x_stride);
          } else {
            outLine +=
                (size_t(height) - 1 - (size_t(y) + v)) * size_t(x_stride);
          }

          if (reinterpret_cast<const unsigned char *>(line_ptr + width) >
              (data_ptr + data_len)) {
            // Insufficient data size
            return false;
          }

          for (int u = 0; u < width; u++) {
            float val;
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            outLine[u] = val;
          }
        } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
          const unsigned int *line_ptr = reinterpret_cast<const unsigned int *>(
              data_ptr + v * pixel_data_size * size_t(width) +
              channel_offset_list[c] * static_cast<size_t>(width));

          unsigned int *outLine =
              reinterpret_cast<unsigned int *>(out_images[c]);
          if (line_order == 0) {
            outLine += (size_t(y) + v) * size_t(x_stride);
          } else {
            outLine +=
                (size_t(height) - 1 - (size_t(y) + v)) * size_t(x_stride);
          }

          for (int u = 0; u < width; u++) {
            if (reinterpret_cast<const unsigned char *>(line_ptr + u) >=
                (data_ptr + data_len)) {
              // Corrupsed data?
              return false;
            }

            unsigned int val;
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            outLine[u] = val;
          }
        }
      }
    }
  }

  return true;
}

static bool DecodeTiledPixelData(
    unsigned char **out_images, int *width, int *height,
    const int *requested_pixel_types, const unsigned char *data_ptr,
    size_t data_len, int compression_type, int line_order, int data_width,
    int data_height, int tile_offset_x, int tile_offset_y, int tile_size_x,
    int tile_size_y, size_t pixel_data_size, size_t num_attributes,
    const EXRAttribute *attributes, size_t num_channels,
    const EXRChannelInfo *channels,
    const std::vector<size_t> &channel_offset_list) {
  assert(tile_offset_x * tile_size_x < data_width);
  assert(tile_offset_y * tile_size_y < data_height);

  // Compute actual image size in a tile.
  if ((tile_offset_x + 1) * tile_size_x >= data_width) {
    (*width) = data_width - (tile_offset_x * tile_size_x);
  } else {
    (*width) = tile_size_x;
  }

  if ((tile_offset_y + 1) * tile_size_y >= data_height) {
    (*height) = data_height - (tile_offset_y * tile_size_y);
  } else {
    (*height) = tile_size_y;
  }

  // Image size = tile size.
  return DecodePixelData(out_images, requested_pixel_types, data_ptr, data_len,
                         compression_type, line_order, (*width), tile_size_y,
                         /* stride */ tile_size_x, /* y */ 0, /* line_no */ 0,
                         (*height), pixel_data_size, num_attributes, attributes,
                         num_channels, channels, channel_offset_list);
}

static bool ComputeChannelLayout(std::vector<size_t> *channel_offset_list,
                                 int *pixel_data_size, size_t *channel_offset,
                                 int num_channels,
                                 const EXRChannelInfo *channels) {
  channel_offset_list->resize(static_cast<size_t>(num_channels));

  (*pixel_data_size) = 0;
  (*channel_offset) = 0;

  for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
    (*channel_offset_list)[c] = (*channel_offset);
    if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
      (*pixel_data_size) += sizeof(unsigned short);
      (*channel_offset) += sizeof(unsigned short);
    } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
      (*pixel_data_size) += sizeof(float);
      (*channel_offset) += sizeof(float);
    } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
      (*pixel_data_size) += sizeof(unsigned int);
      (*channel_offset) += sizeof(unsigned int);
    } else {
      // ???
      return false;
    }
  }
  return true;
}

static unsigned char **AllocateImage(int num_channels,
                                     const EXRChannelInfo *channels,
                                     const int *requested_pixel_types,
                                     int data_width, int data_height) {
  unsigned char **images =
      reinterpret_cast<unsigned char **>(static_cast<float **>(
          malloc(sizeof(float *) * static_cast<size_t>(num_channels))));

  for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
    size_t data_len =
        static_cast<size_t>(data_width) * static_cast<size_t>(data_height);
    if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
      // pixel_data_size += sizeof(unsigned short);
      // channel_offset += sizeof(unsigned short);
      // Alloc internal image for half type.
      if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
        images[c] =
            reinterpret_cast<unsigned char *>(static_cast<unsigned short *>(
                malloc(sizeof(unsigned short) * data_len)));
      } else if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT) {
        images[c] = reinterpret_cast<unsigned char *>(
            static_cast<float *>(malloc(sizeof(float) * data_len)));
      } else {
        assert(0);
      }
    } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
      // pixel_data_size += sizeof(float);
      // channel_offset += sizeof(float);
      images[c] = reinterpret_cast<unsigned char *>(
          static_cast<float *>(malloc(sizeof(float) * data_len)));
    } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
      // pixel_data_size += sizeof(unsigned int);
      // channel_offset += sizeof(unsigned int);
      images[c] = reinterpret_cast<unsigned char *>(
          static_cast<unsigned int *>(malloc(sizeof(unsigned int) * data_len)));
    } else {
      assert(0);
    }
  }

  return images;
}

static int ParseEXRHeader(HeaderInfo *info, bool *empty_header,
                          const EXRVersion *version, std::string *err,
                          const unsigned char *buf, size_t size) {
  const char *marker = reinterpret_cast<const char *>(&buf[0]);

  if (empty_header) {
    (*empty_header) = false;
  }

  if (version->multipart) {
    if (size > 0 && marker[0] == '\0') {
      // End of header list.
      if (empty_header) {
        (*empty_header) = true;
      }
      return TINYEXR_SUCCESS;
    }
  }

  // According to the spec, the header of every OpenEXR file must contain at
  // least the following attributes:
  //
  // channels chlist
  // compression compression
  // dataWindow box2i
  // displayWindow box2i
  // lineOrder lineOrder
  // pixelAspectRatio float
  // screenWindowCenter v2f
  // screenWindowWidth float
  bool has_channels = false;
  bool has_compression = false;
  bool has_data_window = false;
  bool has_display_window = false;
  bool has_line_order = false;
  bool has_pixel_aspect_ratio = false;
  bool has_screen_window_center = false;
  bool has_screen_window_width = false;

  info->data_window[0] = 0;
  info->data_window[1] = 0;
  info->data_window[2] = 0;
  info->data_window[3] = 0;
  info->line_order = 0;  // @fixme
  info->display_window[0] = 0;
  info->display_window[1] = 0;
  info->display_window[2] = 0;
  info->display_window[3] = 0;
  info->screen_window_center[0] = 0.0f;
  info->screen_window_center[1] = 0.0f;
  info->screen_window_width = -1.0f;
  info->pixel_aspect_ratio = -1.0f;

  info->tile_size_x = -1;
  info->tile_size_y = -1;
  info->tile_level_mode = -1;
  info->tile_rounding_mode = -1;

  info->attributes.clear();

  // Read attributes
  size_t orig_size = size;
  for (size_t nattr = 0; nattr < TINYEXR_MAX_HEADER_ATTRIBUTES; nattr++) {
    if (0 == size) {
      if (err) {
        (*err) += "Insufficient data size for attributes.\n";
      }
      return TINYEXR_ERROR_INVALID_DATA;
    } else if (marker[0] == '\0') {
      size--;
      break;
    }

    std::string attr_name;
    std::string attr_type;
    std::vector<unsigned char> data;
    size_t marker_size;
    if (!tinyexr::ReadAttribute(&attr_name, &attr_type, &data, &marker_size,
                                marker, size)) {
      if (err) {
        (*err) += "Failed to read attribute.\n";
      }
      return TINYEXR_ERROR_INVALID_DATA;
    }
    marker += marker_size;
    size -= marker_size;

    if (version->tiled && attr_name.compare("tiles") == 0) {
      unsigned int x_size, y_size;
      unsigned char tile_mode;
      assert(data.size() == 9);
      memcpy(&x_size, &data.at(0), sizeof(int));
      memcpy(&y_size, &data.at(4), sizeof(int));
      tile_mode = data[8];
      tinyexr::swap4(&x_size);
      tinyexr::swap4(&y_size);

      info->tile_size_x = static_cast<int>(x_size);
      info->tile_size_y = static_cast<int>(y_size);

      // mode = levelMode + roundingMode * 16
      info->tile_level_mode = tile_mode & 0x3;
      info->tile_rounding_mode = (tile_mode >> 4) & 0x1;

    } else if (attr_name.compare("compression") == 0) {
      bool ok = false;
      if (data[0] < TINYEXR_COMPRESSIONTYPE_PIZ) {
        ok = true;
      }

      if (data[0] == TINYEXR_COMPRESSIONTYPE_PIZ) {
#if TINYEXR_USE_PIZ
        ok = true;
#else
        if (err) {
          (*err) = "PIZ compression is not supported.";
        }
        return TINYEXR_ERROR_UNSUPPORTED_FORMAT;
#endif
      }

      if (data[0] == TINYEXR_COMPRESSIONTYPE_ZFP) {
#if TINYEXR_USE_ZFP
        ok = true;
#else
        if (err) {
          (*err) = "ZFP compression is not supported.";
        }
        return TINYEXR_ERROR_UNSUPPORTED_FORMAT;
#endif
      }

      if (!ok) {
        if (err) {
          (*err) = "Unknown compression type.";
        }
        return TINYEXR_ERROR_UNSUPPORTED_FORMAT;
      }

      info->compression_type = static_cast<int>(data[0]);
      has_compression = true;

    } else if (attr_name.compare("channels") == 0) {
      // name: zero-terminated string, from 1 to 255 bytes long
      // pixel type: int, possible values are: UINT = 0 HALF = 1 FLOAT = 2
      // pLinear: unsigned char, possible values are 0 and 1
      // reserved: three chars, should be zero
      // xSampling: int
      // ySampling: int

      if (!ReadChannelInfo(info->channels, data)) {
        if (err) {
          (*err) += "Failed to parse channel info.\n";
        }
        return TINYEXR_ERROR_INVALID_DATA;
      }

      if (info->channels.size() < 1) {
        if (err) {
          (*err) += "# of channels is zero.\n";
        }
        return TINYEXR_ERROR_INVALID_DATA;
      }

      has_channels = true;

    } else if (attr_name.compare("dataWindow") == 0) {
      if (data.size() >= 16) {
        memcpy(&info->data_window[0], &data.at(0), sizeof(int));
        memcpy(&info->data_window[1], &data.at(4), sizeof(int));
        memcpy(&info->data_window[2], &data.at(8), sizeof(int));
        memcpy(&info->data_window[3], &data.at(12), sizeof(int));
        tinyexr::swap4(reinterpret_cast<unsigned int *>(&info->data_window[0]));
        tinyexr::swap4(reinterpret_cast<unsigned int *>(&info->data_window[1]));
        tinyexr::swap4(reinterpret_cast<unsigned int *>(&info->data_window[2]));
        tinyexr::swap4(reinterpret_cast<unsigned int *>(&info->data_window[3]));
        has_data_window = true;
      }
    } else if (attr_name.compare("displayWindow") == 0) {
      if (data.size() >= 16) {
        memcpy(&info->display_window[0], &data.at(0), sizeof(int));
        memcpy(&info->display_window[1], &data.at(4), sizeof(int));
        memcpy(&info->display_window[2], &data.at(8), sizeof(int));
        memcpy(&info->display_window[3], &data.at(12), sizeof(int));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->display_window[0]));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->display_window[1]));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->display_window[2]));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->display_window[3]));

        has_display_window = true;
      }
    } else if (attr_name.compare("lineOrder") == 0) {
      if (data.size() >= 1) {
        info->line_order = static_cast<int>(data[0]);
        has_line_order = true;
      }
    } else if (attr_name.compare("pixelAspectRatio") == 0) {
      if (data.size() >= sizeof(float)) {
        memcpy(&info->pixel_aspect_ratio, &data.at(0), sizeof(float));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->pixel_aspect_ratio));
        has_pixel_aspect_ratio = true;
      }
    } else if (attr_name.compare("screenWindowCenter") == 0) {
      if (data.size() >= 8) {
        memcpy(&info->screen_window_center[0], &data.at(0), sizeof(float));
        memcpy(&info->screen_window_center[1], &data.at(4), sizeof(float));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->screen_window_center[0]));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->screen_window_center[1]));
        has_screen_window_center = true;
      }
    } else if (attr_name.compare("screenWindowWidth") == 0) {
      if (data.size() >= sizeof(float)) {
        memcpy(&info->screen_window_width, &data.at(0), sizeof(float));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->screen_window_width));

        has_screen_window_width = true;
      }
    } else if (attr_name.compare("chunkCount") == 0) {
      if (data.size() >= sizeof(int)) {
        memcpy(&info->chunk_count, &data.at(0), sizeof(int));
        tinyexr::swap4(reinterpret_cast<unsigned int *>(&info->chunk_count));
      }
    } else {
      // Custom attribute(up to TINYEXR_MAX_CUSTOM_ATTRIBUTES)
      if (info->attributes.size() < TINYEXR_MAX_CUSTOM_ATTRIBUTES) {
        EXRAttribute attrib;
#ifdef _MSC_VER
        strncpy_s(attrib.name, attr_name.c_str(), 255);
        strncpy_s(attrib.type, attr_type.c_str(), 255);
#else
        strncpy(attrib.name, attr_name.c_str(), 255);
        strncpy(attrib.type, attr_type.c_str(), 255);
#endif
        attrib.name[255] = '\0';
        attrib.type[255] = '\0';
        attrib.size = static_cast<int>(data.size());
        attrib.value = static_cast<unsigned char *>(malloc(data.size()));
        memcpy(reinterpret_cast<char *>(attrib.value), &data.at(0),
               data.size());
        info->attributes.push_back(attrib);
      }
    }
  }

  // Check if required attributes exist
  {
    std::stringstream ss_err;

    if (!has_compression) {
      ss_err << "\"compression\" attribute not found in the header."
             << std::endl;
    }

    if (!has_channels) {
      ss_err << "\"channels\" attribute not found in the header." << std::endl;
    }

    if (!has_line_order) {
      ss_err << "\"lineOrder\" attribute not found in the header." << std::endl;
    }

    if (!has_display_window) {
      ss_err << "\"displayWindow\" attribute not found in the header."
             << std::endl;
    }

    if (!has_data_window) {
      ss_err << "\"dataWindow\" attribute not found in the header or invalid."
             << std::endl;
    }

    if (!has_pixel_aspect_ratio) {
      ss_err << "\"pixelAspectRatio\" attribute not found in the header."
             << std::endl;
    }

    if (!has_screen_window_width) {
      ss_err << "\"screenWindowWidth\" attribute not found in the header."
             << std::endl;
    }

    if (!has_screen_window_center) {
      ss_err << "\"screenWindowCenter\" attribute not found in the header."
             << std::endl;
    }

    if (!(ss_err.str().empty())) {
      if (err) {
        (*err) += ss_err.str();
      }
      return TINYEXR_ERROR_INVALID_HEADER;
    }
  }

  info->header_len = static_cast<unsigned int>(orig_size - size);

  return TINYEXR_SUCCESS;
}

// C++ HeaderInfo to C EXRHeader conversion.
static void ConvertHeader(EXRHeader *exr_header, const HeaderInfo &info) {
  exr_header->pixel_aspect_ratio = info.pixel_aspect_ratio;
  exr_header->screen_window_center[0] = info.screen_window_center[0];
  exr_header->screen_window_center[1] = info.screen_window_center[1];
  exr_header->screen_window_width = info.screen_window_width;
  exr_header->chunk_count = info.chunk_count;
  exr_header->display_window[0] = info.display_window[0];
  exr_header->display_window[1] = info.display_window[1];
  exr_header->display_window[2] = info.display_window[2];
  exr_header->display_window[3] = info.display_window[3];
  exr_header->data_window[0] = info.data_window[0];
  exr_header->data_window[1] = info.data_window[1];
  exr_header->data_window[2] = info.data_window[2];
  exr_header->data_window[3] = info.data_window[3];
  exr_header->line_order = info.line_order;
  exr_header->compression_type = info.compression_type;

  exr_header->tile_size_x = info.tile_size_x;
  exr_header->tile_size_y = info.tile_size_y;
  exr_header->tile_level_mode = info.tile_level_mode;
  exr_header->tile_rounding_mode = info.tile_rounding_mode;

  exr_header->num_channels = static_cast<int>(info.channels.size());

  exr_header->channels = static_cast<EXRChannelInfo *>(malloc(
      sizeof(EXRChannelInfo) * static_cast<size_t>(exr_header->num_channels)));
  for (size_t c = 0; c < static_cast<size_t>(exr_header->num_channels); c++) {
#ifdef _MSC_VER
    strncpy_s(exr_header->channels[c].name, info.channels[c].name.c_str(), 255);
#else
    strncpy(exr_header->channels[c].name, info.channels[c].name.c_str(), 255);
#endif
    // manually add '\0' for safety.
    exr_header->channels[c].name[255] = '\0';

    exr_header->channels[c].pixel_type = info.channels[c].pixel_type;
    exr_header->channels[c].p_linear = info.channels[c].p_linear;
    exr_header->channels[c].x_sampling = info.channels[c].x_sampling;
    exr_header->channels[c].y_sampling = info.channels[c].y_sampling;
  }

  exr_header->pixel_types = static_cast<int *>(
      malloc(sizeof(int) * static_cast<size_t>(exr_header->num_channels)));
  for (size_t c = 0; c < static_cast<size_t>(exr_header->num_channels); c++) {
    exr_header->pixel_types[c] = info.channels[c].pixel_type;
  }

  // Initially fill with values of `pixel_types`
  exr_header->requested_pixel_types = static_cast<int *>(
      malloc(sizeof(int) * static_cast<size_t>(exr_header->num_channels)));
  for (size_t c = 0; c < static_cast<size_t>(exr_header->num_channels); c++) {
    exr_header->requested_pixel_types[c] = info.channels[c].pixel_type;
  }

  exr_header->num_custom_attributes = static_cast<int>(info.attributes.size());

  if (exr_header->num_custom_attributes > 0) {
    // TODO(syoyo): Report warning when # of attributes exceeds
    // `TINYEXR_MAX_CUSTOM_ATTRIBUTES`
    if (exr_header->num_custom_attributes > TINYEXR_MAX_CUSTOM_ATTRIBUTES) {
      exr_header->num_custom_attributes = TINYEXR_MAX_CUSTOM_ATTRIBUTES;
    }

    exr_header->custom_attributes = static_cast<EXRAttribute *>(malloc(
        sizeof(EXRAttribute) * size_t(exr_header->num_custom_attributes)));

    for (size_t i = 0; i < info.attributes.size(); i++) {
      memcpy(exr_header->custom_attributes[i].name, info.attributes[i].name,
             256);
      memcpy(exr_header->custom_attributes[i].type, info.attributes[i].type,
             256);
      exr_header->custom_attributes[i].size = info.attributes[i].size;
      // Just copy poiner
      exr_header->custom_attributes[i].value = info.attributes[i].value;
    }

  } else {
    exr_header->custom_attributes = NULL;
  }

  exr_header->header_len = info.header_len;
}

static int DecodeChunk(EXRImage *exr_image, const EXRHeader *exr_header,
                       const std::vector<tinyexr::tinyexr_uint64> &offsets,
                       const unsigned char *head, const size_t size,
                       std::string *err) {
  int num_channels = exr_header->num_channels;

  int num_scanline_blocks = 1;
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZIP) {
    num_scanline_blocks = 16;
  } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
    num_scanline_blocks = 32;
  } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
    num_scanline_blocks = 16;
  }

  int data_width = exr_header->data_window[2] - exr_header->data_window[0] + 1;
  int data_height = exr_header->data_window[3] - exr_header->data_window[1] + 1;

  if ((data_width < 0) || (data_height < 0)) {
    if (err) {
      std::stringstream ss;
      ss << "Invalid data width or data height: " << data_width << ", "
         << data_height << std::endl;
      (*err) += ss.str();
    }
    return TINYEXR_ERROR_INVALID_DATA;
  }

  // Do not allow too large data_width and data_height. header invalid?
  {
    const int threshold = 1024 * 8192;  // heuristics
    if ((data_width > threshold) || (data_height > threshold)) {
      if (err) {
        std::stringstream ss;
        ss << "data_with or data_height too large. data_width: " << data_width
           << ", "
           << "data_height = " << data_height << std::endl;
        (*err) += ss.str();
      }
      return TINYEXR_ERROR_INVALID_DATA;
    }
  }

  size_t num_blocks = offsets.size();

  std::vector<size_t> channel_offset_list;
  int pixel_data_size = 0;
  size_t channel_offset = 0;
  if (!tinyexr::ComputeChannelLayout(&channel_offset_list, &pixel_data_size,
                                     &channel_offset, num_channels,
                                     exr_header->channels)) {
    if (err) {
      (*err) += "Failed to compute channel layout.\n";
    }
    return TINYEXR_ERROR_INVALID_DATA;
  }

  bool invalid_data = false;  // TODO(LTE): Use atomic lock for MT safety.

  if (exr_header->tiled) {
    // value check
    if (exr_header->tile_size_x < 0) {
      if (err) {
        std::stringstream ss;
        ss << "Invalid tile size x : " << exr_header->tile_size_x << "\n";
        (*err) += ss.str();
      }
      return TINYEXR_ERROR_INVALID_HEADER;
    }

    if (exr_header->tile_size_y < 0) {
      if (err) {
        std::stringstream ss;
        ss << "Invalid tile size y : " << exr_header->tile_size_y << "\n";
        (*err) += ss.str();
      }
      return TINYEXR_ERROR_INVALID_HEADER;
    }

    size_t num_tiles = offsets.size();  // = # of blocks

    exr_image->tiles = static_cast<EXRTile *>(
        calloc(sizeof(EXRTile), static_cast<size_t>(num_tiles)));

    int err_code = TINYEXR_SUCCESS;

#if TINYEXR_USE_THREAD > 0

    std::vector<std::thread> workers;
    std::atomic<size_t> tile_count(0);

    int num_threads = std::max(1, int(std::thread::hardware_concurrency()));
    if (num_threads > int(num_tiles)) {
      num_threads = int(num_tiles);
    }

    for (int t = 0; t < num_threads; t++) {
      workers.emplace_back(std::thread([&]() {
        size_t tile_idx = 0;
        while ((tile_idx = tile_count++) < num_tiles) {

#else
    for (size_t tile_idx = 0; tile_idx < num_tiles; tile_idx++) {
#endif
          // Allocate memory for each tile.
          exr_image->tiles[tile_idx].images = tinyexr::AllocateImage(
              num_channels, exr_header->channels,
              exr_header->requested_pixel_types, exr_header->tile_size_x,
              exr_header->tile_size_y);

          // 16 byte: tile coordinates
          // 4 byte : data size
          // ~      : data(uncompressed or compressed)
          if (offsets[tile_idx] + sizeof(int) * 5 > size) {
            // TODO(LTE): atomic
            if (err) {
              (*err) += "Insufficient data size.\n";
            }
            err_code = TINYEXR_ERROR_INVALID_DATA;
            break;
          }

          size_t data_size =
              size_t(size - (offsets[tile_idx] + sizeof(int) * 5));
          const unsigned char *data_ptr =
              reinterpret_cast<const unsigned char *>(head + offsets[tile_idx]);

          int tile_coordinates[4];
          memcpy(tile_coordinates, data_ptr, sizeof(int) * 4);
          tinyexr::swap4(
              reinterpret_cast<unsigned int *>(&tile_coordinates[0]));
          tinyexr::swap4(
              reinterpret_cast<unsigned int *>(&tile_coordinates[1]));
          tinyexr::swap4(
              reinterpret_cast<unsigned int *>(&tile_coordinates[2]));
          tinyexr::swap4(
              reinterpret_cast<unsigned int *>(&tile_coordinates[3]));

          // @todo{ LoD }
          if (tile_coordinates[2] != 0) {
            err_code = TINYEXR_ERROR_UNSUPPORTED_FEATURE;
            break;
          }
          if (tile_coordinates[3] != 0) {
            err_code = TINYEXR_ERROR_UNSUPPORTED_FEATURE;
            break;
          }

          int data_len;
          memcpy(&data_len, data_ptr + 16,
                 sizeof(int));  // 16 = sizeof(tile_coordinates)
          tinyexr::swap4(reinterpret_cast<unsigned int *>(&data_len));

          if (data_len < 4 || size_t(data_len) > data_size) {
            // TODO(LTE): atomic
            if (err) {
              (*err) += "Insufficient data length.\n";
            }
            err_code = TINYEXR_ERROR_INVALID_DATA;
            break;
          }

          // Move to data addr: 20 = 16 + 4;
          data_ptr += 20;

          bool ret = tinyexr::DecodeTiledPixelData(
              exr_image->tiles[tile_idx].images,
              &(exr_image->tiles[tile_idx].width),
              &(exr_image->tiles[tile_idx].height),
              exr_header->requested_pixel_types, data_ptr,
              static_cast<size_t>(data_len), exr_header->compression_type,
              exr_header->line_order, data_width, data_height,
              tile_coordinates[0], tile_coordinates[1], exr_header->tile_size_x,
              exr_header->tile_size_y, static_cast<size_t>(pixel_data_size),
              static_cast<size_t>(exr_header->num_custom_attributes),
              exr_header->custom_attributes,
              static_cast<size_t>(exr_header->num_channels),
              exr_header->channels, channel_offset_list);

          if (!ret) {
            // TODO(LTE): atomic
            if (err) {
              (*err) += "Failed to decode tile data.\n";
            }
            err_code = TINYEXR_ERROR_INVALID_DATA;
          }

          exr_image->tiles[tile_idx].offset_x = tile_coordinates[0];
          exr_image->tiles[tile_idx].offset_y = tile_coordinates[1];
          exr_image->tiles[tile_idx].level_x = tile_coordinates[2];
          exr_image->tiles[tile_idx].level_y = tile_coordinates[3];

#if TINYEXR_USE_THREAD > 0
        }
      }));
    }  // num_thread loop

    for (auto &t : workers) {
      t.join();
    }

#else
    }
#endif

    if (err_code != TINYEXR_SUCCESS) {
      return err_code;
    }

    exr_image->num_tiles = static_cast<int>(num_tiles);
  } else {  // scanline format

    // Don't allow too large image(256GB * pixel_data_size or more). Workaround
    // for #104.
    size_t total_data_len =
        size_t(data_width) * size_t(data_height) * size_t(num_channels);
    const bool total_data_len_overflown =
        sizeof(void *) == 8 ? (total_data_len >= 0x4000000000) : false;
    if ((total_data_len == 0) || total_data_len_overflown) {
      if (err) {
        std::stringstream ss;
        ss << "Image data size is zero or too large: width = " << data_width
           << ", height = " << data_height << ", channels = " << num_channels
           << std::endl;
        (*err) += ss.str();
      }
      return TINYEXR_ERROR_INVALID_DATA;
    }

    exr_image->images = tinyexr::AllocateImage(
        num_channels, exr_header->channels, exr_header->requested_pixel_types,
        data_width, data_height);

#if TINYEXR_USE_THREAD > 0
    std::vector<std::thread> workers;
    std::atomic<int> y_count(0);

    int num_threads = std::max(1, int(std::thread::hardware_concurrency()));
    if (num_threads > int(num_blocks)) {
      num_threads = int(num_blocks);
    }

    for (int t = 0; t < num_threads; t++) {
      workers.emplace_back(std::thread([&]() {
        int y = 0;
        while ((y = y_count++) < int(num_blocks)) {

#else

#if TINYEXR_USE_OPENMP
#pragma omp parallel for
#endif
    for (int y = 0; y < static_cast<int>(num_blocks); y++) {

#endif
          size_t y_idx = static_cast<size_t>(y);

          if (offsets[y_idx] + sizeof(int) * 2 > size) {
            invalid_data = true;
          } else {
            // 4 byte: scan line
            // 4 byte: data size
            // ~     : pixel data(uncompressed or compressed)
            size_t data_size =
                size_t(size - (offsets[y_idx] + sizeof(int) * 2));
            const unsigned char *data_ptr =
                reinterpret_cast<const unsigned char *>(head + offsets[y_idx]);

            int line_no;
            memcpy(&line_no, data_ptr, sizeof(int));
            int data_len;
            memcpy(&data_len, data_ptr + 4, sizeof(int));
            tinyexr::swap4(reinterpret_cast<unsigned int *>(&line_no));
            tinyexr::swap4(reinterpret_cast<unsigned int *>(&data_len));

            if (size_t(data_len) > data_size) {
              invalid_data = true;

            } else if ((line_no > (2 << 20)) || (line_no < -(2 << 20))) {
              // Too large value. Assume this is invalid
              // 2**20 = 1048576 = heuristic value.
              invalid_data = true;
            } else if (data_len == 0) {
              // TODO(syoyo): May be ok to raise the threshold for example
              // `data_len < 4`
              invalid_data = true;
            } else {
              // line_no may be negative.
              int end_line_no = (std::min)(line_no + num_scanline_blocks,
                                           (exr_header->data_window[3] + 1));

              int num_lines = end_line_no - line_no;

              if (num_lines <= 0) {
                invalid_data = true;
              } else {
                // Move to data addr: 8 = 4 + 4;
                data_ptr += 8;

                // Adjust line_no with data_window.bmin.y

                // overflow check
                tinyexr_int64 lno =
                    static_cast<tinyexr_int64>(line_no) -
                    static_cast<tinyexr_int64>(exr_header->data_window[1]);
                if (lno > std::numeric_limits<int>::max()) {
                  line_no = -1;  // invalid
                } else if (lno < -std::numeric_limits<int>::max()) {
                  line_no = -1;  // invalid
                } else {
                  line_no -= exr_header->data_window[1];
                }

                if (line_no < 0) {
                  invalid_data = true;
                } else {
                  if (!tinyexr::DecodePixelData(
                          exr_image->images, exr_header->requested_pixel_types,
                          data_ptr, static_cast<size_t>(data_len),
                          exr_header->compression_type, exr_header->line_order,
                          data_width, data_height, data_width, y, line_no,
                          num_lines, static_cast<size_t>(pixel_data_size),
                          static_cast<size_t>(
                              exr_header->num_custom_attributes),
                          exr_header->custom_attributes,
                          static_cast<size_t>(exr_header->num_channels),
                          exr_header->channels, channel_offset_list)) {
                    invalid_data = true;
                  }
                }
              }
            }
          }

#if TINYEXR_USE_THREAD > 0
        }
      }));
    }

    for (auto &t : workers) {
      t.join();
    }
#else
    }  // omp parallel
#endif
  }

  if (invalid_data) {
    if (err) {
      std::stringstream ss;
      (*err) += "Invalid data found when decoding pixels.\n";
    }
    return TINYEXR_ERROR_INVALID_DATA;
  }

  // Overwrite `pixel_type` with `requested_pixel_type`.
  {
    for (int c = 0; c < exr_header->num_channels; c++) {
      exr_header->pixel_types[c] = exr_header->requested_pixel_types[c];
    }
  }

  {
    exr_image->num_channels = num_channels;

    exr_image->width = data_width;
    exr_image->height = data_height;
  }

  return TINYEXR_SUCCESS;
}

static bool ReconstructLineOffsets(
    std::vector<tinyexr::tinyexr_uint64> *offsets, size_t n,
    const unsigned char *head, const unsigned char *marker, const size_t size) {
  assert(head < marker);
  assert(offsets->size() == n);

  for (size_t i = 0; i < n; i++) {
    size_t offset = static_cast<size_t>(marker - head);
    // Offset should not exceed whole EXR file/data size.
    if ((offset + sizeof(tinyexr::tinyexr_uint64)) >= size) {
      return false;
    }

    int y;
    unsigned int data_len;

    memcpy(&y, marker, sizeof(int));
    memcpy(&data_len, marker + 4, sizeof(unsigned int));

    if (data_len >= size) {
      return false;
    }

    tinyexr::swap4(reinterpret_cast<unsigned int *>(&y));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&data_len));

    (*offsets)[i] = offset;

    marker += data_len + 8;  // 8 = 4 bytes(y) + 4 bytes(data_len)
  }

  return true;
}

static int DecodeEXRImage(EXRImage *exr_image, const EXRHeader *exr_header,
                          const unsigned char *head,
                          const unsigned char *marker, const size_t size,
                          const char **err) {
  if (exr_image == NULL || exr_header == NULL || head == NULL ||
      marker == NULL || (size <= tinyexr::kEXRVersionSize)) {
    tinyexr::SetErrorMessage("Invalid argument for DecodeEXRImage().", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  int num_scanline_blocks = 1;
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZIP) {
    num_scanline_blocks = 16;
  } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
    num_scanline_blocks = 32;
  } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
    num_scanline_blocks = 16;
  }

  int data_width = exr_header->data_window[2] - exr_header->data_window[0];
  if (data_width >= std::numeric_limits<int>::max()) {
    // Issue 63
    tinyexr::SetErrorMessage("Invalid data width value", err);
    return TINYEXR_ERROR_INVALID_DATA;
  }
  data_width++;

  int data_height = exr_header->data_window[3] - exr_header->data_window[1];
  if (data_height >= std::numeric_limits<int>::max()) {
    tinyexr::SetErrorMessage("Invalid data height value", err);
    return TINYEXR_ERROR_INVALID_DATA;
  }
  data_height++;

  if ((data_width < 0) || (data_height < 0)) {
    tinyexr::SetErrorMessage("data width or data height is negative.", err);
    return TINYEXR_ERROR_INVALID_DATA;
  }

  // Do not allow too large data_width and data_height. header invalid?
  {
    const int threshold = 1024 * 8192;  // heuristics
    if (data_width > threshold) {
      tinyexr::SetErrorMessage("data width too large.", err);
      return TINYEXR_ERROR_INVALID_DATA;
    }
    if (data_height > threshold) {
      tinyexr::SetErrorMessage("data height too large.", err);
      return TINYEXR_ERROR_INVALID_DATA;
    }
  }

  // Read offset tables.
  size_t num_blocks = 0;

  if (exr_header->chunk_count > 0) {
    // Use `chunkCount` attribute.
    num_blocks = static_cast<size_t>(exr_header->chunk_count);
  } else if (exr_header->tiled) {
    // @todo { LoD }
    size_t num_x_tiles = static_cast<size_t>(data_width) /
                         static_cast<size_t>(exr_header->tile_size_x);
    if (num_x_tiles * static_cast<size_t>(exr_header->tile_size_x) <
        static_cast<size_t>(data_width)) {
      num_x_tiles++;
    }
    size_t num_y_tiles = static_cast<size_t>(data_height) /
                         static_cast<size_t>(exr_header->tile_size_y);
    if (num_y_tiles * static_cast<size_t>(exr_header->tile_size_y) <
        static_cast<size_t>(data_height)) {
      num_y_tiles++;
    }

    num_blocks = num_x_tiles * num_y_tiles;
  } else {
    num_blocks = static_cast<size_t>(data_height) /
                 static_cast<size_t>(num_scanline_blocks);
    if (num_blocks * static_cast<size_t>(num_scanline_blocks) <
        static_cast<size_t>(data_height)) {
      num_blocks++;
    }
  }

  std::vector<tinyexr::tinyexr_uint64> offsets(num_blocks);

  for (size_t y = 0; y < num_blocks; y++) {
    tinyexr::tinyexr_uint64 offset;
    // Issue #81
    if ((marker + sizeof(tinyexr_uint64)) >= (head + size)) {
      tinyexr::SetErrorMessage("Insufficient data size in offset table.", err);
      return TINYEXR_ERROR_INVALID_DATA;
    }

    memcpy(&offset, marker, sizeof(tinyexr::tinyexr_uint64));
    tinyexr::swap8(&offset);
    if (offset >= size) {
      tinyexr::SetErrorMessage("Invalid offset value in DecodeEXRImage.", err);
      return TINYEXR_ERROR_INVALID_DATA;
    }
    marker += sizeof(tinyexr::tinyexr_uint64);  // = 8
    offsets[y] = offset;
  }

  // If line offsets are invalid, we try to reconstruct it.
  // See OpenEXR/IlmImf/ImfScanLineInputFile.cpp::readLineOffsets() for details.
  for (size_t y = 0; y < num_blocks; y++) {
    if (offsets[y] <= 0) {
      // TODO(syoyo) Report as warning?
      // if (err) {
      //  stringstream ss;
      //  ss << "Incomplete lineOffsets." << std::endl;
      //  (*err) += ss.str();
      //}
      bool ret =
          ReconstructLineOffsets(&offsets, num_blocks, head, marker, size);
      if (ret) {
        // OK
        break;
      } else {
        tinyexr::SetErrorMessage(
            "Cannot reconstruct lineOffset table in DecodeEXRImage.", err);
        return TINYEXR_ERROR_INVALID_DATA;
      }
    }
  }

  {
    std::string e;
    int ret = DecodeChunk(exr_image, exr_header, offsets, head, size, &e);

    if (ret != TINYEXR_SUCCESS) {
      if (!e.empty()) {
        tinyexr::SetErrorMessage(e, err);
      }

#if 1
      FreeEXRImage(exr_image);
#else
      // release memory(if exists)
      if ((exr_header->num_channels > 0) && exr_image && exr_image->images) {
        for (size_t c = 0; c < size_t(exr_header->num_channels); c++) {
          if (exr_image->images[c]) {
            free(exr_image->images[c]);
            exr_image->images[c] = NULL;
          }
        }
        free(exr_image->images);
        exr_image->images = NULL;
      }
#endif
    }

    return ret;
  }
}

static void GetLayers(const EXRHeader& exr_header, std::vector<std::string>& layer_names) {
  // Naive implementation
  // Group channels by layers
  // go over all channel names, split by periods
  // collect unique names
  layer_names.clear();
  for (int c = 0; c < exr_header.num_channels; c++) {
    std::string full_name(exr_header.channels[c].name);
    const size_t pos = full_name.find_last_of('.');
    if (pos != std::string::npos && pos != 0 && pos + 1 < full_name.size()) {
      full_name.erase(pos);
      if (std::find(layer_names.begin(), layer_names.end(), full_name) == layer_names.end())
        layer_names.push_back(full_name);
    }
  }
}

struct LayerChannel {
  explicit LayerChannel (size_t i, std::string n)
    : index(i)
    , name(n)
  {}
  size_t index;
  std::string name;
};

static void ChannelsInLayer(const EXRHeader& exr_header, const std::string layer_name, std::vector<LayerChannel>& channels) {
  channels.clear();
  for (int c = 0; c < exr_header.num_channels; c++) {
    std::string ch_name(exr_header.channels[c].name);
    if (layer_name.empty()) {
      const size_t pos = ch_name.find_last_of('.');
      if (pos != std::string::npos && pos < ch_name.size()) {
        ch_name = ch_name.substr(pos + 1);
      }
    } else {
      const size_t pos = ch_name.find(layer_name + '.');
      if (pos == std::string::npos)
        continue;
      if (pos == 0) {
        ch_name = ch_name.substr(layer_name.size() + 1);
      }
    }
    LayerChannel ch(size_t(c), ch_name);
    channels.push_back(ch);
  }
}

}  // namespace tinyexr

int EXRLayers(const char *filename, const char **layer_names[], int *num_layers, const char **err) {
  EXRVersion exr_version;
  EXRHeader exr_header;
  InitEXRHeader(&exr_header);

  {
    int ret = ParseEXRVersionFromFile(&exr_version, filename);
    if (ret != TINYEXR_SUCCESS) {
      tinyexr::SetErrorMessage("Invalid EXR header.", err);
      return ret;
    }

    if (exr_version.multipart || exr_version.non_image) {
      tinyexr::SetErrorMessage(
        "Loading multipart or DeepImage is not supported  in LoadEXR() API",
        err);
      return TINYEXR_ERROR_INVALID_DATA;  // @fixme.
    }
  }

  int ret = ParseEXRHeaderFromFile(&exr_header, &exr_version, filename, err);
  if (ret != TINYEXR_SUCCESS) {
    FreeEXRHeader(&exr_header);
    return ret;
  }

  std::vector<std::string> layer_vec;
  tinyexr::GetLayers(exr_header, layer_vec);

  (*num_layers) = int(layer_vec.size());
  (*layer_names) = static_cast<const char **>(
    malloc(sizeof(const char *) * static_cast<size_t>(layer_vec.size())));
  for (size_t c = 0; c < static_cast<size_t>(layer_vec.size()); c++) {
#ifdef _MSC_VER
    (*layer_names)[c] = _strdup(layer_vec[c].c_str());
#else
    (*layer_names)[c] = strdup(layer_vec[c].c_str());
#endif
  }

  FreeEXRHeader(&exr_header);
  return TINYEXR_SUCCESS;
}

int LoadEXR(float **out_rgba, int *width, int *height, const char *filename,
            const char **err) {
  return LoadEXRWithLayer(out_rgba, width, height, filename, /* layername */NULL, err);
}

int LoadEXRWithLayer(float **out_rgba, int *width, int *height, const char *filename, const char *layername,
            const char **err) {
  if (out_rgba == NULL) {
    tinyexr::SetErrorMessage("Invalid argument for LoadEXR()", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  EXRVersion exr_version;
  EXRImage exr_image;
  EXRHeader exr_header;
  InitEXRHeader(&exr_header);
  InitEXRImage(&exr_image);

  {
    int ret = ParseEXRVersionFromFile(&exr_version, filename);
    if (ret != TINYEXR_SUCCESS) {
      std::stringstream ss;
      ss << "Failed to open EXR file or read version info from EXR file. code(" << ret << ")";
      tinyexr::SetErrorMessage(ss.str(), err);
      return ret;
    }

    if (exr_version.multipart || exr_version.non_image) {
      tinyexr::SetErrorMessage(
          "Loading multipart or DeepImage is not supported  in LoadEXR() API",
          err);
      return TINYEXR_ERROR_INVALID_DATA;  // @fixme.
    }
  }

  {
    int ret = ParseEXRHeaderFromFile(&exr_header, &exr_version, filename, err);
    if (ret != TINYEXR_SUCCESS) {
      FreeEXRHeader(&exr_header);
      return ret;
    }
  }

  // Read HALF channel as FLOAT.
  for (int i = 0; i < exr_header.num_channels; i++) {
    if (exr_header.pixel_types[i] == TINYEXR_PIXELTYPE_HALF) {
      exr_header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
    }
  }

  // TODO: Probably limit loading to layers (channels) selected by layer index
  {
    int ret = LoadEXRImageFromFile(&exr_image, &exr_header, filename, err);
    if (ret != TINYEXR_SUCCESS) {
      FreeEXRHeader(&exr_header);
      return ret;
    }
  }

  // RGBA
  int idxR = -1;
  int idxG = -1;
  int idxB = -1;
  int idxA = -1;

  std::vector<std::string> layer_names;
  tinyexr::GetLayers(exr_header, layer_names);

  std::vector<tinyexr::LayerChannel> channels;
  tinyexr::ChannelsInLayer(exr_header, layername == NULL ? "" : std::string(layername), channels);

  if (channels.size() < 1) {
    tinyexr::SetErrorMessage("Layer Not Found", err);
    FreeEXRHeader(&exr_header);
    FreeEXRImage(&exr_image);
    return TINYEXR_ERROR_LAYER_NOT_FOUND;
  }

  size_t ch_count = channels.size() < 4 ? channels.size() : 4;
  for (size_t c = 0; c < ch_count; c++) {
    const tinyexr::LayerChannel &ch = channels[c];

    if (ch.name == "R") {
      idxR = int(ch.index);
    }
    else if (ch.name == "G") {
      idxG = int(ch.index);
    }
    else if (ch.name == "B") {
      idxB = int(ch.index);
    }
    else if (ch.name == "A") {
      idxA = int(ch.index);
    }
  }

  if (channels.size() == 1) {
    int chIdx = int(channels.front().index);
    // Grayscale channel only.

    (*out_rgba) = reinterpret_cast<float *>(
        malloc(4 * sizeof(float) * static_cast<size_t>(exr_image.width) *
               static_cast<size_t>(exr_image.height)));

    if (exr_header.tiled) {
      for (int it = 0; it < exr_image.num_tiles; it++) {
        for (int j = 0; j < exr_header.tile_size_y; j++) {
          for (int i = 0; i < exr_header.tile_size_x; i++) {
            const int ii =
                exr_image.tiles[it].offset_x * exr_header.tile_size_x + i;
            const int jj =
                exr_image.tiles[it].offset_y * exr_header.tile_size_y + j;
            const int idx = ii + jj * exr_image.width;

            // out of region check.
            if (ii >= exr_image.width) {
              continue;
            }
            if (jj >= exr_image.height) {
              continue;
            }
            const int srcIdx = i + j * exr_header.tile_size_x;
            unsigned char **src = exr_image.tiles[it].images;
            (*out_rgba)[4 * idx + 0] =
                reinterpret_cast<float **>(src)[chIdx][srcIdx];
            (*out_rgba)[4 * idx + 1] =
                reinterpret_cast<float **>(src)[chIdx][srcIdx];
            (*out_rgba)[4 * idx + 2] =
                reinterpret_cast<float **>(src)[chIdx][srcIdx];
            (*out_rgba)[4 * idx + 3] =
                reinterpret_cast<float **>(src)[chIdx][srcIdx];
          }
        }
      }
    } else {
      for (int i = 0; i < exr_image.width * exr_image.height; i++) {
        const float val = reinterpret_cast<float **>(exr_image.images)[chIdx][i];
        (*out_rgba)[4 * i + 0] = val;
        (*out_rgba)[4 * i + 1] = val;
        (*out_rgba)[4 * i + 2] = val;
        (*out_rgba)[4 * i + 3] = val;
      }
    }
  } else {
    // Assume RGB(A)

    if (idxR == -1) {
      tinyexr::SetErrorMessage("R channel not found", err);

      FreeEXRHeader(&exr_header);
      FreeEXRImage(&exr_image);
      return TINYEXR_ERROR_INVALID_DATA;
    }

    if (idxG == -1) {
      tinyexr::SetErrorMessage("G channel not found", err);
      FreeEXRHeader(&exr_header);
      FreeEXRImage(&exr_image);
      return TINYEXR_ERROR_INVALID_DATA;
    }

    if (idxB == -1) {
      tinyexr::SetErrorMessage("B channel not found", err);
      FreeEXRHeader(&exr_header);
      FreeEXRImage(&exr_image);
      return TINYEXR_ERROR_INVALID_DATA;
    }

    (*out_rgba) = reinterpret_cast<float *>(
        malloc(4 * sizeof(float) * static_cast<size_t>(exr_image.width) *
               static_cast<size_t>(exr_image.height)));
    if (exr_header.tiled) {
      for (int it = 0; it < exr_image.num_tiles; it++) {
        for (int j = 0; j < exr_header.tile_size_y; j++) {
          for (int i = 0; i < exr_header.tile_size_x; i++) {
            const int ii =
                exr_image.tiles[it].offset_x * exr_header.tile_size_x + i;
            const int jj =
                exr_image.tiles[it].offset_y * exr_header.tile_size_y + j;
            const int idx = ii + jj * exr_image.width;

            // out of region check.
            if (ii >= exr_image.width) {
              continue;
            }
            if (jj >= exr_image.height) {
              continue;
            }
            const int srcIdx = i + j * exr_header.tile_size_x;
            unsigned char **src = exr_image.tiles[it].images;
            (*out_rgba)[4 * idx + 0] =
                reinterpret_cast<float **>(src)[idxR][srcIdx];
            (*out_rgba)[4 * idx + 1] =
                reinterpret_cast<float **>(src)[idxG][srcIdx];
            (*out_rgba)[4 * idx + 2] =
                reinterpret_cast<float **>(src)[idxB][srcIdx];
            if (idxA != -1) {
              (*out_rgba)[4 * idx + 3] =
                  reinterpret_cast<float **>(src)[idxA][srcIdx];
            } else {
              (*out_rgba)[4 * idx + 3] = 1.0;
            }
          }
        }
      }
    } else {
      for (int i = 0; i < exr_image.width * exr_image.height; i++) {
        (*out_rgba)[4 * i + 0] =
            reinterpret_cast<float **>(exr_image.images)[idxR][i];
        (*out_rgba)[4 * i + 1] =
            reinterpret_cast<float **>(exr_image.images)[idxG][i];
        (*out_rgba)[4 * i + 2] =
            reinterpret_cast<float **>(exr_image.images)[idxB][i];
        if (idxA != -1) {
          (*out_rgba)[4 * i + 3] =
              reinterpret_cast<float **>(exr_image.images)[idxA][i];
        } else {
          (*out_rgba)[4 * i + 3] = 1.0;
        }
      }
    }
  }

  (*width) = exr_image.width;
  (*height) = exr_image.height;

  FreeEXRHeader(&exr_header);
  FreeEXRImage(&exr_image);

  return TINYEXR_SUCCESS;
}

int IsEXR(const char *filename) {
  EXRVersion exr_version;

  int ret = ParseEXRVersionFromFile(&exr_version, filename);
  if (ret != TINYEXR_SUCCESS) {
    return ret;
  }

  return TINYEXR_SUCCESS;
}

int ParseEXRHeaderFromMemory(EXRHeader *exr_header, const EXRVersion *version,
                             const unsigned char *memory, size_t size,
                             const char **err) {
  if (memory == NULL || exr_header == NULL) {
    tinyexr::SetErrorMessage(
        "Invalid argument. `memory` or `exr_header` argument is null in "
        "ParseEXRHeaderFromMemory()",
        err);

    // Invalid argument
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  if (size < tinyexr::kEXRVersionSize) {
    tinyexr::SetErrorMessage("Insufficient header/data size.\n", err);
    return TINYEXR_ERROR_INVALID_DATA;
  }

  const unsigned char *marker = memory + tinyexr::kEXRVersionSize;
  size_t marker_size = size - tinyexr::kEXRVersionSize;

  tinyexr::HeaderInfo info;
  info.clear();

  std::string err_str;
  int ret = ParseEXRHeader(&info, NULL, version, &err_str, marker, marker_size);

  if (ret != TINYEXR_SUCCESS) {
    if (err && !err_str.empty()) {
      tinyexr::SetErrorMessage(err_str, err);
    }
  }

  ConvertHeader(exr_header, info);

  // transfoer `tiled` from version.
  exr_header->tiled = version->tiled;

  return ret;
}

int LoadEXRFromMemory(float **out_rgba, int *width, int *height,
                      const unsigned char *memory, size_t size,
                      const char **err) {
  if (out_rgba == NULL || memory == NULL) {
    tinyexr::SetErrorMessage("Invalid argument for LoadEXRFromMemory", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  EXRVersion exr_version;
  EXRImage exr_image;
  EXRHeader exr_header;

  InitEXRHeader(&exr_header);

  int ret = ParseEXRVersionFromMemory(&exr_version, memory, size);
  if (ret != TINYEXR_SUCCESS) {
    std::stringstream ss;
    ss << "Failed to parse EXR version. code(" << ret << ")";
    tinyexr::SetErrorMessage(ss.str(), err);
    return ret;
  }

  ret = ParseEXRHeaderFromMemory(&exr_header, &exr_version, memory, size, err);
  if (ret != TINYEXR_SUCCESS) {
    return ret;
  }

  // Read HALF channel as FLOAT.
  for (int i = 0; i < exr_header.num_channels; i++) {
    if (exr_header.pixel_types[i] == TINYEXR_PIXELTYPE_HALF) {
      exr_header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
    }
  }

  InitEXRImage(&exr_image);
  ret = LoadEXRImageFromMemory(&exr_image, &exr_header, memory, size, err);
  if (ret != TINYEXR_SUCCESS) {
    return ret;
  }

  // RGBA
  int idxR = -1;
  int idxG = -1;
  int idxB = -1;
  int idxA = -1;
  for (int c = 0; c < exr_header.num_channels; c++) {
    if (strcmp(exr_header.channels[c].name, "R") == 0) {
      idxR = c;
    } else if (strcmp(exr_header.channels[c].name, "G") == 0) {
      idxG = c;
    } else if (strcmp(exr_header.channels[c].name, "B") == 0) {
      idxB = c;
    } else if (strcmp(exr_header.channels[c].name, "A") == 0) {
      idxA = c;
    }
  }

  // TODO(syoyo): Refactor removing same code as used in LoadEXR().
  if (exr_header.num_channels == 1) {
    // Grayscale channel only.

    (*out_rgba) = reinterpret_cast<float *>(
        malloc(4 * sizeof(float) * static_cast<size_t>(exr_image.width) *
               static_cast<size_t>(exr_image.height)));

    if (exr_header.tiled) {
      for (int it = 0; it < exr_image.num_tiles; it++) {
        for (int j = 0; j < exr_header.tile_size_y; j++) {
          for (int i = 0; i < exr_header.tile_size_x; i++) {
            const int ii =
                exr_image.tiles[it].offset_x * exr_header.tile_size_x + i;
            const int jj =
                exr_image.tiles[it].offset_y * exr_header.tile_size_y + j;
            const int idx = ii + jj * exr_image.width;

            // out of region check.
            if (ii >= exr_image.width) {
              continue;
            }
            if (jj >= exr_image.height) {
              continue;
            }
            const int srcIdx = i + j * exr_header.tile_size_x;
            unsigned char **src = exr_image.tiles[it].images;
            (*out_rgba)[4 * idx + 0] =
                reinterpret_cast<float **>(src)[0][srcIdx];
            (*out_rgba)[4 * idx + 1] =
                reinterpret_cast<float **>(src)[0][srcIdx];
            (*out_rgba)[4 * idx + 2] =
                reinterpret_cast<float **>(src)[0][srcIdx];
            (*out_rgba)[4 * idx + 3] =
                reinterpret_cast<float **>(src)[0][srcIdx];
          }
        }
      }
    } else {
      for (int i = 0; i < exr_image.width * exr_image.height; i++) {
        const float val = reinterpret_cast<float **>(exr_image.images)[0][i];
        (*out_rgba)[4 * i + 0] = val;
        (*out_rgba)[4 * i + 1] = val;
        (*out_rgba)[4 * i + 2] = val;
        (*out_rgba)[4 * i + 3] = val;
      }
    }

  } else {
    // TODO(syoyo): Support non RGBA image.

    if (idxR == -1) {
      tinyexr::SetErrorMessage("R channel not found", err);

      // @todo { free exr_image }
      return TINYEXR_ERROR_INVALID_DATA;
    }

    if (idxG == -1) {
      tinyexr::SetErrorMessage("G channel not found", err);
      // @todo { free exr_image }
      return TINYEXR_ERROR_INVALID_DATA;
    }

    if (idxB == -1) {
      tinyexr::SetErrorMessage("B channel not found", err);
      // @todo { free exr_image }
      return TINYEXR_ERROR_INVALID_DATA;
    }

    (*out_rgba) = reinterpret_cast<float *>(
        malloc(4 * sizeof(float) * static_cast<size_t>(exr_image.width) *
               static_cast<size_t>(exr_image.height)));

    if (exr_header.tiled) {
      for (int it = 0; it < exr_image.num_tiles; it++) {
        for (int j = 0; j < exr_header.tile_size_y; j++)
          for (int i = 0; i < exr_header.tile_size_x; i++) {
            const int ii =
                exr_image.tiles[it].offset_x * exr_header.tile_size_x + i;
            const int jj =
                exr_image.tiles[it].offset_y * exr_header.tile_size_y + j;
            const int idx = ii + jj * exr_image.width;

            // out of region check.
            if (ii >= exr_image.width) {
              continue;
            }
            if (jj >= exr_image.height) {
              continue;
            }
            const int srcIdx = i + j * exr_header.tile_size_x;
            unsigned char **src = exr_image.tiles[it].images;
            (*out_rgba)[4 * idx + 0] =
                reinterpret_cast<float **>(src)[idxR][srcIdx];
            (*out_rgba)[4 * idx + 1] =
                reinterpret_cast<float **>(src)[idxG][srcIdx];
            (*out_rgba)[4 * idx + 2] =
                reinterpret_cast<float **>(src)[idxB][srcIdx];
            if (idxA != -1) {
              (*out_rgba)[4 * idx + 3] =
                  reinterpret_cast<float **>(src)[idxA][srcIdx];
            } else {
              (*out_rgba)[4 * idx + 3] = 1.0;
            }
          }
      }
    } else {
      for (int i = 0; i < exr_image.width * exr_image.height; i++) {
        (*out_rgba)[4 * i + 0] =
            reinterpret_cast<float **>(exr_image.images)[idxR][i];
        (*out_rgba)[4 * i + 1] =
            reinterpret_cast<float **>(exr_image.images)[idxG][i];
        (*out_rgba)[4 * i + 2] =
            reinterpret_cast<float **>(exr_image.images)[idxB][i];
        if (idxA != -1) {
          (*out_rgba)[4 * i + 3] =
              reinterpret_cast<float **>(exr_image.images)[idxA][i];
        } else {
          (*out_rgba)[4 * i + 3] = 1.0;
        }
      }
    }
  }

  (*width) = exr_image.width;
  (*height) = exr_image.height;

  FreeEXRHeader(&exr_header);
  FreeEXRImage(&exr_image);

  return TINYEXR_SUCCESS;
}

int LoadEXRImageFromFile(EXRImage *exr_image, const EXRHeader *exr_header,
                         const char *filename, const char **err) {
  if (exr_image == NULL) {
    tinyexr::SetErrorMessage("Invalid argument for LoadEXRImageFromFile", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

#ifdef _WIN32
  FILE *fp = NULL;
  fopen_s(&fp, filename, "rb");
#else
  FILE *fp = fopen(filename, "rb");
#endif
  if (!fp) {
    tinyexr::SetErrorMessage("Cannot read file " + std::string(filename), err);
    return TINYEXR_ERROR_CANT_OPEN_FILE;
  }

  size_t filesize;
  // Compute size
  fseek(fp, 0, SEEK_END);
  filesize = static_cast<size_t>(ftell(fp));
  fseek(fp, 0, SEEK_SET);

  if (filesize < 16) {
    tinyexr::SetErrorMessage("File size too short " + std::string(filename),
                             err);
    return TINYEXR_ERROR_INVALID_FILE;
  }

  std::vector<unsigned char> buf(filesize);  // @todo { use mmap }
  {
    size_t ret;
    ret = fread(&buf[0], 1, filesize, fp);
    assert(ret == filesize);
    fclose(fp);
    (void)ret;
  }

  return LoadEXRImageFromMemory(exr_image, exr_header, &buf.at(0), filesize,
                                err);
}

int LoadEXRImageFromMemory(EXRImage *exr_image, const EXRHeader *exr_header,
                           const unsigned char *memory, const size_t size,
                           const char **err) {
  if (exr_image == NULL || memory == NULL ||
      (size < tinyexr::kEXRVersionSize)) {
    tinyexr::SetErrorMessage("Invalid argument for LoadEXRImageFromMemory",
                             err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  if (exr_header->header_len == 0) {
    tinyexr::SetErrorMessage("EXRHeader variable is not initialized.", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  const unsigned char *head = memory;
  const unsigned char *marker = reinterpret_cast<const unsigned char *>(
      memory + exr_header->header_len +
      8);  // +8 for magic number + version header.
  return tinyexr::DecodeEXRImage(exr_image, exr_header, head, marker, size,
                                 err);
}

size_t SaveEXRImageToMemory(const EXRImage *exr_image,
                            const EXRHeader *exr_header,
                            unsigned char **memory_out, const char **err) {
  if (exr_image == NULL || memory_out == NULL ||
      exr_header->compression_type < 0) {
    tinyexr::SetErrorMessage("Invalid argument for SaveEXRImageToMemory", err);
    return 0;
  }

#if !TINYEXR_USE_PIZ
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
    tinyexr::SetErrorMessage("PIZ compression is not supported in this build",
                             err);
    return 0;
  }
#endif

#if !TINYEXR_USE_ZFP
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
    tinyexr::SetErrorMessage("ZFP compression is not supported in this build",
                             err);
    return 0;
  }
#endif

#if TINYEXR_USE_ZFP
  for (size_t i = 0; i < static_cast<size_t>(exr_header->num_channels); i++) {
    if (exr_header->requested_pixel_types[i] != TINYEXR_PIXELTYPE_FLOAT) {
      tinyexr::SetErrorMessage("Pixel type must be FLOAT for ZFP compression",
                               err);
      return 0;
    }
  }
#endif

  std::vector<unsigned char> memory;

  // Header
  {
    const char header[] = {0x76, 0x2f, 0x31, 0x01};
    memory.insert(memory.end(), header, header + 4);
  }

  // Version, scanline.
  {
    char marker[] = {2, 0, 0, 0};
    /* @todo
    if (exr_header->tiled) {
      marker[1] |= 0x2;
    }
    if (exr_header->long_name) {
      marker[1] |= 0x4;
    }
    if (exr_header->non_image) {
      marker[1] |= 0x8;
    }
    if (exr_header->multipart) {
      marker[1] |= 0x10;
    }
    */
    memory.insert(memory.end(), marker, marker + 4);
  }

  int num_scanlines = 1;
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZIP) {
    num_scanlines = 16;
  } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
    num_scanlines = 32;
  } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
    num_scanlines = 16;
  }

  // Write attributes.
  std::vector<tinyexr::ChannelInfo> channels;
  {
    std::vector<unsigned char> data;

    for (int c = 0; c < exr_header->num_channels; c++) {
      tinyexr::ChannelInfo info;
      info.p_linear = 0;
      info.pixel_type = exr_header->requested_pixel_types[c];
      info.x_sampling = 1;
      info.y_sampling = 1;
      info.name = std::string(exr_header->channels[c].name);
      channels.push_back(info);
    }

    tinyexr::WriteChannelInfo(data, channels);

    tinyexr::WriteAttributeToMemory(&memory, "channels", "chlist", &data.at(0),
                                    static_cast<int>(data.size()));
  }

  {
    int comp = exr_header->compression_type;
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&comp));
    tinyexr::WriteAttributeToMemory(
        &memory, "compression", "compression",
        reinterpret_cast<const unsigned char *>(&comp), 1);
  }

  {
    int data[4] = {0, 0, exr_image->width - 1, exr_image->height - 1};
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&data[0]));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&data[1]));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&data[2]));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&data[3]));
    tinyexr::WriteAttributeToMemory(
        &memory, "dataWindow", "box2i",
        reinterpret_cast<const unsigned char *>(data), sizeof(int) * 4);
    tinyexr::WriteAttributeToMemory(
        &memory, "displayWindow", "box2i",
        reinterpret_cast<const unsigned char *>(data), sizeof(int) * 4);
  }

  {
    unsigned char line_order = 0;  // @fixme { read line_order from EXRHeader }
    tinyexr::WriteAttributeToMemory(&memory, "lineOrder", "lineOrder",
                                    &line_order, 1);
  }

  {
    float aspectRatio = 1.0f;
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&aspectRatio));
    tinyexr::WriteAttributeToMemory(
        &memory, "pixelAspectRatio", "float",
        reinterpret_cast<const unsigned char *>(&aspectRatio), sizeof(float));
  }

  {
    float center[2] = {0.0f, 0.0f};
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&center[0]));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&center[1]));
    tinyexr::WriteAttributeToMemory(
        &memory, "screenWindowCenter", "v2f",
        reinterpret_cast<const unsigned char *>(center), 2 * sizeof(float));
  }

  {
    float w = static_cast<float>(exr_image->width);
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&w));
    tinyexr::WriteAttributeToMemory(&memory, "screenWindowWidth", "float",
                                    reinterpret_cast<const unsigned char *>(&w),
                                    sizeof(float));
  }

  // Custom attributes
  if (exr_header->num_custom_attributes > 0) {
    for (int i = 0; i < exr_header->num_custom_attributes; i++) {
      tinyexr::WriteAttributeToMemory(
          &memory, exr_header->custom_attributes[i].name,
          exr_header->custom_attributes[i].type,
          reinterpret_cast<const unsigned char *>(
              exr_header->custom_attributes[i].value),
          exr_header->custom_attributes[i].size);
    }
  }

  {  // end of header
    unsigned char e = 0;
    memory.push_back(e);
  }

  int num_blocks = exr_image->height / num_scanlines;
  if (num_blocks * num_scanlines < exr_image->height) {
    num_blocks++;
  }

  std::vector<tinyexr::tinyexr_uint64> offsets(static_cast<size_t>(num_blocks));

  size_t headerSize = memory.size();
  tinyexr::tinyexr_uint64 offset =
      headerSize +
      static_cast<size_t>(num_blocks) *
          sizeof(
              tinyexr::tinyexr_int64);  // sizeof(header) + sizeof(offsetTable)

  std::vector<std::vector<unsigned char> > data_list(
      static_cast<size_t>(num_blocks));
  std::vector<size_t> channel_offset_list(
      static_cast<size_t>(exr_header->num_channels));

  int pixel_data_size = 0;
  size_t channel_offset = 0;
  for (size_t c = 0; c < static_cast<size_t>(exr_header->num_channels); c++) {
    channel_offset_list[c] = channel_offset;
    if (exr_header->requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
      pixel_data_size += sizeof(unsigned short);
      channel_offset += sizeof(unsigned short);
    } else if (exr_header->requested_pixel_types[c] ==
               TINYEXR_PIXELTYPE_FLOAT) {
      pixel_data_size += sizeof(float);
      channel_offset += sizeof(float);
    } else if (exr_header->requested_pixel_types[c] == TINYEXR_PIXELTYPE_UINT) {
      pixel_data_size += sizeof(unsigned int);
      channel_offset += sizeof(unsigned int);
    } else {
      assert(0);
    }
  }

#if TINYEXR_USE_ZFP
  tinyexr::ZFPCompressionParam zfp_compression_param;

  // Use ZFP compression parameter from custom attributes(if such a parameter
  // exists)
  {
    bool ret = tinyexr::FindZFPCompressionParam(
        &zfp_compression_param, exr_header->custom_attributes,
        exr_header->num_custom_attributes);

    if (!ret) {
      // Use predefined compression parameter.
      zfp_compression_param.type = 0;
      zfp_compression_param.rate = 2;
    }
  }
#endif

  // TOOD(LTE): C++11 thread

// Use signed int since some OpenMP compiler doesn't allow unsigned type for
// `parallel for`
#if TINYEXR_USE_OPENMP
#pragma omp parallel for
#endif
  for (int i = 0; i < num_blocks; i++) {
    size_t ii = static_cast<size_t>(i);
    int start_y = num_scanlines * i;
    int endY = (std::min)(num_scanlines * (i + 1), exr_image->height);
    int h = endY - start_y;

    std::vector<unsigned char> buf(
        static_cast<size_t>(exr_image->width * h * pixel_data_size));

    for (size_t c = 0; c < static_cast<size_t>(exr_header->num_channels); c++) {
      if (exr_header->pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
        if (exr_header->requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT) {
          for (int y = 0; y < h; y++) {
            // Assume increasing Y
            float *line_ptr = reinterpret_cast<float *>(&buf.at(
                static_cast<size_t>(pixel_data_size * y * exr_image->width) +
                channel_offset_list[c] *
                    static_cast<size_t>(exr_image->width)));
            for (int x = 0; x < exr_image->width; x++) {
              tinyexr::FP16 h16;
              h16.u = reinterpret_cast<unsigned short **>(
                  exr_image->images)[c][(y + start_y) * exr_image->width + x];

              tinyexr::FP32 f32 = half_to_float(h16);

              tinyexr::swap4(reinterpret_cast<unsigned int *>(&f32.f));

              // line_ptr[x] = f32.f;
              tinyexr::cpy4(line_ptr + x, &(f32.f));
            }
          }
        } else if (exr_header->requested_pixel_types[c] ==
                   TINYEXR_PIXELTYPE_HALF) {
          for (int y = 0; y < h; y++) {
            // Assume increasing Y
            unsigned short *line_ptr = reinterpret_cast<unsigned short *>(
                &buf.at(static_cast<size_t>(pixel_data_size * y *
                                            exr_image->width) +
                        channel_offset_list[c] *
                            static_cast<size_t>(exr_image->width)));
            for (int x = 0; x < exr_image->width; x++) {
              unsigned short val = reinterpret_cast<unsigned short **>(
                  exr_image->images)[c][(y + start_y) * exr_image->width + x];

              tinyexr::swap2(&val);

              // line_ptr[x] = val;
              tinyexr::cpy2(line_ptr + x, &val);
            }
          }
        } else {
          assert(0);
        }

      } else if (exr_header->pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT) {
        if (exr_header->requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
          for (int y = 0; y < h; y++) {
            // Assume increasing Y
            unsigned short *line_ptr = reinterpret_cast<unsigned short *>(
                &buf.at(static_cast<size_t>(pixel_data_size * y *
                                            exr_image->width) +
                        channel_offset_list[c] *
                            static_cast<size_t>(exr_image->width)));
            for (int x = 0; x < exr_image->width; x++) {
              tinyexr::FP32 f32;
              f32.f = reinterpret_cast<float **>(
                  exr_image->images)[c][(y + start_y) * exr_image->width + x];

              tinyexr::FP16 h16;
              h16 = float_to_half_full(f32);

              tinyexr::swap2(reinterpret_cast<unsigned short *>(&h16.u));

              // line_ptr[x] = h16.u;
              tinyexr::cpy2(line_ptr + x, &(h16.u));
            }
          }
        } else if (exr_header->requested_pixel_types[c] ==
                   TINYEXR_PIXELTYPE_FLOAT) {
          for (int y = 0; y < h; y++) {
            // Assume increasing Y
            float *line_ptr = reinterpret_cast<float *>(&buf.at(
                static_cast<size_t>(pixel_data_size * y * exr_image->width) +
                channel_offset_list[c] *
                    static_cast<size_t>(exr_image->width)));
            for (int x = 0; x < exr_image->width; x++) {
              float val = reinterpret_cast<float **>(
                  exr_image->images)[c][(y + start_y) * exr_image->width + x];

              tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

              // line_ptr[x] = val;
              tinyexr::cpy4(line_ptr + x, &val);
            }
          }
        } else {
          assert(0);
        }
      } else if (exr_header->pixel_types[c] == TINYEXR_PIXELTYPE_UINT) {
        for (int y = 0; y < h; y++) {
          // Assume increasing Y
          unsigned int *line_ptr = reinterpret_cast<unsigned int *>(&buf.at(
              static_cast<size_t>(pixel_data_size * y * exr_image->width) +
              channel_offset_list[c] * static_cast<size_t>(exr_image->width)));
          for (int x = 0; x < exr_image->width; x++) {
            unsigned int val = reinterpret_cast<unsigned int **>(
                exr_image->images)[c][(y + start_y) * exr_image->width + x];

            tinyexr::swap4(&val);

            // line_ptr[x] = val;
            tinyexr::cpy4(line_ptr + x, &val);
          }
        }
      }
    }

    if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_NONE) {
      // 4 byte: scan line
      // 4 byte: data size
      // ~     : pixel data(uncompressed)
      std::vector<unsigned char> header(8);
      unsigned int data_len = static_cast<unsigned int>(buf.size());
      memcpy(&header.at(0), &start_y, sizeof(int));
      memcpy(&header.at(4), &data_len, sizeof(unsigned int));

      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(0)));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(4)));

      data_list[ii].insert(data_list[ii].end(), header.begin(), header.end());
      data_list[ii].insert(data_list[ii].end(), buf.begin(),
                           buf.begin() + data_len);

    } else if ((exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZIPS) ||
               (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZIP)) {
#if TINYEXR_USE_MINIZ
      std::vector<unsigned char> block(mz_compressBound(
          static_cast<unsigned long>(buf.size())));
#else
      std::vector<unsigned char> block(
          compressBound(static_cast<uLong>(buf.size())));
#endif
      tinyexr::tinyexr_uint64 outSize = block.size();

      tinyexr::CompressZip(&block.at(0), outSize,
                           reinterpret_cast<const unsigned char *>(&buf.at(0)),
                           static_cast<unsigned long>(buf.size()));

      // 4 byte: scan line
      // 4 byte: data size
      // ~     : pixel data(compressed)
      std::vector<unsigned char> header(8);
      unsigned int data_len = static_cast<unsigned int>(outSize);  // truncate
      memcpy(&header.at(0), &start_y, sizeof(int));
      memcpy(&header.at(4), &data_len, sizeof(unsigned int));

      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(0)));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(4)));

      data_list[ii].insert(data_list[ii].end(), header.begin(), header.end());
      data_list[ii].insert(data_list[ii].end(), block.begin(),
                           block.begin() + data_len);

    } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_RLE) {
      // (buf.size() * 3) / 2 would be enough.
      std::vector<unsigned char> block((buf.size() * 3) / 2);

      tinyexr::tinyexr_uint64 outSize = block.size();

      tinyexr::CompressRle(&block.at(0), outSize,
                           reinterpret_cast<const unsigned char *>(&buf.at(0)),
                           static_cast<unsigned long>(buf.size()));

      // 4 byte: scan line
      // 4 byte: data size
      // ~     : pixel data(compressed)
      std::vector<unsigned char> header(8);
      unsigned int data_len = static_cast<unsigned int>(outSize);  // truncate
      memcpy(&header.at(0), &start_y, sizeof(int));
      memcpy(&header.at(4), &data_len, sizeof(unsigned int));

      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(0)));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(4)));

      data_list[ii].insert(data_list[ii].end(), header.begin(), header.end());
      data_list[ii].insert(data_list[ii].end(), block.begin(),
                           block.begin() + data_len);

    } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
#if TINYEXR_USE_PIZ
      unsigned int bufLen =
          8192 + static_cast<unsigned int>(
                     2 * static_cast<unsigned int>(
                             buf.size()));  // @fixme { compute good bound. }
      std::vector<unsigned char> block(bufLen);
      unsigned int outSize = static_cast<unsigned int>(block.size());

      CompressPiz(&block.at(0), &outSize,
                  reinterpret_cast<const unsigned char *>(&buf.at(0)),
                  buf.size(), channels, exr_image->width, h);

      // 4 byte: scan line
      // 4 byte: data size
      // ~     : pixel data(compressed)
      std::vector<unsigned char> header(8);
      unsigned int data_len = outSize;
      memcpy(&header.at(0), &start_y, sizeof(int));
      memcpy(&header.at(4), &data_len, sizeof(unsigned int));

      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(0)));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(4)));

      data_list[ii].insert(data_list[ii].end(), header.begin(), header.end());
      data_list[ii].insert(data_list[ii].end(), block.begin(),
                           block.begin() + data_len);

#else
      assert(0);
#endif
    } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
#if TINYEXR_USE_ZFP
      std::vector<unsigned char> block;
      unsigned int outSize;

      tinyexr::CompressZfp(
          &block, &outSize, reinterpret_cast<const float *>(&buf.at(0)),
          exr_image->width, h, exr_header->num_channels, zfp_compression_param);

      // 4 byte: scan line
      // 4 byte: data size
      // ~     : pixel data(compressed)
      std::vector<unsigned char> header(8);
      unsigned int data_len = outSize;
      memcpy(&header.at(0), &start_y, sizeof(int));
      memcpy(&header.at(4), &data_len, sizeof(unsigned int));

      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(0)));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(4)));

      data_list[ii].insert(data_list[ii].end(), header.begin(), header.end());
      data_list[ii].insert(data_list[ii].end(), block.begin(),
                           block.begin() + data_len);

#else
      assert(0);
#endif
    } else {
      assert(0);
    }
  }  // omp parallel

  for (size_t i = 0; i < static_cast<size_t>(num_blocks); i++) {
    offsets[i] = offset;
    tinyexr::swap8(reinterpret_cast<tinyexr::tinyexr_uint64 *>(&offsets[i]));
    offset += data_list[i].size();
  }

  size_t totalSize = static_cast<size_t>(offset);
  {
    memory.insert(
        memory.end(), reinterpret_cast<unsigned char *>(&offsets.at(0)),
        reinterpret_cast<unsigned char *>(&offsets.at(0)) +
            sizeof(tinyexr::tinyexr_uint64) * static_cast<size_t>(num_blocks));
  }

  if (memory.size() == 0) {
    tinyexr::SetErrorMessage("Output memory size is zero", err);
    return 0;
  }

  (*memory_out) = static_cast<unsigned char *>(malloc(totalSize));
  memcpy((*memory_out), &memory.at(0), memory.size());
  unsigned char *memory_ptr = *memory_out + memory.size();

  for (size_t i = 0; i < static_cast<size_t>(num_blocks); i++) {
    memcpy(memory_ptr, &data_list[i].at(0), data_list[i].size());
    memory_ptr += data_list[i].size();
  }

  return totalSize;  // OK
}

int SaveEXRImageToFile(const EXRImage *exr_image, const EXRHeader *exr_header,
                       const char *filename, const char **err) {
  if (exr_image == NULL || filename == NULL ||
      exr_header->compression_type < 0) {
    tinyexr::SetErrorMessage("Invalid argument for SaveEXRImageToFile", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

#if !TINYEXR_USE_PIZ
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
    tinyexr::SetErrorMessage("PIZ compression is not supported in this build",
                             err);
    return TINYEXR_ERROR_UNSUPPORTED_FEATURE;
  }
#endif

#if !TINYEXR_USE_ZFP
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
    tinyexr::SetErrorMessage("ZFP compression is not supported in this build",
                             err);
    return TINYEXR_ERROR_UNSUPPORTED_FEATURE;
  }
#endif

#ifdef _WIN32
  FILE *fp = NULL;
  fopen_s(&fp, filename, "wb");
#else
  FILE *fp = fopen(filename, "wb");
#endif
  if (!fp) {
    tinyexr::SetErrorMessage("Cannot write a file", err);
    return TINYEXR_ERROR_CANT_WRITE_FILE;
  }

  unsigned char *mem = NULL;
  size_t mem_size = SaveEXRImageToMemory(exr_image, exr_header, &mem, err);
  if (mem_size == 0) {
    return TINYEXR_ERROR_SERIALZATION_FAILED;
  }

  size_t written_size = 0;
  if ((mem_size > 0) && mem) {
    written_size = fwrite(mem, 1, mem_size, fp);
  }
  free(mem);

  fclose(fp);

  if (written_size != mem_size) {
    tinyexr::SetErrorMessage("Cannot write a file", err);
    return TINYEXR_ERROR_CANT_WRITE_FILE;
  }

  return TINYEXR_SUCCESS;
}

int LoadDeepEXR(DeepImage *deep_image, const char *filename, const char **err) {
  if (deep_image == NULL) {
    tinyexr::SetErrorMessage("Invalid argument for LoadDeepEXR", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

#ifdef _MSC_VER
  FILE *fp = NULL;
  errno_t errcode = fopen_s(&fp, filename, "rb");
  if ((0 != errcode) || (!fp)) {
    tinyexr::SetErrorMessage("Cannot read a file " + std::string(filename),
                             err);
    return TINYEXR_ERROR_CANT_OPEN_FILE;
  }
#else
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    tinyexr::SetErrorMessage("Cannot read a file " + std::string(filename),
                             err);
    return TINYEXR_ERROR_CANT_OPEN_FILE;
  }
#endif

  size_t filesize;
  // Compute size
  fseek(fp, 0, SEEK_END);
  filesize = static_cast<size_t>(ftell(fp));
  fseek(fp, 0, SEEK_SET);

  if (filesize == 0) {
    fclose(fp);
    tinyexr::SetErrorMessage("File size is zero : " + std::string(filename),
                             err);
    return TINYEXR_ERROR_INVALID_FILE;
  }

  std::vector<char> buf(filesize);  // @todo { use mmap }
  {
    size_t ret;
    ret = fread(&buf[0], 1, filesize, fp);
    assert(ret == filesize);
    (void)ret;
  }
  fclose(fp);

  const char *head = &buf[0];
  const char *marker = &buf[0];

  // Header check.
  {
    const char header[] = {0x76, 0x2f, 0x31, 0x01};

    if (memcmp(marker, header, 4) != 0) {
      tinyexr::SetErrorMessage("Invalid magic number", err);
      return TINYEXR_ERROR_INVALID_MAGIC_NUMBER;
    }
    marker += 4;
  }

  // Version, scanline.
  {
    // ver 2.0, scanline, deep bit on(0x800)
    // must be [2, 0, 0, 0]
    if (marker[0] != 2 || marker[1] != 8 || marker[2] != 0 || marker[3] != 0) {
      tinyexr::SetErrorMessage("Unsupported version or scanline", err);
      return TINYEXR_ERROR_UNSUPPORTED_FORMAT;
    }

    marker += 4;
  }

  int dx = -1;
  int dy = -1;
  int dw = -1;
  int dh = -1;
  int num_scanline_blocks = 1;  // 16 for ZIP compression.
  int compression_type = -1;
  int num_channels = -1;
  std::vector<tinyexr::ChannelInfo> channels;

  // Read attributes
  size_t size = filesize - tinyexr::kEXRVersionSize;
  for (;;) {
    if (0 == size) {
      return TINYEXR_ERROR_INVALID_DATA;
    } else if (marker[0] == '\0') {
      marker++;
      size--;
      break;
    }

    std::string attr_name;
    std::string attr_type;
    std::vector<unsigned char> data;
    size_t marker_size;
    if (!tinyexr::ReadAttribute(&attr_name, &attr_type, &data, &marker_size,
                                marker, size)) {
      std::stringstream ss;
      ss << "Failed to parse attribute\n";
      tinyexr::SetErrorMessage(ss.str(), err);
      return TINYEXR_ERROR_INVALID_DATA;
    }
    marker += marker_size;
    size -= marker_size;

    if (attr_name.compare("compression") == 0) {
      compression_type = data[0];
      if (compression_type > TINYEXR_COMPRESSIONTYPE_PIZ) {
        std::stringstream ss;
        ss << "Unsupported compression type : " << compression_type;
        tinyexr::SetErrorMessage(ss.str(), err);
        return TINYEXR_ERROR_UNSUPPORTED_FORMAT;
      }

      if (compression_type == TINYEXR_COMPRESSIONTYPE_ZIP) {
        num_scanline_blocks = 16;
      }

    } else if (attr_name.compare("channels") == 0) {
      // name: zero-terminated string, from 1 to 255 bytes long
      // pixel type: int, possible values are: UINT = 0 HALF = 1 FLOAT = 2
      // pLinear: unsigned char, possible values are 0 and 1
      // reserved: three chars, should be zero
      // xSampling: int
      // ySampling: int

      if (!tinyexr::ReadChannelInfo(channels, data)) {
        tinyexr::SetErrorMessage("Failed to parse channel info", err);
        return TINYEXR_ERROR_INVALID_DATA;
      }

      num_channels = static_cast<int>(channels.size());

      if (num_channels < 1) {
        tinyexr::SetErrorMessage("Invalid channels format", err);
        return TINYEXR_ERROR_INVALID_DATA;
      }

    } else if (attr_name.compare("dataWindow") == 0) {
      memcpy(&dx, &data.at(0), sizeof(int));
      memcpy(&dy, &data.at(4), sizeof(int));
      memcpy(&dw, &data.at(8), sizeof(int));
      memcpy(&dh, &data.at(12), sizeof(int));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&dx));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&dy));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&dw));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&dh));

    } else if (attr_name.compare("displayWindow") == 0) {
      int x;
      int y;
      int w;
      int h;
      memcpy(&x, &data.at(0), sizeof(int));
      memcpy(&y, &data.at(4), sizeof(int));
      memcpy(&w, &data.at(8), sizeof(int));
      memcpy(&h, &data.at(12), sizeof(int));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&x));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&y));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&w));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&h));
    }
  }

  assert(dx >= 0);
  assert(dy >= 0);
  assert(dw >= 0);
  assert(dh >= 0);
  assert(num_channels >= 1);

  int data_width = dw - dx + 1;
  int data_height = dh - dy + 1;

  std::vector<float> image(
      static_cast<size_t>(data_width * data_height * 4));  // 4 = RGBA

  // Read offset tables.
  int num_blocks = data_height / num_scanline_blocks;
  if (num_blocks * num_scanline_blocks < data_height) {
    num_blocks++;
  }

  std::vector<tinyexr::tinyexr_int64> offsets(static_cast<size_t>(num_blocks));

  for (size_t y = 0; y < static_cast<size_t>(num_blocks); y++) {
    tinyexr::tinyexr_int64 offset;
    memcpy(&offset, marker, sizeof(tinyexr::tinyexr_int64));
    tinyexr::swap8(reinterpret_cast<tinyexr::tinyexr_uint64 *>(&offset));
    marker += sizeof(tinyexr::tinyexr_int64);  // = 8
    offsets[y] = offset;
  }

#if TINYEXR_USE_PIZ
  if ((compression_type == TINYEXR_COMPRESSIONTYPE_NONE) ||
      (compression_type == TINYEXR_COMPRESSIONTYPE_RLE) ||
      (compression_type == TINYEXR_COMPRESSIONTYPE_ZIPS) ||
      (compression_type == TINYEXR_COMPRESSIONTYPE_ZIP) ||
      (compression_type == TINYEXR_COMPRESSIONTYPE_PIZ)) {
#else
  if ((compression_type == TINYEXR_COMPRESSIONTYPE_NONE) ||
      (compression_type == TINYEXR_COMPRESSIONTYPE_RLE) ||
      (compression_type == TINYEXR_COMPRESSIONTYPE_ZIPS) ||
      (compression_type == TINYEXR_COMPRESSIONTYPE_ZIP)) {
#endif
    // OK
  } else {
    tinyexr::SetErrorMessage("Unsupported compression format", err);
    return TINYEXR_ERROR_UNSUPPORTED_FORMAT;
  }

  deep_image->image = static_cast<float ***>(
      malloc(sizeof(float **) * static_cast<size_t>(num_channels)));
  for (int c = 0; c < num_channels; c++) {
    deep_image->image[c] = static_cast<float **>(
        malloc(sizeof(float *) * static_cast<size_t>(data_height)));
    for (int y = 0; y < data_height; y++) {
    }
  }

  deep_image->offset_table = static_cast<int **>(
      malloc(sizeof(int *) * static_cast<size_t>(data_height)));
  for (int y = 0; y < data_height; y++) {
    deep_image->offset_table[y] = static_cast<int *>(
        malloc(sizeof(int) * static_cast<size_t>(data_width)));
  }

  for (size_t y = 0; y < static_cast<size_t>(num_blocks); y++) {
    const unsigned char *data_ptr =
        reinterpret_cast<const unsigned char *>(head + offsets[y]);

    // int: y coordinate
    // int64: packed size of pixel offset table
    // int64: packed size of sample data
    // int64: unpacked size of sample data
    // compressed pixel offset table
    // compressed sample data
    int line_no;
    tinyexr::tinyexr_int64 packedOffsetTableSize;
    tinyexr::tinyexr_int64 packedSampleDataSize;
    tinyexr::tinyexr_int64 unpackedSampleDataSize;
    memcpy(&line_no, data_ptr, sizeof(int));
    memcpy(&packedOffsetTableSize, data_ptr + 4,
           sizeof(tinyexr::tinyexr_int64));
    memcpy(&packedSampleDataSize, data_ptr + 12,
           sizeof(tinyexr::tinyexr_int64));
    memcpy(&unpackedSampleDataSize, data_ptr + 20,
           sizeof(tinyexr::tinyexr_int64));

    tinyexr::swap4(reinterpret_cast<unsigned int *>(&line_no));
    tinyexr::swap8(
        reinterpret_cast<tinyexr::tinyexr_uint64 *>(&packedOffsetTableSize));
    tinyexr::swap8(
        reinterpret_cast<tinyexr::tinyexr_uint64 *>(&packedSampleDataSize));
    tinyexr::swap8(
        reinterpret_cast<tinyexr::tinyexr_uint64 *>(&unpackedSampleDataSize));

    std::vector<int> pixelOffsetTable(static_cast<size_t>(data_width));

    // decode pixel offset table.
    {
      unsigned long dstLen =
          static_cast<unsigned long>(pixelOffsetTable.size() * sizeof(int));
      if (!tinyexr::DecompressZip(
              reinterpret_cast<unsigned char *>(&pixelOffsetTable.at(0)),
              &dstLen, data_ptr + 28,
              static_cast<unsigned long>(packedOffsetTableSize))) {
        return false;
      }

      assert(dstLen == pixelOffsetTable.size() * sizeof(int));
      for (size_t i = 0; i < static_cast<size_t>(data_width); i++) {
        deep_image->offset_table[y][i] = pixelOffsetTable[i];
      }
    }

    std::vector<unsigned char> sample_data(
        static_cast<size_t>(unpackedSampleDataSize));

    // decode sample data.
    {
      unsigned long dstLen = static_cast<unsigned long>(unpackedSampleDataSize);
      if (dstLen) {
        if (!tinyexr::DecompressZip(
                reinterpret_cast<unsigned char *>(&sample_data.at(0)), &dstLen,
                data_ptr + 28 + packedOffsetTableSize,
                static_cast<unsigned long>(packedSampleDataSize))) {
          return false;
        }
        assert(dstLen == static_cast<unsigned long>(unpackedSampleDataSize));
      }
    }

    // decode sample
    int sampleSize = -1;
    std::vector<int> channel_offset_list(static_cast<size_t>(num_channels));
    {
      int channel_offset = 0;
      for (size_t i = 0; i < static_cast<size_t>(num_channels); i++) {
        channel_offset_list[i] = channel_offset;
        if (channels[i].pixel_type == TINYEXR_PIXELTYPE_UINT) {  // UINT
          channel_offset += 4;
        } else if (channels[i].pixel_type == TINYEXR_PIXELTYPE_HALF) {  // half
          channel_offset += 2;
        } else if (channels[i].pixel_type ==
                   TINYEXR_PIXELTYPE_FLOAT) {  // float
          channel_offset += 4;
        } else {
          assert(0);
        }
      }
      sampleSize = channel_offset;
    }
    assert(sampleSize >= 2);

    assert(static_cast<size_t>(
               pixelOffsetTable[static_cast<size_t>(data_width - 1)] *
               sampleSize) == sample_data.size());
    int samples_per_line = static_cast<int>(sample_data.size()) / sampleSize;

    //
    // Alloc memory
    //

    //
    // pixel data is stored as image[channels][pixel_samples]
    //
    {
      tinyexr::tinyexr_uint64 data_offset = 0;
      for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
        deep_image->image[c][y] = static_cast<float *>(
            malloc(sizeof(float) * static_cast<size_t>(samples_per_line)));

        if (channels[c].pixel_type == 0) {  // UINT
          for (size_t x = 0; x < static_cast<size_t>(samples_per_line); x++) {
            unsigned int ui;
            unsigned int *src_ptr = reinterpret_cast<unsigned int *>(
                &sample_data.at(size_t(data_offset) + x * sizeof(int)));
            tinyexr::cpy4(&ui, src_ptr);
            deep_image->image[c][y][x] = static_cast<float>(ui);  // @fixme
          }
          data_offset +=
              sizeof(unsigned int) * static_cast<size_t>(samples_per_line);
        } else if (channels[c].pixel_type == 1) {  // half
          for (size_t x = 0; x < static_cast<size_t>(samples_per_line); x++) {
            tinyexr::FP16 f16;
            const unsigned short *src_ptr = reinterpret_cast<unsigned short *>(
                &sample_data.at(size_t(data_offset) + x * sizeof(short)));
            tinyexr::cpy2(&(f16.u), src_ptr);
            tinyexr::FP32 f32 = half_to_float(f16);
            deep_image->image[c][y][x] = f32.f;
          }
          data_offset += sizeof(short) * static_cast<size_t>(samples_per_line);
        } else {  // float
          for (size_t x = 0; x < static_cast<size_t>(samples_per_line); x++) {
            float f;
            const float *src_ptr = reinterpret_cast<float *>(
                &sample_data.at(size_t(data_offset) + x * sizeof(float)));
            tinyexr::cpy4(&f, src_ptr);
            deep_image->image[c][y][x] = f;
          }
          data_offset += sizeof(float) * static_cast<size_t>(samples_per_line);
        }
      }
    }
  }  // y

  deep_image->width = data_width;
  deep_image->height = data_height;

  deep_image->channel_names = static_cast<const char **>(
      malloc(sizeof(const char *) * static_cast<size_t>(num_channels)));
  for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
#ifdef _WIN32
    deep_image->channel_names[c] = _strdup(channels[c].name.c_str());
#else
    deep_image->channel_names[c] = strdup(channels[c].name.c_str());
#endif
  }
  deep_image->num_channels = num_channels;

  return TINYEXR_SUCCESS;
}

void InitEXRImage(EXRImage *exr_image) {
  if (exr_image == NULL) {
    return;
  }

  exr_image->width = 0;
  exr_image->height = 0;
  exr_image->num_channels = 0;

  exr_image->images = NULL;
  exr_image->tiles = NULL;

  exr_image->num_tiles = 0;
}

void FreeEXRErrorMessage(const char *msg) {
  if (msg) {
    free(reinterpret_cast<void *>(const_cast<char *>(msg)));
  }
  return;
}

void InitEXRHeader(EXRHeader *exr_header) {
  if (exr_header == NULL) {
    return;
  }

  memset(exr_header, 0, sizeof(EXRHeader));
}

int FreeEXRHeader(EXRHeader *exr_header) {
  if (exr_header == NULL) {
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  if (exr_header->channels) {
    free(exr_header->channels);
  }

  if (exr_header->pixel_types) {
    free(exr_header->pixel_types);
  }

  if (exr_header->requested_pixel_types) {
    free(exr_header->requested_pixel_types);
  }

  for (int i = 0; i < exr_header->num_custom_attributes; i++) {
    if (exr_header->custom_attributes[i].value) {
      free(exr_header->custom_attributes[i].value);
    }
  }

  if (exr_header->custom_attributes) {
    free(exr_header->custom_attributes);
  }

  return TINYEXR_SUCCESS;
}

int FreeEXRImage(EXRImage *exr_image) {
  if (exr_image == NULL) {
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  for (int i = 0; i < exr_image->num_channels; i++) {
    if (exr_image->images && exr_image->images[i]) {
      free(exr_image->images[i]);
    }
  }

  if (exr_image->images) {
    free(exr_image->images);
  }

  if (exr_image->tiles) {
    for (int tid = 0; tid < exr_image->num_tiles; tid++) {
      for (int i = 0; i < exr_image->num_channels; i++) {
        if (exr_image->tiles[tid].images && exr_image->tiles[tid].images[i]) {
          free(exr_image->tiles[tid].images[i]);
        }
      }
      if (exr_image->tiles[tid].images) {
        free(exr_image->tiles[tid].images);
      }
    }
    free(exr_image->tiles);
  }

  return TINYEXR_SUCCESS;
}

int ParseEXRHeaderFromFile(EXRHeader *exr_header, const EXRVersion *exr_version,
                           const char *filename, const char **err) {
  if (exr_header == NULL || exr_version == NULL || filename == NULL) {
    tinyexr::SetErrorMessage("Invalid argument for ParseEXRHeaderFromFile",
                             err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

#ifdef _WIN32
  FILE *fp = NULL;
  fopen_s(&fp, filename, "rb");
#else
  FILE *fp = fopen(filename, "rb");
#endif
  if (!fp) {
    tinyexr::SetErrorMessage("Cannot read file " + std::string(filename), err);
    return TINYEXR_ERROR_CANT_OPEN_FILE;
  }

  size_t filesize;
  // Compute size
  fseek(fp, 0, SEEK_END);
  filesize = static_cast<size_t>(ftell(fp));
  fseek(fp, 0, SEEK_SET);

  std::vector<unsigned char> buf(filesize);  // @todo { use mmap }
  {
    size_t ret;
    ret = fread(&buf[0], 1, filesize, fp);
    assert(ret == filesize);
    fclose(fp);

    if (ret != filesize) {
      tinyexr::SetErrorMessage("fread() error on " + std::string(filename),
                               err);
      return TINYEXR_ERROR_INVALID_FILE;
    }
  }

  return ParseEXRHeaderFromMemory(exr_header, exr_version, &buf.at(0), filesize,
                                  err);
}

int ParseEXRMultipartHeaderFromMemory(EXRHeader ***exr_headers,
                                      int *num_headers,
                                      const EXRVersion *exr_version,
                                      const unsigned char *memory, size_t size,
                                      const char **err) {
  if (memory == NULL || exr_headers == NULL || num_headers == NULL ||
      exr_version == NULL) {
    // Invalid argument
    tinyexr::SetErrorMessage(
        "Invalid argument for ParseEXRMultipartHeaderFromMemory", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  if (size < tinyexr::kEXRVersionSize) {
    tinyexr::SetErrorMessage("Data size too short", err);
    return TINYEXR_ERROR_INVALID_DATA;
  }

  const unsigned char *marker = memory + tinyexr::kEXRVersionSize;
  size_t marker_size = size - tinyexr::kEXRVersionSize;

  std::vector<tinyexr::HeaderInfo> infos;

  for (;;) {
    tinyexr::HeaderInfo info;
    info.clear();

    std::string err_str;
    bool empty_header = false;
    int ret = ParseEXRHeader(&info, &empty_header, exr_version, &err_str,
                             marker, marker_size);

    if (ret != TINYEXR_SUCCESS) {
      tinyexr::SetErrorMessage(err_str, err);
      return ret;
    }

    if (empty_header) {
      marker += 1;  // skip '\0'
      break;
    }

    // `chunkCount` must exist in the header.
    if (info.chunk_count == 0) {
      tinyexr::SetErrorMessage(
          "`chunkCount' attribute is not found in the header.", err);
      return TINYEXR_ERROR_INVALID_DATA;
    }

    infos.push_back(info);

    // move to next header.
    marker += info.header_len;
    size -= info.header_len;
  }

  // allocate memory for EXRHeader and create array of EXRHeader pointers.
  (*exr_headers) =
      static_cast<EXRHeader **>(malloc(sizeof(EXRHeader *) * infos.size()));
  for (size_t i = 0; i < infos.size(); i++) {
    EXRHeader *exr_header = static_cast<EXRHeader *>(malloc(sizeof(EXRHeader)));

    ConvertHeader(exr_header, infos[i]);

    // transfoer `tiled` from version.
    exr_header->tiled = exr_version->tiled;

    (*exr_headers)[i] = exr_header;
  }

  (*num_headers) = static_cast<int>(infos.size());

  return TINYEXR_SUCCESS;
}

int ParseEXRMultipartHeaderFromFile(EXRHeader ***exr_headers, int *num_headers,
                                    const EXRVersion *exr_version,
                                    const char *filename, const char **err) {
  if (exr_headers == NULL || num_headers == NULL || exr_version == NULL ||
      filename == NULL) {
    tinyexr::SetErrorMessage(
        "Invalid argument for ParseEXRMultipartHeaderFromFile()", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

#ifdef _WIN32
  FILE *fp = NULL;
  fopen_s(&fp, filename, "rb");
#else
  FILE *fp = fopen(filename, "rb");
#endif
  if (!fp) {
    tinyexr::SetErrorMessage("Cannot read file " + std::string(filename), err);
    return TINYEXR_ERROR_CANT_OPEN_FILE;
  }

  size_t filesize;
  // Compute size
  fseek(fp, 0, SEEK_END);
  filesize = static_cast<size_t>(ftell(fp));
  fseek(fp, 0, SEEK_SET);

  std::vector<unsigned char> buf(filesize);  // @todo { use mmap }
  {
    size_t ret;
    ret = fread(&buf[0], 1, filesize, fp);
    assert(ret == filesize);
    fclose(fp);

    if (ret != filesize) {
      tinyexr::SetErrorMessage("`fread' error. file may be corrupted.", err);
      return TINYEXR_ERROR_INVALID_FILE;
    }
  }

  return ParseEXRMultipartHeaderFromMemory(
      exr_headers, num_headers, exr_version, &buf.at(0), filesize, err);
}

int ParseEXRVersionFromMemory(EXRVersion *version, const unsigned char *memory,
                              size_t size) {
  if (version == NULL || memory == NULL) {
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  if (size < tinyexr::kEXRVersionSize) {
    return TINYEXR_ERROR_INVALID_DATA;
  }

  const unsigned char *marker = memory;

  // Header check.
  {
    const char header[] = {0x76, 0x2f, 0x31, 0x01};

    if (memcmp(marker, header, 4) != 0) {
      return TINYEXR_ERROR_INVALID_MAGIC_NUMBER;
    }
    marker += 4;
  }

  version->tiled = false;
  version->long_name = false;
  version->non_image = false;
  version->multipart = false;

  // Parse version header.
  {
    // must be 2
    if (marker[0] != 2) {
      return TINYEXR_ERROR_INVALID_EXR_VERSION;
    }

    if (version == NULL) {
      return TINYEXR_SUCCESS;  // May OK
    }

    version->version = 2;

    if (marker[1] & 0x2) {  // 9th bit
      version->tiled = true;
    }
    if (marker[1] & 0x4) {  // 10th bit
      version->long_name = true;
    }
    if (marker[1] & 0x8) {        // 11th bit
      version->non_image = true;  // (deep image)
    }
    if (marker[1] & 0x10) {  // 12th bit
      version->multipart = true;
    }
  }

  return TINYEXR_SUCCESS;
}

int ParseEXRVersionFromFile(EXRVersion *version, const char *filename) {
  if (filename == NULL) {
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

#ifdef _WIN32
  FILE *fp = NULL;
  fopen_s(&fp, filename, "rb");
#else
  FILE *fp = fopen(filename, "rb");
#endif
  if (!fp) {
    return TINYEXR_ERROR_CANT_OPEN_FILE;
  }

  size_t file_size;
  // Compute size
  fseek(fp, 0, SEEK_END);
  file_size = static_cast<size_t>(ftell(fp));
  fseek(fp, 0, SEEK_SET);

  if (file_size < tinyexr::kEXRVersionSize) {
    return TINYEXR_ERROR_INVALID_FILE;
  }

  unsigned char buf[tinyexr::kEXRVersionSize];
  size_t ret = fread(&buf[0], 1, tinyexr::kEXRVersionSize, fp);
  fclose(fp);

  if (ret != tinyexr::kEXRVersionSize) {
    return TINYEXR_ERROR_INVALID_FILE;
  }

  return ParseEXRVersionFromMemory(version, buf, tinyexr::kEXRVersionSize);
}

int LoadEXRMultipartImageFromMemory(EXRImage *exr_images,
                                    const EXRHeader **exr_headers,
                                    unsigned int num_parts,
                                    const unsigned char *memory,
                                    const size_t size, const char **err) {
  if (exr_images == NULL || exr_headers == NULL || num_parts == 0 ||
      memory == NULL || (size <= tinyexr::kEXRVersionSize)) {
    tinyexr::SetErrorMessage(
        "Invalid argument for LoadEXRMultipartImageFromMemory()", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  // compute total header size.
  size_t total_header_size = 0;
  for (unsigned int i = 0; i < num_parts; i++) {
    if (exr_headers[i]->header_len == 0) {
      tinyexr::SetErrorMessage("EXRHeader variable is not initialized.", err);
      return TINYEXR_ERROR_INVALID_ARGUMENT;
    }

    total_header_size += exr_headers[i]->header_len;
  }

  const char *marker = reinterpret_cast<const char *>(
      memory + total_header_size + 4 +
      4);  // +8 for magic number and version header.

  marker += 1;  // Skip empty header.

  // NOTE 1:
  //   In multipart image, There is 'part number' before chunk data.
  //   4 byte : part number
  //   4+     : chunk
  //
  // NOTE 2:
  //   EXR spec says 'part number' is 'unsigned long' but actually this is
  //   'unsigned int(4 bytes)' in OpenEXR implementation...
  //   http://www.openexr.com/openexrfilelayout.pdf

  // Load chunk offset table.
  std::vector<std::vector<tinyexr::tinyexr_uint64> > chunk_offset_table_list;
  for (size_t i = 0; i < static_cast<size_t>(num_parts); i++) {
    std::vector<tinyexr::tinyexr_uint64> offset_table(
        static_cast<size_t>(exr_headers[i]->chunk_count));

    for (size_t c = 0; c < offset_table.size(); c++) {
      tinyexr::tinyexr_uint64 offset;
      memcpy(&offset, marker, 8);
      tinyexr::swap8(&offset);

      if (offset >= size) {
        tinyexr::SetErrorMessage("Invalid offset size in EXR header chunks.",
                                 err);
        return TINYEXR_ERROR_INVALID_DATA;
      }

      offset_table[c] = offset + 4;  // +4 to skip 'part number'
      marker += 8;
    }

    chunk_offset_table_list.push_back(offset_table);
  }

  // Decode image.
  for (size_t i = 0; i < static_cast<size_t>(num_parts); i++) {
    std::vector<tinyexr::tinyexr_uint64> &offset_table =
        chunk_offset_table_list[i];

    // First check 'part number' is identitical to 'i'
    for (size_t c = 0; c < offset_table.size(); c++) {
      const unsigned char *part_number_addr =
          memory + offset_table[c] - 4;  // -4 to move to 'part number' field.
      unsigned int part_no;
      memcpy(&part_no, part_number_addr, sizeof(unsigned int));  // 4
      tinyexr::swap4(&part_no);

      if (part_no != i) {
        tinyexr::SetErrorMessage("Invalid `part number' in EXR header chunks.",
                                 err);
        return TINYEXR_ERROR_INVALID_DATA;
      }
    }

    std::string e;
    int ret = tinyexr::DecodeChunk(&exr_images[i], exr_headers[i], offset_table,
                                   memory, size, &e);
    if (ret != TINYEXR_SUCCESS) {
      if (!e.empty()) {
        tinyexr::SetErrorMessage(e, err);
      }
      return ret;
    }
  }

  return TINYEXR_SUCCESS;
}

int LoadEXRMultipartImageFromFile(EXRImage *exr_images,
                                  const EXRHeader **exr_headers,
                                  unsigned int num_parts, const char *filename,
                                  const char **err) {
  if (exr_images == NULL || exr_headers == NULL || num_parts == 0) {
    tinyexr::SetErrorMessage(
        "Invalid argument for LoadEXRMultipartImageFromFile", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

#ifdef _WIN32
  FILE *fp = NULL;
  fopen_s(&fp, filename, "rb");
#else
  FILE *fp = fopen(filename, "rb");
#endif
  if (!fp) {
    tinyexr::SetErrorMessage("Cannot read file " + std::string(filename), err);
    return TINYEXR_ERROR_CANT_OPEN_FILE;
  }

  size_t filesize;
  // Compute size
  fseek(fp, 0, SEEK_END);
  filesize = static_cast<size_t>(ftell(fp));
  fseek(fp, 0, SEEK_SET);

  std::vector<unsigned char> buf(filesize);  //  @todo { use mmap }
  {
    size_t ret;
    ret = fread(&buf[0], 1, filesize, fp);
    assert(ret == filesize);
    fclose(fp);
    (void)ret;
  }

  return LoadEXRMultipartImageFromMemory(exr_images, exr_headers, num_parts,
                                         &buf.at(0), filesize, err);
}

int SaveEXR(const float *data, int width, int height, int components,
            const int save_as_fp16, const char *outfilename, const char **err) {
  if ((components == 1) || components == 3 || components == 4) {
    // OK
  } else {
    std::stringstream ss;
    ss << "Unsupported component value : " << components << std::endl;

    tinyexr::SetErrorMessage(ss.str(), err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  EXRHeader header;
  InitEXRHeader(&header);

  if ((width < 16) && (height < 16)) {
    // No compression for small image.
    header.compression_type = TINYEXR_COMPRESSIONTYPE_NONE;
  } else {
    header.compression_type = TINYEXR_COMPRESSIONTYPE_ZIP;
  }

  EXRImage image;
  InitEXRImage(&image);

  image.num_channels = components;

  std::vector<float> images[4];

  if (components == 1) {
    images[0].resize(static_cast<size_t>(width * height));
    memcpy(images[0].data(), data, sizeof(float) * size_t(width * height));
  } else {
    images[0].resize(static_cast<size_t>(width * height));
    images[1].resize(static_cast<size_t>(width * height));
    images[2].resize(static_cast<size_t>(width * height));
    images[3].resize(static_cast<size_t>(width * height));

    // Split RGB(A)RGB(A)RGB(A)... into R, G and B(and A) layers
    for (size_t i = 0; i < static_cast<size_t>(width * height); i++) {
      images[0][i] = data[static_cast<size_t>(components) * i + 0];
      images[1][i] = data[static_cast<size_t>(components) * i + 1];
      images[2][i] = data[static_cast<size_t>(components) * i + 2];
      if (components == 4) {
        images[3][i] = data[static_cast<size_t>(components) * i + 3];
      }
    }
  }

  float *image_ptr[4] = {0, 0, 0, 0};
  if (components == 4) {
    image_ptr[0] = &(images[3].at(0));  // A
    image_ptr[1] = &(images[2].at(0));  // B
    image_ptr[2] = &(images[1].at(0));  // G
    image_ptr[3] = &(images[0].at(0));  // R
  } else if (components == 3) {
    image_ptr[0] = &(images[2].at(0));  // B
    image_ptr[1] = &(images[1].at(0));  // G
    image_ptr[2] = &(images[0].at(0));  // R
  } else if (components == 1) {
    image_ptr[0] = &(images[0].at(0));  // A
  }

  image.images = reinterpret_cast<unsigned char **>(image_ptr);
  image.width = width;
  image.height = height;

  header.num_channels = components;
  header.channels = static_cast<EXRChannelInfo *>(malloc(
      sizeof(EXRChannelInfo) * static_cast<size_t>(header.num_channels)));
  // Must be (A)BGR order, since most of EXR viewers expect this channel order.
  if (components == 4) {
#ifdef _MSC_VER
    strncpy_s(header.channels[0].name, "A", 255);
    strncpy_s(header.channels[1].name, "B", 255);
    strncpy_s(header.channels[2].name, "G", 255);
    strncpy_s(header.channels[3].name, "R", 255);
#else
    strncpy(header.channels[0].name, "A", 255);
    strncpy(header.channels[1].name, "B", 255);
    strncpy(header.channels[2].name, "G", 255);
    strncpy(header.channels[3].name, "R", 255);
#endif
    header.channels[0].name[strlen("A")] = '\0';
    header.channels[1].name[strlen("B")] = '\0';
    header.channels[2].name[strlen("G")] = '\0';
    header.channels[3].name[strlen("R")] = '\0';
  } else if (components == 3) {
#ifdef _MSC_VER
    strncpy_s(header.channels[0].name, "B", 255);
    strncpy_s(header.channels[1].name, "G", 255);
    strncpy_s(header.channels[2].name, "R", 255);
#else
    strncpy(header.channels[0].name, "B", 255);
    strncpy(header.channels[1].name, "G", 255);
    strncpy(header.channels[2].name, "R", 255);
#endif
    header.channels[0].name[strlen("B")] = '\0';
    header.channels[1].name[strlen("G")] = '\0';
    header.channels[2].name[strlen("R")] = '\0';
  } else {
#ifdef _MSC_VER
    strncpy_s(header.channels[0].name, "A", 255);
#else
    strncpy(header.channels[0].name, "A", 255);
#endif
    header.channels[0].name[strlen("A")] = '\0';
  }

  header.pixel_types = static_cast<int *>(
      malloc(sizeof(int) * static_cast<size_t>(header.num_channels)));
  header.requested_pixel_types = static_cast<int *>(
      malloc(sizeof(int) * static_cast<size_t>(header.num_channels)));
  for (int i = 0; i < header.num_channels; i++) {
    header.pixel_types[i] =
        TINYEXR_PIXELTYPE_FLOAT;  // pixel type of input image

    if (save_as_fp16 > 0) {
      header.requested_pixel_types[i] =
          TINYEXR_PIXELTYPE_HALF;  // save with half(fp16) pixel format
    } else {
      header.requested_pixel_types[i] =
          TINYEXR_PIXELTYPE_FLOAT;  // save with float(fp32) pixel format(i.e.
                                    // no precision reduction)
    }
  }

  int ret = SaveEXRImageToFile(&image, &header, outfilename, err);
  if (ret != TINYEXR_SUCCESS) {
    return ret;
  }

  free(header.channels);
  free(header.pixel_types);
  free(header.requested_pixel_types);

  return ret;
}

#ifdef __clang__
// zero-as-null-ppinter-constant
#pragma clang diagnostic pop
#endif
