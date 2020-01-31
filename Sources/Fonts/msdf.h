/* msdf
  Handles multi-channel signed distance field bitmap
  generation from given ttf (stb_truetype.h) font.
  https://github.com/exezin/msdf-c

  Depends on stb_truetype.h to load the ttf file.

  This is in an unstable state, ymmv.

  Based on the C++ implementation by Viktor Chlumský.
  https://github.com/Chlumsky/msdfgen

  Current issues:

   *Glyph alignment seems off
   *Lack of multi-byte character support
   *Error correction appears to be wrong (pixel clash)
*/

#ifndef EX_MSDF_H
#define EX_MSDF_H

#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int left_bearing;
  int advance;
  int ix0, ix1;
  int iy0, iy1;
} ex_metrics_t;

typedef struct stbtt_fontinfo stbtt_fontinfo;

/*
  Generates a bitmap from the specified character (c)
  Bitmap is a 3-channel float array (3*w*h)
 */
float* ex_msdf_glyph(stbtt_fontinfo *font, uint32_t c, size_t w, size_t h, ex_metrics_t *metrics);

static inline uint32_t ex_utf8(const char *c) {
  uint32_t val = 0;

  if ((c[0] & 0xF8) == 0xF0) {
    // 4 byte
    val |= (c[3] & 0x3F);
    val |= (c[2] & 0x3F) << 6;
    val |= (c[1] & 0x3F) << 12;
    val |= (c[0] & 0x07) << 18;
  }
  else if ((c[0] & 0xF0) == 0xE0) {
    // 3 byte
    val |= (c[2] & 0x3F);
    val |= (c[1] & 0x3F) << 6;
    val |= (c[0] & 0x0F) << 12;
  }
  else if ((c[0] & 0xE0) == 0xC0) {
    // 2 byte
    val |= (c[1] & 0x3F);
    val |= (c[0] & 0x1F) << 6;
  }
  else {
    // 1 byte
    val = c[0];
  }

  return val;
}

#ifdef __cplusplus
}
#endif

#endif // EX_MSDF_H