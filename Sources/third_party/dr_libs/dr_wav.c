#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#include <stdlib.h>
#include <string.h> /* For memcpy(), memset() */
#include <limits.h> /* For INT_MAX */

#ifndef DR_WAV_NO_STDIO
#include <stdio.h>
#include <wchar.h>
#endif

/* Standard library stuff. */
#ifndef DRWAV_ASSERT
#include <assert.h>
#define DRWAV_ASSERT(expression)           assert(expression)
#endif
#ifndef DRWAV_MALLOC
#define DRWAV_MALLOC(sz)                   malloc((sz))
#endif
#ifndef DRWAV_REALLOC
#define DRWAV_REALLOC(p, sz)               realloc((p), (sz))
#endif
#ifndef DRWAV_FREE
#define DRWAV_FREE(p)                      free((p))
#endif
#ifndef DRWAV_COPY_MEMORY
#define DRWAV_COPY_MEMORY(dst, src, sz)    memcpy((dst), (src), (sz))
#endif
#ifndef DRWAV_ZERO_MEMORY
#define DRWAV_ZERO_MEMORY(p, sz)           memset((p), 0, (sz))
#endif

#define drwav_countof(x)                   (sizeof(x) / sizeof(x[0]))
#define drwav_align(x, a)                  ((((x) + (a) - 1) / (a)) * (a))
#define drwav_min(a, b)                    (((a) < (b)) ? (a) : (b))
#define drwav_max(a, b)                    (((a) > (b)) ? (a) : (b))
#define drwav_clamp(x, lo, hi)             (drwav_max((lo), drwav_min((hi), (x))))

#define DRWAV_MAX_SIMD_VECTOR_SIZE         64  /* 64 for AVX-512 in the future. */

/* CPU architecture. */
#if defined(__x86_64__) || defined(_M_X64)
    #define DRWAV_X64
#elif defined(__i386) || defined(_M_IX86)
    #define DRWAV_X86
#elif defined(__arm__) || defined(_M_ARM)
    #define DRWAV_ARM
#endif

#ifdef _MSC_VER
    #define DRWAV_INLINE __forceinline
#elif defined(__GNUC__)
    /*
    I've had a bug report where GCC is emitting warnings about functions possibly not being inlineable. This warning happens when
    the __attribute__((always_inline)) attribute is defined without an "inline" statement. I think therefore there must be some
    case where "__inline__" is not always defined, thus the compiler emitting these warnings. When using -std=c89 or -ansi on the
    command line, we cannot use the "inline" keyword and instead need to use "__inline__". In an attempt to work around this issue
    I am using "__inline__" only when we're compiling in strict ANSI mode.
    */
    #if defined(__STRICT_ANSI__)
        #define DRWAV_INLINE __inline__ __attribute__((always_inline))
    #else
        #define DRWAV_INLINE inline __attribute__((always_inline))
    #endif
#else
    #define DRWAV_INLINE
#endif

#if defined(SIZE_MAX)
    #define DRWAV_SIZE_MAX  SIZE_MAX
#else
    #if defined(_WIN64) || defined(_LP64) || defined(__LP64__)
        #define DRWAV_SIZE_MAX  ((drwav_uint64)0xFFFFFFFFFFFFFFFF)
    #else
        #define DRWAV_SIZE_MAX  0xFFFFFFFF
    #endif
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1300
    #define DRWAV_HAS_BYTESWAP16_INTRINSIC
    #define DRWAV_HAS_BYTESWAP32_INTRINSIC
    #define DRWAV_HAS_BYTESWAP64_INTRINSIC
#elif defined(__clang__)
    #if defined(__has_builtin)
        #if __has_builtin(__builtin_bswap16)
            #define DRWAV_HAS_BYTESWAP16_INTRINSIC
        #endif
        #if __has_builtin(__builtin_bswap32)
            #define DRWAV_HAS_BYTESWAP32_INTRINSIC
        #endif
        #if __has_builtin(__builtin_bswap64)
            #define DRWAV_HAS_BYTESWAP64_INTRINSIC
        #endif
    #endif
#elif defined(__GNUC__)
    #if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
        #define DRWAV_HAS_BYTESWAP32_INTRINSIC
        #define DRWAV_HAS_BYTESWAP64_INTRINSIC
    #endif
    #if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))
        #define DRWAV_HAS_BYTESWAP16_INTRINSIC
    #endif
#endif

/*
These limits are used for basic validation when initializing the decoder. If you exceed these limits, first of all: what on Earth are
you doing?! (Let me know, I'd be curious!) Second, you can adjust these by #define-ing them before the dr_wav implementation.
*/
#ifndef DRWAV_MAX_SAMPLE_RATE
#define DRWAV_MAX_SAMPLE_RATE       384000
#endif
#ifndef DRWAV_MAX_CHANNELS
#define DRWAV_MAX_CHANNELS          256
#endif
#ifndef DRWAV_MAX_BITS_PER_SAMPLE
#define DRWAV_MAX_BITS_PER_SAMPLE   64
#endif

static const drwav_uint8 drwavGUID_W64_RIFF[16] = {0x72,0x69,0x66,0x66, 0x2E,0x91, 0xCF,0x11, 0xA5,0xD6, 0x28,0xDB,0x04,0xC1,0x00,0x00};    /* 66666972-912E-11CF-A5D6-28DB04C10000 */
static const drwav_uint8 drwavGUID_W64_WAVE[16] = {0x77,0x61,0x76,0x65, 0xF3,0xAC, 0xD3,0x11, 0x8C,0xD1, 0x00,0xC0,0x4F,0x8E,0xDB,0x8A};    /* 65766177-ACF3-11D3-8CD1-00C04F8EDB8A */
static const drwav_uint8 drwavGUID_W64_JUNK[16] = {0x6A,0x75,0x6E,0x6B, 0xF3,0xAC, 0xD3,0x11, 0x8C,0xD1, 0x00,0xC0,0x4F,0x8E,0xDB,0x8A};    /* 6B6E756A-ACF3-11D3-8CD1-00C04F8EDB8A */
static const drwav_uint8 drwavGUID_W64_FMT [16] = {0x66,0x6D,0x74,0x20, 0xF3,0xAC, 0xD3,0x11, 0x8C,0xD1, 0x00,0xC0,0x4F,0x8E,0xDB,0x8A};    /* 20746D66-ACF3-11D3-8CD1-00C04F8EDB8A */
static const drwav_uint8 drwavGUID_W64_FACT[16] = {0x66,0x61,0x63,0x74, 0xF3,0xAC, 0xD3,0x11, 0x8C,0xD1, 0x00,0xC0,0x4F,0x8E,0xDB,0x8A};    /* 74636166-ACF3-11D3-8CD1-00C04F8EDB8A */
static const drwav_uint8 drwavGUID_W64_DATA[16] = {0x64,0x61,0x74,0x61, 0xF3,0xAC, 0xD3,0x11, 0x8C,0xD1, 0x00,0xC0,0x4F,0x8E,0xDB,0x8A};    /* 61746164-ACF3-11D3-8CD1-00C04F8EDB8A */
static const drwav_uint8 drwavGUID_W64_SMPL[16] = {0x73,0x6D,0x70,0x6C, 0xF3,0xAC, 0xD3,0x11, 0x8C,0xD1, 0x00,0xC0,0x4F,0x8E,0xDB,0x8A};    /* 6C706D73-ACF3-11D3-8CD1-00C04F8EDB8A */

static DRWAV_INLINE drwav_bool32 drwav__guid_equal(const drwav_uint8 a[16], const drwav_uint8 b[16])
{
    int i;
    for (i = 0; i < 16; i += 1) {
        if (a[i] != b[i]) {
            return DRWAV_FALSE;
        }
    }

    return DRWAV_TRUE;
}

static DRWAV_INLINE drwav_bool32 drwav__fourcc_equal(const unsigned char* a, const char* b)
{
    return
        a[0] == b[0] &&
        a[1] == b[1] &&
        a[2] == b[2] &&
        a[3] == b[3];
}



static DRWAV_INLINE int drwav__is_little_endian()
{
#if defined(DRWAV_X86) || defined(DRWAV_X64)
    return DRWAV_TRUE;
#elif defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN
    return DRWAV_TRUE;
#else
    int n = 1;
    return (*(char*)&n) == 1;
#endif
}

static DRWAV_INLINE unsigned short drwav__bytes_to_u16(const unsigned char* data)
{
    return (data[0] << 0) | (data[1] << 8);
}

static DRWAV_INLINE short drwav__bytes_to_s16(const unsigned char* data)
{
    return (short)drwav__bytes_to_u16(data);
}

static DRWAV_INLINE unsigned int drwav__bytes_to_u32(const unsigned char* data)
{
    return (data[0] << 0) | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
}

static DRWAV_INLINE drwav_uint64 drwav__bytes_to_u64(const unsigned char* data)
{
    return
        ((drwav_uint64)data[0] <<  0) | ((drwav_uint64)data[1] <<  8) | ((drwav_uint64)data[2] << 16) | ((drwav_uint64)data[3] << 24) |
        ((drwav_uint64)data[4] << 32) | ((drwav_uint64)data[5] << 40) | ((drwav_uint64)data[6] << 48) | ((drwav_uint64)data[7] << 56);
}

static DRWAV_INLINE void drwav__bytes_to_guid(const unsigned char* data, drwav_uint8* guid)
{
    int i;
    for (i = 0; i < 16; ++i) {
        guid[i] = data[i];
    }
}


static DRWAV_INLINE drwav_uint16 drwav__bswap16(drwav_uint16 n)
{
#ifdef DRWAV_HAS_BYTESWAP16_INTRINSIC
    #if defined(_MSC_VER)
        return _byteswap_ushort(n);
    #elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap16(n);
    #else
        #error "This compiler does not support the byte swap intrinsic."
    #endif
#else
    return ((n & 0xFF00) >> 8) |
           ((n & 0x00FF) << 8);
#endif
}

static DRWAV_INLINE drwav_uint32 drwav__bswap32(drwav_uint32 n)
{
#ifdef DRWAV_HAS_BYTESWAP32_INTRINSIC
    #if defined(_MSC_VER)
        return _byteswap_ulong(n);
    #elif defined(__GNUC__) || defined(__clang__)
        #if defined(DRWAV_ARM) && (defined(__ARM_ARCH) && __ARM_ARCH >= 6) && !defined(DRWAV_64BIT)   /* <-- 64-bit inline assembly has not been tested, so disabling for now. */
            /* Inline assembly optimized implementation for ARM. In my testing, GCC does not generate optimized code with __builtin_bswap32(). */
            drwav_uint32 r;
            __asm__ __volatile__ (
            #if defined(DRWAV_64BIT)
                "rev %w[out], %w[in]" : [out]"=r"(r) : [in]"r"(n)   /* <-- This is untested. If someone in the community could test this, that would be appreciated! */
            #else
                "rev %[out], %[in]" : [out]"=r"(r) : [in]"r"(n)
            #endif
            );
            return r;
        #else
            return __builtin_bswap32(n);
        #endif
    #else
        #error "This compiler does not support the byte swap intrinsic."
    #endif
#else
    return ((n & 0xFF000000) >> 24) |
           ((n & 0x00FF0000) >>  8) |
           ((n & 0x0000FF00) <<  8) |
           ((n & 0x000000FF) << 24);
#endif
}

static DRWAV_INLINE drwav_uint64 drwav__bswap64(drwav_uint64 n)
{
#ifdef DRWAV_HAS_BYTESWAP64_INTRINSIC
    #if defined(_MSC_VER)
        return _byteswap_uint64(n);
    #elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap64(n);
    #else
        #error "This compiler does not support the byte swap intrinsic."
    #endif
#else
    return ((n & (drwav_uint64)0xFF00000000000000) >> 56) |
           ((n & (drwav_uint64)0x00FF000000000000) >> 40) |
           ((n & (drwav_uint64)0x0000FF0000000000) >> 24) |
           ((n & (drwav_uint64)0x000000FF00000000) >>  8) |
           ((n & (drwav_uint64)0x00000000FF000000) <<  8) |
           ((n & (drwav_uint64)0x0000000000FF0000) << 24) |
           ((n & (drwav_uint64)0x000000000000FF00) << 40) |
           ((n & (drwav_uint64)0x00000000000000FF) << 56);
#endif
}


static DRWAV_INLINE drwav_int16 drwav__bswap_s16(drwav_int16 n)
{
    return (drwav_int16)drwav__bswap16((drwav_uint16)n);
}

static DRWAV_INLINE void drwav__bswap_samples_s16(drwav_int16* pSamples, drwav_uint64 sampleCount)
{
    drwav_uint64 iSample;
    for (iSample = 0; iSample < sampleCount; iSample += 1) {
        pSamples[iSample] = drwav__bswap_s16(pSamples[iSample]);
    }
}


static DRWAV_INLINE void drwav__bswap_s24(drwav_uint8* p)
{
    drwav_uint8 t;
    t = p[0];
    p[0] = p[2];
    p[2] = t;
}

static DRWAV_INLINE void drwav__bswap_samples_s24(drwav_uint8* pSamples, drwav_uint64 sampleCount)
{
    drwav_uint64 iSample;
    for (iSample = 0; iSample < sampleCount; iSample += 1) {
        drwav_uint8* pSample = pSamples + (iSample*3);
        drwav__bswap_s24(pSample);
    }
}


static DRWAV_INLINE drwav_int32 drwav__bswap_s32(drwav_int32 n)
{
    return (drwav_int32)drwav__bswap32((drwav_uint32)n);
}

static DRWAV_INLINE void drwav__bswap_samples_s32(drwav_int32* pSamples, drwav_uint64 sampleCount)
{
    drwav_uint64 iSample;
    for (iSample = 0; iSample < sampleCount; iSample += 1) {
        pSamples[iSample] = drwav__bswap_s32(pSamples[iSample]);
    }
}


static DRWAV_INLINE float drwav__bswap_f32(float n)
{
    union {
        drwav_uint32 i;
        float f;
    } x;
    x.f = n;
    x.i = drwav__bswap32(x.i);

    return x.f;
}

static DRWAV_INLINE void drwav__bswap_samples_f32(float* pSamples, drwav_uint64 sampleCount)
{
    drwav_uint64 iSample;
    for (iSample = 0; iSample < sampleCount; iSample += 1) {
        pSamples[iSample] = drwav__bswap_f32(pSamples[iSample]);
    }
}


static DRWAV_INLINE double drwav__bswap_f64(double n)
{
    union {
        drwav_uint64 i;
        double f;
    } x;
    x.f = n;
    x.i = drwav__bswap64(x.i);

    return x.f;
}

static DRWAV_INLINE void drwav__bswap_samples_f64(double* pSamples, drwav_uint64 sampleCount)
{
    drwav_uint64 iSample;
    for (iSample = 0; iSample < sampleCount; iSample += 1) {
        pSamples[iSample] = drwav__bswap_f64(pSamples[iSample]);
    }
}


static DRWAV_INLINE void drwav__bswap_samples_pcm(void* pSamples, drwav_uint64 sampleCount, drwav_uint32 bytesPerSample)
{
    /* Assumes integer PCM. Floating point PCM is done in drwav__bswap_samples_ieee(). */
    switch (bytesPerSample)
    {
        case 2: /* s16, s12 (loosely packed) */
        {
            drwav__bswap_samples_s16((drwav_int16*)pSamples, sampleCount);
        } break;
        case 3: /* s24 */
        {
            drwav__bswap_samples_s24((drwav_uint8*)pSamples, sampleCount);
        } break;
        case 4: /* s32 */
        {
            drwav__bswap_samples_s32((drwav_int32*)pSamples, sampleCount);
        } break;
        default:
        {
            /* Unsupported format. */
            DRWAV_ASSERT(DRWAV_FALSE);
        } break;
    }
}

static DRWAV_INLINE void drwav__bswap_samples_ieee(void* pSamples, drwav_uint64 sampleCount, drwav_uint32 bytesPerSample)
{
    switch (bytesPerSample)
    {
    #if 0   /* Contributions welcome for f16 support. */
        case 2: /* f16 */
        {
            drwav__bswap_samples_f16((drwav_float16*)pSamples, sampleCount);
        } break;
    #endif
        case 4: /* f32 */
        {
            drwav__bswap_samples_f32((float*)pSamples, sampleCount);
        } break;
        case 8: /* f64 */
        {
            drwav__bswap_samples_f64((double*)pSamples, sampleCount);
        } break;
        default:
        {
            /* Unsupported format. */
            DRWAV_ASSERT(DRWAV_FALSE);
        } break;
    }
}

static DRWAV_INLINE void drwav__bswap_samples(void* pSamples, drwav_uint64 sampleCount, drwav_uint32 bytesPerSample, drwav_uint16 format)
{
    switch (format)
    {
        case DR_WAVE_FORMAT_PCM:
        {
            drwav__bswap_samples_pcm(pSamples, sampleCount, bytesPerSample);
        } break;

        case DR_WAVE_FORMAT_IEEE_FLOAT:
        {
            drwav__bswap_samples_ieee(pSamples, sampleCount, bytesPerSample);
        } break;

        case DR_WAVE_FORMAT_ALAW:
        case DR_WAVE_FORMAT_MULAW:
        {
            drwav__bswap_samples_s16((drwav_int16*)pSamples, sampleCount);
        } break;

        case DR_WAVE_FORMAT_ADPCM:
        case DR_WAVE_FORMAT_DVI_ADPCM:
        default:
        {
            /* Unsupported format. */
            DRWAV_ASSERT(DRWAV_FALSE);
        } break;
    }
}


static void* drwav__malloc_default(size_t sz, void* pUserData)
{
    (void)pUserData;
    return DRWAV_MALLOC(sz);
}

static void* drwav__realloc_default(void* p, size_t sz, void* pUserData)
{
    (void)pUserData;
    return DRWAV_REALLOC(p, sz);
}

static void drwav__free_default(void* p, void* pUserData)
{
    (void)pUserData;
    DRWAV_FREE(p);
}


static void* drwav__malloc_from_callbacks(size_t sz, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks == NULL) {
        return NULL;
    }

    if (pAllocationCallbacks->onMalloc != NULL) {
        return pAllocationCallbacks->onMalloc(sz, pAllocationCallbacks->pUserData);
    }

    /* Try using realloc(). */
    if (pAllocationCallbacks->onRealloc != NULL) {
        return pAllocationCallbacks->onRealloc(NULL, sz, pAllocationCallbacks->pUserData);
    }

    return NULL;
}

static void* drwav__realloc_from_callbacks(void* p, size_t szNew, size_t szOld, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks == NULL) {
        return NULL;
    }

    if (pAllocationCallbacks->onRealloc != NULL) {
        return pAllocationCallbacks->onRealloc(p, szNew, pAllocationCallbacks->pUserData);
    }

    /* Try emulating realloc() in terms of malloc()/free(). */
    if (pAllocationCallbacks->onMalloc != NULL && pAllocationCallbacks->onFree != NULL) {
        void* p2;

        p2 = pAllocationCallbacks->onMalloc(szNew, pAllocationCallbacks->pUserData);
        if (p2 == NULL) {
            return NULL;
        }

        if (p != NULL) {
            DRWAV_COPY_MEMORY(p2, p, szOld);
            pAllocationCallbacks->onFree(p, pAllocationCallbacks->pUserData);
        }

        return p2;
    }

    return NULL;
}

static void drwav__free_from_callbacks(void* p, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (p == NULL || pAllocationCallbacks == NULL) {
        return;
    }

    if (pAllocationCallbacks->onFree != NULL) {
        pAllocationCallbacks->onFree(p, pAllocationCallbacks->pUserData);
    }
}


drwav_allocation_callbacks drwav_copy_allocation_callbacks_or_defaults(const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks != NULL) {
        /* Copy. */
        return *pAllocationCallbacks;
    } else {
        /* Defaults. */
        drwav_allocation_callbacks allocationCallbacks;
        allocationCallbacks.pUserData = NULL;
        allocationCallbacks.onMalloc  = drwav__malloc_default;
        allocationCallbacks.onRealloc = drwav__realloc_default;
        allocationCallbacks.onFree    = drwav__free_default;
        return allocationCallbacks;
    }
}


static DRWAV_INLINE drwav_bool32 drwav__is_compressed_format_tag(drwav_uint16 formatTag)
{
    return
        formatTag == DR_WAVE_FORMAT_ADPCM ||
        formatTag == DR_WAVE_FORMAT_DVI_ADPCM;
}

static unsigned int drwav__chunk_padding_size_riff(drwav_uint64 chunkSize)
{
    return (unsigned int)(chunkSize % 2);
}

static unsigned int drwav__chunk_padding_size_w64(drwav_uint64 chunkSize)
{
    return (unsigned int)(chunkSize % 8);
}

drwav_uint64 drwav_read_pcm_frames_s16__msadpcm(drwav* pWav, drwav_uint64 samplesToRead, drwav_int16* pBufferOut);
drwav_uint64 drwav_read_pcm_frames_s16__ima(drwav* pWav, drwav_uint64 samplesToRead, drwav_int16* pBufferOut);
drwav_bool32 drwav_init_write__internal(drwav* pWav, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount);

static drwav_result drwav__read_chunk_header(drwav_read_proc onRead, void* pUserData, drwav_container container, drwav_uint64* pRunningBytesReadOut, drwav_chunk_header* pHeaderOut)
{
    if (container == drwav_container_riff) {
        unsigned char sizeInBytes[4];

        if (onRead(pUserData, pHeaderOut->id.fourcc, 4) != 4) {
            return DRWAV_EOF;
        }

        if (onRead(pUserData, sizeInBytes, 4) != 4) {
            return DRWAV_INVALID_FILE;
        }

        pHeaderOut->sizeInBytes = drwav__bytes_to_u32(sizeInBytes);
        pHeaderOut->paddingSize = drwav__chunk_padding_size_riff(pHeaderOut->sizeInBytes);
        *pRunningBytesReadOut += 8;
    } else {
        unsigned char sizeInBytes[8];

        if (onRead(pUserData, pHeaderOut->id.guid, 16) != 16) {
            return DRWAV_EOF;
        }

        if (onRead(pUserData, sizeInBytes, 8) != 8) {
            return DRWAV_INVALID_FILE;
        }

        pHeaderOut->sizeInBytes = drwav__bytes_to_u64(sizeInBytes) - 24;    /* <-- Subtract 24 because w64 includes the size of the header. */
        pHeaderOut->paddingSize = drwav__chunk_padding_size_w64(pHeaderOut->sizeInBytes);
        *pRunningBytesReadOut += 24;
    }

    return DRWAV_SUCCESS;
}

static drwav_bool32 drwav__seek_forward(drwav_seek_proc onSeek, drwav_uint64 offset, void* pUserData)
{
    drwav_uint64 bytesRemainingToSeek = offset;
    while (bytesRemainingToSeek > 0) {
        if (bytesRemainingToSeek > 0x7FFFFFFF) {
            if (!onSeek(pUserData, 0x7FFFFFFF, drwav_seek_origin_current)) {
                return DRWAV_FALSE;
            }
            bytesRemainingToSeek -= 0x7FFFFFFF;
        } else {
            if (!onSeek(pUserData, (int)bytesRemainingToSeek, drwav_seek_origin_current)) {
                return DRWAV_FALSE;
            }
            bytesRemainingToSeek = 0;
        }
    }

    return DRWAV_TRUE;
}

static drwav_bool32 drwav__seek_from_start(drwav_seek_proc onSeek, drwav_uint64 offset, void* pUserData)
{
    if (offset <= 0x7FFFFFFF) {
        return onSeek(pUserData, (int)offset, drwav_seek_origin_start);
    }

    /* Larger than 32-bit seek. */
    if (!onSeek(pUserData, 0x7FFFFFFF, drwav_seek_origin_start)) {
        return DRWAV_FALSE;
    }
    offset -= 0x7FFFFFFF;

    for (;;) {
        if (offset <= 0x7FFFFFFF) {
            return onSeek(pUserData, (int)offset, drwav_seek_origin_current);
        }

        if (!onSeek(pUserData, 0x7FFFFFFF, drwav_seek_origin_current)) {
            return DRWAV_FALSE;
        }
        offset -= 0x7FFFFFFF;
    }

    /* Should never get here. */
    /*return DRWAV_TRUE; */
}


static drwav_bool32 drwav__read_fmt(drwav_read_proc onRead, drwav_seek_proc onSeek, void* pUserData, drwav_container container, drwav_uint64* pRunningBytesReadOut, drwav_fmt* fmtOut)
{
    drwav_chunk_header header;
    unsigned char fmt[16];

    if (drwav__read_chunk_header(onRead, pUserData, container, pRunningBytesReadOut, &header) != DRWAV_SUCCESS) {
        return DRWAV_FALSE;
    }


    /* Skip non-fmt chunks. */
    while ((container == drwav_container_riff && !drwav__fourcc_equal(header.id.fourcc, "fmt ")) || (container == drwav_container_w64 && !drwav__guid_equal(header.id.guid, drwavGUID_W64_FMT))) {
        if (!drwav__seek_forward(onSeek, header.sizeInBytes + header.paddingSize, pUserData)) {
            return DRWAV_FALSE;
        }
        *pRunningBytesReadOut += header.sizeInBytes + header.paddingSize;

        /* Try the next header. */
        if (drwav__read_chunk_header(onRead, pUserData, container, pRunningBytesReadOut, &header) != DRWAV_SUCCESS) {
            return DRWAV_FALSE;
        }
    }


    /* Validation. */
    if (container == drwav_container_riff) {
        if (!drwav__fourcc_equal(header.id.fourcc, "fmt ")) {
            return DRWAV_FALSE;
        }
    } else {
        if (!drwav__guid_equal(header.id.guid, drwavGUID_W64_FMT)) {
            return DRWAV_FALSE;
        }
    }


    if (onRead(pUserData, fmt, sizeof(fmt)) != sizeof(fmt)) {
        return DRWAV_FALSE;
    }
    *pRunningBytesReadOut += sizeof(fmt);

    fmtOut->formatTag      = drwav__bytes_to_u16(fmt + 0);
    fmtOut->channels       = drwav__bytes_to_u16(fmt + 2);
    fmtOut->sampleRate     = drwav__bytes_to_u32(fmt + 4);
    fmtOut->avgBytesPerSec = drwav__bytes_to_u32(fmt + 8);
    fmtOut->blockAlign     = drwav__bytes_to_u16(fmt + 12);
    fmtOut->bitsPerSample  = drwav__bytes_to_u16(fmt + 14);

    fmtOut->extendedSize       = 0;
    fmtOut->validBitsPerSample = 0;
    fmtOut->channelMask        = 0;
    memset(fmtOut->subFormat, 0, sizeof(fmtOut->subFormat));

    if (header.sizeInBytes > 16) {
        unsigned char fmt_cbSize[2];
        int bytesReadSoFar = 0;

        if (onRead(pUserData, fmt_cbSize, sizeof(fmt_cbSize)) != sizeof(fmt_cbSize)) {
            return DRWAV_FALSE;    /* Expecting more data. */
        }
        *pRunningBytesReadOut += sizeof(fmt_cbSize);

        bytesReadSoFar = 18;

        fmtOut->extendedSize = drwav__bytes_to_u16(fmt_cbSize);
        if (fmtOut->extendedSize > 0) {
            /* Simple validation. */
            if (fmtOut->formatTag == DR_WAVE_FORMAT_EXTENSIBLE) {
                if (fmtOut->extendedSize != 22) {
                    return DRWAV_FALSE;
                }
            }

            if (fmtOut->formatTag == DR_WAVE_FORMAT_EXTENSIBLE) {
                unsigned char fmtext[22];
                if (onRead(pUserData, fmtext, fmtOut->extendedSize) != fmtOut->extendedSize) {
                    return DRWAV_FALSE;    /* Expecting more data. */
                }

                fmtOut->validBitsPerSample = drwav__bytes_to_u16(fmtext + 0);
                fmtOut->channelMask        = drwav__bytes_to_u32(fmtext + 2);
                drwav__bytes_to_guid(fmtext + 6, fmtOut->subFormat);
            } else {
                if (!onSeek(pUserData, fmtOut->extendedSize, drwav_seek_origin_current)) {
                    return DRWAV_FALSE;
                }
            }
            *pRunningBytesReadOut += fmtOut->extendedSize;

            bytesReadSoFar += fmtOut->extendedSize;
        }

        /* Seek past any leftover bytes. For w64 the leftover will be defined based on the chunk size. */
        if (!onSeek(pUserData, (int)(header.sizeInBytes - bytesReadSoFar), drwav_seek_origin_current)) {
            return DRWAV_FALSE;
        }
        *pRunningBytesReadOut += (header.sizeInBytes - bytesReadSoFar);
    }

    if (header.paddingSize > 0) {
        if (!onSeek(pUserData, header.paddingSize, drwav_seek_origin_current)) {
            return DRWAV_FALSE;
        }
        *pRunningBytesReadOut += header.paddingSize;
    }

    return DRWAV_TRUE;
}


size_t drwav__on_read(drwav_read_proc onRead, void* pUserData, void* pBufferOut, size_t bytesToRead, drwav_uint64* pCursor)
{
    size_t bytesRead;

    DRWAV_ASSERT(onRead != NULL);
    DRWAV_ASSERT(pCursor != NULL);

    bytesRead = onRead(pUserData, pBufferOut, bytesToRead);
    *pCursor += bytesRead;
    return bytesRead;
}

drwav_bool32 drwav__on_seek(drwav_seek_proc onSeek, void* pUserData, int offset, drwav_seek_origin origin, drwav_uint64* pCursor)
{
    DRWAV_ASSERT(onSeek != NULL);
    DRWAV_ASSERT(pCursor != NULL);

    if (!onSeek(pUserData, offset, origin)) {
        return DRWAV_FALSE;
    }

    if (origin == drwav_seek_origin_start) {
        *pCursor = offset;
    } else {
        *pCursor += offset;
    }

    return DRWAV_TRUE;
}



static drwav_uint32 drwav_get_bytes_per_pcm_frame(drwav* pWav)
{
    /*
    The bytes per frame is a bit ambiguous. It can be either be based on the bits per sample, or the block align. The way I'm doing it here
    is that if the bits per sample is a multiple of 8, use floor(bitsPerSample*channels/8), otherwise fall back to the block align.
    */
    if ((pWav->bitsPerSample & 0x7) == 0) {
        /* Bits per sample is a multiple of 8. */
        return (pWav->bitsPerSample * pWav->fmt.channels) >> 3;
    } else {
        return pWav->fmt.blockAlign;
    }
}


drwav_bool32 drwav_preinit(drwav* pWav, drwav_read_proc onRead, drwav_seek_proc onSeek, void* pReadSeekUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pWav == NULL || onRead == NULL || onSeek == NULL) {
        return DRWAV_FALSE;
    }

    DRWAV_ZERO_MEMORY(pWav, sizeof(*pWav));
    pWav->onRead    = onRead;
    pWav->onSeek    = onSeek;
    pWav->pUserData = pReadSeekUserData;
    pWav->allocationCallbacks = drwav_copy_allocation_callbacks_or_defaults(pAllocationCallbacks);

    if (pWav->allocationCallbacks.onFree == NULL || (pWav->allocationCallbacks.onMalloc == NULL && pWav->allocationCallbacks.onRealloc == NULL)) {
        return DRWAV_FALSE;    /* Invalid allocation callbacks. */
    }

    return DRWAV_TRUE;
}

drwav_bool32 drwav_init__internal(drwav* pWav, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags)
{
    /* This function assumes drwav_preinit() has been called beforehand. */

    drwav_uint64 cursor;    /* <-- Keeps track of the byte position so we can seek to specific locations. */
    drwav_bool32 sequential;
    unsigned char riff[4];
    drwav_fmt fmt;
    unsigned short translatedFormatTag;
    drwav_uint64 sampleCountFromFactChunk;
    drwav_bool32 foundDataChunk;
    drwav_uint64 dataChunkSize;
    drwav_uint64 chunkSize;

    cursor = 0;
    sequential = (flags & DRWAV_SEQUENTIAL) != 0;

    /* The first 4 bytes should be the RIFF identifier. */
    if (drwav__on_read(pWav->onRead, pWav->pUserData, riff, sizeof(riff), &cursor) != sizeof(riff)) {
        return DRWAV_FALSE;
    }

    /*
    The first 4 bytes can be used to identify the container. For RIFF files it will start with "RIFF" and for
    w64 it will start with "riff".
    */
    if (drwav__fourcc_equal(riff, "RIFF")) {
        pWav->container = drwav_container_riff;
    } else if (drwav__fourcc_equal(riff, "riff")) {
        int i;
        drwav_uint8 riff2[12];

        pWav->container = drwav_container_w64;

        /* Check the rest of the GUID for validity. */
        if (drwav__on_read(pWav->onRead, pWav->pUserData, riff2, sizeof(riff2), &cursor) != sizeof(riff2)) {
            return DRWAV_FALSE;
        }

        for (i = 0; i < 12; ++i) {
            if (riff2[i] != drwavGUID_W64_RIFF[i+4]) {
                return DRWAV_FALSE;
            }
        }
    } else {
        return DRWAV_FALSE;   /* Unknown or unsupported container. */
    }


    if (pWav->container == drwav_container_riff) {
        unsigned char chunkSizeBytes[4];
        unsigned char wave[4];

        /* RIFF/WAVE */
        if (drwav__on_read(pWav->onRead, pWav->pUserData, chunkSizeBytes, sizeof(chunkSizeBytes), &cursor) != sizeof(chunkSizeBytes)) {
            return DRWAV_FALSE;
        }

        if (drwav__bytes_to_u32(chunkSizeBytes) < 36) {
            return DRWAV_FALSE;    /* Chunk size should always be at least 36 bytes. */
        }

        if (drwav__on_read(pWav->onRead, pWav->pUserData, wave, sizeof(wave), &cursor) != sizeof(wave)) {
            return DRWAV_FALSE;
        }

        if (!drwav__fourcc_equal(wave, "WAVE")) {
            return DRWAV_FALSE;    /* Expecting "WAVE". */
        }
    } else {
        unsigned char chunkSizeBytes[8];
        drwav_uint8 wave[16];

        /* W64 */
        if (drwav__on_read(pWav->onRead, pWav->pUserData, chunkSizeBytes, sizeof(chunkSizeBytes), &cursor) != sizeof(chunkSizeBytes)) {
            return DRWAV_FALSE;
        }

        if (drwav__bytes_to_u64(chunkSizeBytes) < 80) {
            return DRWAV_FALSE;
        }

        if (drwav__on_read(pWav->onRead, pWav->pUserData, wave, sizeof(wave), &cursor) != sizeof(wave)) {
            return DRWAV_FALSE;
        }

        if (!drwav__guid_equal(wave, drwavGUID_W64_WAVE)) {
            return DRWAV_FALSE;
        }
    }


    /* The next bytes should be the "fmt " chunk. */
    if (!drwav__read_fmt(pWav->onRead, pWav->onSeek, pWav->pUserData, pWav->container, &cursor, &fmt)) {
        return DRWAV_FALSE;    /* Failed to read the "fmt " chunk. */
    }

    /* Basic validation. */
    if ((fmt.sampleRate    == 0 || fmt.sampleRate    > DRWAV_MAX_SAMPLE_RATE)     ||
        (fmt.channels      == 0 || fmt.channels      > DRWAV_MAX_CHANNELS)        ||
        (fmt.bitsPerSample == 0 || fmt.bitsPerSample > DRWAV_MAX_BITS_PER_SAMPLE) ||
        fmt.blockAlign == 0) {
        return DRWAV_FALSE; /* Probably an invalid WAV file. */
    }


    /* Translate the internal format. */
    translatedFormatTag = fmt.formatTag;
    if (translatedFormatTag == DR_WAVE_FORMAT_EXTENSIBLE) {
        translatedFormatTag = drwav__bytes_to_u16(fmt.subFormat + 0);
    }



    sampleCountFromFactChunk = 0;

    /*
    We need to enumerate over each chunk for two reasons:
      1) The "data" chunk may not be the next one
      2) We may want to report each chunk back to the client
    
    In order to correctly report each chunk back to the client we will need to keep looping until the end of the file.
    */
    foundDataChunk = DRWAV_FALSE;
    dataChunkSize = 0;

    /* The next chunk we care about is the "data" chunk. This is not necessarily the next chunk so we'll need to loop. */
    for (;;)
    {
        drwav_chunk_header header;
        drwav_result result = drwav__read_chunk_header(pWav->onRead, pWav->pUserData, pWav->container, &cursor, &header);
        if (result != DRWAV_SUCCESS) {
            if (!foundDataChunk) {
                return DRWAV_FALSE;
            } else {
                break;  /* Probably at the end of the file. Get out of the loop. */
            }
        }

        /* Tell the client about this chunk. */
        if (!sequential && onChunk != NULL) {
            drwav_uint64 callbackBytesRead = onChunk(pChunkUserData, pWav->onRead, pWav->onSeek, pWav->pUserData, &header);

            /*
            dr_wav may need to read the contents of the chunk, so we now need to seek back to the position before
            we called the callback.
            */
            if (callbackBytesRead > 0) {
                if (!drwav__seek_from_start(pWav->onSeek, cursor, pWav->pUserData)) {
                    return DRWAV_FALSE;
                }
            }
        }
        

        if (!foundDataChunk) {
            pWav->dataChunkDataPos = cursor;
        }

        chunkSize = header.sizeInBytes;
        if (pWav->container == drwav_container_riff) {
            if (drwav__fourcc_equal(header.id.fourcc, "data")) {
                foundDataChunk = DRWAV_TRUE;
                dataChunkSize = chunkSize;
            }
        } else {
            if (drwav__guid_equal(header.id.guid, drwavGUID_W64_DATA)) {
                foundDataChunk = DRWAV_TRUE;
                dataChunkSize = chunkSize;
            }
        }

        /*
        If at this point we have found the data chunk and we're running in sequential mode, we need to break out of this loop. The reason for
        this is that we would otherwise require a backwards seek which sequential mode forbids.
        */
        if (foundDataChunk && sequential) {
            break;
        }

        /* Optional. Get the total sample count from the FACT chunk. This is useful for compressed formats. */
        if (pWav->container == drwav_container_riff) {
            if (drwav__fourcc_equal(header.id.fourcc, "fact")) {
                drwav_uint32 sampleCount;
                if (drwav__on_read(pWav->onRead, pWav->pUserData, &sampleCount, 4, &cursor) != 4) {
                    return DRWAV_FALSE;
                }
                chunkSize -= 4;

                if (!foundDataChunk) {
                    pWav->dataChunkDataPos = cursor;
                }

                /*
                The sample count in the "fact" chunk is either unreliable, or I'm not understanding it properly. For now I am only enabling this
                for Microsoft ADPCM formats.
                */
                if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
                    sampleCountFromFactChunk = sampleCount;
                } else {
                    sampleCountFromFactChunk = 0;
                }
            }
        } else {
            if (drwav__guid_equal(header.id.guid, drwavGUID_W64_FACT)) {
                if (drwav__on_read(pWav->onRead, pWav->pUserData, &sampleCountFromFactChunk, 8, &cursor) != 8) {
                    return DRWAV_FALSE;
                }
                chunkSize -= 8;

                if (!foundDataChunk) {
                    pWav->dataChunkDataPos = cursor;
                }
            }
        }

        /* "smpl" chunk. */
        if (pWav->container == drwav_container_riff) {
            if (drwav__fourcc_equal(header.id.fourcc, "smpl")) {
                unsigned char smplHeaderData[36];    /* 36 = size of the smpl header section, not including the loop data. */
                if (chunkSize >= sizeof(smplHeaderData)) {
                    drwav_uint64 bytesJustRead = drwav__on_read(pWav->onRead, pWav->pUserData, smplHeaderData, sizeof(smplHeaderData), &cursor);
                    chunkSize -= bytesJustRead;

                    if (bytesJustRead == sizeof(smplHeaderData)) {
                        drwav_uint32 iLoop;

                        pWav->smpl.manufacturer      = drwav__bytes_to_u32(smplHeaderData+0);
                        pWav->smpl.product           = drwav__bytes_to_u32(smplHeaderData+4);
                        pWav->smpl.samplePeriod      = drwav__bytes_to_u32(smplHeaderData+8);
                        pWav->smpl.midiUnityNotes    = drwav__bytes_to_u32(smplHeaderData+12);
                        pWav->smpl.midiPitchFraction = drwav__bytes_to_u32(smplHeaderData+16);
                        pWav->smpl.smpteFormat       = drwav__bytes_to_u32(smplHeaderData+20);
                        pWav->smpl.smpteOffset       = drwav__bytes_to_u32(smplHeaderData+24);
                        pWav->smpl.numSampleLoops    = drwav__bytes_to_u32(smplHeaderData+28);
                        pWav->smpl.samplerData       = drwav__bytes_to_u32(smplHeaderData+32);

                        for (iLoop = 0; iLoop < pWav->smpl.numSampleLoops && iLoop < drwav_countof(pWav->smpl.loops); ++iLoop) {
                            unsigned char smplLoopData[24];  /* 24 = size of a loop section in the smpl chunk. */
                            bytesJustRead = drwav__on_read(pWav->onRead, pWav->pUserData, smplLoopData, sizeof(smplLoopData), &cursor);
                            chunkSize -= bytesJustRead;

                            if (bytesJustRead == sizeof(smplLoopData)) {
                                pWav->smpl.loops[iLoop].cuePointId = drwav__bytes_to_u32(smplLoopData+0);
                                pWav->smpl.loops[iLoop].type       = drwav__bytes_to_u32(smplLoopData+4);
                                pWav->smpl.loops[iLoop].start      = drwav__bytes_to_u32(smplLoopData+8);
                                pWav->smpl.loops[iLoop].end        = drwav__bytes_to_u32(smplLoopData+12);
                                pWav->smpl.loops[iLoop].fraction   = drwav__bytes_to_u32(smplLoopData+16);
                                pWav->smpl.loops[iLoop].playCount  = drwav__bytes_to_u32(smplLoopData+20);
                            } else {
                                break;  /* Break from the smpl loop for loop. */
                            }
                        }
                    }
                } else {
                    /* Looks like invalid data. Ignore the chunk. */
                }
            }
        } else {
            if (drwav__guid_equal(header.id.guid, drwavGUID_W64_SMPL)) {
                /*
                This path will be hit when a W64 WAV file contains a smpl chunk. I don't have a sample file to test this path, so a contribution
                is welcome to add support for this.
                */
            }
        }

        /* Make sure we seek past the padding. */
        chunkSize += header.paddingSize;
        if (!drwav__seek_forward(pWav->onSeek, chunkSize, pWav->pUserData)) {
            break;
        }
        cursor += chunkSize;

        if (!foundDataChunk) {
            pWav->dataChunkDataPos = cursor;
        }
    }

    /* If we haven't found a data chunk, return an error. */
    if (!foundDataChunk) {
        return DRWAV_FALSE;
    }

    /* We may have moved passed the data chunk. If so we need to move back. If running in sequential mode we can assume we are already sitting on the data chunk. */
    if (!sequential) {
        if (!drwav__seek_from_start(pWav->onSeek, pWav->dataChunkDataPos, pWav->pUserData)) {
            return DRWAV_FALSE;
        }
        cursor = pWav->dataChunkDataPos;
    }
    

    /* At this point we should be sitting on the first byte of the raw audio data. */

    pWav->fmt                 = fmt;
    pWav->sampleRate          = fmt.sampleRate;
    pWav->channels            = fmt.channels;
    pWav->bitsPerSample       = fmt.bitsPerSample;
    pWav->bytesRemaining      = dataChunkSize;
    pWav->translatedFormatTag = translatedFormatTag;
    pWav->dataChunkDataSize   = dataChunkSize;

    if (sampleCountFromFactChunk != 0) {
        pWav->totalPCMFrameCount = sampleCountFromFactChunk;
    } else {
        pWav->totalPCMFrameCount = dataChunkSize / drwav_get_bytes_per_pcm_frame(pWav);

        if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
            drwav_uint64 totalBlockHeaderSizeInBytes;
            drwav_uint64 blockCount = dataChunkSize / fmt.blockAlign;

            /* Make sure any trailing partial block is accounted for. */
            if ((blockCount * fmt.blockAlign) < dataChunkSize) {
                blockCount += 1;
            }

            /* We decode two samples per byte. There will be blockCount headers in the data chunk. This is enough to know how to calculate the total PCM frame count. */
            totalBlockHeaderSizeInBytes = blockCount * (6*fmt.channels);
            pWav->totalPCMFrameCount = ((dataChunkSize - totalBlockHeaderSizeInBytes) * 2) / fmt.channels;
        }
        if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
            drwav_uint64 totalBlockHeaderSizeInBytes;
            drwav_uint64 blockCount = dataChunkSize / fmt.blockAlign;

            /* Make sure any trailing partial block is accounted for. */
            if ((blockCount * fmt.blockAlign) < dataChunkSize) {
                blockCount += 1;
            }

            /* We decode two samples per byte. There will be blockCount headers in the data chunk. This is enough to know how to calculate the total PCM frame count. */
            totalBlockHeaderSizeInBytes = blockCount * (4*fmt.channels);
            pWav->totalPCMFrameCount = ((dataChunkSize - totalBlockHeaderSizeInBytes) * 2) / fmt.channels;

            /* The header includes a decoded sample for each channel which acts as the initial predictor sample. */
            pWav->totalPCMFrameCount += blockCount;
        }
    }

    /* Some formats only support a certain number of channels. */
    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM || pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
        if (pWav->channels > 2) {
            return DRWAV_FALSE;
        }
    }

#ifdef DR_WAV_LIBSNDFILE_COMPAT
    /*
    I use libsndfile as a benchmark for testing, however in the version I'm using (from the Windows installer on the libsndfile website),
    it appears the total sample count libsndfile uses for MS-ADPCM is incorrect. It would seem they are computing the total sample count
    from the number of blocks, however this results in the inclusion of extra silent samples at the end of the last block. The correct
    way to know the total sample count is to inspect the "fact" chunk, which should always be present for compressed formats, and should
    always include the sample count. This little block of code below is only used to emulate the libsndfile logic so I can properly run my
    correctness tests against libsndfile, and is disabled by default.
    */
    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
        drwav_uint64 blockCount = dataChunkSize / fmt.blockAlign;
        pWav->totalPCMFrameCount = (((blockCount * (fmt.blockAlign - (6*pWav->channels))) * 2)) / fmt.channels;  /* x2 because two samples per byte. */
    }
    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
        drwav_uint64 blockCount = dataChunkSize / fmt.blockAlign;
        pWav->totalPCMFrameCount = (((blockCount * (fmt.blockAlign - (4*pWav->channels))) * 2) + (blockCount * pWav->channels)) / fmt.channels;
    }
#endif

    return DRWAV_TRUE;
}

drwav_bool32 drwav_init(drwav* pWav, drwav_read_proc onRead, drwav_seek_proc onSeek, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_ex(pWav, onRead, onSeek, NULL, pUserData, NULL, 0, pAllocationCallbacks);
}

drwav_bool32 drwav_init_ex(drwav* pWav, drwav_read_proc onRead, drwav_seek_proc onSeek, drwav_chunk_proc onChunk, void* pReadSeekUserData, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (!drwav_preinit(pWav, onRead, onSeek, pReadSeekUserData, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    return drwav_init__internal(pWav, onChunk, pChunkUserData, flags);
}


static drwav_uint32 drwav__riff_chunk_size_riff(drwav_uint64 dataChunkSize)
{
    drwav_uint32 dataSubchunkPaddingSize = drwav__chunk_padding_size_riff(dataChunkSize);

    if (dataChunkSize <= (0xFFFFFFFFUL - 36 - dataSubchunkPaddingSize)) {
        return 36 + (drwav_uint32)(dataChunkSize + dataSubchunkPaddingSize);
    } else {
        return 0xFFFFFFFF;
    }
}

static drwav_uint32 drwav__data_chunk_size_riff(drwav_uint64 dataChunkSize)
{
    if (dataChunkSize <= 0xFFFFFFFFUL) {
        return (drwav_uint32)dataChunkSize;
    } else {
        return 0xFFFFFFFFUL;
    }
}

static drwav_uint64 drwav__riff_chunk_size_w64(drwav_uint64 dataChunkSize)
{
    drwav_uint64 dataSubchunkPaddingSize = drwav__chunk_padding_size_w64(dataChunkSize);

    return 80 + 24 + dataChunkSize + dataSubchunkPaddingSize;   /* +24 because W64 includes the size of the GUID and size fields. */
}

static drwav_uint64 drwav__data_chunk_size_w64(drwav_uint64 dataChunkSize)
{
    return 24 + dataChunkSize;        /* +24 because W64 includes the size of the GUID and size fields. */
}


drwav_bool32 drwav_preinit_write(drwav* pWav, const drwav_data_format* pFormat, drwav_bool32 isSequential, drwav_write_proc onWrite, drwav_seek_proc onSeek, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pWav == NULL || onWrite == NULL) {
        return DRWAV_FALSE;
    }

    if (!isSequential && onSeek == NULL) {
        return DRWAV_FALSE; /* <-- onSeek is required when in non-sequential mode. */
    }

    /* Not currently supporting compressed formats. Will need to add support for the "fact" chunk before we enable this. */
    if (pFormat->format == DR_WAVE_FORMAT_EXTENSIBLE) {
        return DRWAV_FALSE;
    }
    if (pFormat->format == DR_WAVE_FORMAT_ADPCM || pFormat->format == DR_WAVE_FORMAT_DVI_ADPCM) {
        return DRWAV_FALSE;
    }

    DRWAV_ZERO_MEMORY(pWav, sizeof(*pWav));
    pWav->onWrite   = onWrite;
    pWav->onSeek    = onSeek;
    pWav->pUserData = pUserData;
    pWav->allocationCallbacks = drwav_copy_allocation_callbacks_or_defaults(pAllocationCallbacks);

    if (pWav->allocationCallbacks.onFree == NULL || (pWav->allocationCallbacks.onMalloc == NULL && pWav->allocationCallbacks.onRealloc == NULL)) {
        return DRWAV_FALSE;    /* Invalid allocation callbacks. */
    }

    pWav->fmt.formatTag = (drwav_uint16)pFormat->format;
    pWav->fmt.channels = (drwav_uint16)pFormat->channels;
    pWav->fmt.sampleRate = pFormat->sampleRate;
    pWav->fmt.avgBytesPerSec = (drwav_uint32)((pFormat->bitsPerSample * pFormat->sampleRate * pFormat->channels) / 8);
    pWav->fmt.blockAlign = (drwav_uint16)((pFormat->channels * pFormat->bitsPerSample) / 8);
    pWav->fmt.bitsPerSample = (drwav_uint16)pFormat->bitsPerSample;
    pWav->fmt.extendedSize = 0;
    pWav->isSequentialWrite = isSequential;

    return DRWAV_TRUE;
}

drwav_bool32 drwav_init_write__internal(drwav* pWav, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount)
{
    /* The function assumes drwav_preinit_write() was called beforehand. */

    size_t runningPos = 0;
    drwav_uint64 initialDataChunkSize = 0;
    drwav_uint64 chunkSizeFMT;

    /*
    The initial values for the "RIFF" and "data" chunks depends on whether or not we are initializing in sequential mode or not. In
    sequential mode we set this to its final values straight away since they can be calculated from the total sample count. In non-
    sequential mode we initialize it all to zero and fill it out in drwav_uninit() using a backwards seek.
    */
    if (pWav->isSequentialWrite) {
        initialDataChunkSize = (totalSampleCount * pWav->fmt.bitsPerSample) / 8;

        /*
        The RIFF container has a limit on the number of samples. drwav is not allowing this. There's no practical limits for Wave64
        so for the sake of simplicity I'm not doing any validation for that.
        */
        if (pFormat->container == drwav_container_riff) {
            if (initialDataChunkSize > (0xFFFFFFFFUL - 36)) {
                return DRWAV_FALSE; /* Not enough room to store every sample. */
            }
        }
    }

    pWav->dataChunkDataSizeTargetWrite = initialDataChunkSize;


    /* "RIFF" chunk. */
    if (pFormat->container == drwav_container_riff) {
        drwav_uint32 chunkSizeRIFF = 36 + (drwav_uint32)initialDataChunkSize;   /* +36 = "RIFF"+[RIFF Chunk Size]+"WAVE" + [sizeof "fmt " chunk] */
        runningPos += pWav->onWrite(pWav->pUserData, "RIFF", 4);
        runningPos += pWav->onWrite(pWav->pUserData, &chunkSizeRIFF, 4);
        runningPos += pWav->onWrite(pWav->pUserData, "WAVE", 4);
    } else {
        drwav_uint64 chunkSizeRIFF = 80 + 24 + initialDataChunkSize;   /* +24 because W64 includes the size of the GUID and size fields. */
        runningPos += pWav->onWrite(pWav->pUserData, drwavGUID_W64_RIFF, 16);
        runningPos += pWav->onWrite(pWav->pUserData, &chunkSizeRIFF, 8);
        runningPos += pWav->onWrite(pWav->pUserData, drwavGUID_W64_WAVE, 16);
    }

    /* "fmt " chunk. */
    if (pFormat->container == drwav_container_riff) {
        chunkSizeFMT = 16;
        runningPos += pWav->onWrite(pWav->pUserData, "fmt ", 4);
        runningPos += pWav->onWrite(pWav->pUserData, &chunkSizeFMT, 4);
    } else {
        chunkSizeFMT = 40;
        runningPos += pWav->onWrite(pWav->pUserData, drwavGUID_W64_FMT, 16);
        runningPos += pWav->onWrite(pWav->pUserData, &chunkSizeFMT, 8);
    }

    runningPos += pWav->onWrite(pWav->pUserData, &pWav->fmt.formatTag,      2);
    runningPos += pWav->onWrite(pWav->pUserData, &pWav->fmt.channels,       2);
    runningPos += pWav->onWrite(pWav->pUserData, &pWav->fmt.sampleRate,     4);
    runningPos += pWav->onWrite(pWav->pUserData, &pWav->fmt.avgBytesPerSec, 4);
    runningPos += pWav->onWrite(pWav->pUserData, &pWav->fmt.blockAlign,     2);
    runningPos += pWav->onWrite(pWav->pUserData, &pWav->fmt.bitsPerSample,  2);

    pWav->dataChunkDataPos = runningPos;

    /* "data" chunk. */
    if (pFormat->container == drwav_container_riff) {
        drwav_uint32 chunkSizeDATA = (drwav_uint32)initialDataChunkSize;
        runningPos += pWav->onWrite(pWav->pUserData, "data", 4);
        runningPos += pWav->onWrite(pWav->pUserData, &chunkSizeDATA, 4);
    } else {
        drwav_uint64 chunkSizeDATA = 24 + initialDataChunkSize; /* +24 because W64 includes the size of the GUID and size fields. */
        runningPos += pWav->onWrite(pWav->pUserData, drwavGUID_W64_DATA, 16);
        runningPos += pWav->onWrite(pWav->pUserData, &chunkSizeDATA, 8);
    }


    /* Simple validation. */
    if (pFormat->container == drwav_container_riff) {
        if (runningPos != 20 + chunkSizeFMT + 8) {
            return DRWAV_FALSE;
        }
    } else {
        if (runningPos != 40 + chunkSizeFMT + 24) {
            return DRWAV_FALSE;
        }
    }
    

    /* Set some properties for the client's convenience. */
    pWav->container = pFormat->container;
    pWav->channels = (drwav_uint16)pFormat->channels;
    pWav->sampleRate = pFormat->sampleRate;
    pWav->bitsPerSample = (drwav_uint16)pFormat->bitsPerSample;
    pWav->translatedFormatTag = (drwav_uint16)pFormat->format;

    return DRWAV_TRUE;
}


drwav_bool32 drwav_init_write(drwav* pWav, const drwav_data_format* pFormat, drwav_write_proc onWrite, drwav_seek_proc onSeek, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (!drwav_preinit_write(pWav, pFormat, DRWAV_FALSE, onWrite, onSeek, pUserData, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    return drwav_init_write__internal(pWav, pFormat, 0);               /* DRWAV_FALSE = Not Sequential */
}

drwav_bool32 drwav_init_write_sequential(drwav* pWav, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_write_proc onWrite, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (!drwav_preinit_write(pWav, pFormat, DRWAV_TRUE, onWrite, NULL, pUserData, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    return drwav_init_write__internal(pWav, pFormat, totalSampleCount); /* DRWAV_TRUE = Sequential */
}

drwav_bool32 drwav_init_write_sequential_pcm_frames(drwav* pWav, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, drwav_write_proc onWrite, void* pUserData, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pFormat == NULL) {
        return DRWAV_FALSE;
    }

    return drwav_init_write_sequential(pWav, pFormat, totalPCMFrameCount*pFormat->channels, onWrite, pUserData, pAllocationCallbacks);
}

drwav_uint64 drwav_target_write_size_bytes(drwav_data_format const *format, drwav_uint64 totalSampleCount)
{
    drwav_uint64 targetDataSizeBytes = (totalSampleCount * format->channels * format->bitsPerSample/8);
    drwav_uint64 riffChunkSizeBytes;
    drwav_uint64 fileSizeBytes;

    if (format->container == drwav_container_riff) {
        riffChunkSizeBytes = drwav__riff_chunk_size_riff(targetDataSizeBytes);
        fileSizeBytes = (8 + riffChunkSizeBytes); /* +8 because WAV doesn't include the size of the ChunkID and ChunkSize fields. */
    } else {
        riffChunkSizeBytes = drwav__riff_chunk_size_w64(targetDataSizeBytes);
        fileSizeBytes = riffChunkSizeBytes;
    }

    return fileSizeBytes;
}


#ifndef DR_WAV_NO_STDIO
FILE* drwav_fopen(const char* filePath, const char* openMode)
{
    FILE* pFile;
#if defined(_MSC_VER) && _MSC_VER >= 1400
    if (fopen_s(&pFile, filePath, openMode) != 0) {
        return NULL;
    }
#else
    pFile = fopen(filePath, openMode);
    if (pFile == NULL) {
        return NULL;
    }
#endif

    return pFile;
}

FILE* drwav_wfopen(const wchar_t* pFilePath, const wchar_t* pOpenMode, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile;

#if defined(_WIN32)
    (void)pAllocationCallbacks;
    #if defined(_MSC_VER) && _MSC_VER >= 1400
        if (_wfopen_s(&pFile, pFilePath, pOpenMode) != 0) {
            return NULL;
        }
    #else
        pFile = _wfopen(pFilePath, pOpenMode);
        if (pFile == NULL) {
            return NULL;
        }
    #endif
#else
    /*
    Use fopen() on anything other than Windows. Requires a conversion. This is annoying because fopen() is locale specific. The only real way I can
    think of to do this is with wcsrtombs(). Note that wcstombs() is apparently not thread-safe because it uses a static global mbstate_t object for
    maintaining state. I've checked this with -std=c89 and it works, but if somebody get's a compiler error I'll look into improving compatibility.
    */
    {
        mbstate_t mbs;
        size_t lenMB;
        const wchar_t* pFilePathTemp = pFilePath;
        char* pFilePathMB = NULL;
        const wchar_t* pOpenModeMBTemp = pOpenMode;
        char pOpenModeMB[16];
        drwav_allocation_callbacks allocationCallbacks;

        allocationCallbacks = drwav_copy_allocation_callbacks_or_defaults(pAllocationCallbacks);

        /* Get the length first. */
        DRWAV_ZERO_MEMORY(&mbs, sizeof(mbs));
        lenMB = wcsrtombs(NULL, &pFilePathTemp, 0, &mbs);
        if (lenMB == (size_t)-1) {
            return NULL;
        }

        pFilePathMB = (char*)drwav__malloc_from_callbacks(lenMB + 1, &allocationCallbacks);
        if (pFilePathMB == NULL) {
            return NULL;
        }

        pFilePathTemp = pFilePath;
        DRWAV_ZERO_MEMORY(&mbs, sizeof(mbs));
        wcsrtombs(pFilePathMB, &pFilePathTemp, lenMB + 1, &mbs);

        DRWAV_ZERO_MEMORY(&mbs, sizeof(mbs));
        wcsrtombs(pOpenModeMB, &pOpenModeMBTemp, sizeof(pOpenModeMB), &mbs);

        pFile = fopen(pFilePathMB, pOpenModeMB);

        drwav__free_from_callbacks(pFilePathMB, &allocationCallbacks);
    }
#endif

    return pFile;
}


static size_t drwav__on_read_stdio(void* pUserData, void* pBufferOut, size_t bytesToRead)
{
    return fread(pBufferOut, 1, bytesToRead, (FILE*)pUserData);
}

static size_t drwav__on_write_stdio(void* pUserData, const void* pData, size_t bytesToWrite)
{
    return fwrite(pData, 1, bytesToWrite, (FILE*)pUserData);
}

static drwav_bool32 drwav__on_seek_stdio(void* pUserData, int offset, drwav_seek_origin origin)
{
    return fseek((FILE*)pUserData, offset, (origin == drwav_seek_origin_current) ? SEEK_CUR : SEEK_SET) == 0;
}

drwav_bool32 drwav_init_file(drwav* pWav, const char* filename, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_ex(pWav, filename, NULL, NULL, 0, pAllocationCallbacks);
}


drwav_bool32 drwav_init_file__internal_FILE(drwav* pWav, FILE* pFile, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (!drwav_preinit(pWav, drwav__on_read_stdio, drwav__on_seek_stdio, (void*)pFile, pAllocationCallbacks)) {
        fclose(pFile);
        return DRWAV_FALSE;
    }

    return drwav_init__internal(pWav, onChunk, pChunkUserData, flags);
}

drwav_bool32 drwav_init_file_ex(drwav* pWav, const char* filename, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile = drwav_fopen(filename, "rb");
    if (pFile == NULL) {
        return DRWAV_FALSE;
    }

    /* This takes ownership of the FILE* object. */
    return drwav_init_file__internal_FILE(pWav, pFile, onChunk, pChunkUserData, flags, pAllocationCallbacks);
}

drwav_bool32 drwav_init_file_w(drwav* pWav, const wchar_t* filename, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_ex_w(pWav, filename, NULL, NULL, 0, pAllocationCallbacks);
}

drwav_bool32 drwav_init_file_ex_w(drwav* pWav, const wchar_t* filename, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile = drwav_wfopen(filename, L"rb", pAllocationCallbacks);
    if (pFile == NULL) {
        return DRWAV_FALSE;
    }

    /* This takes ownership of the FILE* object. */
    return drwav_init_file__internal_FILE(pWav, pFile, onChunk, pChunkUserData, flags, pAllocationCallbacks);
}


drwav_bool32 drwav_init_file_write__internal_FILE(drwav* pWav, FILE* pFile, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_bool32 isSequential, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (!drwav_preinit_write(pWav, pFormat, isSequential, drwav__on_write_stdio, drwav__on_seek_stdio, (void*)pFile, pAllocationCallbacks)) {
        fclose(pFile);
        return DRWAV_FALSE;
    }

    return drwav_init_write__internal(pWav, pFormat, totalSampleCount);
}

drwav_bool32 drwav_init_file_write__internal(drwav* pWav, const char* filename, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_bool32 isSequential, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile = drwav_fopen(filename, "wb");
    if (pFile == NULL) {
        return DRWAV_FALSE;
    }

    /* This takes ownership of the FILE* object. */
    return drwav_init_file_write__internal_FILE(pWav, pFile, pFormat, totalSampleCount, isSequential, pAllocationCallbacks);
}

drwav_bool32 drwav_init_file_write_w__internal(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_bool32 isSequential, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    FILE* pFile = drwav_wfopen(filename, L"wb", pAllocationCallbacks);
    if (pFile == NULL) {
        return DRWAV_FALSE;
    }

    /* This takes ownership of the FILE* object. */
    return drwav_init_file_write__internal_FILE(pWav, pFile, pFormat, totalSampleCount, isSequential, pAllocationCallbacks);
}

drwav_bool32 drwav_init_file_write(drwav* pWav, const char* filename, const drwav_data_format* pFormat, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_write__internal(pWav, filename, pFormat, 0, DRWAV_FALSE, pAllocationCallbacks);
}

drwav_bool32 drwav_init_file_write_sequential(drwav* pWav, const char* filename, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_write__internal(pWav, filename, pFormat, totalSampleCount, DRWAV_TRUE, pAllocationCallbacks);
}

drwav_bool32 drwav_init_file_write_sequential_pcm_frames(drwav* pWav, const char* filename, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pFormat == NULL) {
        return DRWAV_FALSE;
    }

    return drwav_init_file_write_sequential(pWav, filename, pFormat, totalPCMFrameCount*pFormat->channels, pAllocationCallbacks);
}

drwav_bool32 drwav_init_file_write_w(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_write_w__internal(pWav, filename, pFormat, 0, DRWAV_FALSE, pAllocationCallbacks);
}

drwav_bool32 drwav_init_file_write_sequential_w(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_file_write_w__internal(pWav, filename, pFormat, totalSampleCount, DRWAV_TRUE, pAllocationCallbacks);
}

drwav_bool32 drwav_init_file_write_sequential_pcm_frames_w(drwav* pWav, const wchar_t* filename, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pFormat == NULL) {
        return DRWAV_FALSE;
    }

    return drwav_init_file_write_sequential_w(pWav, filename, pFormat, totalPCMFrameCount*pFormat->channels, pAllocationCallbacks);
}
#endif  /* DR_WAV_NO_STDIO */


static size_t drwav__on_read_memory(void* pUserData, void* pBufferOut, size_t bytesToRead)
{
    drwav* pWav = (drwav*)pUserData;
    size_t bytesRemaining;

    DRWAV_ASSERT(pWav != NULL);
    DRWAV_ASSERT(pWav->memoryStream.dataSize >= pWav->memoryStream.currentReadPos);

    bytesRemaining = pWav->memoryStream.dataSize - pWav->memoryStream.currentReadPos;
    if (bytesToRead > bytesRemaining) {
        bytesToRead = bytesRemaining;
    }

    if (bytesToRead > 0) {
        DRWAV_COPY_MEMORY(pBufferOut, pWav->memoryStream.data + pWav->memoryStream.currentReadPos, bytesToRead);
        pWav->memoryStream.currentReadPos += bytesToRead;
    }

    return bytesToRead;
}

static drwav_bool32 drwav__on_seek_memory(void* pUserData, int offset, drwav_seek_origin origin)
{
    drwav* pWav = (drwav*)pUserData;
    DRWAV_ASSERT(pWav != NULL);

    if (origin == drwav_seek_origin_current) {
        if (offset > 0) {
            if (pWav->memoryStream.currentReadPos + offset > pWav->memoryStream.dataSize) {
                return DRWAV_FALSE; /* Trying to seek too far forward. */
            }
        } else {
            if (pWav->memoryStream.currentReadPos < (size_t)-offset) {
                return DRWAV_FALSE; /* Trying to seek too far backwards. */
            }
        }

        /* This will never underflow thanks to the clamps above. */
        pWav->memoryStream.currentReadPos += offset;
    } else {
        if ((drwav_uint32)offset <= pWav->memoryStream.dataSize) {
            pWav->memoryStream.currentReadPos = offset;
        } else {
            return DRWAV_FALSE; /* Trying to seek too far forward. */
        }
    }
    
    return DRWAV_TRUE;
}

static size_t drwav__on_write_memory(void* pUserData, const void* pDataIn, size_t bytesToWrite)
{
    drwav* pWav = (drwav*)pUserData;
    size_t bytesRemaining;

    DRWAV_ASSERT(pWav != NULL);
    DRWAV_ASSERT(pWav->memoryStreamWrite.dataCapacity >= pWav->memoryStreamWrite.currentWritePos);

    bytesRemaining = pWav->memoryStreamWrite.dataCapacity - pWav->memoryStreamWrite.currentWritePos;
    if (bytesRemaining < bytesToWrite) {
        /* Need to reallocate. */
        void* pNewData;
        size_t newDataCapacity = (pWav->memoryStreamWrite.dataCapacity == 0) ? 256 : pWav->memoryStreamWrite.dataCapacity * 2;

        /* If doubling wasn't enough, just make it the minimum required size to write the data. */
        if ((newDataCapacity - pWav->memoryStreamWrite.currentWritePos) < bytesToWrite) {
            newDataCapacity = pWav->memoryStreamWrite.currentWritePos + bytesToWrite;
        }

        pNewData = drwav__realloc_from_callbacks(*pWav->memoryStreamWrite.ppData, newDataCapacity, pWav->memoryStreamWrite.dataCapacity, &pWav->allocationCallbacks);
        if (pNewData == NULL) {
            return 0;
        }

        *pWav->memoryStreamWrite.ppData = pNewData;
        pWav->memoryStreamWrite.dataCapacity = newDataCapacity;
    }

    DRWAV_COPY_MEMORY(((drwav_uint8*)(*pWav->memoryStreamWrite.ppData)) + pWav->memoryStreamWrite.currentWritePos, pDataIn, bytesToWrite);

    pWav->memoryStreamWrite.currentWritePos += bytesToWrite;
    if (pWav->memoryStreamWrite.dataSize < pWav->memoryStreamWrite.currentWritePos) {
        pWav->memoryStreamWrite.dataSize = pWav->memoryStreamWrite.currentWritePos;
    }

    *pWav->memoryStreamWrite.pDataSize = pWav->memoryStreamWrite.dataSize;

    return bytesToWrite;
}

static drwav_bool32 drwav__on_seek_memory_write(void* pUserData, int offset, drwav_seek_origin origin)
{
    drwav* pWav = (drwav*)pUserData;
    DRWAV_ASSERT(pWav != NULL);

    if (origin == drwav_seek_origin_current) {
        if (offset > 0) {
            if (pWav->memoryStreamWrite.currentWritePos + offset > pWav->memoryStreamWrite.dataSize) {
                offset = (int)(pWav->memoryStreamWrite.dataSize - pWav->memoryStreamWrite.currentWritePos);  /* Trying to seek too far forward. */
            }
        } else {
            if (pWav->memoryStreamWrite.currentWritePos < (size_t)-offset) {
                offset = -(int)pWav->memoryStreamWrite.currentWritePos;  /* Trying to seek too far backwards. */
            }
        }

        /* This will never underflow thanks to the clamps above. */
        pWav->memoryStreamWrite.currentWritePos += offset;
    } else {
        if ((drwav_uint32)offset <= pWav->memoryStreamWrite.dataSize) {
            pWav->memoryStreamWrite.currentWritePos = offset;
        } else {
            pWav->memoryStreamWrite.currentWritePos = pWav->memoryStreamWrite.dataSize;  /* Trying to seek too far forward. */
        }
    }
    
    return DRWAV_TRUE;
}

drwav_bool32 drwav_init_memory(drwav* pWav, const void* data, size_t dataSize, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_memory_ex(pWav, data, dataSize, NULL, NULL, 0, pAllocationCallbacks);
}

drwav_bool32 drwav_init_memory_ex(drwav* pWav, const void* data, size_t dataSize, drwav_chunk_proc onChunk, void* pChunkUserData, drwav_uint32 flags, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (data == NULL || dataSize == 0) {
        return DRWAV_FALSE;
    }

    if (!drwav_preinit(pWav, drwav__on_read_memory, drwav__on_seek_memory, pWav, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    pWav->memoryStream.data = (const unsigned char*)data;
    pWav->memoryStream.dataSize = dataSize;
    pWav->memoryStream.currentReadPos = 0;

    return drwav_init__internal(pWav, onChunk, pChunkUserData, flags);
}


drwav_bool32 drwav_init_memory_write__internal(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, drwav_bool32 isSequential, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (ppData == NULL || pDataSize == NULL) {
        return DRWAV_FALSE;
    }

    *ppData = NULL; /* Important because we're using realloc()! */
    *pDataSize = 0;

    if (!drwav_preinit_write(pWav, pFormat, isSequential, drwav__on_write_memory, drwav__on_seek_memory_write, pWav, pAllocationCallbacks)) {
        return DRWAV_FALSE;
    }

    pWav->memoryStreamWrite.ppData = ppData;
    pWav->memoryStreamWrite.pDataSize = pDataSize;
    pWav->memoryStreamWrite.dataSize = 0;
    pWav->memoryStreamWrite.dataCapacity = 0;
    pWav->memoryStreamWrite.currentWritePos = 0;

    return drwav_init_write__internal(pWav, pFormat, totalSampleCount);
}

drwav_bool32 drwav_init_memory_write(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_memory_write__internal(pWav, ppData, pDataSize, pFormat, 0, DRWAV_FALSE, pAllocationCallbacks);
}

drwav_bool32 drwav_init_memory_write_sequential(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, drwav_uint64 totalSampleCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    return drwav_init_memory_write__internal(pWav, ppData, pDataSize, pFormat, totalSampleCount, DRWAV_TRUE, pAllocationCallbacks);
}

drwav_bool32 drwav_init_memory_write_sequential_pcm_frames(drwav* pWav, void** ppData, size_t* pDataSize, const drwav_data_format* pFormat, drwav_uint64 totalPCMFrameCount, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pFormat == NULL) {
        return DRWAV_FALSE;
    }

    return drwav_init_memory_write_sequential(pWav, ppData, pDataSize, pFormat, totalPCMFrameCount*pFormat->channels, pAllocationCallbacks);
}



drwav_result drwav_uninit(drwav* pWav)
{
    drwav_result result = DRWAV_SUCCESS;

    if (pWav == NULL) {
        return DRWAV_INVALID_ARGS;
    }

    /*
    If the drwav object was opened in write mode we'll need to finalize a few things:
      - Make sure the "data" chunk is aligned to 16-bits for RIFF containers, or 64 bits for W64 containers.
      - Set the size of the "data" chunk.
    */
    if (pWav->onWrite != NULL) {
        drwav_uint32 paddingSize = 0;

        /* Padding. Do not adjust pWav->dataChunkDataSize - this should not include the padding. */
        if (pWav->container == drwav_container_riff) {
            paddingSize = drwav__chunk_padding_size_riff(pWav->dataChunkDataSize);
        } else {
            paddingSize = drwav__chunk_padding_size_w64(pWav->dataChunkDataSize);
        }
        
        if (paddingSize > 0) {
            drwav_uint64 paddingData = 0;
            pWav->onWrite(pWav->pUserData, &paddingData, paddingSize);
        }

        /*
        Chunk sizes. When using sequential mode, these will have been filled in at initialization time. We only need
        to do this when using non-sequential mode.
        */
        if (pWav->onSeek && !pWav->isSequentialWrite) {
            if (pWav->container == drwav_container_riff) {
                /* The "RIFF" chunk size. */
                if (pWav->onSeek(pWav->pUserData, 4, drwav_seek_origin_start)) {
                    drwav_uint32 riffChunkSize = drwav__riff_chunk_size_riff(pWav->dataChunkDataSize);
                    pWav->onWrite(pWav->pUserData, &riffChunkSize, 4);
                }

                /* the "data" chunk size. */
                if (pWav->onSeek(pWav->pUserData, (int)pWav->dataChunkDataPos + 4, drwav_seek_origin_start)) {
                    drwav_uint32 dataChunkSize = drwav__data_chunk_size_riff(pWav->dataChunkDataSize);
                    pWav->onWrite(pWav->pUserData, &dataChunkSize, 4);
                }
            } else {
                /* The "RIFF" chunk size. */
                if (pWav->onSeek(pWav->pUserData, 16, drwav_seek_origin_start)) {
                    drwav_uint64 riffChunkSize = drwav__riff_chunk_size_w64(pWav->dataChunkDataSize);
                    pWav->onWrite(pWav->pUserData, &riffChunkSize, 8);
                }

                /* The "data" chunk size. */
                if (pWav->onSeek(pWav->pUserData, (int)pWav->dataChunkDataPos + 16, drwav_seek_origin_start)) {
                    drwav_uint64 dataChunkSize = drwav__data_chunk_size_w64(pWav->dataChunkDataSize);
                    pWav->onWrite(pWav->pUserData, &dataChunkSize, 8);
                }
            }
        }

        /* Validation for sequential mode. */
        if (pWav->isSequentialWrite) {
            if (pWav->dataChunkDataSize != pWav->dataChunkDataSizeTargetWrite) {
                result = DRWAV_INVALID_FILE;
            }
        }
    }

#ifndef DR_WAV_NO_STDIO
    /*
    If we opened the file with drwav_open_file() we will want to close the file handle. We can know whether or not drwav_open_file()
    was used by looking at the onRead and onSeek callbacks.
    */
    if (pWav->onRead == drwav__on_read_stdio || pWav->onWrite == drwav__on_write_stdio) {
        fclose((FILE*)pWav->pUserData);
    }
#endif

    return result;
}



size_t drwav_read_raw(drwav* pWav, size_t bytesToRead, void* pBufferOut)
{
    size_t bytesRead;

    if (pWav == NULL || bytesToRead == 0 || pBufferOut == NULL) {
        return 0;
    }

    if (bytesToRead > pWav->bytesRemaining) {
        bytesToRead = (size_t)pWav->bytesRemaining;
    }

    bytesRead = pWav->onRead(pWav->pUserData, pBufferOut, bytesToRead);

    pWav->bytesRemaining -= bytesRead;
    return bytesRead;
}



drwav_uint64 drwav_read_pcm_frames_le(drwav* pWav, drwav_uint64 framesToRead, void* pBufferOut)
{
    drwav_uint32 bytesPerFrame;

    if (pWav == NULL || framesToRead == 0 || pBufferOut == NULL) {
        return 0;
    }

    /* Cannot use this function for compressed formats. */
    if (drwav__is_compressed_format_tag(pWav->translatedFormatTag)) {
        return 0;
    }

    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    /* Don't try to read more samples than can potentially fit in the output buffer. */
    if (framesToRead * bytesPerFrame > DRWAV_SIZE_MAX) {
        framesToRead = DRWAV_SIZE_MAX / bytesPerFrame;
    }

    return drwav_read_raw(pWav, (size_t)(framesToRead * bytesPerFrame), pBufferOut) / bytesPerFrame;
}

drwav_uint64 drwav_read_pcm_frames_be(drwav* pWav, drwav_uint64 framesToRead, void* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_le(pWav, framesToRead, pBufferOut);
    drwav__bswap_samples(pBufferOut, framesRead*pWav->channels, drwav_get_bytes_per_pcm_frame(pWav)/pWav->channels, pWav->translatedFormatTag);

    return framesRead;
}

drwav_uint64 drwav_read_pcm_frames(drwav* pWav, drwav_uint64 framesToRead, void* pBufferOut)
{
    if (drwav__is_little_endian()) {
        return drwav_read_pcm_frames_le(pWav, framesToRead, pBufferOut);
    } else {
        return drwav_read_pcm_frames_be(pWav, framesToRead, pBufferOut);
    }
}



drwav_bool32 drwav_seek_to_first_pcm_frame(drwav* pWav)
{
    if (pWav->onWrite != NULL) {
        return DRWAV_FALSE; /* No seeking in write mode. */
    }

    if (!pWav->onSeek(pWav->pUserData, (int)pWav->dataChunkDataPos, drwav_seek_origin_start)) {
        return DRWAV_FALSE;
    }

    if (drwav__is_compressed_format_tag(pWav->translatedFormatTag)) {
        pWav->compressed.iCurrentPCMFrame = 0;
    }
    
    pWav->bytesRemaining = pWav->dataChunkDataSize;
    return DRWAV_TRUE;
}

drwav_bool32 drwav_seek_to_pcm_frame(drwav* pWav, drwav_uint64 targetFrameIndex)
{
    /* Seeking should be compatible with wave files > 2GB. */

    if (pWav == NULL || pWav->onSeek == NULL) {
        return DRWAV_FALSE;
    }

    /* No seeking in write mode. */
    if (pWav->onWrite != NULL) {
        return DRWAV_FALSE;
    }

    /* If there are no samples, just return DRWAV_TRUE without doing anything. */
    if (pWav->totalPCMFrameCount == 0) {
        return DRWAV_TRUE;
    }

    /* Make sure the sample is clamped. */
    if (targetFrameIndex >= pWav->totalPCMFrameCount) {
        targetFrameIndex  = pWav->totalPCMFrameCount - 1;
    }

    /*
    For compressed formats we just use a slow generic seek. If we are seeking forward we just seek forward. If we are going backwards we need
    to seek back to the start.
    */
    if (drwav__is_compressed_format_tag(pWav->translatedFormatTag)) {
        /* TODO: This can be optimized. */
        
        /*
        If we're seeking forward it's simple - just keep reading samples until we hit the sample we're requesting. If we're seeking backwards,
        we first need to seek back to the start and then just do the same thing as a forward seek.
        */
        if (targetFrameIndex < pWav->compressed.iCurrentPCMFrame) {
            if (!drwav_seek_to_first_pcm_frame(pWav)) {
                return DRWAV_FALSE;
            }
        }

        if (targetFrameIndex > pWav->compressed.iCurrentPCMFrame) {
            drwav_uint64 offsetInFrames = targetFrameIndex - pWav->compressed.iCurrentPCMFrame;

            drwav_int16 devnull[2048];
            while (offsetInFrames > 0) {
                drwav_uint64 framesRead = 0;
                drwav_uint64 framesToRead = offsetInFrames;
                if (framesToRead > drwav_countof(devnull)/pWav->channels) {
                    framesToRead = drwav_countof(devnull)/pWav->channels;
                }

                if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
                    framesRead = drwav_read_pcm_frames_s16__msadpcm(pWav, framesToRead, devnull);
                } else if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
                    framesRead = drwav_read_pcm_frames_s16__ima(pWav, framesToRead, devnull);
                } else {
                    assert(DRWAV_FALSE);    /* If this assertion is triggered it means I've implemented a new compressed format but forgot to add a branch for it here. */
                }

                if (framesRead != framesToRead) {
                    return DRWAV_FALSE;
                }

                offsetInFrames -= framesRead;
            }
        }
    } else {
        drwav_uint64 totalSizeInBytes;
        drwav_uint64 currentBytePos;
        drwav_uint64 targetBytePos;
        drwav_uint64 offset;

        totalSizeInBytes = pWav->totalPCMFrameCount * drwav_get_bytes_per_pcm_frame(pWav);
        DRWAV_ASSERT(totalSizeInBytes >= pWav->bytesRemaining);

        currentBytePos = totalSizeInBytes - pWav->bytesRemaining;
        targetBytePos  = targetFrameIndex * drwav_get_bytes_per_pcm_frame(pWav);

        if (currentBytePos < targetBytePos) {
            /* Offset forwards. */
            offset = (targetBytePos - currentBytePos);
        } else {
            /* Offset backwards. */
            if (!drwav_seek_to_first_pcm_frame(pWav)) {
                return DRWAV_FALSE;
            }
            offset = targetBytePos;
        }

        while (offset > 0) {
            int offset32 = ((offset > INT_MAX) ? INT_MAX : (int)offset);
            if (!pWav->onSeek(pWav->pUserData, offset32, drwav_seek_origin_current)) {
                return DRWAV_FALSE;
            }

            pWav->bytesRemaining -= offset32;
            offset -= offset32;
        }
    }

    return DRWAV_TRUE;
}


size_t drwav_write_raw(drwav* pWav, size_t bytesToWrite, const void* pData)
{
    size_t bytesWritten;

    if (pWav == NULL || bytesToWrite == 0 || pData == NULL) {
        return 0;
    }

    bytesWritten = pWav->onWrite(pWav->pUserData, pData, bytesToWrite);
    pWav->dataChunkDataSize += bytesWritten;

    return bytesWritten;
}


drwav_uint64 drwav_write_pcm_frames_le(drwav* pWav, drwav_uint64 framesToWrite, const void* pData)
{
    drwav_uint64 bytesToWrite;
    drwav_uint64 bytesWritten;
    const drwav_uint8* pRunningData;

    if (pWav == NULL || framesToWrite == 0 || pData == NULL) {
        return 0;
    }

    bytesToWrite = ((framesToWrite * pWav->channels * pWav->bitsPerSample) / 8);
    if (bytesToWrite > DRWAV_SIZE_MAX) {
        return 0;
    }

    bytesWritten = 0;
    pRunningData = (const drwav_uint8*)pData;

    while (bytesToWrite > 0) {
        size_t bytesJustWritten;
        drwav_uint64 bytesToWriteThisIteration;

        bytesToWriteThisIteration = bytesToWrite;
        DRWAV_ASSERT(bytesToWriteThisIteration <= DRWAV_SIZE_MAX);  /* <-- This is checked above. */

        bytesJustWritten = drwav_write_raw(pWav, (size_t)bytesToWriteThisIteration, pRunningData);
        if (bytesJustWritten == 0) {
            break;
        }

        bytesToWrite -= bytesJustWritten;
        bytesWritten += bytesJustWritten;
        pRunningData += bytesJustWritten;
    }

    return (bytesWritten * 8) / pWav->bitsPerSample / pWav->channels;
}

drwav_uint64 drwav_write_pcm_frames_be(drwav* pWav, drwav_uint64 framesToWrite, const void* pData)
{
    drwav_uint64 bytesToWrite;
    drwav_uint64 bytesWritten;
    drwav_uint32 bytesPerSample;
    const drwav_uint8* pRunningData;

    if (pWav == NULL || framesToWrite == 0 || pData == NULL) {
        return 0;
    }

    bytesToWrite = ((framesToWrite * pWav->channels * pWav->bitsPerSample) / 8);
    if (bytesToWrite > DRWAV_SIZE_MAX) {
        return 0;
    }

    bytesWritten = 0;
    pRunningData = (const drwav_uint8*)pData;

    bytesPerSample = drwav_get_bytes_per_pcm_frame(pWav) / pWav->channels;
    
    while (bytesToWrite > 0) {
        drwav_uint8 temp[4096];
        drwav_uint32 sampleCount;
        size_t bytesJustWritten;
        drwav_uint64 bytesToWriteThisIteration;

        bytesToWriteThisIteration = bytesToWrite;
        DRWAV_ASSERT(bytesToWriteThisIteration <= DRWAV_SIZE_MAX);  /* <-- This is checked above. */

        /*
        WAV files are always little-endian. We need to byte swap on big-endian architectures. Since our input buffer is read-only we need
        to use an intermediary buffer for the conversion.
        */
        sampleCount = sizeof(temp)/bytesPerSample;

        if (bytesToWriteThisIteration > ((drwav_uint64)sampleCount)*bytesPerSample) {
            bytesToWriteThisIteration = ((drwav_uint64)sampleCount)*bytesPerSample;
        }

        DRWAV_COPY_MEMORY(temp, pRunningData, (size_t)bytesToWriteThisIteration);
        drwav__bswap_samples(temp, sampleCount, bytesPerSample, pWav->translatedFormatTag);

        bytesJustWritten = drwav_write_raw(pWav, (size_t)bytesToWriteThisIteration, temp);
        if (bytesJustWritten == 0) {
            break;
        }

        bytesToWrite -= bytesJustWritten;
        bytesWritten += bytesJustWritten;
        pRunningData += bytesJustWritten;
    }

    return (bytesWritten * 8) / pWav->bitsPerSample / pWav->channels;
}

drwav_uint64 drwav_write_pcm_frames(drwav* pWav, drwav_uint64 framesToWrite, const void* pData)
{
    if (drwav__is_little_endian()) {
        return drwav_write_pcm_frames_le(pWav, framesToWrite, pData);
    } else {
        return drwav_write_pcm_frames_be(pWav, framesToWrite, pData);
    }
}


drwav_uint64 drwav_read_pcm_frames_s16__msadpcm(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead = 0;

    DRWAV_ASSERT(pWav != NULL);
    DRWAV_ASSERT(framesToRead > 0);
    DRWAV_ASSERT(pBufferOut != NULL);

    /* TODO: Lots of room for optimization here. */

    while (framesToRead > 0 && pWav->compressed.iCurrentPCMFrame < pWav->totalPCMFrameCount) {
        /* If there are no cached frames we need to load a new block. */
        if (pWav->msadpcm.cachedFrameCount == 0 && pWav->msadpcm.bytesRemainingInBlock == 0) {
            if (pWav->channels == 1) {
                /* Mono. */
                drwav_uint8 header[7];
                if (pWav->onRead(pWav->pUserData, header, sizeof(header)) != sizeof(header)) {
                    return totalFramesRead;
                }
                pWav->msadpcm.bytesRemainingInBlock = pWav->fmt.blockAlign - sizeof(header);

                pWav->msadpcm.predictor[0]     = header[0];
                pWav->msadpcm.delta[0]         = drwav__bytes_to_s16(header + 1);
                pWav->msadpcm.prevFrames[0][1] = (drwav_int32)drwav__bytes_to_s16(header + 3);
                pWav->msadpcm.prevFrames[0][0] = (drwav_int32)drwav__bytes_to_s16(header + 5);
                pWav->msadpcm.cachedFrames[2]  = pWav->msadpcm.prevFrames[0][0];
                pWav->msadpcm.cachedFrames[3]  = pWav->msadpcm.prevFrames[0][1];
                pWav->msadpcm.cachedFrameCount = 2;
            } else {
                /* Stereo. */
                drwav_uint8 header[14];
                if (pWav->onRead(pWav->pUserData, header, sizeof(header)) != sizeof(header)) {
                    return totalFramesRead;
                }
                pWav->msadpcm.bytesRemainingInBlock = pWav->fmt.blockAlign - sizeof(header);

                pWav->msadpcm.predictor[0] = header[0];
                pWav->msadpcm.predictor[1] = header[1];
                pWav->msadpcm.delta[0] = drwav__bytes_to_s16(header + 2);
                pWav->msadpcm.delta[1] = drwav__bytes_to_s16(header + 4);
                pWav->msadpcm.prevFrames[0][1] = (drwav_int32)drwav__bytes_to_s16(header + 6);
                pWav->msadpcm.prevFrames[1][1] = (drwav_int32)drwav__bytes_to_s16(header + 8);
                pWav->msadpcm.prevFrames[0][0] = (drwav_int32)drwav__bytes_to_s16(header + 10);
                pWav->msadpcm.prevFrames[1][0] = (drwav_int32)drwav__bytes_to_s16(header + 12);

                pWav->msadpcm.cachedFrames[0] = pWav->msadpcm.prevFrames[0][0];
                pWav->msadpcm.cachedFrames[1] = pWav->msadpcm.prevFrames[1][0];
                pWav->msadpcm.cachedFrames[2] = pWav->msadpcm.prevFrames[0][1];
                pWav->msadpcm.cachedFrames[3] = pWav->msadpcm.prevFrames[1][1];
                pWav->msadpcm.cachedFrameCount = 2;
            }
        }

        /* Output anything that's cached. */
        while (framesToRead > 0 && pWav->msadpcm.cachedFrameCount > 0 && pWav->compressed.iCurrentPCMFrame < pWav->totalPCMFrameCount) {
            drwav_uint32 iSample = 0;
            for (iSample = 0; iSample < pWav->channels; iSample += 1) {
                pBufferOut[iSample] = (drwav_int16)pWav->msadpcm.cachedFrames[(drwav_countof(pWav->msadpcm.cachedFrames) - (pWav->msadpcm.cachedFrameCount*pWav->channels)) + iSample];
            }

            pBufferOut      += pWav->channels;
            framesToRead    -= 1;
            totalFramesRead += 1;
            pWav->compressed.iCurrentPCMFrame += 1;
            pWav->msadpcm.cachedFrameCount -= 1;
        }

        if (framesToRead == 0) {
            return totalFramesRead;
        }


        /*
        If there's nothing left in the cache, just go ahead and load more. If there's nothing left to load in the current block we just continue to the next
        loop iteration which will trigger the loading of a new block.
        */
        if (pWav->msadpcm.cachedFrameCount == 0) {
            if (pWav->msadpcm.bytesRemainingInBlock == 0) {
                continue;
            } else {
                static drwav_int32 adaptationTable[] = { 
                    230, 230, 230, 230, 307, 409, 512, 614, 
                    768, 614, 512, 409, 307, 230, 230, 230 
                };
                static drwav_int32 coeff1Table[] = { 256, 512, 0, 192, 240, 460,  392 };
                static drwav_int32 coeff2Table[] = { 0,  -256, 0, 64,  0,  -208, -232 };

                drwav_uint8 nibbles;
                drwav_int32 nibble0;
                drwav_int32 nibble1;

                if (pWav->onRead(pWav->pUserData, &nibbles, 1) != 1) {
                    return totalFramesRead;
                }
                pWav->msadpcm.bytesRemainingInBlock -= 1;

                /* TODO: Optimize away these if statements. */
                nibble0 = ((nibbles & 0xF0) >> 4); if ((nibbles & 0x80)) { nibble0 |= 0xFFFFFFF0UL; }
                nibble1 = ((nibbles & 0x0F) >> 0); if ((nibbles & 0x08)) { nibble1 |= 0xFFFFFFF0UL; }

                if (pWav->channels == 1) {
                    /* Mono. */
                    drwav_int32 newSample0;
                    drwav_int32 newSample1;

                    newSample0  = ((pWav->msadpcm.prevFrames[0][1] * coeff1Table[pWav->msadpcm.predictor[0]]) + (pWav->msadpcm.prevFrames[0][0] * coeff2Table[pWav->msadpcm.predictor[0]])) >> 8;
                    newSample0 += nibble0 * pWav->msadpcm.delta[0];
                    newSample0  = drwav_clamp(newSample0, -32768, 32767);

                    pWav->msadpcm.delta[0] = (adaptationTable[((nibbles & 0xF0) >> 4)] * pWav->msadpcm.delta[0]) >> 8;
                    if (pWav->msadpcm.delta[0] < 16) {
                        pWav->msadpcm.delta[0] = 16;
                    }

                    pWav->msadpcm.prevFrames[0][0] = pWav->msadpcm.prevFrames[0][1];
                    pWav->msadpcm.prevFrames[0][1] = newSample0;


                    newSample1  = ((pWav->msadpcm.prevFrames[0][1] * coeff1Table[pWav->msadpcm.predictor[0]]) + (pWav->msadpcm.prevFrames[0][0] * coeff2Table[pWav->msadpcm.predictor[0]])) >> 8;
                    newSample1 += nibble1 * pWav->msadpcm.delta[0];
                    newSample1  = drwav_clamp(newSample1, -32768, 32767);

                    pWav->msadpcm.delta[0] = (adaptationTable[((nibbles & 0x0F) >> 0)] * pWav->msadpcm.delta[0]) >> 8;
                    if (pWav->msadpcm.delta[0] < 16) {
                        pWav->msadpcm.delta[0] = 16;
                    }

                    pWav->msadpcm.prevFrames[0][0] = pWav->msadpcm.prevFrames[0][1];
                    pWav->msadpcm.prevFrames[0][1] = newSample1;


                    pWav->msadpcm.cachedFrames[2] = newSample0;
                    pWav->msadpcm.cachedFrames[3] = newSample1;
                    pWav->msadpcm.cachedFrameCount = 2;
                } else {
                    /* Stereo. */
                    drwav_int32 newSample0;
                    drwav_int32 newSample1;

                    /* Left. */
                    newSample0  = ((pWav->msadpcm.prevFrames[0][1] * coeff1Table[pWav->msadpcm.predictor[0]]) + (pWav->msadpcm.prevFrames[0][0] * coeff2Table[pWav->msadpcm.predictor[0]])) >> 8;
                    newSample0 += nibble0 * pWav->msadpcm.delta[0];
                    newSample0  = drwav_clamp(newSample0, -32768, 32767);

                    pWav->msadpcm.delta[0] = (adaptationTable[((nibbles & 0xF0) >> 4)] * pWav->msadpcm.delta[0]) >> 8;
                    if (pWav->msadpcm.delta[0] < 16) {
                        pWav->msadpcm.delta[0] = 16;
                    }

                    pWav->msadpcm.prevFrames[0][0] = pWav->msadpcm.prevFrames[0][1];
                    pWav->msadpcm.prevFrames[0][1] = newSample0;


                    /* Right. */
                    newSample1  = ((pWav->msadpcm.prevFrames[1][1] * coeff1Table[pWav->msadpcm.predictor[1]]) + (pWav->msadpcm.prevFrames[1][0] * coeff2Table[pWav->msadpcm.predictor[1]])) >> 8;
                    newSample1 += nibble1 * pWav->msadpcm.delta[1];
                    newSample1  = drwav_clamp(newSample1, -32768, 32767);

                    pWav->msadpcm.delta[1] = (adaptationTable[((nibbles & 0x0F) >> 0)] * pWav->msadpcm.delta[1]) >> 8;
                    if (pWav->msadpcm.delta[1] < 16) {
                        pWav->msadpcm.delta[1] = 16;
                    }

                    pWav->msadpcm.prevFrames[1][0] = pWav->msadpcm.prevFrames[1][1];
                    pWav->msadpcm.prevFrames[1][1] = newSample1;

                    pWav->msadpcm.cachedFrames[2] = newSample0;
                    pWav->msadpcm.cachedFrames[3] = newSample1;
                    pWav->msadpcm.cachedFrameCount = 1;
                }
            }
        }
    }

    return totalFramesRead;
}


drwav_uint64 drwav_read_pcm_frames_s16__ima(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead = 0;

    DRWAV_ASSERT(pWav != NULL);
    DRWAV_ASSERT(framesToRead > 0);
    DRWAV_ASSERT(pBufferOut != NULL);

    /* TODO: Lots of room for optimization here. */

    while (framesToRead > 0 && pWav->compressed.iCurrentPCMFrame < pWav->totalPCMFrameCount) {
        /* If there are no cached samples we need to load a new block. */
        if (pWav->ima.cachedFrameCount == 0 && pWav->ima.bytesRemainingInBlock == 0) {
            if (pWav->channels == 1) {
                /* Mono. */
                drwav_uint8 header[4];
                if (pWav->onRead(pWav->pUserData, header, sizeof(header)) != sizeof(header)) {
                    return totalFramesRead;
                }
                pWav->ima.bytesRemainingInBlock = pWav->fmt.blockAlign - sizeof(header);

                pWav->ima.predictor[0] = drwav__bytes_to_s16(header + 0);
                pWav->ima.stepIndex[0] = header[2];
                pWav->ima.cachedFrames[drwav_countof(pWav->ima.cachedFrames) - 1] = pWav->ima.predictor[0];
                pWav->ima.cachedFrameCount = 1;
            } else {
                /* Stereo. */
                drwav_uint8 header[8];
                if (pWav->onRead(pWav->pUserData, header, sizeof(header)) != sizeof(header)) {
                    return totalFramesRead;
                }
                pWav->ima.bytesRemainingInBlock = pWav->fmt.blockAlign - sizeof(header);

                pWav->ima.predictor[0] = drwav__bytes_to_s16(header + 0);
                pWav->ima.stepIndex[0] = header[2];
                pWav->ima.predictor[1] = drwav__bytes_to_s16(header + 4);
                pWav->ima.stepIndex[1] = header[6];

                pWav->ima.cachedFrames[drwav_countof(pWav->ima.cachedFrames) - 2] = pWav->ima.predictor[0];
                pWav->ima.cachedFrames[drwav_countof(pWav->ima.cachedFrames) - 1] = pWav->ima.predictor[1];
                pWav->ima.cachedFrameCount = 1;
            }
        }

        /* Output anything that's cached. */
        while (framesToRead > 0 && pWav->ima.cachedFrameCount > 0 && pWav->compressed.iCurrentPCMFrame < pWav->totalPCMFrameCount) {
            drwav_uint32 iSample;
            for (iSample = 0; iSample < pWav->channels; iSample += 1) {
                pBufferOut[iSample] = (drwav_int16)pWav->ima.cachedFrames[(drwav_countof(pWav->ima.cachedFrames) - (pWav->ima.cachedFrameCount*pWav->channels)) + iSample];
            }

            pBufferOut      += pWav->channels;
            framesToRead    -= 1;
            totalFramesRead += 1;
            pWav->compressed.iCurrentPCMFrame += 1;
            pWav->ima.cachedFrameCount -= 1;
        }

        if (framesToRead == 0) {
            return totalFramesRead;
        }

        /*
        If there's nothing left in the cache, just go ahead and load more. If there's nothing left to load in the current block we just continue to the next
        loop iteration which will trigger the loading of a new block.
        */
        if (pWav->ima.cachedFrameCount == 0) {
            if (pWav->ima.bytesRemainingInBlock == 0) {
                continue;
            } else {
                static drwav_int32 indexTable[16] = {
                    -1, -1, -1, -1, 2, 4, 6, 8,
                    -1, -1, -1, -1, 2, 4, 6, 8
                };

                static drwav_int32 stepTable[89] = {
                    7,     8,     9,     10,    11,    12,    13,    14,    16,    17, 
                    19,    21,    23,    25,    28,    31,    34,    37,    41,    45, 
                    50,    55,    60,    66,    73,    80,    88,    97,    107,   118, 
                    130,   143,   157,   173,   190,   209,   230,   253,   279,   307,
                    337,   371,   408,   449,   494,   544,   598,   658,   724,   796,
                    876,   963,   1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066, 
                    2272,  2499,  2749,  3024,  3327,  3660,  4026,  4428,  4871,  5358,
                    5894,  6484,  7132,  7845,  8630,  9493,  10442, 11487, 12635, 13899, 
                    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767 
                };

                drwav_uint32 iChannel;

                /*
                From what I can tell with stereo streams, it looks like every 4 bytes (8 samples) is for one channel. So it goes 4 bytes for the
                left channel, 4 bytes for the right channel.
                */
                pWav->ima.cachedFrameCount = 8;
                for (iChannel = 0; iChannel < pWav->channels; ++iChannel) {
                    drwav_uint32 iByte;
                    drwav_uint8 nibbles[4];
                    if (pWav->onRead(pWav->pUserData, &nibbles, 4) != 4) {
                        pWav->ima.cachedFrameCount = 0;
                        return totalFramesRead;
                    }
                    pWav->ima.bytesRemainingInBlock -= 4;

                    for (iByte = 0; iByte < 4; ++iByte) {
                        drwav_uint8 nibble0 = ((nibbles[iByte] & 0x0F) >> 0);
                        drwav_uint8 nibble1 = ((nibbles[iByte] & 0xF0) >> 4);

                        drwav_int32 step      = stepTable[pWav->ima.stepIndex[iChannel]];
                        drwav_int32 predictor = pWav->ima.predictor[iChannel];

                        drwav_int32      diff  = step >> 3;
                        if (nibble0 & 1) diff += step >> 2;
                        if (nibble0 & 2) diff += step >> 1;
                        if (nibble0 & 4) diff += step;
                        if (nibble0 & 8) diff  = -diff;

                        predictor = drwav_clamp(predictor + diff, -32768, 32767);
                        pWav->ima.predictor[iChannel] = predictor;
                        pWav->ima.stepIndex[iChannel] = drwav_clamp(pWav->ima.stepIndex[iChannel] + indexTable[nibble0], 0, (drwav_int32)drwav_countof(stepTable)-1);
                        pWav->ima.cachedFrames[(drwav_countof(pWav->ima.cachedFrames) - (pWav->ima.cachedFrameCount*pWav->channels)) + (iByte*2+0)*pWav->channels + iChannel] = predictor;


                        step      = stepTable[pWav->ima.stepIndex[iChannel]];
                        predictor = pWav->ima.predictor[iChannel];

                                         diff  = step >> 3;
                        if (nibble1 & 1) diff += step >> 2;
                        if (nibble1 & 2) diff += step >> 1;
                        if (nibble1 & 4) diff += step;
                        if (nibble1 & 8) diff  = -diff;

                        predictor = drwav_clamp(predictor + diff, -32768, 32767);
                        pWav->ima.predictor[iChannel] = predictor;
                        pWav->ima.stepIndex[iChannel] = drwav_clamp(pWav->ima.stepIndex[iChannel] + indexTable[nibble1], 0, (drwav_int32)drwav_countof(stepTable)-1);
                        pWav->ima.cachedFrames[(drwav_countof(pWav->ima.cachedFrames) - (pWav->ima.cachedFrameCount*pWav->channels)) + (iByte*2+1)*pWav->channels + iChannel] = predictor;
                    }
                }
            }
        }
    }

    return totalFramesRead;
}


#ifndef DR_WAV_NO_CONVERSION_API
static unsigned short g_drwavAlawTable[256] = {
    0xEA80, 0xEB80, 0xE880, 0xE980, 0xEE80, 0xEF80, 0xEC80, 0xED80, 0xE280, 0xE380, 0xE080, 0xE180, 0xE680, 0xE780, 0xE480, 0xE580, 
    0xF540, 0xF5C0, 0xF440, 0xF4C0, 0xF740, 0xF7C0, 0xF640, 0xF6C0, 0xF140, 0xF1C0, 0xF040, 0xF0C0, 0xF340, 0xF3C0, 0xF240, 0xF2C0, 
    0xAA00, 0xAE00, 0xA200, 0xA600, 0xBA00, 0xBE00, 0xB200, 0xB600, 0x8A00, 0x8E00, 0x8200, 0x8600, 0x9A00, 0x9E00, 0x9200, 0x9600, 
    0xD500, 0xD700, 0xD100, 0xD300, 0xDD00, 0xDF00, 0xD900, 0xDB00, 0xC500, 0xC700, 0xC100, 0xC300, 0xCD00, 0xCF00, 0xC900, 0xCB00, 
    0xFEA8, 0xFEB8, 0xFE88, 0xFE98, 0xFEE8, 0xFEF8, 0xFEC8, 0xFED8, 0xFE28, 0xFE38, 0xFE08, 0xFE18, 0xFE68, 0xFE78, 0xFE48, 0xFE58, 
    0xFFA8, 0xFFB8, 0xFF88, 0xFF98, 0xFFE8, 0xFFF8, 0xFFC8, 0xFFD8, 0xFF28, 0xFF38, 0xFF08, 0xFF18, 0xFF68, 0xFF78, 0xFF48, 0xFF58, 
    0xFAA0, 0xFAE0, 0xFA20, 0xFA60, 0xFBA0, 0xFBE0, 0xFB20, 0xFB60, 0xF8A0, 0xF8E0, 0xF820, 0xF860, 0xF9A0, 0xF9E0, 0xF920, 0xF960, 
    0xFD50, 0xFD70, 0xFD10, 0xFD30, 0xFDD0, 0xFDF0, 0xFD90, 0xFDB0, 0xFC50, 0xFC70, 0xFC10, 0xFC30, 0xFCD0, 0xFCF0, 0xFC90, 0xFCB0, 
    0x1580, 0x1480, 0x1780, 0x1680, 0x1180, 0x1080, 0x1380, 0x1280, 0x1D80, 0x1C80, 0x1F80, 0x1E80, 0x1980, 0x1880, 0x1B80, 0x1A80, 
    0x0AC0, 0x0A40, 0x0BC0, 0x0B40, 0x08C0, 0x0840, 0x09C0, 0x0940, 0x0EC0, 0x0E40, 0x0FC0, 0x0F40, 0x0CC0, 0x0C40, 0x0DC0, 0x0D40, 
    0x5600, 0x5200, 0x5E00, 0x5A00, 0x4600, 0x4200, 0x4E00, 0x4A00, 0x7600, 0x7200, 0x7E00, 0x7A00, 0x6600, 0x6200, 0x6E00, 0x6A00, 
    0x2B00, 0x2900, 0x2F00, 0x2D00, 0x2300, 0x2100, 0x2700, 0x2500, 0x3B00, 0x3900, 0x3F00, 0x3D00, 0x3300, 0x3100, 0x3700, 0x3500, 
    0x0158, 0x0148, 0x0178, 0x0168, 0x0118, 0x0108, 0x0138, 0x0128, 0x01D8, 0x01C8, 0x01F8, 0x01E8, 0x0198, 0x0188, 0x01B8, 0x01A8, 
    0x0058, 0x0048, 0x0078, 0x0068, 0x0018, 0x0008, 0x0038, 0x0028, 0x00D8, 0x00C8, 0x00F8, 0x00E8, 0x0098, 0x0088, 0x00B8, 0x00A8, 
    0x0560, 0x0520, 0x05E0, 0x05A0, 0x0460, 0x0420, 0x04E0, 0x04A0, 0x0760, 0x0720, 0x07E0, 0x07A0, 0x0660, 0x0620, 0x06E0, 0x06A0, 
    0x02B0, 0x0290, 0x02F0, 0x02D0, 0x0230, 0x0210, 0x0270, 0x0250, 0x03B0, 0x0390, 0x03F0, 0x03D0, 0x0330, 0x0310, 0x0370, 0x0350
};

static unsigned short g_drwavMulawTable[256] = {
    0x8284, 0x8684, 0x8A84, 0x8E84, 0x9284, 0x9684, 0x9A84, 0x9E84, 0xA284, 0xA684, 0xAA84, 0xAE84, 0xB284, 0xB684, 0xBA84, 0xBE84, 
    0xC184, 0xC384, 0xC584, 0xC784, 0xC984, 0xCB84, 0xCD84, 0xCF84, 0xD184, 0xD384, 0xD584, 0xD784, 0xD984, 0xDB84, 0xDD84, 0xDF84, 
    0xE104, 0xE204, 0xE304, 0xE404, 0xE504, 0xE604, 0xE704, 0xE804, 0xE904, 0xEA04, 0xEB04, 0xEC04, 0xED04, 0xEE04, 0xEF04, 0xF004, 
    0xF0C4, 0xF144, 0xF1C4, 0xF244, 0xF2C4, 0xF344, 0xF3C4, 0xF444, 0xF4C4, 0xF544, 0xF5C4, 0xF644, 0xF6C4, 0xF744, 0xF7C4, 0xF844, 
    0xF8A4, 0xF8E4, 0xF924, 0xF964, 0xF9A4, 0xF9E4, 0xFA24, 0xFA64, 0xFAA4, 0xFAE4, 0xFB24, 0xFB64, 0xFBA4, 0xFBE4, 0xFC24, 0xFC64, 
    0xFC94, 0xFCB4, 0xFCD4, 0xFCF4, 0xFD14, 0xFD34, 0xFD54, 0xFD74, 0xFD94, 0xFDB4, 0xFDD4, 0xFDF4, 0xFE14, 0xFE34, 0xFE54, 0xFE74, 
    0xFE8C, 0xFE9C, 0xFEAC, 0xFEBC, 0xFECC, 0xFEDC, 0xFEEC, 0xFEFC, 0xFF0C, 0xFF1C, 0xFF2C, 0xFF3C, 0xFF4C, 0xFF5C, 0xFF6C, 0xFF7C, 
    0xFF88, 0xFF90, 0xFF98, 0xFFA0, 0xFFA8, 0xFFB0, 0xFFB8, 0xFFC0, 0xFFC8, 0xFFD0, 0xFFD8, 0xFFE0, 0xFFE8, 0xFFF0, 0xFFF8, 0x0000, 
    0x7D7C, 0x797C, 0x757C, 0x717C, 0x6D7C, 0x697C, 0x657C, 0x617C, 0x5D7C, 0x597C, 0x557C, 0x517C, 0x4D7C, 0x497C, 0x457C, 0x417C, 
    0x3E7C, 0x3C7C, 0x3A7C, 0x387C, 0x367C, 0x347C, 0x327C, 0x307C, 0x2E7C, 0x2C7C, 0x2A7C, 0x287C, 0x267C, 0x247C, 0x227C, 0x207C, 
    0x1EFC, 0x1DFC, 0x1CFC, 0x1BFC, 0x1AFC, 0x19FC, 0x18FC, 0x17FC, 0x16FC, 0x15FC, 0x14FC, 0x13FC, 0x12FC, 0x11FC, 0x10FC, 0x0FFC, 
    0x0F3C, 0x0EBC, 0x0E3C, 0x0DBC, 0x0D3C, 0x0CBC, 0x0C3C, 0x0BBC, 0x0B3C, 0x0ABC, 0x0A3C, 0x09BC, 0x093C, 0x08BC, 0x083C, 0x07BC, 
    0x075C, 0x071C, 0x06DC, 0x069C, 0x065C, 0x061C, 0x05DC, 0x059C, 0x055C, 0x051C, 0x04DC, 0x049C, 0x045C, 0x041C, 0x03DC, 0x039C, 
    0x036C, 0x034C, 0x032C, 0x030C, 0x02EC, 0x02CC, 0x02AC, 0x028C, 0x026C, 0x024C, 0x022C, 0x020C, 0x01EC, 0x01CC, 0x01AC, 0x018C, 
    0x0174, 0x0164, 0x0154, 0x0144, 0x0134, 0x0124, 0x0114, 0x0104, 0x00F4, 0x00E4, 0x00D4, 0x00C4, 0x00B4, 0x00A4, 0x0094, 0x0084, 
    0x0078, 0x0070, 0x0068, 0x0060, 0x0058, 0x0050, 0x0048, 0x0040, 0x0038, 0x0030, 0x0028, 0x0020, 0x0018, 0x0010, 0x0008, 0x0000
};

static DRWAV_INLINE drwav_int16 drwav__alaw_to_s16(drwav_uint8 sampleIn)
{
    return (short)g_drwavAlawTable[sampleIn];
}

static DRWAV_INLINE drwav_int16 drwav__mulaw_to_s16(drwav_uint8 sampleIn)
{
    return (short)g_drwavMulawTable[sampleIn];
}



static void drwav__pcm_to_s16(drwav_int16* pOut, const unsigned char* pIn, size_t totalSampleCount, unsigned int bytesPerSample)
{
    unsigned int i;

    /* Special case for 8-bit sample data because it's treated as unsigned. */
    if (bytesPerSample == 1) {
        drwav_u8_to_s16(pOut, pIn, totalSampleCount);
        return;
    }


    /* Slightly more optimal implementation for common formats. */
    if (bytesPerSample == 2) {
        for (i = 0; i < totalSampleCount; ++i) {
           *pOut++ = ((const drwav_int16*)pIn)[i];
        }
        return;
    }
    if (bytesPerSample == 3) {
        drwav_s24_to_s16(pOut, pIn, totalSampleCount);
        return;
    }
    if (bytesPerSample == 4) {
        drwav_s32_to_s16(pOut, (const drwav_int32*)pIn, totalSampleCount);
        return;
    }


    /* Anything more than 64 bits per sample is not supported. */
    if (bytesPerSample > 8) {
        DRWAV_ZERO_MEMORY(pOut, totalSampleCount * sizeof(*pOut));
        return;
    }


    /* Generic, slow converter. */
    for (i = 0; i < totalSampleCount; ++i) {
        drwav_uint64 sample = 0;
        unsigned int shift  = (8 - bytesPerSample) * 8;

        unsigned int j;
        for (j = 0; j < bytesPerSample; j += 1) {
            DRWAV_ASSERT(j < 8);
            sample |= (drwav_uint64)(pIn[j]) << shift;
            shift  += 8;
        }

        pIn += j;
        *pOut++ = (drwav_int16)((drwav_int64)sample >> 48);
    }
}

static void drwav__ieee_to_s16(drwav_int16* pOut, const unsigned char* pIn, size_t totalSampleCount, unsigned int bytesPerSample)
{
    if (bytesPerSample == 4) {
        drwav_f32_to_s16(pOut, (const float*)pIn, totalSampleCount);
        return;
    } else if (bytesPerSample == 8) {
        drwav_f64_to_s16(pOut, (const double*)pIn, totalSampleCount);
        return;
    } else {
        /* Only supporting 32- and 64-bit float. Output silence in all other cases. Contributions welcome for 16-bit float. */
        DRWAV_ZERO_MEMORY(pOut, totalSampleCount * sizeof(*pOut));
        return;
    }
}

drwav_uint64 drwav_read_pcm_frames_s16__pcm(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint32 bytesPerFrame;
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];

    /* Fast path. */
    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM && pWav->bitsPerSample == 16) {
        return drwav_read_pcm_frames(pWav, framesToRead, pBufferOut);
    }
    
    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;
    
    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav__pcm_to_s16(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels), bytesPerFrame/pWav->channels);

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_s16__ieee(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];

    drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;
    
    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav__ieee_to_s16(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels), bytesPerFrame/pWav->channels);

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_s16__alaw(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];

    drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;
    
    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav_alaw_to_s16(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels));

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_s16__mulaw(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];

    drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav_mulaw_to_s16(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels));

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_s16(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    if (pWav == NULL || framesToRead == 0 || pBufferOut == NULL) {
        return 0;
    }

    /* Don't try to read more samples than can potentially fit in the output buffer. */
    if (framesToRead * pWav->channels * sizeof(drwav_int16) > DRWAV_SIZE_MAX) {
        framesToRead = DRWAV_SIZE_MAX / sizeof(drwav_int16) / pWav->channels;
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM) {
        return drwav_read_pcm_frames_s16__pcm(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_IEEE_FLOAT) {
        return drwav_read_pcm_frames_s16__ieee(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ALAW) {
        return drwav_read_pcm_frames_s16__alaw(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_MULAW) {
        return drwav_read_pcm_frames_s16__mulaw(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
        return drwav_read_pcm_frames_s16__msadpcm(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
        return drwav_read_pcm_frames_s16__ima(pWav, framesToRead, pBufferOut);
    }

    return 0;
}

drwav_uint64 drwav_read_pcm_frames_s16le(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_s16(pWav, framesToRead, pBufferOut);
    if (!drwav__is_little_endian()) {
        drwav__bswap_samples_s16(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}

drwav_uint64 drwav_read_pcm_frames_s16be(drwav* pWav, drwav_uint64 framesToRead, drwav_int16* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_s16(pWav, framesToRead, pBufferOut);
    if (drwav__is_little_endian()) {
        drwav__bswap_samples_s16(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}


void drwav_u8_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    int r;
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        int x = pIn[i];
        r = x << 8;
        r = r - 32768;
        pOut[i] = (short)r;
    }
}

void drwav_s24_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    int r;
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        int x = ((int)(((unsigned int)(((const unsigned char*)pIn)[i*3+0]) << 8) | ((unsigned int)(((const unsigned char*)pIn)[i*3+1]) << 16) | ((unsigned int)(((const unsigned char*)pIn)[i*3+2])) << 24)) >> 8;
        r = x >> 8;
        pOut[i] = (short)r;
    }
}

void drwav_s32_to_s16(drwav_int16* pOut, const drwav_int32* pIn, size_t sampleCount)
{
    int r;
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        int x = pIn[i];
        r = x >> 16;
        pOut[i] = (short)r;
    }
}

void drwav_f32_to_s16(drwav_int16* pOut, const float* pIn, size_t sampleCount)
{
    int r;
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        float x = pIn[i];
        float c;
        c = ((x < -1) ? -1 : ((x > 1) ? 1 : x));
        c = c + 1;
        r = (int)(c * 32767.5f);
        r = r - 32768;
        pOut[i] = (short)r;
    }
}

void drwav_f64_to_s16(drwav_int16* pOut, const double* pIn, size_t sampleCount)
{
    int r;
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        double x = pIn[i];
        double c;
        c = ((x < -1) ? -1 : ((x > 1) ? 1 : x));
        c = c + 1;
        r = (int)(c * 32767.5);
        r = r - 32768;
        pOut[i] = (short)r;
    }
}

void drwav_alaw_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        pOut[i] = drwav__alaw_to_s16(pIn[i]);
    }
}

void drwav_mulaw_to_s16(drwav_int16* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;
    for (i = 0; i < sampleCount; ++i) {
        pOut[i] = drwav__mulaw_to_s16(pIn[i]);
    }
}



static void drwav__pcm_to_f32(float* pOut, const unsigned char* pIn, size_t sampleCount, unsigned int bytesPerSample)
{
    unsigned int i;

    /* Special case for 8-bit sample data because it's treated as unsigned. */
    if (bytesPerSample == 1) {
        drwav_u8_to_f32(pOut, pIn, sampleCount);
        return;
    }

    /* Slightly more optimal implementation for common formats. */
    if (bytesPerSample == 2) {
        drwav_s16_to_f32(pOut, (const drwav_int16*)pIn, sampleCount);
        return;
    }
    if (bytesPerSample == 3) {
        drwav_s24_to_f32(pOut, pIn, sampleCount);
        return;
    }
    if (bytesPerSample == 4) {
        drwav_s32_to_f32(pOut, (const drwav_int32*)pIn, sampleCount);
        return;
    }


    /* Anything more than 64 bits per sample is not supported. */
    if (bytesPerSample > 8) {
        DRWAV_ZERO_MEMORY(pOut, sampleCount * sizeof(*pOut));
        return;
    }


    /* Generic, slow converter. */
    for (i = 0; i < sampleCount; ++i) {
        drwav_uint64 sample = 0;
        unsigned int shift  = (8 - bytesPerSample) * 8;

        unsigned int j;
        for (j = 0; j < bytesPerSample; j += 1) {
            DRWAV_ASSERT(j < 8);
            sample |= (drwav_uint64)(pIn[j]) << shift;
            shift  += 8;
        }

        pIn += j;
        *pOut++ = (float)((drwav_int64)sample / 9223372036854775807.0);
    }
}

static void drwav__ieee_to_f32(float* pOut, const unsigned char* pIn, size_t sampleCount, unsigned int bytesPerSample)
{
    if (bytesPerSample == 4) {
        unsigned int i;
        for (i = 0; i < sampleCount; ++i) {
            *pOut++ = ((const float*)pIn)[i];
        }
        return;
    } else if (bytesPerSample == 8) {
        drwav_f64_to_f32(pOut, (const double*)pIn, sampleCount);
        return;
    } else {
        /* Only supporting 32- and 64-bit float. Output silence in all other cases. Contributions welcome for 16-bit float. */
        DRWAV_ZERO_MEMORY(pOut, sampleCount * sizeof(*pOut));
        return;
    }
}


drwav_uint64 drwav_read_pcm_frames_f32__pcm(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];

    drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav__pcm_to_f32(pBufferOut, sampleData, (size_t)framesRead*pWav->channels, bytesPerFrame/pWav->channels);

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_f32__msadpcm(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    /*
    We're just going to borrow the implementation from the drwav_read_s16() since ADPCM is a little bit more complicated than other formats and I don't
    want to duplicate that code.
    */
    drwav_uint64 totalFramesRead = 0;
    drwav_int16 samples16[2048];
    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames_s16(pWav, drwav_min(framesToRead, drwav_countof(samples16)/pWav->channels), samples16);
        if (framesRead == 0) {
            break;
        }

        drwav_s16_to_f32(pBufferOut, samples16, (size_t)(framesRead*pWav->channels));   /* <-- Safe cast because we're clamping to 2048. */

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_f32__ima(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    /*
    We're just going to borrow the implementation from the drwav_read_s16() since IMA-ADPCM is a little bit more complicated than other formats and I don't
    want to duplicate that code.
    */
    drwav_uint64 totalFramesRead = 0;
    drwav_int16 samples16[2048];
    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames_s16(pWav, drwav_min(framesToRead, drwav_countof(samples16)/pWav->channels), samples16);
        if (framesRead == 0) {
            break;
        }

        drwav_s16_to_f32(pBufferOut, samples16, (size_t)(framesRead*pWav->channels));   /* <-- Safe cast because we're clamping to 2048. */

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_f32__ieee(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];
    drwav_uint32 bytesPerFrame;

    /* Fast path. */
    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_IEEE_FLOAT && pWav->bitsPerSample == 32) {
        return drwav_read_pcm_frames(pWav, framesToRead, pBufferOut);
    }
    
    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav__ieee_to_f32(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels), bytesPerFrame/pWav->channels);

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_f32__alaw(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];
    drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav_alaw_to_f32(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels));

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_f32__mulaw(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];

    drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav_mulaw_to_f32(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels));

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_f32(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    if (pWav == NULL || framesToRead == 0 || pBufferOut == NULL) {
        return 0;
    }

    /* Don't try to read more samples than can potentially fit in the output buffer. */
    if (framesToRead * pWav->channels * sizeof(float) > DRWAV_SIZE_MAX) {
        framesToRead = DRWAV_SIZE_MAX / sizeof(float) / pWav->channels;
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM) {
        return drwav_read_pcm_frames_f32__pcm(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
        return drwav_read_pcm_frames_f32__msadpcm(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_IEEE_FLOAT) {
        return drwav_read_pcm_frames_f32__ieee(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ALAW) {
        return drwav_read_pcm_frames_f32__alaw(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_MULAW) {
        return drwav_read_pcm_frames_f32__mulaw(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
        return drwav_read_pcm_frames_f32__ima(pWav, framesToRead, pBufferOut);
    }

    return 0;
}

drwav_uint64 drwav_read_pcm_frames_f32le(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_f32(pWav, framesToRead, pBufferOut);
    if (!drwav__is_little_endian()) {
        drwav__bswap_samples_f32(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}

drwav_uint64 drwav_read_pcm_frames_f32be(drwav* pWav, drwav_uint64 framesToRead, float* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_f32(pWav, framesToRead, pBufferOut);
    if (drwav__is_little_endian()) {
        drwav__bswap_samples_f32(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}


void drwav_u8_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

#ifdef DR_WAV_LIBSNDFILE_COMPAT
    /*
    It appears libsndfile uses slightly different logic for the u8 -> f32 conversion to dr_wav, which in my opinion is incorrect. It appears
    libsndfile performs the conversion something like "f32 = (u8 / 256) * 2 - 1", however I think it should be "f32 = (u8 / 255) * 2 - 1" (note
    the divisor of 256 vs 255). I use libsndfile as a benchmark for testing, so I'm therefore leaving this block here just for my automated
    correctness testing. This is disabled by default.
    */
    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = (pIn[i] / 256.0f) * 2 - 1;
    }
#else
    for (i = 0; i < sampleCount; ++i) {
        float x = pIn[i];
        x = x * 0.00784313725490196078f;    /* 0..255 to 0..2 */
        x = x - 1;                          /* 0..2 to -1..1 */

        *pOut++ = x;
    }
#endif
}

void drwav_s16_to_f32(float* pOut, const drwav_int16* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = pIn[i] * 0.000030517578125f;
    }
}

void drwav_s24_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        double x = (double)(((drwav_int32)(((drwav_uint32)(pIn[i*3+0]) << 8) | ((drwav_uint32)(pIn[i*3+1]) << 16) | ((drwav_uint32)(pIn[i*3+2])) << 24)) >> 8);
        *pOut++ = (float)(x * 0.00000011920928955078125);
    }
}

void drwav_s32_to_f32(float* pOut, const drwav_int32* pIn, size_t sampleCount)
{
    size_t i;
    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = (float)(pIn[i] / 2147483648.0);
    }
}

void drwav_f64_to_f32(float* pOut, const double* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = (float)pIn[i];
    }
}

void drwav_alaw_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = drwav__alaw_to_s16(pIn[i]) / 32768.0f;
    }
}

void drwav_mulaw_to_f32(float* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = drwav__mulaw_to_s16(pIn[i]) / 32768.0f;
    }
}



static void drwav__pcm_to_s32(drwav_int32* pOut, const unsigned char* pIn, size_t totalSampleCount, unsigned int bytesPerSample)
{
    unsigned int i;

    /* Special case for 8-bit sample data because it's treated as unsigned. */
    if (bytesPerSample == 1) {
        drwav_u8_to_s32(pOut, pIn, totalSampleCount);
        return;
    }

    /* Slightly more optimal implementation for common formats. */
    if (bytesPerSample == 2) {
        drwav_s16_to_s32(pOut, (const drwav_int16*)pIn, totalSampleCount);
        return;
    }
    if (bytesPerSample == 3) {
        drwav_s24_to_s32(pOut, pIn, totalSampleCount);
        return;
    }
    if (bytesPerSample == 4) {
        for (i = 0; i < totalSampleCount; ++i) {
           *pOut++ = ((const drwav_int32*)pIn)[i];
        }
        return;
    }


    /* Anything more than 64 bits per sample is not supported. */
    if (bytesPerSample > 8) {
        DRWAV_ZERO_MEMORY(pOut, totalSampleCount * sizeof(*pOut));
        return;
    }


    /* Generic, slow converter. */
    for (i = 0; i < totalSampleCount; ++i) {
        drwav_uint64 sample = 0;
        unsigned int shift  = (8 - bytesPerSample) * 8;

        unsigned int j;
        for (j = 0; j < bytesPerSample; j += 1) {
            DRWAV_ASSERT(j < 8);
            sample |= (drwav_uint64)(pIn[j]) << shift;
            shift  += 8;
        }

        pIn += j;
        *pOut++ = (drwav_int32)((drwav_int64)sample >> 32);
    }
}

static void drwav__ieee_to_s32(drwav_int32* pOut, const unsigned char* pIn, size_t totalSampleCount, unsigned int bytesPerSample)
{
    if (bytesPerSample == 4) {
        drwav_f32_to_s32(pOut, (const float*)pIn, totalSampleCount);
        return;
    } else if (bytesPerSample == 8) {
        drwav_f64_to_s32(pOut, (const double*)pIn, totalSampleCount);
        return;
    } else {
        /* Only supporting 32- and 64-bit float. Output silence in all other cases. Contributions welcome for 16-bit float. */
        DRWAV_ZERO_MEMORY(pOut, totalSampleCount * sizeof(*pOut));
        return;
    }
}


drwav_uint64 drwav_read_pcm_frames_s32__pcm(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];
    drwav_uint32 bytesPerFrame;

    /* Fast path. */
    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM && pWav->bitsPerSample == 32) {
        return drwav_read_pcm_frames(pWav, framesToRead, pBufferOut);
    }
    
    bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav__pcm_to_s32(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels), bytesPerFrame/pWav->channels);

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_s32__msadpcm(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    /*
    We're just going to borrow the implementation from the drwav_read_s16() since ADPCM is a little bit more complicated than other formats and I don't
    want to duplicate that code.
    */
    drwav_uint64 totalFramesRead = 0;
    drwav_int16 samples16[2048];
    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames_s16(pWav, drwav_min(framesToRead, drwav_countof(samples16)/pWav->channels), samples16);
        if (framesRead == 0) {
            break;
        }

        drwav_s16_to_s32(pBufferOut, samples16, (size_t)(framesRead*pWav->channels));   /* <-- Safe cast because we're clamping to 2048. */

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_s32__ima(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    /*
    We're just going to borrow the implementation from the drwav_read_s16() since IMA-ADPCM is a little bit more complicated than other formats and I don't
    want to duplicate that code.
    */
    drwav_uint64 totalFramesRead = 0;
    drwav_int16 samples16[2048];
    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames_s16(pWav, drwav_min(framesToRead, drwav_countof(samples16)/pWav->channels), samples16);
        if (framesRead == 0) {
            break;
        }

        drwav_s16_to_s32(pBufferOut, samples16, (size_t)(framesRead*pWav->channels));   /* <-- Safe cast because we're clamping to 2048. */

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_s32__ieee(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];

    drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav__ieee_to_s32(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels), bytesPerFrame/pWav->channels);

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_s32__alaw(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];

    drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav_alaw_to_s32(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels));

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_s32__mulaw(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 totalFramesRead;
    unsigned char sampleData[4096];

    drwav_uint32 bytesPerFrame = drwav_get_bytes_per_pcm_frame(pWav);
    if (bytesPerFrame == 0) {
        return 0;
    }

    totalFramesRead = 0;

    while (framesToRead > 0) {
        drwav_uint64 framesRead = drwav_read_pcm_frames(pWav, drwav_min(framesToRead, sizeof(sampleData)/bytesPerFrame), sampleData);
        if (framesRead == 0) {
            break;
        }

        drwav_mulaw_to_s32(pBufferOut, sampleData, (size_t)(framesRead*pWav->channels));

        pBufferOut      += framesRead*pWav->channels;
        framesToRead    -= framesRead;
        totalFramesRead += framesRead;
    }

    return totalFramesRead;
}

drwav_uint64 drwav_read_pcm_frames_s32(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    if (pWav == NULL || framesToRead == 0 || pBufferOut == NULL) {
        return 0;
    }

    /* Don't try to read more samples than can potentially fit in the output buffer. */
    if (framesToRead * pWav->channels * sizeof(drwav_int32) > DRWAV_SIZE_MAX) {
        framesToRead = DRWAV_SIZE_MAX / sizeof(drwav_int32) / pWav->channels;
    }


    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_PCM) {
        return drwav_read_pcm_frames_s32__pcm(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ADPCM) {
        return drwav_read_pcm_frames_s32__msadpcm(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_IEEE_FLOAT) {
        return drwav_read_pcm_frames_s32__ieee(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_ALAW) {
        return drwav_read_pcm_frames_s32__alaw(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_MULAW) {
        return drwav_read_pcm_frames_s32__mulaw(pWav, framesToRead, pBufferOut);
    }

    if (pWav->translatedFormatTag == DR_WAVE_FORMAT_DVI_ADPCM) {
        return drwav_read_pcm_frames_s32__ima(pWav, framesToRead, pBufferOut);
    }

    return 0;
}

drwav_uint64 drwav_read_pcm_frames_s32le(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_s32(pWav, framesToRead, pBufferOut);
    if (!drwav__is_little_endian()) {
        drwav__bswap_samples_s32(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}

drwav_uint64 drwav_read_pcm_frames_s32be(drwav* pWav, drwav_uint64 framesToRead, drwav_int32* pBufferOut)
{
    drwav_uint64 framesRead = drwav_read_pcm_frames_s32(pWav, framesToRead, pBufferOut);
    if (drwav__is_little_endian()) {
        drwav__bswap_samples_s32(pBufferOut, framesRead*pWav->channels);
    }

    return framesRead;
}


void drwav_u8_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = ((int)pIn[i] - 128) << 24;
    }
}

void drwav_s16_to_s32(drwav_int32* pOut, const drwav_int16* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = pIn[i] << 16;
    }
}

void drwav_s24_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        unsigned int s0 = pIn[i*3 + 0];
        unsigned int s1 = pIn[i*3 + 1];
        unsigned int s2 = pIn[i*3 + 2];

        drwav_int32 sample32 = (drwav_int32)((s0 << 8) | (s1 << 16) | (s2 << 24));
        *pOut++ = sample32;
    }
}

void drwav_f32_to_s32(drwav_int32* pOut, const float* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = (drwav_int32)(2147483648.0 * pIn[i]);
    }
}

void drwav_f64_to_s32(drwav_int32* pOut, const double* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = (drwav_int32)(2147483648.0 * pIn[i]);
    }
}

void drwav_alaw_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i = 0; i < sampleCount; ++i) {
        *pOut++ = ((drwav_int32)drwav__alaw_to_s16(pIn[i])) << 16;
    }
}

void drwav_mulaw_to_s32(drwav_int32* pOut, const drwav_uint8* pIn, size_t sampleCount)
{
    size_t i;

    if (pOut == NULL || pIn == NULL) {
        return;
    }

    for (i= 0; i < sampleCount; ++i) {
        *pOut++ = ((drwav_int32)drwav__mulaw_to_s16(pIn[i])) << 16;
    }
}



drwav_int16* drwav__read_pcm_frames_and_close_s16(drwav* pWav, unsigned int* channels, unsigned int* sampleRate, drwav_uint64* totalFrameCount)
{
    drwav_uint64 sampleDataSize;
    drwav_int16* pSampleData;
    drwav_uint64 framesRead;

    DRWAV_ASSERT(pWav != NULL);

    sampleDataSize = pWav->totalPCMFrameCount * pWav->channels * sizeof(drwav_int16);
    if (sampleDataSize > DRWAV_SIZE_MAX) {
        drwav_uninit(pWav);
        return NULL;    /* File's too big. */
    }

    pSampleData = (drwav_int16*)drwav__malloc_from_callbacks((size_t)sampleDataSize, &pWav->allocationCallbacks); /* <-- Safe cast due to the check above. */
    if (pSampleData == NULL) {
        drwav_uninit(pWav);
        return NULL;    /* Failed to allocate memory. */
    }

    framesRead = drwav_read_pcm_frames_s16(pWav, (size_t)pWav->totalPCMFrameCount, pSampleData);
    if (framesRead != pWav->totalPCMFrameCount) {
        drwav__free_from_callbacks(pSampleData, &pWav->allocationCallbacks);
        drwav_uninit(pWav);
        return NULL;    /* There was an error reading the samples. */
    }

    drwav_uninit(pWav);

    if (sampleRate) {
        *sampleRate = pWav->sampleRate;
    }
    if (channels) {
        *channels = pWav->channels;
    }
    if (totalFrameCount) {
        *totalFrameCount = pWav->totalPCMFrameCount;
    }

    return pSampleData;
}

float* drwav__read_pcm_frames_and_close_f32(drwav* pWav, unsigned int* channels, unsigned int* sampleRate, drwav_uint64* totalFrameCount)
{
    drwav_uint64 sampleDataSize;
    float* pSampleData;
    drwav_uint64 framesRead;

    DRWAV_ASSERT(pWav != NULL);

    sampleDataSize = pWav->totalPCMFrameCount * pWav->channels * sizeof(float);
    if (sampleDataSize > DRWAV_SIZE_MAX) {
        drwav_uninit(pWav);
        return NULL;    /* File's too big. */
    }

    pSampleData = (float*)drwav__malloc_from_callbacks((size_t)sampleDataSize, &pWav->allocationCallbacks); /* <-- Safe cast due to the check above. */
    if (pSampleData == NULL) {
        drwav_uninit(pWav);
        return NULL;    /* Failed to allocate memory. */
    }

    framesRead = drwav_read_pcm_frames_f32(pWav, (size_t)pWav->totalPCMFrameCount, pSampleData);
    if (framesRead != pWav->totalPCMFrameCount) {
        drwav__free_from_callbacks(pSampleData, &pWav->allocationCallbacks);
        drwav_uninit(pWav);
        return NULL;    /* There was an error reading the samples. */
    }

    drwav_uninit(pWav);

    if (sampleRate) {
        *sampleRate = pWav->sampleRate;
    }
    if (channels) {
        *channels = pWav->channels;
    }
    if (totalFrameCount) {
        *totalFrameCount = pWav->totalPCMFrameCount;
    }

    return pSampleData;
}

drwav_int32* drwav__read_pcm_frames_and_close_s32(drwav* pWav, unsigned int* channels, unsigned int* sampleRate, drwav_uint64* totalFrameCount)
{
    drwav_uint64 sampleDataSize;
    drwav_int32* pSampleData;
    drwav_uint64 framesRead;

    DRWAV_ASSERT(pWav != NULL);

    sampleDataSize = pWav->totalPCMFrameCount * pWav->channels * sizeof(drwav_int32);
    if (sampleDataSize > DRWAV_SIZE_MAX) {
        drwav_uninit(pWav);
        return NULL;    /* File's too big. */
    }

    pSampleData = (drwav_int32*)drwav__malloc_from_callbacks((size_t)sampleDataSize, &pWav->allocationCallbacks); /* <-- Safe cast due to the check above. */
    if (pSampleData == NULL) {
        drwav_uninit(pWav);
        return NULL;    /* Failed to allocate memory. */
    }

    framesRead = drwav_read_pcm_frames_s32(pWav, (size_t)pWav->totalPCMFrameCount, pSampleData);
    if (framesRead != pWav->totalPCMFrameCount) {
        drwav__free_from_callbacks(pSampleData, &pWav->allocationCallbacks);
        drwav_uninit(pWav);
        return NULL;    /* There was an error reading the samples. */
    }

    drwav_uninit(pWav);

    if (sampleRate) {
        *sampleRate = pWav->sampleRate;
    }
    if (channels) {
        *channels = pWav->channels;
    }
    if (totalFrameCount) {
        *totalFrameCount = pWav->totalPCMFrameCount;
    }

    return pSampleData;
}



drwav_int16* drwav_open_and_read_pcm_frames_s16(drwav_read_proc onRead, drwav_seek_proc onSeek, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init(&wav, onRead, onSeek, pUserData, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s16(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

float* drwav_open_and_read_pcm_frames_f32(drwav_read_proc onRead, drwav_seek_proc onSeek, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init(&wav, onRead, onSeek, pUserData, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_f32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

drwav_int32* drwav_open_and_read_pcm_frames_s32(drwav_read_proc onRead, drwav_seek_proc onSeek, void* pUserData, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init(&wav, onRead, onSeek, pUserData, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

#ifndef DR_WAV_NO_STDIO
drwav_int16* drwav_open_file_and_read_pcm_frames_s16(const char* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s16(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

float* drwav_open_file_and_read_pcm_frames_f32(const char* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_f32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

drwav_int32* drwav_open_file_and_read_pcm_frames_s32(const char* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}


drwav_int16* drwav_open_file_and_read_pcm_frames_s16_w(const wchar_t* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (channelsOut) {
        *channelsOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file_w(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s16(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

float* drwav_open_file_and_read_pcm_frames_f32_w(const wchar_t* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (channelsOut) {
        *channelsOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file_w(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_f32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

drwav_int32* drwav_open_file_and_read_pcm_frames_s32_w(const wchar_t* filename, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (channelsOut) {
        *channelsOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_file_w(&wav, filename, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}
#endif

drwav_int16* drwav_open_memory_and_read_pcm_frames_s16(const void* data, size_t dataSize, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_memory(&wav, data, dataSize, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s16(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

float* drwav_open_memory_and_read_pcm_frames_f32(const void* data, size_t dataSize, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_memory(&wav, data, dataSize, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_f32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}

drwav_int32* drwav_open_memory_and_read_pcm_frames_s32(const void* data, size_t dataSize, unsigned int* channelsOut, unsigned int* sampleRateOut, drwav_uint64* totalFrameCountOut, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    drwav wav;

    if (channelsOut) {
        *channelsOut = 0;
    }
    if (sampleRateOut) {
        *sampleRateOut = 0;
    }
    if (totalFrameCountOut) {
        *totalFrameCountOut = 0;
    }

    if (!drwav_init_memory(&wav, data, dataSize, pAllocationCallbacks)) {
        return NULL;
    }

    return drwav__read_pcm_frames_and_close_s32(&wav, channelsOut, sampleRateOut, totalFrameCountOut);
}
#endif  /* DR_WAV_NO_CONVERSION_API */


void drwav_free(void* p, const drwav_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks != NULL) {
        drwav__free_from_callbacks(p, pAllocationCallbacks);
    } else {
        drwav__free_default(p, NULL);
    }
}
