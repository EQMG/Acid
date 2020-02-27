#define TINY_DNG_LOADER_IMPLEMENTATION
#include "tiny_dng.h"

#include <stdint.h>  // for lj92
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <map>
#include <sstream>

// #include <iostream> // dbg

#ifdef TINY_DNG_LOADER_PROFILING
// Requires C++11 feature
#include <chrono>
#endif

#ifdef TINY_DNG_LOADER_USE_THREAD
#include <algorithm>
#include <atomic>
#include <thread>
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvariadic-macros"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

#ifdef TINY_DNG_LOADER_DEBUG
#define TINY_DNG_DPRINTF(...) printf(__VA_ARGS__)
#else
#define TINY_DNG_DPRINTF(...)
#endif

#if !defined(TINY_DNG_NO_EXCEPTION)
#define TINY_DNG_ASSERT(assertion, text) \
  do {                                   \
    if ((assertion) == 0) {              \
      throw std::runtime_error(text);    \
    }                                    \
  } while (false)

#define TINY_DNG_ABORT(text)        \
  do {                              \
    throw std::runtime_error(text); \
  } while (false)

#else
#define TINY_DNG_ASSERT(assertion, text)                                    \
  do {                                                                      \
    if ((assertion) == 0) {                                                 \
      std::cerr << __FILE__ << ":" << __LINE__ << " " << text << std::endl; \
      abort();                                                              \
    }                                                                       \
  } while (false)
#define TINY_DNG_ABORT(text)                                              \
  do {                                                                    \
    std::cerr << __FILE__ << ":" << __LINE__ << " " << text << std::endl; \
    abort();                                                              \
  } while (false)
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-extensions"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wconditional-uninitialized"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#if __has_warning("-Wcomma")
#pragma clang diagnostic ignored "-Wcomma"
#endif
#if __has_warning("-Wcast-qual")
#pragma clang diagnostic ignored "-Wcast-qual"
#endif
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#if __has_warning("-Wparentheses-equality")
#pragma clang diagnostic ignored "-Wparentheses-equality"
#endif
#if __has_warning("-Wextra-semi-stmt")
#pragma clang diagnostic ignored "-Wextra-semi-stmt"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4334)
#pragma warning(disable : 4244)
#endif

#ifdef TINY_DNG_LOADER_ENABLE_ZIP
#ifndef TINY_DNG_LOADER_USE_SYSTEM_ZLIB
#include <miniz/miniz.h>
#endif
#endif

// linjpgd to decode jpeg image.
#include <libjpgd/jpgd.h>

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace tinydng {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-extensions"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wconditional-uninitialized"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#if __has_warning("-Wcomma")
#pragma clang diagnostic ignored "-Wcomma"
#endif
#if __has_warning("-Wcast-qual")
#pragma clang diagnostic ignored "-Wcast-qual"
#endif
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4334)
#pragma warning(disable : 4244)
#endif

namespace {
// Begin liblj92, Lossless JPEG decode/encoder ------------------------------

/*
lj92.c
(c) Andrew Baldwin 2014

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

enum LJ92_ERRORS {
  LJ92_ERROR_NONE = 0,
  LJ92_ERROR_CORRUPT = -1,
  LJ92_ERROR_NO_MEMORY = -2,
  LJ92_ERROR_BAD_HANDLE = -3,
  LJ92_ERROR_TOO_WIDE = -4
};

typedef struct _ljp* lj92;

/* Parse a lossless JPEG (1992) structure returning
 * - a handle that can be used to decode the data
 * - width/height/bitdepth of the data
 * Returns status code.
 * If status == LJ92_ERROR_NONE, handle must be closed with lj92_close
 */
int lj92_open(lj92* lj,                          // Return handle here
              const uint8_t* data, int datalen,  // The encoded data
              int* width, int* height,
              int* bitdepth);  // Width, height and bitdepth

/* Release a decoder object */
void lj92_close(lj92 lj);

/*
 * Decode previously opened lossless JPEG (1992) into a 2D tile of memory
 * Starting at target, write writeLength 16bit values, then skip 16bit
 * skipLength value before writing again
 * If linearize is not NULL, use table at linearize to convert data values from
 * output value to target value
 * Data is only correct if LJ92_ERROR_NONE is returned
 */
int lj92_decode(
    lj92 lj, uint16_t* target, int writeLength,
    int skipLength,  // The image is written to target as a tile
    uint16_t* linearize,
    int linearizeLength);  // If not null, linearize the data using this table

#if 0
/*
 * Encode a grayscale image supplied as 16bit values within the given bitdepth
 * Read from tile in the image
 * Apply delinearization if given
 * Return the encoded lossless JPEG stream
 */
int lj92_encode(uint16_t* image, int width, int height, int bitdepth,
                       int readLength, int skipLength, uint16_t* delinearize,
                       int delinearizeLength, uint8_t** encoded,
                       int* encodedLength);
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

//#define SLOW_HUFF
//#define LJ92_DEBUG

#define LJ92_MAX_COMPONENTS (16)

typedef struct _ljp {
  u8* data;
  u8* dataend;
  int datalen;
  int scanstart;
  int ix;
  int x;           // Width
  int y;           // Height
  int bits;        // Bit depth
  int components;  // Components(Nf)
  int writelen;    // Write rows this long
  int skiplen;     // Skip this many values after each row
  u16* linearize;  // Linearization table
  int linlen;
  int sssshist[16];

// Huffman table - only one supported, and probably needed
#ifdef SLOW_HUFF
  // NOTE: Huffman table for each components is not supported for SLOW_HUFF code
  // path.
  int* maxcode;
  int* mincode;
  int* valptr;
  u8* huffval;
  int* huffsize;
  int* huffcode;
#else
  // Huffman table for each components
  u16* hufflut[LJ92_MAX_COMPONENTS];
  int huffbits[LJ92_MAX_COMPONENTS];
  int num_huff_idx;
#endif
  // Parse state
  int cnt;
  u32 b;
  u16* image;
  u16* rowcache;
  u16* outrow[2];
} ljp;

static int find(ljp* self) {
  int ix = self->ix;
  u8* data = self->data;
  while (data[ix] != 0xFF && ix < (self->datalen - 1)) {
    ix += 1;
  }
  ix += 2;
  if (ix >= self->datalen) {
    // TINY_DNG_DPRINTF("idx = %d, datalen = %\d\n", ix, self->datalen);
    return -1;
  }
  self->ix = ix;
  // TINY_DNG_DPRINTF("ix = %d, data = %d\n", ix, data[ix - 1]);
  return data[ix - 1];
}

#define BEH(ptr) ((((int)(*&ptr)) << 8) | (*(&ptr + 1)))

static int parseHuff(ljp* self) {
  int ret = LJ92_ERROR_CORRUPT;
  u8* huffhead =
      &self->data
           [self->ix];  // xstruct.unpack('>HB16B',self.data[self.ix:self.ix+19])
  u8* bits = &huffhead[2];
  bits[0] = 0;  // Because table starts from 1
  int hufflen = BEH(huffhead[0]);
  if ((self->ix + hufflen) >= self->datalen) return ret;
#ifdef SLOW_HUFF
  u8* huffval = calloc(hufflen - 19, sizeof(u8));
  if (huffval == NULL) return LJ92_ERROR_NO_MEMORY;
  self->huffval = huffval;
  for (int hix = 0; hix < (hufflen - 19); hix++) {
    huffval[hix] = self->data[self->ix + 19 + hix];
#ifdef LJ92_DEBUG
    TINY_DNG_DPRINTF("huffval[%d]=%d\n", hix, huffval[hix]);
#endif
  }
  self->ix += hufflen;
  // Generate huffman table
  int k = 0;
  int i = 1;
  int j = 1;
  int huffsize_needed = 1;
  // First calculate how long huffsize needs to be
  while (i <= 16) {
    while (j <= bits[i]) {
      huffsize_needed++;
      k = k + 1;
      j = j + 1;
    }
    i = i + 1;
    j = 1;
  }
  // Now allocate and do it
  int* huffsize = calloc(huffsize_needed, sizeof(int));
  if (huffsize == NULL) return LJ92_ERROR_NO_MEMORY;
  self->huffsize = huffsize;
  k = 0;
  i = 1;
  j = 1;
  // First calculate how long huffsize needs to be
  int hsix = 0;
  while (i <= 16) {
    while (j <= bits[i]) {
      huffsize[hsix++] = i;
      k = k + 1;
      j = j + 1;
    }
    i = i + 1;
    j = 1;
  }
  huffsize[hsix++] = 0;

  // Calculate the size of huffcode array
  int huffcode_needed = 0;
  k = 0;
  int code = 0;
  int si = huffsize[0];
  while (1) {
    while (huffsize[k] == si) {
      huffcode_needed++;
      code = code + 1;
      k = k + 1;
    }
    if (huffsize[k] == 0) break;
    while (huffsize[k] != si) {
      code = code << 1;
      si = si + 1;
    }
  }
  // Now fill it
  int* huffcode = calloc(huffcode_needed, sizeof(int));
  if (huffcode == NULL) return LJ92_ERROR_NO_MEMORY;
  self->huffcode = huffcode;
  int hcix = 0;
  k = 0;
  code = 0;
  si = huffsize[0];
  while (1) {
    while (huffsize[k] == si) {
      huffcode[hcix++] = code;
      code = code + 1;
      k = k + 1;
    }
    if (huffsize[k] == 0) break;
    while (huffsize[k] != si) {
      code = code << 1;
      si = si + 1;
    }
  }

  i = 0;
  j = 0;

  int* maxcode = calloc(17, sizeof(int));
  if (maxcode == NULL) return LJ92_ERROR_NO_MEMORY;
  self->maxcode = maxcode;
  int* mincode = calloc(17, sizeof(int));
  if (mincode == NULL) return LJ92_ERROR_NO_MEMORY;
  self->mincode = mincode;
  int* valptr = calloc(17, sizeof(int));
  if (valptr == NULL) return LJ92_ERROR_NO_MEMORY;
  self->valptr = valptr;

  while (1) {
    while (1) {
      i++;
      if (i > 16) break;
      if (bits[i] != 0) break;
      maxcode[i] = -1;
    }
    if (i > 16) break;
    valptr[i] = j;
    mincode[i] = huffcode[j];
    j = j + bits[i] - 1;
    maxcode[i] = huffcode[j];
    j++;
  }
  free(huffsize);
  self->huffsize = NULL;
  free(huffcode);
  self->huffcode = NULL;
  ret = LJ92_ERROR_NONE;
#else
  /* Calculate huffman direct lut */
  // How many bits in the table - find highest entry
  u8* huffvals = &self->data[self->ix + 19];
  int maxbits = 16;
  while (maxbits > 0) {
    if (bits[maxbits]) break;
    maxbits--;
  }
  self->huffbits[self->num_huff_idx] = maxbits;
  /* Now fill the lut */
  u16* hufflut = (u16*)malloc((1 << maxbits) * sizeof(u16));
  // TINY_DNG_DPRINTF("maxbits = %d\n", maxbits);
  if (hufflut == NULL) return LJ92_ERROR_NO_MEMORY;
  self->hufflut[self->num_huff_idx] = hufflut;
  int i = 0;
  int hv = 0;
  int rv = 0;
  int vl = 0;  // i
  int hcode;
  int bitsused = 1;
#ifdef LJ92_DEBUG
  TINY_DNG_DPRINTF("%04x:%x:%d:%x\n", i, huffvals[hv], bitsused,
                   1 << (maxbits - bitsused));
#endif
  while (i < 1 << maxbits) {
    if (bitsused > maxbits) {
      break;  // Done. Should never get here!
    }
    if (vl >= bits[bitsused]) {
      bitsused++;
      vl = 0;
      continue;
    }
    if (rv == 1 << (maxbits - bitsused)) {
      rv = 0;
      vl++;
      hv++;
#ifdef LJ92_DEBUG
      TINY_DNG_DPRINTF("%04x:%x:%d:%x\n", i, huffvals[hv], bitsused,
                       1 << (maxbits - bitsused));
#endif
      continue;
    }
    hcode = huffvals[hv];
    hufflut[i] = hcode << 8 | bitsused;
    // TINY_DNG_DPRINTF("%d %d %d\n",i,bitsused,hcode);
    i++;
    rv++;
  }
  ret = LJ92_ERROR_NONE;
#endif
  self->num_huff_idx++;

  return ret;
}

static int parseSof3(ljp* self) {
  if (self->ix + 6 >= self->datalen) return LJ92_ERROR_CORRUPT;
  self->y = BEH(self->data[self->ix + 3]);
  self->x = BEH(self->data[self->ix + 5]);
  self->bits = self->data[self->ix + 2];
  self->components = self->data[self->ix + 7];
  self->ix += BEH(self->data[self->ix]);

  TINY_DNG_ASSERT(self->components >= 1 && self->components < 6,
                  "Invalid number of components.");

  return LJ92_ERROR_NONE;
}

static int parseBlock(ljp* self, int marker) {
  (void)marker;
  self->ix += BEH(self->data[self->ix]);
  if (self->ix >= self->datalen) {
    TINY_DNG_DPRINTF("parseBlock: ix %d, datalen %d\n", self->ix,
                     self->datalen);
    return LJ92_ERROR_CORRUPT;
  }
  return LJ92_ERROR_NONE;
}

#ifdef SLOW_HUFF
static int nextbit(ljp* self) {
  u32 b = self->b;
  if (self->cnt == 0) {
    u8* data = &self->data[self->ix];
    u32 next = *data++;
    b = next;
    if (next == 0xff) {
      data++;
      self->ix++;
    }
    self->ix++;
    self->cnt = 8;
  }
  int bit = b >> 7;
  self->cnt--;
  self->b = (b << 1) & 0xFF;
  return bit;
}

static int decode(ljp* self) {
  int i = 1;
  int code = nextbit(self);
  while (code > self->maxcode[i]) {
    i++;
    code = (code << 1) + nextbit(self);
  }
  int j = self->valptr[i];
  j = j + code - self->mincode[i];
  int value = self->huffval[j];
  return value;
}

static int receive(ljp* self, int ssss) {
  int i = 0;
  int v = 0;
  while (i != ssss) {
    i++;
    v = (v << 1) + nextbit(self);
  }
  return v;
}

static int extend(ljp* self, int v, int t) {
  int vt = 1 << (t - 1);
  if (v < vt) {
    vt = (-1 << t) + 1;
    v = v + vt;
  }
  return v;
}
#endif

inline static int nextdiff(ljp* self, int component_idx, int Px) {
  (void)Px;
#ifdef SLOW_HUFF
  int t = decode(self);
  int diff = receive(self, t);
  diff = extend(self, diff, t);
// TINY_DNG_DPRINTF("%d %d %d %x\n",Px+diff,Px,diff,t);//,index,usedbits);
#else
  TINY_DNG_ASSERT(component_idx <= self->num_huff_idx, "Invalid huff index.");
  u32 b = self->b;
  int cnt = self->cnt;
  int huffbits = self->huffbits[component_idx];
  int ix = self->ix;
  int next;
  while (cnt < huffbits) {
    next = *(u16*)&self->data[ix];
    int one = next & 0xFF;
    int two = next >> 8;
    b = (b << 16) | (one << 8) | two;
    cnt += 16;
    ix += 2;
    if (one == 0xFF) {
      // TINY_DNG_DPRINTF("%x %x %x %x %d\n",one,two,b,b>>8,cnt);
      b >>= 8;
      cnt -= 8;
    } else if (two == 0xFF)
      ix++;
  }
  int index = b >> (cnt - huffbits);
  // TINY_DNG_DPRINTF("component_idx = %d / %d, index = %d\n", component_idx,
  // self->components, index);

  u16 ssssused = self->hufflut[component_idx][index];
  int usedbits = ssssused & 0xFF;
  int t = ssssused >> 8;
  self->sssshist[t]++;
  cnt -= usedbits;
  int keepbitsmask = (1 << cnt) - 1;
  b &= keepbitsmask;
  while (cnt < t) {
    next = *(u16*)&self->data[ix];
    int one = next & 0xFF;
    int two = next >> 8;
    b = (b << 16) | (one << 8) | two;
    cnt += 16;
    ix += 2;
    if (one == 0xFF) {
      b >>= 8;
      cnt -= 8;
    } else if (two == 0xFF)
      ix++;
  }
  cnt -= t;
  int diff = b >> cnt;
  int vt = 1 << (t - 1);
  if (diff < vt) {
    vt = (-1 << t) + 1;
    diff += vt;
  }
  keepbitsmask = (1 << cnt) - 1;
  self->b = b & keepbitsmask;
  self->cnt = cnt;
  self->ix = ix;
// TINY_DNG_DPRINTF("%d %d\n",t,diff);
// TINY_DNG_DPRINTF("%d %d %d %x %x %d\n",Px+diff,Px,diff,t,index,usedbits);
#ifdef LJ92_DEBUG
#endif
#endif
  return diff;
}

static int parsePred6(ljp* self) {
  TINY_DNG_DPRINTF("parsePred6\n");
  int ret = LJ92_ERROR_CORRUPT;
  self->ix = self->scanstart;
  // int compcount = self->data[self->ix+2];
  self->ix += BEH(self->data[self->ix]);
  self->cnt = 0;
  self->b = 0;
  int write = self->writelen;
  // Now need to decode huffman coded values
  int c = 0;
  int pixels = self->y * self->x;
  u16* out = self->image;
  u16* temprow;
  u16* thisrow = self->outrow[0];
  u16* lastrow = self->outrow[1];

  // First pixel predicted from base value
  int diff;
  int Px;
  int col = 0;
  int row = 0;
  int left = 0;
  int linear;
  TINY_DNG_ASSERT(self->num_huff_idx <= self->components,
                  "Invalid number of huff indices.");
  // First pixel
  diff = nextdiff(self, self->num_huff_idx - 1,
                  0);  // FIXME(syoyo): Is using (self->num_huff_idx-1) correct?
  Px = 1 << (self->bits - 1);
  left = Px + diff;
  if (self->linearize)
    linear = self->linearize[left];
  else
    linear = left;
  thisrow[col++] = left;
  out[c++] = linear;
  if (self->ix >= self->datalen) {
    TINY_DNG_DPRINTF("ix = %d, datalen = %d\n", self->ix, self->datalen);
    return ret;
  }
  --write;
  int rowcount = self->x - 1;
  while (rowcount--) {
    diff = nextdiff(self, self->num_huff_idx - 1, 0);
    Px = left;
    left = Px + diff;
    if (self->linearize)
      linear = self->linearize[left];
    else
      linear = left;
    thisrow[col++] = left;
    out[c++] = linear;
    // TINY_DNG_DPRINTF("%d %d %d %d
    // %x\n",col-1,diff,left,thisrow[col-1],&thisrow[col-1]);
    if (self->ix >= self->datalen) {
      TINY_DNG_DPRINTF("a: self->ix = %d, datalen = %d\n", self->ix,
                       self->datalen);
      return ret;
    }
    if (--write == 0) {
      out += self->skiplen;
      write = self->writelen;
    }
  }
  temprow = lastrow;
  lastrow = thisrow;
  thisrow = temprow;
  row++;
  // TINY_DNG_DPRINTF("%x %x\n",thisrow,lastrow);
  while (c < pixels) {
    col = 0;
    diff = nextdiff(self, self->num_huff_idx - 1, 0);
    Px = lastrow[col];  // Use value above for first pixel in row
    left = Px + diff;
    if (self->linearize) {
      if (left > self->linlen) return LJ92_ERROR_CORRUPT;
      linear = self->linearize[left];
    } else
      linear = left;
    thisrow[col++] = left;
    // TINY_DNG_DPRINTF("%d %d %d %d\n",col,diff,left,lastrow[col]);
    out[c++] = linear;
    if (self->ix >= self->datalen) break;
    rowcount = self->x - 1;
    if (--write == 0) {
      out += self->skiplen;
      write = self->writelen;
    }
    while (rowcount--) {
      diff = nextdiff(self, self->num_huff_idx - 1, 0);
      Px = lastrow[col] + ((left - lastrow[col - 1]) >> 1);
      left = Px + diff;
      // TINY_DNG_DPRINTF("%d %d %d %d %d
      // %x\n",col,diff,left,lastrow[col],lastrow[col-1],&lastrow[col]);
      if (self->linearize) {
        if (left > self->linlen) return LJ92_ERROR_CORRUPT;
        linear = self->linearize[left];
      } else
        linear = left;
      thisrow[col++] = left;
      out[c++] = linear;
      if (--write == 0) {
        out += self->skiplen;
        write = self->writelen;
      }
    }
    temprow = lastrow;
    lastrow = thisrow;
    thisrow = temprow;
    if (self->ix >= self->datalen) break;
  }
  if (c >= pixels) ret = LJ92_ERROR_NONE;
  return ret;
}

static int parseScan(ljp* self) {
  int ret = LJ92_ERROR_CORRUPT;
  memset(self->sssshist, 0, sizeof(self->sssshist));
  self->ix = self->scanstart;
  int compcount = self->data[self->ix + 2];
  int pred = self->data[self->ix + 3 + 2 * compcount];
  if (pred < 0 || pred > 7) return ret;
  if (pred == 6) return parsePred6(self);  // Fast path
  // TINY_DNG_DPRINTF("pref = %d\n", pred);
  self->ix += BEH(self->data[self->ix]);
  self->cnt = 0;
  self->b = 0;
  // int write = self->writelen;
  // Now need to decode huffman coded values
  // int c = 0;
  // int pixels = self->y * self->x * self->components;
  u16* out = self->image;
  u16* thisrow = self->outrow[0];
  u16* lastrow = self->outrow[1];

  // First pixel predicted from base value
  int diff;
  int Px = 0;
  // int col = 0;
  // int row = 0;
  int left = 0;
  // TINY_DNG_DPRINTF("w = %d, h = %d, components = %d, skiplen = %d\n",
  // self->x,
  // self->y,
  //       self->components, self->skiplen);
  for (int row = 0; row < self->y; row++) {
    // TINY_DNG_DPRINTF("row = %d / %d\n", row, self->y);
    for (int col = 0; col < self->x; col++) {
      int colx = col * self->components;
      for (int c = 0; c < self->components; c++) {
        // TINY_DNG_DPRINTF("c = %d, col = %d, row = %d\n", c, col, row);
        if ((col == 0) && (row == 0)) {
          Px = 1 << (self->bits - 1);
        } else if (row == 0) {
          // Px = left;
          TINY_DNG_ASSERT(col > 0, "Unexpected col.");
          Px = thisrow[(col - 1) * self->components + c];
        } else if (col == 0) {
          Px = lastrow[c];  // Use value above for first pixel in row
        } else {
          int prev_colx = (col - 1) * self->components;
          // TINY_DNG_DPRINTF("pred = %d\n", pred);
          switch (pred) {
            case 0:
              Px = 0;
              break;  // No prediction... should not be used
            case 1:
              Px = thisrow[prev_colx + c];
              break;
            case 2:
              Px = lastrow[colx + c];
              break;
            case 3:
              Px = lastrow[prev_colx + c];
              break;
            case 4:
              Px = left + lastrow[colx + c] - lastrow[prev_colx + c];
              break;
            case 5:
              Px = left + ((lastrow[colx + c] - lastrow[prev_colx + c]) >> 1);
              break;
            case 6:
              Px = lastrow[colx + c] + ((left - lastrow[prev_colx + c]) >> 1);
              break;
            case 7:
              Px = (left + lastrow[colx + c]) >> 1;
              break;
          }
        }

        int huff_idx = c;
        if (c >= self->num_huff_idx) {
          // It looks huffman tables are shared for all components.
          // Currently we assume # of huffman tables is 1.
          TINY_DNG_ASSERT(self->num_huff_idx == 1,
                          "Cannot handle >1 huffman tables.");
          huff_idx = 0;  // Look up the first huffman table.
        }

        diff = nextdiff(self, huff_idx, Px);
        left = Px + diff;
        // TINY_DNG_DPRINTF("c[%d] Px = %d, diff = %d, left = %d\n", c, Px,
        // diff, left);
        TINY_DNG_ASSERT(left >= 0 && left < (1 << self->bits),
                        "Error huffman decoding.");
        // TINY_DNG_DPRINTF("pix = %d\n", left);
        // TINY_DNG_DPRINTF("%d %d %d\n",c,diff,left);
        int linear;
        if (self->linearize) {
          if (left > self->linlen) return LJ92_ERROR_CORRUPT;
          linear = self->linearize[left];
        } else {
          linear = left;
        }

        // TINY_DNG_DPRINTF("linear = %d\n", linear);
        thisrow[colx + c] = left;
        out[colx + c] = linear;  // HACK
      }                          // c
    }                            // col

    u16* temprow = lastrow;
    lastrow = thisrow;
    thisrow = temprow;

    out += self->x * self->components + self->skiplen;
    // out += self->skiplen;
    // TINY_DNG_DPRINTF("out = %p, %p, diff = %lld\n", out, self->image, out -
    // self->image);

  }  // row

  ret = LJ92_ERROR_NONE;

  // if (++col == self->x) {
  //	col = 0;
  //	row++;
  //}
  // if (--write == 0) {
  //	out += self->skiplen;
  //	write = self->writelen;
  //}
  // if (self->ix >= self->datalen + 2) break;

  // if (c >= pixels) ret = LJ92_ERROR_NONE;
  /*for (int h=0;h<17;h++) {
      TINY_DNG_DPRINTF("ssss:%d=%d
  (%f)\n",h,self->sssshist[h],(float)self->sssshist[h]/(float)(pixels));
  }*/
  return ret;
}

static int parseImage(ljp* self) {
  // TINY_DNG_DPRINTF("parseImage\n");
  int ret = LJ92_ERROR_NONE;
  while (1) {
    int nextMarker = find(self);
    TINY_DNG_DPRINTF("marker = 0x%08x\n", nextMarker);
    if (nextMarker == 0xc4)
      ret = parseHuff(self);
    else if (nextMarker == 0xc3)
      ret = parseSof3(self);
    else if (nextMarker == 0xfe)  // Comment
      ret = parseBlock(self, nextMarker);
    else if (nextMarker == 0xd9)  // End of image
      break;
    else if (nextMarker == 0xda) {
      self->scanstart = self->ix;
      ret = LJ92_ERROR_NONE;
      break;
    } else if (nextMarker == -1) {
      ret = LJ92_ERROR_CORRUPT;
      break;
    } else
      ret = parseBlock(self, nextMarker);
    if (ret != LJ92_ERROR_NONE) break;
  }
  return ret;
}

static int findSoI(ljp* self) {
  int ret = LJ92_ERROR_CORRUPT;
  if (find(self) == 0xd8) {
    ret = parseImage(self);
  } else {
    TINY_DNG_DPRINTF("findSoI: corrupt\n");
  }
  return ret;
}

static void free_memory(ljp* self) {
#ifdef SLOW_HUFF
  free(self->maxcode);
  self->maxcode = NULL;
  free(self->mincode);
  self->mincode = NULL;
  free(self->valptr);
  self->valptr = NULL;
  free(self->huffval);
  self->huffval = NULL;
  free(self->huffsize);
  self->huffsize = NULL;
  free(self->huffcode);
  self->huffcode = NULL;
#else
  for (int i = 0; i < self->num_huff_idx; i++) {
    free(self->hufflut[i]);
    self->hufflut[i] = NULL;
  }
#endif
  free(self->rowcache);
  self->rowcache = NULL;
}

int lj92_open(lj92* lj, const uint8_t* data, int datalen, int* width,
              int* height, int* bitdepth) {
  ljp* self = (ljp*)calloc(sizeof(ljp), 1);
  if (self == NULL) return LJ92_ERROR_NO_MEMORY;

  self->data = (u8*)data;
  self->dataend = self->data + datalen;
  self->datalen = datalen;
  self->num_huff_idx = 0;

  int ret = findSoI(self);

  if (ret == LJ92_ERROR_NONE) {
    u16* rowcache = (u16*)calloc(self->x * self->components * 2, sizeof(u16));
    if (rowcache == NULL)
      ret = LJ92_ERROR_NO_MEMORY;
    else {
      self->rowcache = rowcache;
      self->outrow[0] = rowcache;
      self->outrow[1] = &rowcache[self->x];
    }
  }

  if (ret != LJ92_ERROR_NONE) {  // Failed, clean up
    *lj = NULL;
    free_memory(self);
    free(self);
  } else {
    *width = self->x;
    *height = self->y;
    *bitdepth = self->bits;
    *lj = self;
  }
  return ret;
}

int lj92_decode(lj92 lj, uint16_t* target, int writeLength, int skipLength,
                uint16_t* linearize, int linearizeLength) {
  int ret = LJ92_ERROR_NONE;
  ljp* self = lj;
  if (self == NULL) return LJ92_ERROR_BAD_HANDLE;
  self->image = target;
  self->writelen = writeLength;
  self->skiplen = skipLength;
  self->linearize = linearize;
  self->linlen = linearizeLength;
  ret = parseScan(self);
  return ret;
}

void lj92_close(lj92 lj) {
  ljp* self = lj;
  if (self != NULL) free_memory(self);
  free(self);
}

#if 0  // not used in tinydngloader
/* Encoder implementation */

// Very simple count leading zero implementation.
static int clz32(unsigned int x) {
  int n;
  if (x == 0) return 32;
  for (n = 0; ((x & 0x80000000) == 0); n++, x <<= 1)
    ;
  return n;
}

typedef struct _lje {
  uint16_t* image;
  int width;
  int height;
  int bitdepth;
  int components;
  int readLength;
  int skipLength;
  uint16_t* delinearize;
  int delinearizeLength;
  uint8_t* encoded;
  int encodedWritten;
  int encodedLength;
  int hist[17];  // SSSS frequency histogram
  int bits[17];
  int huffval[17];
  u16 huffenc[17];
  u16 huffbits[17];
  int huffsym[17];
} lje;

int frequencyScan(lje* self) {
  // Scan through the tile using the standard type 6 prediction
  // Need to cache the previous 2 row in target coordinates because of tiling
  uint16_t* pixel = self->image;
  int pixcount = self->width * self->height;
  int scan = self->readLength;
  uint16_t* rowcache = (uint16_t*)calloc(1, self->width * self->components * 4);
  uint16_t* rows[2];
  rows[0] = rowcache;
  rows[1] = &rowcache[self->width];

  int col = 0;
  int row = 0;
  int Px = 0;
  int32_t diff = 0;
  int maxval = (1 << self->bitdepth);
  while (pixcount--) {
    uint16_t p = *pixel;
    if (self->delinearize) {
      if (p >= self->delinearizeLength) {
        free(rowcache);
        return LJ92_ERROR_TOO_WIDE;
      }
      p = self->delinearize[p];
    }
    if (p >= maxval) {
      free(rowcache);
      return LJ92_ERROR_TOO_WIDE;
    }
    rows[1][col] = p;

    if ((row == 0) && (col == 0))
      Px = 1 << (self->bitdepth - 1);
    else if (row == 0)
      Px = rows[1][col - 1];
    else if (col == 0)
      Px = rows[0][col];
    else
      Px = rows[0][col] + ((rows[1][col - 1] - rows[0][col - 1]) >> 1);
    diff = rows[1][col] - Px;
    // int ssss = 32 - __builtin_clz(abs(diff));
    int ssss = 32 - clz32(abs(diff));
    if (diff == 0) ssss = 0;
    self->hist[ssss]++;
    // TINY_DNG_DPRINTF("%d %d %d %d %d %d\n",col,row,p,Px,diff,ssss);
    pixel++;
    scan--;
    col++;
    if (scan == 0) {
      pixel += self->skipLength;
      scan = self->readLength;
    }
    if (col == self->width) {
      uint16_t* tmprow = rows[1];
      rows[1] = rows[0];
      rows[0] = tmprow;
      col = 0;
      row++;
    }
  }
#ifdef LJ92_DEBUG
  int sort[17];
  for (int h = 0; h < 17; h++) {
    sort[h] = h;
    TINY_DNG_DPRINTF("%d:%d\n", h, self->hist[h]);
  }
#endif
  free(rowcache);
  return LJ92_ERROR_NONE;
}

void createEncodeTable(lje* self) {
  float freq[18];
  int codesize[18];
  int others[18];

  // Calculate frequencies
  float totalpixels = self->width * self->height;
  for (int i = 0; i < 17; i++) {
    freq[i] = (float)(self->hist[i]) / totalpixels;
#ifdef LJ92_DEBUG
    TINY_DNG_DPRINTF("%d:%f\n", i, freq[i]);
#endif
    codesize[i] = 0;
    others[i] = -1;
  }
  codesize[17] = 0;
  others[17] = -1;
  freq[17] = 1.0f;

  float v1f, v2f;
  int v1, v2;

  while (1) {
    v1f = 3.0f;
    v1 = -1;
    for (int i = 0; i < 18; i++) {
      if ((freq[i] <= v1f) && (freq[i] > 0.0f)) {
        v1f = freq[i];
        v1 = i;
      }
    }
#ifdef LJ92_DEBUG
    TINY_DNG_DPRINTF("v1:%d,%f\n", v1, v1f);
#endif
    v2f = 3.0f;
    v2 = -1;
    for (int i = 0; i < 18; i++) {
      if (i == v1) continue;
      if ((freq[i] < v2f) && (freq[i] > 0.0f)) {
        v2f = freq[i];
        v2 = i;
      }
    }
    if (v2 == -1) break;  // Done

    freq[v1] += freq[v2];
    freq[v2] = 0.0f;

    while (1) {
      codesize[v1]++;
      if (others[v1] == -1) break;
      v1 = others[v1];
    }
    others[v1] = v2;
    while (1) {
      codesize[v2]++;
      if (others[v2] == -1) break;
      v2 = others[v2];
    }
  }
  int* bits = self->bits;
  memset(bits, 0, sizeof(self->bits));
  for (int i = 0; i < 18; i++) {
    if (codesize[i] != 0) {
      bits[codesize[i]]++;
    }
  }
#ifdef LJ92_DEBUG
  for (int i = 0; i < 17; i++) {
    TINY_DNG_DPRINTF("bits:%d,%d,%d\n", i, bits[i], codesize[i]);
  }
#endif
  int* huffval = self->huffval;
  int i = 1;
  int k = 0;
  int j;
  memset(huffval, 0, sizeof(self->huffval));
  while (i <= 32) {
    j = 0;
    while (j < 17) {
      if (codesize[j] == i) {
        huffval[k++] = j;
      }
      j++;
    }
    i++;
  }
#ifdef LJ92_DEBUG
  for (i = 0; i < 17; i++) {
    TINY_DNG_DPRINTF("i=%d,huffval[i]=%x\n", i, huffval[i]);
  }
#endif
  int maxbits = 16;
  while (maxbits > 0) {
    if (bits[maxbits]) break;
    maxbits--;
  }
  u16* huffenc = self->huffenc;
  u16* huffbits = self->huffbits;
  int* huffsym = self->huffsym;
  memset(huffenc, 0, sizeof(self->huffenc));
  memset(huffbits, 0, sizeof(self->huffbits));
  memset(self->huffsym, 0, sizeof(self->huffsym));
  i = 0;
  int hv = 0;
  int rv = 0;
  int vl = 0;  // i
  // int hcode;
  int bitsused = 1;
  int sym = 0;
  // TINY_DNG_DPRINTF("%04x:%x:%d:%x\n",i,huffvals[hv],bitsused,1<<(maxbits-bitsused));
  while (i < 1 << maxbits) {
    if (bitsused > maxbits) {
      break;  // Done. Should never get here!
    }
    if (vl >= bits[bitsused]) {
      bitsused++;
      vl = 0;
      continue;
    }
    if (rv == 1 << (maxbits - bitsused)) {
      rv = 0;
      vl++;
      hv++;
      // TINY_DNG_DPRINTF("%04x:%x:%d:%x\n",i,huffvals[hv],bitsused,1<<(maxbits-bitsused));
      continue;
    }
    huffbits[sym] = bitsused;
    huffenc[sym++] = i >> (maxbits - bitsused);
    // TINY_DNG_DPRINTF("%d %d %d\n",i,bitsused,hcode);
    i += (1 << (maxbits - bitsused));
    rv = 1 << (maxbits - bitsused);
  }
  for (i = 0; i < 17; i++) {
    if (huffbits[i] > 0) {
      huffsym[huffval[i]] = i;
    }
#ifdef LJ92_DEBUG
    TINY_DNG_DPRINTF("huffval[%d]=%d,huffenc[%d]=%x,bits=%d\n", i, huffval[i],
                     i, huffenc[i], huffbits[i]);
#endif
    if (huffbits[i] > 0) {
      huffsym[huffval[i]] = i;
    }
  }
#ifdef LJ92_DEBUG
  for (i = 0; i < 17; i++) {
    TINY_DNG_DPRINTF("huffsym[%d]=%d\n", i, huffsym[i]);
  }
#endif
}

void writeHeader(lje* self) {
  int w = self->encodedWritten;
  uint8_t* e = self->encoded;
  e[w++] = 0xff;
  e[w++] = 0xd8;  // SOI
  e[w++] = 0xff;
  e[w++] = 0xc3;  // SOF3
  // Write SOF
  e[w++] = 0x0;
  e[w++] = 11;  // Lf, frame header length
  e[w++] = self->bitdepth;
  e[w++] = self->height >> 8;
  e[w++] = self->height & 0xFF;
  e[w++] = self->width >> 8;
  e[w++] = self->width & 0xFF;
  e[w++] = 1;     // Components
  e[w++] = 0;     // Component ID
  e[w++] = 0x11;  // Component X/Y
  e[w++] = 0;     // Unused (Quantisation)
  e[w++] = 0xff;
  e[w++] = 0xc4;  // HUFF
  // Write HUFF
  int count = 0;
  for (int i = 0; i < 17; i++) {
    count += self->bits[i];
  }
  e[w++] = 0x0;
  e[w++] = 17 + 2 + count;  // Lf, frame header length
  e[w++] = 0;               // Table ID
  for (int i = 1; i < 17; i++) {
    e[w++] = self->bits[i];
  }
  for (int i = 0; i < count; i++) {
    e[w++] = self->huffval[i];
  }
  e[w++] = 0xff;
  e[w++] = 0xda;  // SCAN
  // Write SCAN
  e[w++] = 0x0;
  e[w++] = 8;  // Ls, scan header length
  e[w++] = 1;  // Components
  e[w++] = 0;  //
  e[w++] = 0;  //
  e[w++] = 6;  // Predictor
  e[w++] = 0;  //
  e[w++] = 0;  //
  self->encodedWritten = w;
}

void writePost(lje* self) {
  int w = self->encodedWritten;
  uint8_t* e = self->encoded;
  e[w++] = 0xff;
  e[w++] = 0xd9;  // EOI
  self->encodedWritten = w;
}

void writeBody(lje* self) {
  // Scan through the tile using the standard type 6 prediction
  // Need to cache the previous 2 row in target coordinates because of tiling
  uint16_t* pixel = self->image;
  int pixcount = self->width * self->height;
  int scan = self->readLength;
  uint16_t* rowcache = (uint16_t*)calloc(1, self->width * self->components * 4);
  uint16_t* rows[2];
  rows[0] = rowcache;
  rows[1] = &rowcache[self->width];

  int col = 0;
  int row = 0;
  int Px = 0;
  int32_t diff = 0;
  int bitcount = 0;
  uint8_t* out = self->encoded;
  int w = self->encodedWritten;
  uint8_t next = 0;
  uint8_t nextbits = 8;
  while (pixcount--) {
    uint16_t p = *pixel;
    if (self->delinearize) p = self->delinearize[p];
    rows[1][col] = p;

    if ((row == 0) && (col == 0))
      Px = 1 << (self->bitdepth - 1);
    else if (row == 0)
      Px = rows[1][col - 1];
    else if (col == 0)
      Px = rows[0][col];
    else
      Px = rows[0][col] + ((rows[1][col - 1] - rows[0][col - 1]) >> 1);
    diff = rows[1][col] - Px;
    // int ssss = 32 - __builtin_clz(abs(diff));
    int ssss = 32 - clz32(abs(diff));
    if (diff == 0) ssss = 0;
    // TINY_DNG_DPRINTF("%d %d %d %d %d\n",col,row,Px,diff,ssss);

    // Write the huffman code for the ssss value
    int huffcode = self->huffsym[ssss];
    int huffenc = self->huffenc[huffcode];
    int huffbits = self->huffbits[huffcode];
    bitcount += huffbits + ssss;

    int vt = ssss > 0 ? (1 << (ssss - 1)) : 0;
// TINY_DNG_DPRINTF("%d %d %d %d\n",rows[1][col],Px,diff,Px+diff);
#ifdef LJ92_DEBUG
#endif
    if (diff < vt) diff += (1 << (ssss)) - 1;

    // Write the ssss
    while (huffbits > 0) {
      int usebits = huffbits > nextbits ? nextbits : huffbits;
      // Add top usebits from huffval to next usebits of nextbits
      int tophuff = huffenc >> (huffbits - usebits);
      next |= (tophuff << (nextbits - usebits));
      nextbits -= usebits;
      huffbits -= usebits;
      huffenc &= (1 << huffbits) - 1;
      if (nextbits == 0) {
        out[w++] = next;
        if (next == 0xff) out[w++] = 0x0;
        next = 0;
        nextbits = 8;
      }
    }
    // Write the rest of the bits for the value

    while (ssss > 0) {
      int usebits = ssss > nextbits ? nextbits : ssss;
      // Add top usebits from huffval to next usebits of nextbits
      int tophuff = diff >> (ssss - usebits);
      next |= (tophuff << (nextbits - usebits));
      nextbits -= usebits;
      ssss -= usebits;
      diff &= (1 << ssss) - 1;
      if (nextbits == 0) {
        out[w++] = next;
        if (next == 0xff) out[w++] = 0x0;
        next = 0;
        nextbits = 8;
      }
    }

    // TINY_DNG_DPRINTF("%d %d\n",diff,ssss);
    pixel++;
    scan--;
    col++;
    if (scan == 0) {
      pixel += self->skipLength;
      scan = self->readLength;
    }
    if (col == self->width) {
      uint16_t* tmprow = rows[1];
      rows[1] = rows[0];
      rows[0] = tmprow;
      col = 0;
      row++;
    }
  }
  // Flush the final bits
  if (nextbits < 8) {
    out[w++] = next;
    if (next == 0xff) out[w++] = 0x0;
  }
#ifdef LJ92_DEBUG
  int sort[17];
  for (int h = 0; h < 17; h++) {
    sort[h] = h;
    TINY_DNG_DPRINTF("%d:%d\n", h, self->hist[h]);
  }
  TINY_DNG_DPRINTF("Total bytes: %d\n", bitcount >> 3);
#endif
  free(rowcache);
  self->encodedWritten = w;
}

/* Encoder
 * Read tile from an image and encode in one shot
 * Return the encoded data
 */
int lj92_encode(uint16_t* image, int width, int height, int bitdepth,
                int readLength, int skipLength, uint16_t* delinearize,
                int delinearizeLength, uint8_t** encoded, int* encodedLength) {
  int ret = LJ92_ERROR_NONE;

  lje* self = (lje*)calloc(sizeof(lje), 1);
  if (self == NULL) return LJ92_ERROR_NO_MEMORY;
  self->image = image;
  self->width = width;
  self->height = height;
  self->bitdepth = bitdepth;
  self->readLength = readLength;
  self->skipLength = skipLength;
  self->delinearize = delinearize;
  self->delinearizeLength = delinearizeLength;
  self->encodedLength = width * height * 3 + 200;
  self->encoded = (uint8_t*)malloc(self->encodedLength);
  if (self->encoded == NULL) {
    free(self);
    return LJ92_ERROR_NO_MEMORY;
  }
  // Scan through data to gather frequencies of ssss prefixes
  ret = frequencyScan(self);
  if (ret != LJ92_ERROR_NONE) {
    free(self->encoded);
    free(self);
    return ret;
  }
  // Create encoded table based on frequencies
  createEncodeTable(self);
  // Write JPEG head and scan header
  writeHeader(self);
  // Scan through and do the compression
  writeBody(self);
  // Finish
  writePost(self);
#ifdef LJ92_DEBUG
  TINY_DNG_DPRINTF("written:%d\n", self->encodedWritten);
#endif
  self->encoded = (uint8_t*)realloc(self->encoded, self->encodedWritten);
  self->encodedLength = self->encodedWritten;
  *encoded = self->encoded;
  *encodedLength = self->encodedLength;

  free(self);

  return ret;
}
#endif

// End liblj92 ---------------------------------------------------------
}  // namespace

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef __clang__
#pragma clang diagnostic push
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#endif

typedef enum {
  TAG_NEW_SUBFILE_TYPE = 254,
  TAG_SUBFILE_TYPE = 255,
  TAG_IMAGE_WIDTH = 256,
  TAG_IMAGE_HEIGHT = 257,
  TAG_BITS_PER_SAMPLE = 258,
  TAG_COMPRESSION = 259,
  TAG_STRIP_OFFSET = 273,
  TAG_ORIENTATION = 274,
  TAG_SAMPLES_PER_PIXEL = 277,
  TAG_ROWS_PER_STRIP = 278,
  TAG_STRIP_BYTE_COUNTS = 279,
  TAG_PLANAR_CONFIGURATION = 284,
  TAG_PREDICTOR = 317,
  TAG_SUB_IFDS = 330,
  TAG_TILE_WIDTH = 322,
  TAG_TILE_LENGTH = 323,
  TAG_TILE_OFFSETS = 324,
  TAG_TILE_BYTE_COUNTS = 325,
  TAG_SAMPLE_FORMAT = 339,
  TAG_JPEG_IF_OFFSET = 513,
  TAG_JPEG_IF_BYTE_COUNT = 514,
  TAG_CFA_PATTERN_DIM = 33421,
  TAG_CFA_PATTERN = 33422,
  TAG_CFA_PLANE_COLOR = 50710,
  TAG_CFA_LAYOUT = 50711,
  TAG_BLACK_LEVEL = 50714,
  TAG_WHITE_LEVEL = 50717,
  TAG_COLOR_MATRIX1 = 50721,
  TAG_COLOR_MATRIX2 = 50722,
  TAG_CAMERA_CALIBRATION1 = 50723,
  TAG_CAMERA_CALIBRATION2 = 50724,
  TAG_DNG_VERSION = 50706,
  TAG_ANALOG_BALANCE = 50727,
  TAG_AS_SHOT_NEUTRAL = 50728,
  TAG_CALIBRATION_ILLUMINANT1 = 50778,
  TAG_CALIBRATION_ILLUMINANT2 = 50779,
  TAG_ACTIVE_AREA = 50829,
  TAG_FORWARD_MATRIX1 = 50964,
  TAG_FORWARD_MATRIX2 = 50965,

  // CR2 extension
  // http://lclevy.free.fr/cr2/
  TAG_CR2_META0 = 50648,
  TAG_CR2_META1 = 50656,
  TAG_CR2_SLICES = 50752,
  TAG_CR2_META2 = 50885,

  TAG_INVALID = 65535
} TiffTag;

static void swap2(unsigned short* val) {
  unsigned short tmp = *val;
  unsigned char* dst = reinterpret_cast<unsigned char*>(val);
  unsigned char* src = reinterpret_cast<unsigned char*>(&tmp);

  dst[0] = src[1];
  dst[1] = src[0];
}

static void swap4(unsigned int* val) {
  unsigned int tmp = *val;
  unsigned char* dst = reinterpret_cast<unsigned char*>(val);
  unsigned char* src = reinterpret_cast<unsigned char*>(&tmp);

  dst[0] = src[3];
  dst[1] = src[2];
  dst[2] = src[1];
  dst[3] = src[0];
}

static inline void swap4(int* val) {
  int tmp = *val;
  unsigned char* dst = reinterpret_cast<unsigned char*>(val);
  unsigned char* src = reinterpret_cast<unsigned char*>(&tmp);

  dst[0] = src[3];
  dst[1] = src[2];
  dst[2] = src[1];
  dst[3] = src[0];
}

static inline void swap8(uint64_t* val) {
  uint64_t tmp = (*val);
  unsigned char* dst = reinterpret_cast<unsigned char*>(val);
  unsigned char* src = reinterpret_cast<unsigned char*>(&tmp);

  dst[0] = src[7];
  dst[1] = src[6];
  dst[2] = src[5];
  dst[3] = src[4];
  dst[4] = src[3];
  dst[5] = src[2];
  dst[6] = src[1];
  dst[7] = src[0];
}

static inline void swap8(int64_t* val) {
  int64_t tmp = (*val);
  unsigned char* dst = reinterpret_cast<unsigned char*>(val);
  unsigned char* src = reinterpret_cast<unsigned char*>(&tmp);

  dst[0] = src[7];
  dst[1] = src[6];
  dst[2] = src[5];
  dst[3] = src[4];
  dst[4] = src[3];
  dst[5] = src[2];
  dst[6] = src[1];
  dst[7] = src[0];
}

// For unaligned read

static void cpy2(unsigned short* dst_val, const unsigned short* src_val) {
  unsigned char* dst = reinterpret_cast<unsigned char*>(dst_val);
  const unsigned char* src = reinterpret_cast<const unsigned char*>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
}

static void cpy2(short* dst_val, const short* src_val) {
  unsigned char* dst = reinterpret_cast<unsigned char*>(dst_val);
  const unsigned char* src = reinterpret_cast<const unsigned char*>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
}

static void cpy4(unsigned int* dst_val, const unsigned int* src_val) {
  unsigned char* dst = reinterpret_cast<unsigned char*>(dst_val);
  const unsigned char* src = reinterpret_cast<const unsigned char*>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

static void cpy4(int* dst_val, const int* src_val) {
  unsigned char* dst = reinterpret_cast<unsigned char*>(dst_val);
  const unsigned char* src = reinterpret_cast<const unsigned char*>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

static void cpy8(uint64_t* dst_val, const uint64_t* src_val) {
  unsigned char* dst = reinterpret_cast<unsigned char*>(dst_val);
  const unsigned char* src = reinterpret_cast<const unsigned char*>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
  dst[4] = src[4];
  dst[5] = src[5];
  dst[6] = src[6];
  dst[7] = src[7];
}

static void cpy8(int64_t* dst_val, const int64_t* src_val) {
  unsigned char* dst = reinterpret_cast<unsigned char*>(dst_val);
  const unsigned char* src = reinterpret_cast<const unsigned char*>(src_val);

  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
  dst[4] = src[4];
  dst[5] = src[5];
  dst[6] = src[6];
  dst[7] = src[7];
}

///
/// Simple stream reader
///
class StreamReader {
 public:
  explicit StreamReader(const uint8_t* binary, const size_t length,
                        const bool swap_endian)
      : binary_(binary), length_(length), swap_endian_(swap_endian), idx_(0) {
    (void)pad_;
  }

  bool seek_set(const uint64_t offset) const {
    if (offset > length_) {
      return false;
    }

    idx_ = offset;
    return true;
  }

  bool seek_from_currect(const int64_t offset) const {
    if ((int64_t(idx_) + offset) < 0) {
      return false;
    }

    if (size_t((int64_t(idx_) + offset)) > length_) {
      return false;
    }

    idx_ = size_t(int64_t(idx_) + offset);
    return true;
  }

  size_t read(const size_t n, const uint64_t dst_len,
              unsigned char* dst) const {
    size_t len = n;
    if ((idx_ + len) > length_) {
      len = length_ - idx_;
    }

    if (len > 0) {
      if (dst_len < len) {
        // dst does not have enough space. return 0 for a while.
        return 0;
      }

      memcpy(dst, &binary_[idx_], len);
      idx_ += len;
      return len;

    } else {
      return 0;
    }
  }

  bool read1(unsigned char* ret) const {
    if ((idx_ + 1) > length_) {
      return false;
    }

    const unsigned char val = binary_[idx_];

    (*ret) = val;
    idx_ += 1;

    return true;
  }

  bool read_bool(bool* ret) const {
    if ((idx_ + 1) > length_) {
      return false;
    }

    const char val = static_cast<const char>(binary_[idx_]);

    (*ret) = bool(val);
    idx_ += 1;

    return true;
  }

  bool read1(char* ret) const {
    if ((idx_ + 1) > length_) {
      return false;
    }

    const char val = static_cast<const char>(binary_[idx_]);

    (*ret) = val;
    idx_ += 1;

    return true;
  }

  bool read2(unsigned short* ret) const {
    if ((idx_ + 2) > length_) {
      return false;
    }

    unsigned short val = 0;
    cpy2(&val, reinterpret_cast<const unsigned short*>(&binary_[idx_]));

    if (swap_endian_) {
      swap2(&val);
    }

    (*ret) = val;
    idx_ += 2;

    return true;
  }

  bool read2(short* ret) const {
    if ((idx_ + 2) > length_) {
      return false;
    }

    short val = 0;
    cpy2(&val, reinterpret_cast<const short*>(&binary_[idx_]));

    if (swap_endian_) {
      swap2(reinterpret_cast<unsigned short*>(&val));
    }

    (*ret) = val;
    idx_ += 2;

    return true;
  }

  bool read4(unsigned int* ret) const {
    if ((idx_ + 4) > length_) {
      return false;
    }

    unsigned int val = 0;
    cpy4(&val, reinterpret_cast<const unsigned int*>(&binary_[idx_]));

    if (swap_endian_) {
      swap4(&val);
    }

    (*ret) = val;
    idx_ += 4;

    return true;
  }

  bool read4(int* ret) const {
    if ((idx_ + 4) > length_) {
      return false;
    }

    int val = 0;
    cpy4(&val, reinterpret_cast<const int*>(&binary_[idx_]));

    if (swap_endian_) {
      swap4(&val);
    }

    (*ret) = val;
    idx_ += 4;

    return true;
  }

  bool read8(uint64_t* ret) const {
    if ((idx_ + 8) > length_) {
      return false;
    }

    uint64_t val = 0;
    cpy8(&val, reinterpret_cast<const uint64_t*>(&binary_[idx_]));

    if (swap_endian_) {
      swap8(&val);
    }

    (*ret) = val;
    idx_ += 8;

    return true;
  }

  bool read8(int64_t* ret) const {
    if ((idx_ + 8) > length_) {
      return false;
    }

    int64_t val = 0;
    cpy8(&val, reinterpret_cast<const int64_t*>(&binary_[idx_]));

    if (swap_endian_) {
      swap8(&val);
    }

    (*ret) = val;
    idx_ += 8;

    return true;
  }

  bool read_float(float* ret) const {
    if (!ret) {
      return false;
    }

    float value = 0.0f;
    if (!read4(reinterpret_cast<int*>(&value))) {
      return false;
    }

    (*ret) = value;

    return true;
  }

  bool read_double(double* ret) const {
    if (!ret) {
      return false;
    }

    double value = 0.0;
    if (!read8(reinterpret_cast<uint64_t*>(&value))) {
      return false;
    }

    (*ret) = value;

    return true;
  }

  bool read_uint(int type, unsigned int* ret) const {
    // @todo {8, 9, 10, 11, 12}
    if (type == 3) {
      unsigned short val;
      if (!read2(&val)) {
        return false;
      }
      (*ret) = static_cast<unsigned int>(val);
      return true;
    } else if (type == 5) {
      unsigned int val0;
      if (!read4(&val0)) {
        return false;
      }

      unsigned int val1;
      if (!read4(&val1)) {
        return false;
      }

      (*ret) = static_cast<unsigned int>(val0 / val1);
      return true;

    } else if (type == 4) {
      unsigned int val;
      if (!read4(&val)) {
        return false;
      }
      (*ret) = val;
      return true;
    } else {
      return false;
    }
  }

  bool read_real(int type, double* ret) const {
    // @todo { Support more types. }

    if (type == TYPE_RATIONAL) {
      unsigned int num;
      if (!read4(&num)) {
        return false;
      }
      unsigned int denom;
      if (!read4(&denom)) {
        return false;
      }

      (*ret) = static_cast<double>(num) / static_cast<double>(denom);
      return true;
    } else if (type == TYPE_SRATIONAL) {
      int num;
      if (!read4(&num)) {
        return false;
      }
      int denom;
      if (!read4(&denom)) {
        return false;
      }

      (*ret) = static_cast<double>(num) / static_cast<double>(denom);
      return true;
    } else {
      return false;
    }
    // never come here.
  }

  //
  // Returns a memory address. The begining of address is computed in
  // relative(based on current seek pos). This function is useful when you just
  // want to access the content in read-only mode.
  //
  // Note that the function does not change seek position after the call.
  // This function does the bound check.
  //
  // @param[in] offset Extra byte offset. 0 = use current seek position.
  // @param[in] length Byte length to map.
  //
  // @return nullptr when failed to map address.
  //
  const uint8_t* map_addr(size_t offset, const size_t length) {
    if (length == 0) {
      return NULL;
    }

    if ((idx_ + offset) > length_) {
      return NULL;
    }

    if ((idx_ + offset + length) > length_) {
      return NULL;
    }

    return &binary_[idx_ + offset];
  }

  //
  // Returns a memory address. The begining of address is specified by
  // absolute(ignores current seek pos). This function is useful when you just
  // want to access the content in read-only mode.
  //
  // Note that the function does not change seek position after the call.
  // This function does the bound check.
  //
  // @param[in] pos Absolute position in bytes.
  // @param[in] length Byte length to map.
  //
  // @return nullptr when failed to map address.
  //
  const uint8_t* map_abs_addr(size_t pos, const size_t length) {
    if (length == 0) {
      return NULL;
    }

    if (pos > length_) {
      return NULL;
    }

    if ((pos + length) > length_) {
      return NULL;
    }

    return &binary_[pos];
  }

  size_t tell() const { return idx_; }

  const uint8_t* data() const { return binary_; }

  bool swap_endian() const { return swap_endian_; }

  size_t size() const { return length_; }

 private:
  const uint8_t* binary_;
  const size_t length_;
  bool swap_endian_;
  char pad_[7];
  mutable uint64_t idx_;
};

static bool GetTIFFTag(const StreamReader& sr, unsigned short* tag,
                       unsigned short* type, unsigned int* len,
                       unsigned int* saved_offt) {
  if (!sr.read2(tag)) {
    return false;
  }

  if (!sr.read2(type)) {
    return false;
  }

  if (!sr.read4(len)) {
    return false;
  }

  (*saved_offt) = static_cast<unsigned int>(sr.tell()) + 4;

  size_t typesize_table[] = {1, 1, 1, 2, 4, 8, 1, 1, 2, 4, 8, 4, 8, 4};

  if ((*len) * (typesize_table[(*type) < 14 ? (*type) : 0]) > 4) {
    unsigned int base = 0;  // fixme
    unsigned int offt = 0;
    if (!sr.read4(&offt)) {
      return false;
    }
    if (!sr.seek_set(offt + base)) {
      return false;
    }
  }

  return true;
}

static void InitializeDNGImage(tinydng::DNGImage* image) {
  image->version = 0;

  image->color_matrix1[0][0] = 1.0;
  image->color_matrix1[0][1] = 0.0;
  image->color_matrix1[0][2] = 0.0;
  image->color_matrix1[1][0] = 0.0;
  image->color_matrix1[1][1] = 1.0;
  image->color_matrix1[1][2] = 0.0;
  image->color_matrix1[2][0] = 0.0;
  image->color_matrix1[2][1] = 0.0;
  image->color_matrix1[2][2] = 1.0;

  image->color_matrix2[0][0] = 1.0;
  image->color_matrix2[0][1] = 0.0;
  image->color_matrix2[0][2] = 0.0;
  image->color_matrix2[1][0] = 0.0;
  image->color_matrix2[1][1] = 1.0;
  image->color_matrix2[1][2] = 0.0;
  image->color_matrix2[2][0] = 0.0;
  image->color_matrix2[2][1] = 0.0;
  image->color_matrix2[2][2] = 1.0;

  image->forward_matrix1[0][0] = 1.0;
  image->forward_matrix1[0][1] = 0.0;
  image->forward_matrix1[0][2] = 0.0;
  image->forward_matrix1[1][0] = 0.0;
  image->forward_matrix1[1][1] = 1.0;
  image->forward_matrix1[1][2] = 0.0;
  image->forward_matrix1[2][0] = 0.0;
  image->forward_matrix1[2][1] = 0.0;
  image->forward_matrix1[2][2] = 1.0;

  image->forward_matrix2[0][0] = 1.0;
  image->forward_matrix2[0][1] = 0.0;
  image->forward_matrix2[0][2] = 0.0;
  image->forward_matrix2[1][0] = 0.0;
  image->forward_matrix2[1][1] = 1.0;
  image->forward_matrix2[1][2] = 0.0;
  image->forward_matrix2[2][0] = 0.0;
  image->forward_matrix2[2][1] = 0.0;
  image->forward_matrix2[2][2] = 1.0;

  image->camera_calibration1[0][0] = 1.0;
  image->camera_calibration1[0][1] = 0.0;
  image->camera_calibration1[0][2] = 0.0;
  image->camera_calibration1[1][0] = 0.0;
  image->camera_calibration1[1][1] = 1.0;
  image->camera_calibration1[1][2] = 0.0;
  image->camera_calibration1[2][0] = 0.0;
  image->camera_calibration1[2][1] = 0.0;
  image->camera_calibration1[2][2] = 1.0;

  image->camera_calibration2[0][0] = 1.0;
  image->camera_calibration2[0][1] = 0.0;
  image->camera_calibration2[0][2] = 0.0;
  image->camera_calibration2[1][0] = 0.0;
  image->camera_calibration2[1][1] = 1.0;
  image->camera_calibration2[1][2] = 0.0;
  image->camera_calibration2[2][0] = 0.0;
  image->camera_calibration2[2][1] = 0.0;
  image->camera_calibration2[2][2] = 1.0;

  image->calibration_illuminant1 = LIGHTSOURCE_UNKNOWN;
  image->calibration_illuminant2 = LIGHTSOURCE_UNKNOWN;

  image->white_level[0] = -1;  // White level will be set after parsing TAG.
                               // The spec says: The default value for this
                               // tag is (2 ** BitsPerSample)
                               // -1 for unsigned integer images, and 1.0 for
                               // floating point images.

  image->white_level[1] = -1;
  image->white_level[2] = -1;
  image->white_level[3] = -1;
  image->black_level[0] = 0;
  image->black_level[1] = 0;
  image->black_level[2] = 0;
  image->black_level[3] = 0;

  image->bits_per_sample = 0;

  image->has_active_area = false;
  image->active_area[0] = -1;
  image->active_area[1] = -1;
  image->active_area[2] = -1;
  image->active_area[3] = -1;

  image->cfa_plane_color[0] = 0;
  image->cfa_plane_color[1] = 1;
  image->cfa_plane_color[2] = 2;
  image->cfa_plane_color[3] = 0;  // optional?

  image->cfa_pattern_dim = 2;

  // The spec says default is None, thus fill with -1(=invalid).
  image->cfa_pattern[0][0] = -1;
  image->cfa_pattern[0][1] = -1;
  image->cfa_pattern[1][0] = -1;
  image->cfa_pattern[1][1] = -1;

  image->cfa_layout = 1;

  image->offset = 0;

  image->tile_width = -1;
  image->tile_length = -1;
  image->tile_offset = 0;

  image->planar_configuration = 1;  // chunky

  image->predictor = 1;  // no prediction scheme

  image->has_analog_balance = false;
  image->has_as_shot_neutral = false;

  image->jpeg_byte_count = -1;
  image->strip_byte_count = -1;

  image->samples_per_pixel = 1;
  image->rows_per_strip = -1;  // 2^32 - 1
  image->bits_per_sample_original = 1;

  image->sample_format = SAMPLEFORMAT_UINT;

  image->compression = COMPRESSION_NONE;

  image->orientation = 1;

  image->strips_per_image = -1;  // 2^32 - 1

  // CR2 specific
  image->cr2_slices[0] = 0;
  image->cr2_slices[1] = 0;
  image->cr2_slices[2] = 0;
}

// Check if JPEG data is lossless JPEG or not(baseline JPEG)
static bool IsLosslessJPEG(const uint8_t* header_addr, int data_len, int* width,
                           int* height, int* bits, int* components) {
  TINY_DNG_DPRINTF("islossless jpeg\n");
  int lj_width = 0;
  int lj_height = 0;
  int lj_bits = 0;
  lj92 ljp;
  int ret =
      lj92_open(&ljp, header_addr, data_len, &lj_width, &lj_height, &lj_bits);
  if (ret == LJ92_ERROR_NONE) {
    // TINY_DNG_DPRINTF("w = %d, h = %d, bits = %d, components = %d\n",
    // lj_width,
    // lj_height, lj_bits, ljp->components);

    if ((lj_width == 0) || (lj_height == 0) || (lj_bits == 0) ||
        (lj_bits == 8)) {
      // Looks like baseline JPEG
      lj92_close(ljp);
      return false;
    }

    if (components) (*components) = ljp->components;

    lj92_close(ljp);

    if (width) (*width) = lj_width;
    if (height) (*height) = lj_height;
    if (bits) (*bits) = lj_bits;
  }
  return (ret == LJ92_ERROR_NONE) ? true : false;
}

#ifdef TINY_DNG_LOADER_ENABLE_ZIP

static bool DecompressZIP(unsigned char* dst,
                          unsigned long* uncompressed_size /* inout */,
                          const unsigned char* src, unsigned long src_size,
                          std::string* err) {
  std::vector<unsigned char> tmpBuf(*uncompressed_size);

#ifdef TINY_DNG_LOADER_USE_SYSTEM_ZLIB
  int ret = uncompress(&tmpBuf.at(0), uncompressed_size, src, src_size);
  if (Z_OK != ret) {
    if (err) {
      std::stringstream ss;
      ss << "zlib uncompress failed. code = " << ret << "\n";
      (*err) += ss.str();
    }
    return false;
  }
#else
  int ret = mz_uncompress(&tmpBuf.at(0), uncompressed_size, src, src_size);
  if (MZ_OK != ret) {
    if (err) {
      std::stringstream ss;
      ss << "mz_uncompress failed. code = " << ret << "\n";
      (*err) += ss.str();
    }
    return false;
  }
#endif

  memcpy(dst, tmpBuf.data(), (*uncompressed_size));

  return true;
}

// Assume T = uint8 or uint16
static bool UnpredictImageU8(std::vector<uint8_t>& dst,  // inout
                             int predictor, const size_t width,
                             const size_t rows, const size_t spp) {
  if (predictor == 1) {
    // no prediction shceme
    return true;
  } else if (predictor == 2) {
    // horizontal diff
    const size_t stride = size_t(width * spp);
    for (size_t row = 0; row < rows; row++) {
      for (size_t c = 0; c < spp; c++) {
        unsigned int b = dst[row * stride + c];
        for (size_t col = 1; col < width; col++) {
          // value may overflow(wrap over), but its expected behavior.
          b += dst[stride * row + spp * col + c];
          dst[stride * row + spp * col + c] =
              static_cast<unsigned char>(b & 0xFF);
        }
      }
    }
    return true;
  } else {
    // TODO
    return false;
  }
}

static bool DecompressZIPedTile(const StreamReader& sr, unsigned char* dst_data,
                                int dst_width, const DNGImage& image_info,
                                std::string* err) {
  unsigned int tiff_h = 0, tiff_w = 0;
  int offset = 0;

  (void)dst_data;
  (void)dst_width;

#ifdef TINY_DNG_LOADER_PROFILING
  auto start_t = std::chrono::system_clock::now();
#endif

  TINY_DNG_DPRINTF("tile_offset = %d\n", int(image_info.tile_offset));

  if ((image_info.tile_width > 0) && (image_info.tile_length > 0)) {
    // Assume ZIP is stored in tiled format.
    TINY_DNG_ASSERT(image_info.tile_width > 0, "Invalid tile width.");
    TINY_DNG_ASSERT(image_info.tile_length > 0, "Invalid tile length.");

    // <-       image width(skip len)           ->
    // +-----------------------------------------+
    // |                                         |
    // |              <- tile w  ->              |
    // |              +-----------+              |
    // |              |           | \            |
    // |              |           | |            |
    // |              |           | | tile h     |
    // |              |           | |            |
    // |              |           | /            |
    // |              +-----------+              |
    // |                                         |
    // |                                         |
    // +-----------------------------------------+

    TINY_DNG_DPRINTF("tile = %d, %d\n", image_info.tile_width,
                     image_info.tile_length);
    TINY_DNG_DPRINTF("w, h = %d, %d\n", image_info.width, image_info.height);

    // Currently we only support tile data for tile.length == tiff.height.
    // assert(image_info.tile_length == image_info.height);

    size_t column_step = 0;
    while (tiff_h < static_cast<unsigned int>(image_info.height)) {
      TINY_DNG_DPRINTF("sr tell = %d\n", int(sr.tell()));

      if ((image_info.width <= image_info.tile_width) &&
          (image_info.height <= image_info.tile_length)) {
        // Only 1 tile in the image and tile size is larger than image size.
        // Use image_info.tile_offset
        // FIXME(syoyo): Record tag len in somewhere and lookup it to count the
        // number of tiles in the image.
        offset = int(image_info.tile_offset);
      } else {
        // Read offset to data location.
        if (!sr.read4(&offset)) {
          if (err) {
            (*err) +=
                "Failed to read offset to image data location in "
                "DecompressZip.\n";
          }
          return false;
        }
        TINY_DNG_DPRINTF("offt = %d\n", offset);
      }

      size_t input_len = sr.size() - static_cast<size_t>(offset);
      unsigned long uncompressed_size =
          static_cast<unsigned long>(
              image_info.samples_per_pixel * image_info.tile_width *
              image_info.tile_length * image_info.bits_per_sample) /
          static_cast<unsigned long>(8);

      std::vector<uint8_t> tmp_buf;
      tmp_buf.resize(uncompressed_size);

      if (!DecompressZIP(tmp_buf.data(), &uncompressed_size, sr.data() + offset,
                         input_len, err)) {
        if (err) {
          (*err) += "Failed to decode ZIP data.\n";
        }
        return false;
      }

      if (!UnpredictImageU8(tmp_buf, image_info.predictor,
                            size_t(image_info.tile_width),
                            size_t(image_info.tile_length),
                            size_t(image_info.samples_per_pixel))) {
        if (err) {
          (*err) += "Failed to unpredict ZIP-ed tile image.\n";
        }
        return false;
      }

      // Copy to dest buffer.
      // NOTE: For some DNG file, tiled image may exceed the extent of target
      // image resolution.
      const size_t spp = size_t(image_info.samples_per_pixel);

      for (unsigned int y = 0;
           y < static_cast<unsigned int>(image_info.tile_length); y++) {
        unsigned int y_offset = y + tiff_h;
        if (y_offset >= static_cast<unsigned int>(image_info.height)) {
          continue;
        }

        size_t dst_offset =
            tiff_w + static_cast<unsigned int>(dst_width) * y_offset;

        size_t x_len = static_cast<size_t>(image_info.tile_width);
        if ((tiff_w + static_cast<unsigned int>(image_info.tile_width)) >=
            static_cast<unsigned int>(dst_width)) {
          x_len = static_cast<size_t>(dst_width) - tiff_w;
        }

        for (size_t x = 0; x < x_len; x++) {
          for (size_t c = 0; c < spp; c++) {
            dst_data[spp * (dst_offset + x) + c] =
                tmp_buf[spp * (y * static_cast<size_t>(image_info.tile_width) +
                               x) +
                        c];
          }
        }
      }

      tiff_w += static_cast<unsigned int>(image_info.tile_width);
      column_step++;
      // TINY_DNG_DPRINTF("col = %d, tiff_w = %d / %d\n", column_step, tiff_w,
      // image_info.width);
      if (tiff_w >= static_cast<unsigned int>(image_info.width)) {
        // tiff_h += static_cast<unsigned int>(image_info.tile_length);
        tiff_h += static_cast<unsigned int>(image_info.tile_length);
        // TINY_DNG_DPRINTF("tiff_h = %d\n", tiff_h);
        tiff_w = 0;
        column_step = 0;
      }
    }
  } else {
    // Assume ZIP data is not stored in tiled format.

    TINY_DNG_DPRINTF("width = %d", int(image_info.width));
    TINY_DNG_DPRINTF("height = %d", int(image_info.height));

    TINY_DNG_ASSERT(image_info.offset > 0, "Invalid ZIPed data offset.");
    offset = static_cast<int>(image_info.offset);

    size_t input_len = sr.size() - static_cast<size_t>(offset);
    unsigned long uncompressed_size =
        static_cast<unsigned long>(image_info.samples_per_pixel *
                                   image_info.width * image_info.height *
                                   image_info.bits_per_sample) /
        static_cast<unsigned long>(8);

    std::vector<uint8_t> tmp_buf;
    tmp_buf.resize(uncompressed_size);

    if (!DecompressZIP(tmp_buf.data(), &uncompressed_size, sr.data() + offset,
                       input_len, err)) {
      if (err) {
        (*err) += "Failed to decode non-tiled ZIP data.\n";
      }
      return false;
    }

    if (!UnpredictImageU8(tmp_buf, image_info.predictor,
                          size_t(image_info.tile_width),
                          size_t(image_info.tile_length),
                          size_t(image_info.samples_per_pixel))) {
      if (err) {
        (*err) += "Failed to unpredict ZIP-ed tile image.\n";
      }
      return false;
    }

    memcpy(dst_data, tmp_buf.data(), tmp_buf.size());
  }

#ifdef TINY_DNG_LOADER_PROFILING
  auto end_t = std::chrono::system_clock::now();
  auto ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t);

  std::cout << "DecompressZIP : " << ms.count() << " [ms]" << std::endl;
#endif

  return true;
}
#endif

// Decompress LosslesJPEG adta.
static bool DecompressLosslessJPEG(const StreamReader& sr,
                                   unsigned short* dst_data, int dst_width,
                                   const DNGImage& image_info,
                                   std::string* err) {
  unsigned int tiff_h = 0, tiff_w = 0;
  int offset = 0;

#ifdef TINY_DNG_LOADER_PROFILING
  auto start_t = std::chrono::system_clock::now();
#endif

  if ((image_info.tile_width > 0) && (image_info.tile_length > 0)) {
    // Assume Lossless JPEG data is stored in tiled format.
    TINY_DNG_ASSERT(image_info.tile_width > 0, "Invalid tile width.");
    TINY_DNG_ASSERT(image_info.tile_length > 0, "Invalid tile length.");

    // <-       image width(skip len)           ->
    // +-----------------------------------------+
    // |                                         |
    // |              <- tile w  ->              |
    // |              +-----------+              |
    // |              |           | \            |
    // |              |           | |            |
    // |              |           | | tile h     |
    // |              |           | |            |
    // |              |           | /            |
    // |              +-----------+              |
    // |                                         |
    // |                                         |
    // +-----------------------------------------+

    // TINY_DNG_DPRINTF("tile = %d, %d\n", image_info.tile_width,
    // image_info.tile_length);

    // Currently we only support tile data for tile.length == tiff.height.
    // assert(image_info.tile_length == image_info.height);

    size_t column_step = 0;
    while (tiff_h < static_cast<unsigned int>(image_info.height)) {
      // Read offset to JPEG data location.
      if (!sr.read4(&offset)) {
        if (err) {
          (*err) +=
              "Failed to read offset to JPEG data location in "
              "DecompressLosslessJPEG.\n";
        }
        return false;
      }
      TINY_DNG_DPRINTF("tile offt = %d\n", offset);

      int lj_width = 0;
      int lj_height = 0;
      int lj_bits = 0;
      lj92 ljp;

      size_t input_len = sr.size() - static_cast<size_t>(offset);

      // @fixme { Parse LJPEG header first and set exact compressed LJPEG data
      // length to `data_len` arg. }
      int ret =
          lj92_open(&ljp, reinterpret_cast<const uint8_t*>(sr.data() + offset),
                    /* data_len */ static_cast<int>(input_len), &lj_width,
                    &lj_height, &lj_bits);
      TINY_DNG_DPRINTF("ret = %d\n", ret);
      TINY_DNG_ASSERT(ret == LJ92_ERROR_NONE, "Error opening JPEG stream.");

      // TINY_DNG_DPRINTF("lj %d, %d, %d\n", lj_width, lj_height, lj_bits);
      // TINY_DNG_DPRINTF("ljp x %d, y %d, c %d\n", ljp->x, ljp->y,
      // ljp->components);
      // TINY_DNG_DPRINTF("tile width = %d\n", image_info.tile_width);
      // TINY_DNG_DPRINTF("tile height = %d\n", image_info.tile_length);
      // TINY_DNG_DPRINTF("col = %d, tiff_w = %d / %d\n", column_step, tiff_w,
      // image_info.width);

      TINY_DNG_ASSERT((lj_width * ljp->components * lj_height) ==
                          image_info.tile_width * image_info.tile_length,
                      "Unexpected JPEG tile size.");

      // int write_length = image_info.tile_width;
      // int skip_length = dst_width - image_info.tile_width;
      // TINY_DNG_DPRINTF("write_len = %d, skip_len = %d\n", write_length,
      // skip_length);

      // size_t dst_offset =
      //    column_step * static_cast<size_t>(image_info.tile_width) +
      //    static_cast<unsigned int>(dst_width) * tiff_h;

      // Decode into temporary buffer.

      std::vector<uint16_t> tmpbuf;
      tmpbuf.resize(
          static_cast<size_t>(lj_width * lj_height * ljp->components));

      ret = lj92_decode(ljp, tmpbuf.data(), image_info.tile_width, 0, NULL, 0);
      TINY_DNG_ASSERT(ret == LJ92_ERROR_NONE, "Error decoding JPEG stream.");
      // ret = lj92_decode(ljp, dst_data + dst_offset, write_length,
      // skip_length,
      //                  NULL, 0);

      // Copy to dest buffer.
      // NOTE: For some DNG file, tiled image may exceed the extent of target
      // image resolution.
      for (unsigned int y = 0;
           y < static_cast<unsigned int>(image_info.tile_length); y++) {
        unsigned int y_offset = y + tiff_h;
        if (y_offset >= static_cast<unsigned int>(image_info.height)) {
          continue;
        }

        size_t dst_offset =
            tiff_w + static_cast<unsigned int>(dst_width) * y_offset;

        size_t x_len = static_cast<size_t>(image_info.tile_width);
        if ((tiff_w + static_cast<unsigned int>(image_info.tile_width)) >=
            static_cast<unsigned int>(dst_width)) {
          x_len = static_cast<size_t>(dst_width) - tiff_w;
        }
        for (size_t x = 0; x < x_len; x++) {
          dst_data[dst_offset + x] =
              tmpbuf[y * static_cast<size_t>(image_info.tile_width) + x];
        }
      }

      TINY_DNG_ASSERT(ret == LJ92_ERROR_NONE,
                      "Error opening JPEG stream.");  // @fixme: redundant?

      lj92_close(ljp);

      tiff_w += static_cast<unsigned int>(image_info.tile_width);
      column_step++;
      // TINY_DNG_DPRINTF("col = %d, tiff_w = %d / %d\n", column_step, tiff_w,
      // image_info.width);
      if (tiff_w >= static_cast<unsigned int>(image_info.width)) {
        // tiff_h += static_cast<unsigned int>(image_info.tile_length);
        tiff_h += static_cast<unsigned int>(image_info.tile_length);
        // TINY_DNG_DPRINTF("tiff_h = %d\n", tiff_h);
        tiff_w = 0;
        column_step = 0;
      }
    }
  } else {
    // Assume LJPEG data is not stored in tiled format.

    // Read offset to JPEG data location.
    // offset = static_cast<int>(Read4(fp, swap_endian));

    TINY_DNG_ASSERT(image_info.offset > 0, "Invalid JPEG data offset.");
    offset = static_cast<int>(image_info.offset);

    TINY_DNG_DPRINTF("LJPEG offset %d\n", offset);

    int lj_width = 0;
    int lj_height = 0;
    int lj_bits = 0;
    lj92 ljp;

    size_t input_len = sr.size() - static_cast<size_t>(offset);

    // @fixme { Parse LJPEG header first and set exact compressed LJPEG data
    // length to `data_len` arg. }
    int ret =
        lj92_open(&ljp, reinterpret_cast<const uint8_t*>(sr.data() + offset),
                  /* data_len */ static_cast<int>(input_len), &lj_width,
                  &lj_height, &lj_bits);
    // TINY_DNG_DPRINTF("ret = %d\n", ret);
    TINY_DNG_ASSERT(ret == LJ92_ERROR_NONE, "Error opening JPEG stream.");

    // TINY_DNG_DPRINTF("lj %d, %d, %d\n", lj_width, lj_height, lj_bits);

    int write_length = image_info.width;
    int skip_length = 0;

    ret = lj92_decode(ljp, dst_data, write_length, skip_length, NULL, 0);
    // TINY_DNG_DPRINTF("ret = %d\n", ret);

    TINY_DNG_ASSERT(ret == LJ92_ERROR_NONE, "Error decoding JPEG stream.");

    lj92_close(ljp);
  }

#ifdef TINY_DNG_LOADER_PROFILING
  auto end_t = std::chrono::system_clock::now();
  auto ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t);

  std::cout << "DecompressLosslessJPEG : " << ms.count() << " [ms]"
            << std::endl;
#endif

  return true;
}

// Parse custom TIFF field
// returns -1 when error.
// returns 0 when not found.
// returns 1 when success.
static int ParseCustomField(const StreamReader& sr,
                            const std::vector<FieldInfo>& field_lists,
                            const unsigned short tag, const unsigned short type,
                            const unsigned int len, FieldData* data,
                            std::string* err) {
  bool found = false;

  (void)len;

  // Simple linear search.
  // TODO(syoyo): Use binary search for faster procesing.
  for (size_t i = 0; i < field_lists.size(); i++) {
    if ((field_lists[i].tag > TAG_NEW_SUBFILE_TYPE) &&
        (field_lists[i].tag == tag) && (field_lists[i].type == type)) {
      if ((type == TYPE_BYTE) || (type == TYPE_SBYTE)) {
        data->name = field_lists[i].name;
        data->type = static_cast<DataType>(type);
        unsigned char val;
        if (!sr.read1(&val)) {
          if (err) {
            (*err) += "Failed tp parse custom field.\n";
          }
          return -1;
        }

        data->data.resize(1);
        data->data[0] = val;
        found = true;
      } else if ((type == TYPE_SHORT) || (type == TYPE_SSHORT)) {
        data->name = field_lists[i].name;
        data->type = static_cast<DataType>(type);
        unsigned short val;
        if (!sr.read2(&val)) {
          if (err) {
            (*err) += "Failed tp parse custom field.\n";
          }
          return -1;
        }
        data->data.resize(sizeof(short));
        memcpy(data->data.data(), &val, sizeof(short));
        found = true;
      } else if ((type == TYPE_LONG) || (type == TYPE_SLONG) ||
                 (type == TYPE_FLOAT)) {
        data->name = field_lists[i].name;
        data->type = static_cast<DataType>(type);
        unsigned int val;
        if (!sr.read4(&val)) {
          if (err) {
            (*err) += "Failed tp parse custom field.\n";
          }
          return -1;
        }
        data->data.resize(sizeof(int));
        memcpy(data->data.data(), &val, sizeof(int));
        found = true;
      } else if ((type == TYPE_RATIONAL) || (type == TYPE_SRATIONAL)) {
        data->name = field_lists[i].name;
        data->type = static_cast<DataType>(type);
        unsigned int num, denom;

        if (!sr.read4(&num)) {
          if (err) {
            (*err) += "Failed tp parse custom field.\n";
          }
          return -1;
        }

        if (!sr.read4(&denom)) {
          if (err) {
            (*err) += "Failed tp parse custom field.\n";
          }
          return -1;
        }

        data->data.resize(sizeof(int) * 2);

        // Store rational value as is.
        memcpy(&data->data[0], &num, 4);
        memcpy(&data->data[4], &denom, 4);
      } else {
        // TODO(syoyo): Support more data types.
      }
    }
  }

  return found ? 1 : 0;
}

// Parse TIFF IFD.
// Returns true upon success, false if failed to parse.
static bool ParseTIFFIFD(const StreamReader& sr,
                         const std::vector<FieldInfo>& custom_field_lists,
                         std::vector<tinydng::DNGImage>* images,
                         std::string* warn, std::string* err) {
  (void)warn;

  tinydng::DNGImage image;
  InitializeDNGImage(&image);

  // TINY_DNG_DPRINTF("id = %d\n", idx);
  unsigned short num_entries = 0;
  if (!sr.read2(&num_entries)) {
    if (err) {
      (*err) += "Faild to read the number of entries in TIFF IFD.\n";
    }
    return false;
  }

  if (num_entries == 0) {
    if (err) {
      (*err) += "TIFF IFD cannot have 0 entries.\n";
    }
    return false;
  }

  TINY_DNG_DPRINTF("----------\n");
  TINY_DNG_DPRINTF("num entries %d\n", num_entries);

  // For delayed reading of strip offsets and strip byte counts.
  long offt_strip_offset = 0;
  long offt_strip_byte_counts = 0;

  while (num_entries--) {
    unsigned short tag, type;
    unsigned int len;
    unsigned int saved_offt;
    if (!GetTIFFTag(sr, &tag, &type, &len, &saved_offt)) {
      if (err) {
        (*err) += "Failed to read TIFF Tag.\n";
      }
      return false;
    }

    TINY_DNG_DPRINTF("tag %d\n", tag);
    TINY_DNG_DPRINTF("saved_offt %d\n", saved_offt);
    if (tag < TAG_NEW_SUBFILE_TYPE) {
      if (err) {
        (*err) += "Invalid tag ID.\n";
      }
      return false;
    }

    // TINY_DNG_DPRINTF("tag = %d\n", tag);

    switch (tag) {
      case 2:
      case TAG_IMAGE_WIDTH:
      case 61441:  // ImageWidth
        if (!sr.read_uint(type,
                          reinterpret_cast<unsigned int*>(&(image.width)))) {
          if (err) {
            (*err) += "Failed to read ImageWidth Tag.\n";
          }
          return false;
        }
        // TINY_DNG_DPRINTF("[%d] width = %d\n", idx, info.width);
        break;

      case 3:
      case TAG_IMAGE_HEIGHT:
      case 61442:  // ImageHeight
        if (!sr.read_uint(type,
                          reinterpret_cast<unsigned int*>(&(image.height)))) {
          if (err) {
            (*err) += "Failed to read ImageHeight Tag.\n";
          }
          return false;
        }
        // TINY_DNG_DPRINTF("height = %d\n", image.height);
        break;

      case TAG_BITS_PER_SAMPLE:
      case 61443:  // BitsPerSample
        // image->samples = len & 7;
        if (!sr.read_uint(type, reinterpret_cast<unsigned int*>(
                                    &(image.bits_per_sample_original)))) {
          if (err) {
            (*err) += "Failed to read BitsPerSample Tag.\n";
          }
          return false;
        }
        break;

      case TAG_SAMPLES_PER_PIXEL: {
        short spp = 0;
        if (!sr.read2(&spp)) {
          if (err) {
            (*err) += "Failed to read SamplesPerPixel Tag.\n";
          }
          return false;
        }

        if (spp > 4) {
          if (err) {
            (*err) += "SamplesPerPixel must be less than or equal to 4.\n";
          }
          return false;
        }

        image.samples_per_pixel = static_cast<int>(spp);

        // TINY_DNG_DPRINTF("spp = %d\n", image.samples_per_pixel);
      } break;

      case TAG_ROWS_PER_STRIP: {
        // The TIFF Spec says data type may be SHORT, but assume LONG for a
        // while.
        if (!sr.read4(&image.rows_per_strip)) {
          if (err) {
            (*err) += "Failed to parse RowsPerStrip Tag.\n";
          }
          return false;
        }
        // NOTE: Compute strip_per_image in later since image.height is required
        // to compute strips_per_image.
      } break;

      case TAG_COMPRESSION:
        if (!sr.read_uint(
                type, reinterpret_cast<unsigned int*>(&image.compression))) {
          if (err) {
            (*err) += "Failed to parse Compression Tag.\n";
          }
          return false;
        }

        // TINY_DNG_DPRINTF("tag-compression = %d\n", image.compression);
        break;

      case TAG_STRIP_OFFSET:
      case TAG_JPEG_IF_OFFSET:
        offt_strip_offset = static_cast<long>(sr.tell());
        if (!sr.read4(&image.offset)) {
          if (err) {
            (*err) += "Failed to parse Compression Tag.\n";
          }
          return false;
        }
        // TINY_DNG_DPRINTF("strip_offset = %d\n", image.offset);
        break;

      case TAG_JPEG_IF_BYTE_COUNT:
        if (!sr.read4(&image.jpeg_byte_count)) {
          if (err) {
            (*err) = "Failed to parse JpegIfByteCount Tag.\n";
          }
          return false;
        }
        break;

      case TAG_ORIENTATION:
        if (!sr.read2(&image.orientation)) {
          if (err) {
            (*err) = "Failed to parse Orientation Tag.\n";
          }
          return false;
        }
        break;

      case TAG_STRIP_BYTE_COUNTS:
        offt_strip_byte_counts = static_cast<long>(sr.tell());
        if (!sr.read4(&image.strip_byte_count)) {
          if (err) {
            (*err) = "Failed to parse StripByteCount Tag.\n";
          }
          return false;
        }
        TINY_DNG_DPRINTF("strip_byte_count = %d\n", image.strip_byte_count);
        break;

      case TAG_PLANAR_CONFIGURATION:
        if (!sr.read2(&image.planar_configuration)) {
          if (err) {
            (*err) = "Failed to parse PlanarConfiguration Tag.\n";
          }
          return false;
        }
        break;

      case TAG_PREDICTOR:
        if (!sr.read2(&image.predictor)) {
          if (err) {
            (*err) = "Failed to parse Predictor Tag.\n";
          }
          return false;
        }

        if ((image.predictor < 1) || (image.predictor > 3)) {
          if (err) {
            (*err) = "Predictor value must be 1, 2 or 3.\n";
          }
          return false;
        }
        break;

      case TAG_SAMPLE_FORMAT: {
        short format;
        if (!sr.read2(&format)) {
          if (err) {
            (*err) = "Failed to parse SampleFormat.\n";
          }
          return false;
        }

        if ((format == SAMPLEFORMAT_INT) || (format == SAMPLEFORMAT_UINT) ||
            (format == SAMPLEFORMAT_IEEEFP)) {
          image.sample_format = static_cast<SampleFormat>(format);
        }
      } break;

      case TAG_SUB_IFDS:

      {
        // TINY_DNG_DPRINTF("sub_ifds = %d\n", len);
        for (size_t k = 0; k < len; k++) {
          unsigned int i = static_cast<unsigned int>(sr.tell());
          unsigned int offt;
          if (!sr.read4(&offt)) {
            if (err) {
              (*err) += "Failed to parse SubIFDs Tag.\n";
            }
            return false;
          }

          unsigned int base = 0;  // @fixme
          if (!sr.seek_set(offt + base)) {
            if (err) {
              (*err) += "Failed to seek to SubIFD Tag.\n";
            }
            return false;
          }

          // recursive call
          if (!ParseTIFFIFD(sr, custom_field_lists, images, warn, err)) {
            if (err) {
              (*err) += "Failed to Parse SubIFD Tag.\n";
            }
            return false;
          }

          if (!sr.seek_set(i + 4)) {
            if (err) {
              (*err) += "Failed to rewind to SubIFD Tag position.\n";
            }
            return false;
          }
        }
        // TINY_DNG_DPRINTF("sub_ifds DONE\n");
      }

      break;

      case TAG_TILE_WIDTH:
        if (!sr.read_uint(type,
                          reinterpret_cast<unsigned int*>(&image.tile_width))) {
          if (err) {
            (*err) += "Failed to parse TileWidth Tag.\n";
          }
        }
        // TINY_DNG_DPRINTF("tile_width = %d\n", image.tile_width);
        break;

      case TAG_TILE_LENGTH:
        // TINY_DNG_DPRINTF("tile_length = %d\n", image.tile_length);
        if (!sr.read_uint(
                type, reinterpret_cast<unsigned int*>(&image.tile_length))) {
          if (err) {
            (*err) += "Failed to parse TileLength Tag.\n";
          }
        }
        break;

      case TAG_TILE_OFFSETS:
        if (len > 1) {
          image.tile_offset = static_cast<unsigned int>(sr.tell());
        } else {
          if (!sr.read4(&image.tile_offset)) {
            if (err) {
              (*err) += "Failed to parse TileOffsets Tag.\n";
            }
          }
        }
        TINY_DNG_DPRINTF("tile_offt = %d\n", int(image.tile_offset));
        break;

      case TAG_TILE_BYTE_COUNTS:
        if (len > 1) {
          image.tile_byte_count = static_cast<unsigned int>(sr.tell());
        } else {
          if (!sr.read4(&image.tile_byte_count)) {
            if (err) {
              (*err) += "Failed to parse TileByteCounts Tag.\n";
            }
          }
        }
        break;

      case TAG_CFA_PATTERN_DIM:
        if (!sr.read2(&image.cfa_pattern_dim)) {
          if (err) {
            (*err) = "Failed to parse CFA PatternDim Tag.\n";
          }
          return false;
        }
        break;

      case TAG_CFA_PATTERN: {
        char buf[16];
        size_t readLen = len;
        if (readLen > 16) readLen = 16;
        // Assume 2x2 CFAPattern.
        TINY_DNG_ASSERT(readLen == 4, "Unsupported CFA pattern.");

        if (!sr.read(readLen, 16, reinterpret_cast<unsigned char*>(buf))) {
          if (err) {
            (*err) = "Failed to parse CFA Pattern Tag.\n";
          }
          return false;
        }

        image.cfa_pattern[0][0] = buf[0];
        image.cfa_pattern[0][1] = buf[1];
        image.cfa_pattern[1][0] = buf[2];
        image.cfa_pattern[1][1] = buf[3];
      } break;

      case TAG_DNG_VERSION: {
        char data[4];
        {
          for (int i = 0; i < 4; i++) {
            if (!sr.read1(&data[i])) {
              if (err) {
                (*err) += "Failed to parse DNGVersion Tag.\n";
              }
              return false;
            }
          }
        }

        image.version =
            (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
      } break;

      case TAG_CFA_PLANE_COLOR: {
        char buf[4];
        size_t readLen = len;
        if (readLen > 4) readLen = 4;

        if (!sr.read(readLen, readLen, reinterpret_cast<unsigned char*>(buf))) {
          if (err) {
            (*err) += "Failed to parse CFALayout Tag.\n";
          }
          return false;
        }

        for (size_t i = 0; i < readLen; i++) {
          image.cfa_plane_color[i] = buf[i];
        }
      } break;

      case TAG_CFA_LAYOUT: {
        if (!sr.read4(&image.cfa_layout)) {
          if (err) {
            (*err) += "Failed to parse CFALayout Tag.\n";
          }
          return false;
        }
      } break;

      case TAG_ACTIVE_AREA:

        for (size_t c = 0; c < 4; c++) {
          if (!sr.read_uint(type, reinterpret_cast<unsigned int*>(
                                      &image.active_area[c]))) {
            if (err) {
              (*err) += "Failed to parse ActiveArea Tag.\n";
            }
            return false;
          }
        }

        image.has_active_area = true;

        break;

      case TAG_BLACK_LEVEL: {
        // Assume TAG_SAMPLES_PER_PIXEL is read before
        // FIXME(syoyo): scan TAG_SAMPLES_PER_PIXEL in IFD table in advance.
        for (int s = 0; s < image.samples_per_pixel; s++) {
          if (!sr.read_uint(type, reinterpret_cast<unsigned int*>(
                                      &image.black_level[s]))) {
            if (err) {
              (*err) += "Failed to parse BlackLevel Tag.\n";
            }
            return false;
          }
        }
      } break;

      case TAG_WHITE_LEVEL: {
        // Assume TAG_SAMPLES_PER_PIXEL is read before
        // FIXME(syoyo): scan TAG_SAMPLES_PER_PIXEL in IFD table in advance.
        for (int s = 0; s < image.samples_per_pixel; s++) {
          if (!sr.read_uint(type, reinterpret_cast<unsigned int*>(
                                      &image.white_level[s]))) {
            if (err) {
              (*err) += "Failed to parse WhiteLevel Tag.\n";
            }
            return false;
          }
        }
      } break;

      case TAG_ANALOG_BALANCE:
        // Assume RGB
        if (!sr.read_real(type, &image.analog_balance[0])) {
          if (err) {
            (*err) += "Failed to parse analog balance.\n";
          }
          return false;
        }
        if (!sr.read_real(type, &image.analog_balance[1])) {
          if (err) {
            (*err) += "Failed to parse analog balance.\n";
          }
          return false;
        }
        if (!sr.read_real(type, &image.analog_balance[2])) {
          if (err) {
            (*err) += "Failed to parse analog balance.\n";
          }
          return false;
        }

        image.has_analog_balance = true;
        break;

      case TAG_AS_SHOT_NEUTRAL:
        // Assume RGB
        // TINY_DNG_DPRINTF("ty = %d\n", type);
        for (size_t c = 0; c < 3; c++) {
          if (!sr.read_real(type, &image.as_shot_neutral[c])) {
            if (err) {
              (*err) += "Failed to parse AsShotNeutral Tag.\n";
            }
            return false;
          }
        }

        image.has_as_shot_neutral = true;
        break;

      case TAG_CALIBRATION_ILLUMINANT1: {
        unsigned short val;
        if (!sr.read2(&val)) {
          if (err) {
            (*err) += "Failed to parse CalibrartionIlluminant1 Tag.\n";
          }
          return false;
        }

        image.calibration_illuminant1 = static_cast<LightSource>(val);
      } break;

      case TAG_CALIBRATION_ILLUMINANT2: {
        unsigned short val;
        if (!sr.read2(&val)) {
          if (err) {
            (*err) += "Failed to parse CalibrartionIlluminant2 Tag.\n";
          }
          return false;
        }
        image.calibration_illuminant2 = static_cast<LightSource>(val);
      } break;

      case TAG_COLOR_MATRIX1: {
        for (int c = 0; c < 3; c++) {
          for (int k = 0; k < 3; k++) {
            double val;
            if (!sr.read_real(type, &val)) {
              if (err) {
                (*err) += "Failed to parse ColorMatrix1 Tag.\n";
              }
              return false;
            }

            image.color_matrix1[c][k] = val;
          }
        }
      } break;

      case TAG_COLOR_MATRIX2: {
        for (int c = 0; c < 3; c++) {
          for (int k = 0; k < 3; k++) {
            double val;
            if (!sr.read_real(type, &val)) {
              if (err) {
                (*err) += "Failed to parse ColorMatrix2 Tag.\n";
              }
              return false;
            }
            image.color_matrix2[c][k] = val;
          }
        }
      } break;

      case TAG_FORWARD_MATRIX1: {
        for (int c = 0; c < 3; c++) {
          for (int k = 0; k < 3; k++) {
            double val;
            if (!sr.read_real(type, &val)) {
              if (err) {
                (*err) += "Failed to parse ForwardMatrix1 Tag.\n";
              }
              return false;
            }
            image.forward_matrix1[c][k] = val;
          }
        }
      } break;

      case TAG_FORWARD_MATRIX2: {
        for (int c = 0; c < 3; c++) {
          for (int k = 0; k < 3; k++) {
            double val;
            if (!sr.read_real(type, &val)) {
              if (err) {
                (*err) += "Failed to parse ForwardMatrix2 Tag.\n";
              }
              return false;
            }
            image.forward_matrix2[c][k] = val;
          }
        }
      } break;

      case TAG_CAMERA_CALIBRATION1: {
        for (int c = 0; c < 3; c++) {
          for (int k = 0; k < 3; k++) {
            double val;
            if (!sr.read_real(type, &val)) {
              if (err) {
                (*err) += "Failed to parse CameraCalibration1 Tag.\n";
              }
              return false;
            }
            image.camera_calibration1[c][k] = val;
          }
        }
      } break;

      case TAG_CAMERA_CALIBRATION2: {
        for (int c = 0; c < 3; c++) {
          for (int k = 0; k < 3; k++) {
            double val;
            if (!sr.read_real(type, &val)) {
              if (err) {
                (*err) += "Failed to parse CameraCalibration2 Tag.\n";
              }
              return false;
            }
            image.camera_calibration2[c][k] = val;
          }
        }
      } break;

      case TAG_CR2_SLICES: {
        // Assume 3 ushorts;
        for (size_t c = 0; c < 3; c++) {
          if (!sr.read2(&image.cr2_slices[c])) {
            if (err) {
              (*err) += "Failed to parse CR2Slices Tag.\n";
            }
            return false;
          }
        }
        // TINY_DNG_DPRINTF("cr2_slices = %d, %d, %d\n",
        //  image.cr2_slices[0],
        //  image.cr2_slices[1],
        //  image.cr2_slices[2]);
      } break;

      default: {
        FieldData data;
        int ret = ParseCustomField(sr, custom_field_lists, tag, type, len,
                                   &data, err);
        if (ret == -1) {
          return false;
        }

        if (ret > 0) {
          image.custom_fields.push_back(data);
        }
        // TINY_DNG_DPRINTF("unknown or unsupported tag = %d\n", tag);
      }
    }

    sr.seek_set(saved_offt);
  }

  if (image.rows_per_strip > 0) {
    if (image.height == -1) {
      if (err) {
        (*err) = "image height tag is required to compute StripsPerImage.\n";
      }
      return false;
    }

    // http://www.awaresystems.be/imaging/tiff/tifftags/rowsperstrip.html
    image.strips_per_image =
        static_cast<int>(floor(double(image.height + image.rows_per_strip - 1) /
                               double(image.rows_per_strip)));
    TINY_DNG_DPRINTF("rows_per_strip = %d\n", image.samples_per_pixel);
    TINY_DNG_DPRINTF("strips_per_image = %d\n", image.strips_per_image);
  }

  // Delayed read of strip offsets and strip byte counts
  if (image.strips_per_image > 0) {
    image.strip_byte_counts.clear();
    image.strip_offsets.clear();

    long curr_offt = static_cast<long>(sr.tell());

    if (offt_strip_byte_counts > 0) {
      if (!sr.seek_set(uint64_t(offt_strip_byte_counts))) {
        if (err) {
          (*err) = "Failed to seek.\n";
        }
        return false;
      }

      for (int k = 0; k < image.strips_per_image; k++) {
        unsigned int strip_byte_count;
        if (!sr.read4(&strip_byte_count)) {
          if (err) {
            (*err) += "Failed to read StripByteCount value.";
          }
          return false;
        }
        TINY_DNG_DPRINTF("strip_byte_counts[%d] = %u\n", k, strip_byte_count);
        image.strip_byte_counts.push_back(strip_byte_count);
      }
    }

    if (offt_strip_offset > 0) {
      if (!sr.seek_set(uint64_t(offt_strip_offset))) {
        if (err) {
          (*err) = "Failed to seek.\n";
        }
        return false;
      }

      for (int k = 0; k < image.strips_per_image; k++) {
        unsigned int strip_offset;
        if (!sr.read4(&strip_offset)) {
          if (err) {
            (*err) += "Failed to read StripOffset value.";
          }
          return false;
        }

        TINY_DNG_DPRINTF("strip_offset[%d] = %u\n", k, strip_offset);
        image.strip_offsets.push_back(strip_offset);
      }
    }

    if (!sr.seek_set(uint64_t(curr_offt))) {
      if (err) {
        (*err) = "Failed to seek.\n";
      }
      return false;
    }
  }
  //

  // Add to images.
  images->push_back(image);

  // TINY_DNG_DPRINTF("DONE ---------\n");

  return true;
}

static bool ParseDNGFromMemory(const StreamReader& sr,
                               const std::vector<FieldInfo>& custom_fields,
                               std::vector<tinydng::DNGImage>* images,
                               std::string* warn, std::string* err) {
  if (!images) {
    if (err) {
      (*err) += "Invalid `images` argument.\n";
    }
    return false;
  }

  unsigned int offt;
  if (!sr.read4(&offt)) {
    if (err) {
      (*err) += "Failed to read offset.\n";
    }
    return false;
  }

  TINY_DNG_DPRINTF("First IFD offt: %d\n", offt);

  while (offt) {
    if (!sr.seek_set(offt)) {
      if (err) {
        (*err) += "Failed to seek to TIFF IFD.\n";
      }
      return false;
    }

    // TINY_DNG_DPRINTF("Parse TIFF IFD\n");
    if (!ParseTIFFIFD(sr, custom_fields, images, warn, err)) {
      break;
    }
    // Get next IFD offset(0 = end of file).
    if (!sr.read4(&offt)) {
      if (err) {
        (*err) += "Failed to read next IDF offset.\n";
      }
      return false;
    }

    TINY_DNG_DPRINTF("Next IFD offset = %d\n", offt);
  }

  for (size_t i = 0; i < images->size(); i++) {
    tinydng::DNGImage* image = &((*images)[i]);
    TINY_DNG_ASSERT(image->samples_per_pixel <= 4,
                    "Cannot handle > 4 samples per pixel.");
    for (int s = 0; s < image->samples_per_pixel; s++) {
      if (image->white_level[s] == -1) {
        // Set white level with (2 ** BitsPerSample) according to the DNG spec.
        TINY_DNG_ASSERT(image->bits_per_sample_original > 0,
                        "White level has to be > 0.");

        if (image->bits_per_sample_original >=
            32) {  // workaround for 32bit floating point TIFF.
          image->white_level[s] = -1;
        } else {
          TINY_DNG_ASSERT(image->bits_per_sample_original < 32,
                          "Cannot handle >= 32 bits per sample.");
          image->white_level[s] = (1 << image->bits_per_sample_original);
        }
      }
    }
  }

  return true;
}

static bool IsBigEndian() {
  uint32_t i = 0x01020304;
  char c[4];
  memcpy(c, &i, 4);
  return (c[0] == 1);
}

// C++03 and tiff lzw port of https://github.com/glampert/compression-algorithms
// =============================================================================
//
// File: lzw.hpp
// Author: Guilherme R. Lampert
// Created on: 17/02/16
// Brief: LZW encoder/decoder in C++11 with varying length dictionary codes.
//
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif
namespace lzw {

class Dictionary {
 public:
  struct Entry {
    int code;
    int value;
  };

  // Dictionary entries 0-255 are always reserved to the byte/ASCII range.
  int size_;
  Entry entries_[4096];

  Dictionary();
  int findIndex(int code, int value) const;
  bool add(int code, int value);
  bool flush(int& codeBitsWidth);
  void init();
  int size() { return size_; }
};

// ========================================================
// class Dictionary:
// ========================================================

Dictionary::Dictionary() { init(); }

void Dictionary::init() {
  // First 256 dictionary entries are reserved to the byte/ASCII
  // range. Additional entries follow for the character sequences
  // found in the input. Up to 4096 - 256 (MaxDictEntries - FirstCode).
  size_ = 256;
  for (int i = 0; i < size_; ++i) {
    entries_[i].code = -1;
    entries_[i].value = i;
  }

  // 256 is reserved for ClearCode, 257 is reserved for end of stream, thus
  // FistCode starts with 258
  size_ = 258;  // = 256 + 2
}

int Dictionary::findIndex(const int code, const int value) const {
  if (code == -1) {
    return value;
  }

  // Linear search for now.
  // TODO: Worth optimizing with a proper hash-table?
  for (int i = 0; i < size_; ++i) {
    if (entries_[i].code == code && entries_[i].value == value) {
      return i;
    }
  }

  return -1;
}

bool Dictionary::add(const int code, const int value) {
  TINY_DNG_ASSERT(code <= size_,
                  "`code' must be less than or equal to dictionary size.");
  if (size_ == 4096) {
    TINY_DNG_DPRINTF("Dictionary overflowed!");
    return false;
  }

  TINY_DNG_DPRINTF("add[%d].code = %d\n", size_, code);
  TINY_DNG_DPRINTF("add[%d].value = %d\n", size_, value);
  entries_[size_].code = code;
  entries_[size_].value = value;
  ++size_;
  return true;
}

bool Dictionary::flush(int& codeBitsWidth) {
  if (size_ == ((1 << codeBitsWidth) - 1)) {
    ++codeBitsWidth;
    TINY_DNG_DPRINTF("expand: bits %d\n", codeBitsWidth);
    if (codeBitsWidth > 12)  // MaxDictBits
    {
      // Clear the dictionary (except the first 256 byte entries).
      codeBitsWidth = 9;  // StartBits
      size_ = 256 + 2;    // 256 is reserved for ClearCode, 257 is reserved for
                          // end of stream, thus FistCode starts with 258
      return true;
    }
  }
  return false;
}

class BitStreamReader {
 public:
  // No copy/assignment.
  // BitStreamReader(const BitStreamReader &) = delete;
  // BitStreamReader & operator = (const BitStreamReader &) = delete;

  // BitStreamReader(const BitStreamWriter & bitStreamWriter);
  BitStreamReader(const uint8_t* bitStream, int byteCount, int bitCount);

  bool isEndOfStream() const;
  bool readNextBitLE(int& bitOut);       // little endian
  bool readNextBitBE(int& bitOut);       // big endian
  uint64_t readBitsU64LE(int bitCount);  // little endian
  uint64_t readBitsU64BE(int bitCount);  // big endian
  void reset();

 private:
  const uint8_t*
      stream;  // Pointer to the external bit stream. Not owned by the reader.
  const int
      sizeInBytes;  // Size of the stream *in bytes*. Might include padding.
  const int sizeInBits;  // Size of the stream *in bits*, padding *not* include.
  int currBytePos;       // Current byte being read in the stream.
  int nextBitPos;   // Bit position within the current byte to access next. 0 to
                    // 7.
  int numBitsRead;  // Total bits read from the stream so far. Never includes
                    // byte-rounding padding.
};

// BitStreamReader::BitStreamReader(const BitStreamWriter & bitStreamWriter)
//    : stream(bitStreamWriter.getBitStream())
//    , sizeInBytes(bitStreamWriter.getByteCount())
//    , sizeInBits(bitStreamWriter.getBitCount())
//{
//    reset();
//}

BitStreamReader::BitStreamReader(const unsigned char* bitStream,
                                 const int byteCount, const int bitCount)
    : stream(bitStream), sizeInBytes(byteCount), sizeInBits(bitCount) {
  (void)sizeInBytes;
  reset();
}

bool BitStreamReader::readNextBitLE(int& bitOut) {
  if (numBitsRead >= sizeInBits) {
    return false;  // We are done.
  }

  const uint32_t mask = uint32_t(1) << nextBitPos;
  bitOut = !!(stream[currBytePos] & mask);
  ++numBitsRead;

  if (++nextBitPos == 8) {
    nextBitPos = 0;
    ++currBytePos;
  }
  return true;
}

bool BitStreamReader::readNextBitBE(int& bitOut) {
  if (numBitsRead >= sizeInBits) {
    return false;  // We are done.
  }

  const uint32_t mask = uint32_t(1) << (7 - nextBitPos);
  bitOut = !!(stream[currBytePos] & mask);
  ++numBitsRead;

  if (++nextBitPos == 8) {
    nextBitPos = 0;
    ++currBytePos;
  }
  return true;
}

uint64_t BitStreamReader::readBitsU64LE(const int bitCount) {
  TINY_DNG_ASSERT(bitCount <= 64,
                  "`bitCount' must be less than or equal to 64.");

  uint64_t num = 0;
  for (int b = 0; b < bitCount; ++b) {
    int bit;
    if (!readNextBitLE(bit)) {
      TINY_DNG_DPRINTF(
          "LE: Failed to read bits from stream! Unexpected end.\n");
      break;
    }

    // Based on a "Stanford bit-hack":
    // http://graphics.stanford.edu/~seander/bithacks.html#ConditionalSetOrClearBitsWithoutBranching
    const uint64_t mask = uint64_t(1) << b;
    num = (num & ~mask) | (uint64_t(-bit) & mask);
  }

  return num;
}

uint64_t BitStreamReader::readBitsU64BE(const int bitCount) {
  TINY_DNG_ASSERT(bitCount <= 64,
                  "`bitCount' must be less than or equal to 64.");

  uint64_t num = 0;
  for (int b = 0; b < bitCount; ++b) {
    int bit;
    if (!readNextBitBE(bit)) {
      TINY_DNG_DPRINTF("BE: Failed to read bits from stream! Unexpected end.");
      break;
    }

    TINY_DNG_DPRINTF("bit[%d](count %d) = %d\n", b, bitCount, bit);

    // Based on a "Stanford bit-hack":
    // http://graphics.stanford.edu/~seander/bithacks.html#ConditionalSetOrClearBitsWithoutBranching
    const uint64_t mask = uint64_t(1) << (bitCount - b - 1);
    num = (num & ~mask) | (uint64_t(-bit) & mask);
  }

  TINY_DNG_DPRINTF("num = %d\n", int(num));
  return num;
}

void BitStreamReader::reset() {
  currBytePos = 0;
  nextBitPos = 0;
  numBitsRead = 0;
}

bool BitStreamReader::isEndOfStream() const {
  return numBitsRead >= sizeInBits;
}

// ========================================================
// easyDecode() and helpers:
// ========================================================

static bool outputByte(int code, unsigned char*& output, int outputSizeBytes,
                       int& bytesDecodedSoFar) {
  if (bytesDecodedSoFar >= outputSizeBytes) {
    // LZW_ERROR("Decoder output buffer too small!");
    return false;
  }

  TINY_DNG_ASSERT(code >= 0 && code < 256, "`code' must be within [0, 255].");
  *output++ = static_cast<unsigned char>(code);
  ++bytesDecodedSoFar;
  return true;
}

static bool outputSequence(const Dictionary& dict, int code,
                           unsigned char*& output, int outputSizeBytes,
                           int& bytesDecodedSoFar, int& firstByte) {
  const int MaxDictEntries = 4096;
  (void)MaxDictEntries;

  // A sequence is stored backwards, so we have to write
  // it to a temp then output the buffer in reverse.
  int i = 0;
  unsigned char sequence[4096];
  do {
    TINY_DNG_ASSERT(i < MaxDictEntries - 1 && code >= 0,
                    "Invalid value for `i' or `code'.");
    TINY_DNG_DPRINTF("i = %d, ent[%d].value = %d\n", i, code,
                     dict.entries_[code].value);
    sequence[i++] = static_cast<unsigned char>(dict.entries_[code].value);
    code = dict.entries_[code].code;
  } while (code >= 0);

  firstByte = sequence[--i];
  for (; i >= 0; --i) {
    if (!outputByte(sequence[i], output, outputSizeBytes, bytesDecodedSoFar)) {
      return false;
    }
  }
  return true;
}

static int easyDecode(const unsigned char* compressed,
                      const int compressedSizeBytes,
                      const int compressedSizeBits, unsigned char* uncompressed,
                      const int uncompressedSizeBytes, const bool swap_endian) {
  const int Nil = -1;
  const int MaxDictBits = 12;
  const int StartBits = 9;

  // TIFF specific values
  const int ClearCode = 256;
  const int EndOfInformation = 257;

  if (compressed == NULL || uncompressed == NULL) {
    TINY_DNG_DPRINTF("lzw::easyDecode(): Null data pointer(s)!\n");
    return 0;
  }

  if (compressedSizeBytes <= 0 || compressedSizeBits <= 0 ||
      uncompressedSizeBytes <= 0) {
    TINY_DNG_DPRINTF("lzw::easyDecode(): Bad in/out sizes!n");
    return 0;
  }

  int code = Nil;
  int prevCode = Nil;
  int firstByte = 0;
  int bytesDecoded = 0;
  int codeBitsWidth = StartBits;

  // We'll reconstruct the dictionary based on the
  // bit stream codes. Unlike Huffman encoding, we
  // don't store the dictionary as a prefix to the data.
  Dictionary dictionary;
  BitStreamReader bitStream(compressed, compressedSizeBytes,
                            compressedSizeBits);

  // We check to avoid an overflow of the user buffer.
  // If the buffer is smaller than the decompressed size,
  // TINY_DNG_DPRINTF() is called. If that doesn't throw or
  // terminate we break the loop and return the current
  // decompression count.
  while (!bitStream.isEndOfStream()) {
    TINY_DNG_ASSERT(
        codeBitsWidth <= MaxDictBits,
        "`codeBitsWidth must be less than or equal to `MaxDictBits'.");
    (void)MaxDictBits;

    if (!swap_endian) {  // TODO(syoyo): Detect BE or LE depending on endianness
                         // in stored format and host endian
      code = static_cast<int>(bitStream.readBitsU64BE(codeBitsWidth));
    } else {
      code = static_cast<int>(bitStream.readBitsU64LE(codeBitsWidth));
    }

    TINY_DNG_DPRINTF("code = %d(swap_endian = %d)\n", code, swap_endian);

    // if (code >= dictionary.size()) {
    //  std::cerr << "code = " << code << "dict.size = " << dictionary.size() <<
    //  std::endl;
    //}

    // TINY_DNG_ASSERT(code <= dictionary.size(),
    //                "`code' must be less than or equal to dictionary size.");

    if (code == EndOfInformation) {
      TINY_DNG_DPRINTF("EoI\n");
      break;
    }

    if (code == ClearCode) {
      // Initialize dict.
      dictionary.init();
      codeBitsWidth = StartBits;

      if (!swap_endian) {
        code = static_cast<int>(bitStream.readBitsU64BE(codeBitsWidth));
      } else {
        code = static_cast<int>(bitStream.readBitsU64LE(codeBitsWidth));
      }

      if (code == EndOfInformation) {
        TINY_DNG_DPRINTF("EoI\n");
        break;
      }

      if (!outputByte(code, uncompressed, uncompressedSizeBytes,
                      bytesDecoded)) {
        break;
      }

      prevCode = code;
      continue;
    }

    if (prevCode == Nil) {
      if (!outputByte(code, uncompressed, uncompressedSizeBytes,
                      bytesDecoded)) {
        break;
      }
      firstByte = code;
      prevCode = code;
      continue;
    }

    if (code >= dictionary.size()) {
      if (!outputSequence(dictionary, prevCode, uncompressed,
                          uncompressedSizeBytes, bytesDecoded, firstByte)) {
        break;
      }
      if (!outputByte(firstByte, uncompressed, uncompressedSizeBytes,
                      bytesDecoded)) {
        break;
      }
    } else {
      if (!outputSequence(dictionary, code, uncompressed, uncompressedSizeBytes,
                          bytesDecoded, firstByte)) {
        break;
      }
    }

    dictionary.add(prevCode, firstByte);
    if (dictionary.flush(codeBitsWidth)) {
      TINY_DNG_DPRINTF("flush\n");
      prevCode = Nil;
    } else {
      prevCode = code;
    }
  }

  return bytesDecoded;
}

// =============================================================================
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace lzw

bool LoadDNG(const char* filename, std::vector<FieldInfo>& custom_fields,
             std::vector<DNGImage>* images, std::string* warn,
             std::string* err) {
  (void)warn;
  std::stringstream ss;

  TINY_DNG_ASSERT(images, "Invalid images pointer.");

#ifdef _MSC_VER
  FILE* fp;
  fopen_s(&fp, filename, "rb");
#else
  FILE* fp = fopen(filename, "rb");
#endif
  if (!fp) {
    ss << "File not found or cannot open file " << filename << std::endl;
    if (err) {
      (*err) = ss.str();
    }
    return false;
  }

  if (0 != fseek(fp, 0, SEEK_END)) {
    if (err) {
      (*err) = "Error seeking.\n";
    }
    return false;
  }

  size_t file_size = static_cast<size_t>(ftell(fp));

  std::vector<unsigned char> whole_data;
  {
    whole_data.resize(file_size);
    fseek(fp, 0, SEEK_SET);
    size_t read_len = fread(whole_data.data(), 1, file_size, fp);
    TINY_DNG_ASSERT(read_len == file_size, "Unexpected file size.");

    fseek(fp, 0, SEEK_SET);
  }
  fclose(fp);

  return LoadDNGFromMemory(reinterpret_cast<const char*>(whole_data.data()),
                           static_cast<unsigned int>(whole_data.size()),
                           custom_fields, images, warn, err);
}

bool LoadDNGFromMemory(const char* mem, unsigned int size,
                       std::vector<FieldInfo>& custom_fields,
                       std::vector<DNGImage>* images, std::string* warn,
                       std::string* err) {
  (void)warn;

  if ((mem == NULL) || (size < 32) || (!images)) {
    if (err) {
      (*err) = "Invalid argument. argument is null or invalid.\n";
    }
    return false;
  }

  bool is_dng_big_endian = false;

  const unsigned short magic = *(reinterpret_cast<const unsigned short*>(mem));

  if (magic == 0x4949) {
    // might be TIFF(DNG).
  } else if (magic == 0x4d4d) {
    // might be TIFF(DNG, bigendian).
    is_dng_big_endian = true;
    TINY_DNG_DPRINTF("DNG is big endian\n");
  } else {
    std::stringstream ss;
    ss << "Seems the data is not a DNG format." << std::endl;
    if (err) {
      (*err) = ss.str();
    }

    return false;
  }

  const bool swap_endian = (is_dng_big_endian && (!IsBigEndian()));
  StreamReader sr(reinterpret_cast<const uint8_t*>(mem), size, swap_endian);

  char header[32];

  if (32 != sr.read(32, 32, reinterpret_cast<unsigned char*>(header))) {
    if (err) {
      (*err) = "Error reading header.\n";
    }
    return false;
  }

  // skip magic header
  if (!sr.seek_set(4)) {
    if (err) {
      (*err) += "Failed to seek to offset 4.\n";
    }
    return false;
  }

  bool ret = ParseDNGFromMemory(sr, custom_fields, images, warn, err);

  if (!ret) {
    if (err) {
      (*err) += "Failed to parse DNG data.\n";
    }
    return false;
  }

  for (size_t i = 0; i < images->size(); i++) {
    tinydng::DNGImage* image = &((*images)[i]);

    const size_t data_offset =
        (image->offset > 0) ? image->offset : image->tile_offset;
    TINY_DNG_DPRINTF("data_offset = %d\n", int(data_offset));
    if ((data_offset == 0) || (data_offset > sr.size())) {
      if (err) {
        std::stringstream ss;
        ss << i << "'th image data offset is zero or invalid.\n";
        (*err) += ss.str();
      }
      return false;
    }

    // std::cout << "offt =\n" << image->offset << std::endl;
    // std::cout << "tile_offt = \n" << image->tile_offset << std::endl;
    // std::cout << "data_offset = " << data_offset << std::endl;

    TINY_DNG_DPRINTF("compression = %d\n", image->compression);

    if (image->compression == COMPRESSION_NONE) {  // no compression

      if (image->jpeg_byte_count > 0) {
        // Looks like CR2 IFD#1(thumbnail jpeg image)
        // Currently skip parsing jpeg data.
        // TODO(syoyo): Decode jpeg data.
        image->width = 0;
        image->height = 0;
      } else {
        image->bits_per_sample = image->bits_per_sample_original;
        // std::cout << "sample_per_pixel " << image->samples_per_pixel << "\n";
        // std::cout << "width " << image->width << "\n";
        // std::cout << "height " << image->height << "\n";
        // std::cout << "bps " << image->bits_per_sample << "\n";
        TINY_DNG_ASSERT(
            ((image->width * image->height * image->bits_per_sample) % 8) == 0,
            "Image size must be multiple of 8.");
        const size_t len = size_t(image->samples_per_pixel) *
                           size_t(image->width) * size_t(image->height) *
                           size_t(image->bits_per_sample) / size_t(8);
        TINY_DNG_ASSERT(len > 0, "Unexpected length.");
        image->data.resize(len);
        if (!sr.seek_set(data_offset)) {
          if (err) {
            (*err) += "Failed to seek to uncompressed image data position.\n";
          }
          return false;
        }

        if (!sr.read(len, len, image->data.data())) {
          if (err) {
            (*err) += "Failed to read image data.\n";
          }
          return false;
        }
      }
    } else if (image->compression == COMPRESSION_LZW) {  // lzw compression
      image->bits_per_sample = image->bits_per_sample_original;
      TINY_DNG_DPRINTF("bps = %d\n", image->bits_per_sample);
      TINY_DNG_DPRINTF("counts = %d\n", int(image->strip_byte_counts.size()));
      TINY_DNG_DPRINTF("offsets = %d\n", int(image->strip_offsets.size()));

      image->data.clear();

      if ((image->strip_byte_counts.size() > 0) &&
          (image->strip_byte_counts.size() == image->strip_offsets.size())) {
#if defined(TINY_DNG_LOADER_USE_THREAD)

        const int num_strips = int(image->strip_byte_counts.size());

        std::vector<std::thread> workers;
        std::atomic<size_t> strip_count(0);

        int num_threads = std::max(1, int(std::thread::hardware_concurrency()));
        if (num_threads > num_strips) {
          num_threads = num_strips;
        }

        bool failed = false;

        const size_t dst_strip_len = static_cast<size_t>(
            (image->samples_per_pixel * image->width * image->rows_per_strip *
             image->bits_per_sample) /
            8);

        image->data.resize(dst_strip_len * size_t(num_strips));

        for (int t = 0; t < num_threads; t++) {
          workers.emplace_back(std::thread([&]() {
            size_t k = 0;
            while ((k = strip_count++) < size_t(num_strips)) {
              std::vector<unsigned char> src(image->strip_byte_counts[k]);
              size_t strip_offset = image->strip_offsets[k];
              size_t strip_bytesize = image->strip_byte_counts[k];

              std::vector<unsigned char> dst(dst_strip_len);

              const uint8_t* src_addr =
                  sr.map_abs_addr(strip_offset, strip_bytesize);

              if (!src_addr) {
                // TODO(syoyo): Atomic update
                if (err) {
                  (*err) +=
                      "Cannot read strip_byte_counts bytes from a memory.\n";
                }
                failed = true;
                break;
              }

              TINY_DNG_DPRINTF("easyDecode begin\n");
              int decoded_bytes = lzw::easyDecode(
                  src_addr, int(strip_bytesize),
                  int(strip_bytesize) *
                      image
                          ->bits_per_sample /* FIXME(syoyo): Is this correct? */
                  ,
                  dst.data(), int(dst_strip_len), swap_endian);
              TINY_DNG_DPRINTF("easyDecode done\n");
              TINY_DNG_ASSERT(decoded_bytes > 0,
                              "decoded_ bytes must be non-zero positive.");

              if (image->predictor == 1) {
                // no prediction shceme
              } else if (image->predictor == 2) {
                // horizontal diff

                const size_t stride =
                    size_t(image->width * image->samples_per_pixel);
                const size_t spp = size_t(image->samples_per_pixel);
                for (size_t row = 0; row < size_t(image->rows_per_strip);
                     row++) {
                  for (size_t c = 0; c < size_t(image->samples_per_pixel);
                       c++) {
                    unsigned int b = dst[row * stride + c];
                    for (size_t col = 1; col < size_t(image->width); col++) {
                      // value may overflow(wrap over), but its expected
                      // behavior.
                      b += dst[stride * row + spp * col + c];
                      dst[stride * row + spp * col + c] =
                          static_cast<unsigned char>(b & 0xFF);
                    }
                  }
                }

              } else if (image->predictor == 3) {
                // fp horizontal diff.
                TINY_DNG_ABORT("[TODO] FP horizontal differencing predictor.");
              } else {
                TINY_DNG_ABORT("Invalid predictor value.");
              }

              memcpy(&image->data[k * dst_strip_len], dst.data(),
                     dst_strip_len);
            }
          }));
        }

        for (auto& t : workers) {
          t.join();
        }
#else
        for (size_t k = 0; k < image->strip_byte_counts.size(); k++) {
          std::vector<unsigned char> src(image->strip_byte_counts[k]);
          if (!sr.seek_set(image->strip_offsets[k])) {
            if (err) {
              (*err) += "Failed to seek to strip offset.\n";
            }
            return false;
          }

          const size_t dst_len = static_cast<size_t>(
              (image->samples_per_pixel * image->width * image->rows_per_strip *
               image->bits_per_sample) /
              8);
          std::vector<unsigned char> dst(dst_len);

          if (!sr.read(image->strip_byte_counts[k], image->strip_byte_counts[k],
                       src.data())) {
            if (err) {
              (*err) += "Cannot read strip_byte_counts bytes from stream.\n";
            }
            return false;
          }
          TINY_DNG_DPRINTF("easyDecode begin\n");
          int decoded_bytes = lzw::easyDecode(
              src.data(), int(image->strip_byte_counts[k]),
              int(image->strip_byte_counts[k]) *
                  image->bits_per_sample /* FIXME(syoyo): Is this correct? */,
              dst.data(), int(dst_len), swap_endian);
          TINY_DNG_DPRINTF("easyDecode done\n");
          TINY_DNG_ASSERT(decoded_bytes > 0,
                          "decoded_ bytes must be non-zero positive.");

          if (image->predictor == 1) {
            // no prediction shceme
          } else if (image->predictor == 2) {
            // horizontal diff

            const size_t stride =
                size_t(image->width * image->samples_per_pixel);
            const size_t spp = size_t(image->samples_per_pixel);
            for (size_t row = 0; row < size_t(image->rows_per_strip); row++) {
              for (size_t c = 0; c < size_t(image->samples_per_pixel); c++) {
                unsigned int b = dst[row * stride + c];
                for (size_t col = 1; col < size_t(image->width); col++) {
                  // value may overflow(wrap over), but its expected behavior.
                  b += dst[stride * row + spp * col + c];
                  dst[stride * row + spp * col + c] =
                      static_cast<unsigned char>(b & 0xFF);
                }
              }
            }

          } else if (image->predictor == 3) {
            // fp horizontal diff.
            TINY_DNG_ABORT("[TODO] FP horizontal differencing predictor.");
          } else {
            TINY_DNG_ABORT("Invalid predictor value.");
          }

          std::copy(dst.begin(), dst.end(), std::back_inserter(image->data));
        }

#endif
      } else {
        TINY_DNG_ABORT("Unsupported image strip configuration.");
      }
    } else if (image->compression ==
               COMPRESSION_OLD_JPEG) {  // old jpeg compression

      // std::cout << "IFD " << i << std::endl;

      // First check if JPEG is lossless JPEG
      // TODO(syoyo): Compure conservative data_len.
      if (sr.size() < data_offset) {
        if (err) {
          (*err) += "Unexpected data offset.\n";
        }
        return false;
      }
      size_t data_len = sr.size() - data_offset;
      int lj_width = -1, lj_height = -1, lj_bits = -1, lj_components = -1;
      if (IsLosslessJPEG(sr.data() + data_offset, static_cast<int>(data_len),
                         &lj_width, &lj_height, &lj_bits, &lj_components)) {
        // std::cout << "IFD " << i << " is LJPEG" << std::endl;
        TINY_DNG_DPRINTF("IFD[%d] is LJPEG\n", int(i));

        TINY_DNG_ASSERT(
            lj_width > 0 && lj_height > 0 && lj_bits > 0 && lj_components > 0,
            "Image dimensions must be > 0.");

        // Assume not in tiled format.
        TINY_DNG_ASSERT(image->tile_width == -1 && image->tile_length == -1,
                        "Tiled format not supported tile size.");

        image->height = lj_height;

        // Is Canon CR2?
        const bool is_cr2 = (image->cr2_slices[0] != 0) ? true : false;

        if (is_cr2) {
          // For CR2 RAW, slices[0] * slices[1] + slices[2] = image width
          image->width = image->cr2_slices[0] * image->cr2_slices[1] +
                         image->cr2_slices[2];
        } else {
          image->width = lj_width;
        }

        image->bits_per_sample_original = lj_bits;

        // lj92 decodes data into 16bits, so modify bps.
        image->bits_per_sample = 16;

        TINY_DNG_ASSERT(
            ((image->width * image->height * image->bits_per_sample) % 8) == 0,
            "Image size must be multiple of 8.");
        const size_t len =
            static_cast<size_t>((image->samples_per_pixel * image->width *
                                 image->height * image->bits_per_sample) /
                                8);
        // std::cout << "spp = " << image->samples_per_pixel;
        // std::cout << ", w = " << image->width << ", h = " << image->height <<
        // ", bps = " << image->bits_per_sample << std::endl;
        TINY_DNG_ASSERT(len > 0, "Invalid length.");
        image->data.resize(len);

        if (sr.size() < data_offset) {
          if (err) {
            (*err) += "Unexpected file size or data offset.\n";
          }
          return false;
        }

        std::vector<unsigned short> buf;
        buf.resize(static_cast<size_t>(image->width * image->height *
                                       image->samples_per_pixel));

        bool ok =
            DecompressLosslessJPEG(sr, &buf.at(0), image->width, (*image), err);
        if (!ok) {
          if (err) {
            std::stringstream ss;
            ss << "Failed to decompress LJPEG." << std::endl;
            (*err) = ss.str();
          }
          return false;
        }

        if (is_cr2) {
          // CR2 stores image in tiled format(image slices. left to right).
          // Convert it to scanline format.
          int nslices = image->cr2_slices[0];
          int slice_width = image->cr2_slices[1];
          int slice_remainder_width = image->cr2_slices[2];
          size_t src_offset = 0;

          unsigned short* dst_ptr =
              reinterpret_cast<unsigned short*>(image->data.data());

          for (int slice = 0; slice < nslices; slice++) {
            int x_offset = slice * slice_width;
            for (int y = 0; y < image->height; y++) {
              size_t dst_offset =
                  static_cast<size_t>(y * image->width + x_offset);
              memcpy(&dst_ptr[dst_offset], &buf[src_offset],
                     sizeof(unsigned short) * static_cast<size_t>(slice_width));
              src_offset += static_cast<size_t>(slice_width);
            }
          }

          // remainder(the last slice).
          {
            int x_offset = nslices * slice_width;
            for (int y = 0; y < image->height; y++) {
              size_t dst_offset =
                  static_cast<size_t>(y * image->width + x_offset);
              // std::cout << "y = " << y << ", dst = " << dst_offset << ", src
              // = " << src_offset << ", len = " << buf.size() << std::endl;
              memcpy(&dst_ptr[dst_offset], &buf[src_offset],
                     sizeof(unsigned short) *
                         static_cast<size_t>(slice_remainder_width));
              src_offset += static_cast<size_t>(slice_remainder_width);
            }
          }

        } else {
          memcpy(image->data.data(), static_cast<void*>(&(buf.at(0))), len);
        }
      } else {
#if 0
        // Baseline 8bit JPEG

        image->bits_per_sample = 8;

        size_t jpeg_len = static_cast<size_t>(image->jpeg_byte_count);
        if (image->jpeg_byte_count == -1) {
          // No jpeg datalen. Set to the size of file - offset.
          if (sr.size() < data_offset) {
            if (err) {
              (*err) += "Unexpected file size or data offset.\n";
            }
            return false;
          }
          jpeg_len = sr.size() - data_offset;
        }
        TINY_DNG_ASSERT(jpeg_len > 0, "Invalid length.");

        // Assume RGB jpeg
        int w = 0, h = 0, components = 0;
        unsigned char* decoded_image = stbi_load_from_memory(
            sr.data() + data_offset, static_cast<int>(jpeg_len), &w, &h,
            &components, /* desired_channels */ 3);
        TINY_DNG_ASSERT(decoded_image, "Could not decode JPEG image.");

        // Currently we just discard JPEG image(since JPEG image would be just a
        // thumbnail or LDR image of RAW).
        // TODO(syoyo): Do not discard JPEG image.
        free(decoded_image);

        // std::cout << "w = " << w << std::endl;
        // std::cout << "h = " << w << std::endl;
        // std::cout << "c = " << components << std::endl;

        TINY_DNG_ASSERT(w > 0 && h > 0, "Image dimensions must be > 0.");

        image->width = w;
        image->height = h;
#else
        TINY_DNG_ASSERT(0, "Cannot decode baseline 8bit JPEG image.");
#endif
      }

    } else if (image->compression ==
               COMPRESSION_NEW_JPEG) {  //  new JPEG(baseline DCT JPEG or
                                        //  lossless JPEG)

      // lj92 decodes data into 16bits, so modify bps.
      image->bits_per_sample = 16;

      // std::cout << "w = " << image->width << ", h = " << image->height <<
      // std::endl;

      TINY_DNG_ASSERT(
          ((image->width * image->height * image->bits_per_sample) % 8) == 0,
          "Image must be multiple of 8.");
      const size_t len =
          static_cast<size_t>((image->samples_per_pixel * image->width *
                               image->height * image->bits_per_sample) /
                              8);
      TINY_DNG_ASSERT(len > 0, "Invalid length.");
      image->data.resize(len);

      if (sr.size() < data_offset) {
        if (err) {
          (*err) += "Unexpected file size or data offset.\n";
        }
        return false;
      }

      if (!sr.seek_set(data_offset)) {
        if (err) {
          (*err) += "Failed to seek to data offset(NewJpeg).\n";
        }
        return false;
      }

      bool ok = DecompressLosslessJPEG(
          sr, reinterpret_cast<unsigned short*>(&(image->data.at(0))),
          image->width, (*image), err);
      if (!ok) {
        if (err) {
          std::stringstream ss;
          ss << "Failed to decompress LJPEG." << std::endl;
          (*err) = ss.str();
        }
        return false;
      }

    } else if (image->compression == 8) {  // ZIP
#ifdef TINY_DNG_LOADER_ENABLE_ZIP
      image->bits_per_sample = image->bits_per_sample_original;
      TINY_DNG_DPRINTF("bps = %d\n", image->bits_per_sample);
      TINY_DNG_DPRINTF("data_offset = %d\n", int(data_offset));

      TINY_DNG_DPRINTF("width %d\n", image->width);
      TINY_DNG_DPRINTF("height %d\n", image->height);
      TINY_DNG_DPRINTF("samples_per_pixel %d\n", image->samples_per_pixel);
      TINY_DNG_DPRINTF("bits_per_sample %d\n", image->bits_per_sample);

      const size_t len =
          static_cast<size_t>((image->samples_per_pixel * image->width *
                               image->height * image->bits_per_sample) /
                              8);
      if (len == 0) {
        if (err) {
          (*err) += "Invalid length. in ZIP compressed data.\n";
        }
        return false;
      }

      image->data.resize(len);

      if (sr.size() < data_offset) {
        if (err) {
          (*err) +=
              "Unexpected file size or data offset in ZIP compressed data.\n";
        }
        return false;
      }

      if (!sr.seek_set(data_offset)) {
        if (err) {
          (*err) += "Failed to seek to data offset(ZIP).\n";
        }
        return false;
      }

      bool ok = DecompressZIPedTile(sr, &(image->data.at(0)), image->width,
                                    (*image), err);
      if (!ok) {
        if (err) {
          std::stringstream ss;
          ss << "Failed to decompress ZIP." << std::endl;
          (*err) += ss.str();
        }
        return false;
      }
#else
      if (err) {
        std::stringstream ss;
        ss << "ZIP compression is not supported." << std::endl;
        (*err) = ss.str();
      }
#endif
    } else if (image->compression == 34892) {  // lossy JPEG
      if (err) {
        std::stringstream ss;
        ss << "lossy JPEG compression is not supported." << std::endl;
        (*err) = ss.str();
      }
    } else if (image->compression == 34713) {  // NEF lossless?
      if (err) {
        std::stringstream ss;
        ss << "Seems a NEF RAW. This compression is not supported."
           << std::endl;
        (*err) = ss.str();
      }
    } else {
      if (err) {
        std::stringstream ss;
        ss << "IFD [" << i << "] "
           << " Unsupported compression type : " << image->compression
           << std::endl;
        (*err) = ss.str();
      }
      return false;
    }
  }

  return ret ? true : false;
}

bool IsDNGFromMemory(const char* mem, unsigned int size, std::string* msg) {
  if ((mem == NULL) || (size < 32)) {
    if (msg) {
      (*msg) = "Invalid argument. argument is null or invalid.\n";
    }
    return false;
  }

  const unsigned short magic = *(reinterpret_cast<const unsigned short*>(mem));

  if (magic == 0x4949) {
    // might be TIFF(DNG).
  } else if (magic == 0x4d4d) {
    // might be TIFF(DNG, bigendian).
    if (msg) {
      (*msg) = "DNG is big endian";
    }
  } else {
    return false;
  }

  return true;
}

bool IsDNG(const char* filename, std::string* msg) {
  std::stringstream ss;

#ifdef _MSC_VER
  FILE* fp;
  fopen_s(&fp, filename, "rb");
#else
  FILE* fp = fopen(filename, "rb");
#endif
  if (!fp) {
    ss << "File not found or cannot open file " << filename << std::endl;
    if (msg) {
      (*msg) = ss.str();
    }
    return false;
  }

  if (0 != fseek(fp, 0, SEEK_END)) {
    if (msg) {
      (*msg) = "Error seeking.\n";
    }
    return false;
  }

  size_t file_size = static_cast<size_t>(ftell(fp));

  std::vector<unsigned char> whole_data;
  {
    whole_data.resize(file_size);
    fseek(fp, 0, SEEK_SET);
    size_t read_len = fread(whole_data.data(), 1, file_size, fp);
    TINY_DNG_ASSERT(read_len == file_size, "Unexpected file size.");

    fseek(fp, 0, SEEK_SET);
  }
  fclose(fp);

  return IsDNGFromMemory(reinterpret_cast<const char*>(whole_data.data()),
                         static_cast<unsigned int>(whole_data.size()), msg);
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace tinydng
