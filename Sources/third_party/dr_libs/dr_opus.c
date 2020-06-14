#include "dr_opus.h"

#include <stdlib.h>
#include <string.h>
#ifndef DR_OPUS_NO_STDIO
#include <stdio.h>
#include <wchar.h>      /* For wcslen(), wcsrtombs() */
#endif

/* CPU Architecture. */
#if defined(__x86_64__) || defined(_M_X64)
    #define DROPUS_X64
#elif defined(__i386) || defined(_M_IX86)
    #define DROPUS_X86
#elif defined(__arm__) || defined(_M_ARM)
    #define DROPUS_ARM
#endif

/* Compile-time CPU feature support. */
#if !defined(DR_OPUS_NO_SIMD) && (defined(DROPUS_X86) || defined(DROPUS_X64))
    #if defined(_MSC_VER) && !defined(__clang__)
        #if _MSC_VER >= 1400
            #include <intrin.h>
            static DROPUS_INLINE void dropus__cpuid(int info[4], int fid)
            {
                __cpuid(info, fid);
            }
        #else
            #define DROPUS_NO_CPUID
        #endif
    #else
        #if defined(__GNUC__) || defined(__clang__)
            static DROPUS_INLINE void dropus__cpuid(int info[4], int fid)
            {
                /*
                It looks like the -fPIC option uses the ebx register which GCC complains about. We can work around this by just using a different register, the
                specific register of which I'm letting the compiler decide on. The "k" prefix is used to specify a 32-bit register. The {...} syntax is for
                supporting different assembly dialects.
                
                What's basically happening is that we're saving and restoring the ebx register manually.
                */
                #if defined(DROPUS_X86) && defined(__PIC__)
                    __asm__ __volatile__ (
                        "xchg{l} {%%}ebx, %k1;"
                        "cpuid;"
                        "xchg{l} {%%}ebx, %k1;"
                        : "=a"(info[0]), "=&r"(info[1]), "=c"(info[2]), "=d"(info[3]) : "a"(fid), "c"(0)
                    );
                #else
                    __asm__ __volatile__ (
                        "cpuid" : "=a"(info[0]), "=b"(info[1]), "=c"(info[2]), "=d"(info[3]) : "a"(fid), "c"(0)
                    );
                #endif
            }
        #else
            #define DROPUS_NO_CPUID
        #endif
    #endif
#else
    #define DROPUS_NO_CPUID
#endif


#if defined(_MSC_VER) && _MSC_VER >= 1500 && (defined(DROPUS_X86) || defined(DROPUS_X64))
    #define DROPUS_HAS_LZCNT_INTRINSIC
#elif (defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)))
    #define DROPUS_HAS_LZCNT_INTRINSIC
#elif defined(__clang__)
    #if __has_builtin(__builtin_clzll) || __has_builtin(__builtin_clzl)
        #define DROPUS_HAS_LZCNT_INTRINSIC
    #endif
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
    #define DROPUS_HAS_BYTESWAP16_INTRINSIC
    #define DROPUS_HAS_BYTESWAP32_INTRINSIC
    #define DROPUS_HAS_BYTESWAP64_INTRINSIC
#elif defined(__clang__)
    #if __has_builtin(__builtin_bswap16)
        #define DROPUS_HAS_BYTESWAP16_INTRINSIC
    #endif
    #if __has_builtin(__builtin_bswap32)
        #define DROPUS_HAS_BYTESWAP32_INTRINSIC
    #endif
    #if __has_builtin(__builtin_bswap64)
        #define DROPUS_HAS_BYTESWAP64_INTRINSIC
    #endif
#elif defined(__GNUC__)
    #if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
        #define DROPUS_HAS_BYTESWAP32_INTRINSIC
        #define DROPUS_HAS_BYTESWAP64_INTRINSIC
    #endif
    #if ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))
        #define DROPUS_HAS_BYTESWAP16_INTRINSIC
    #endif
#endif


#ifndef DROPUS_ASSERT
#include <assert.h>
#define DROPUS_ASSERT(expression)           assert(expression)
#endif
#ifndef DROPUS_MALLOC
#define DROPUS_MALLOC(sz)                   malloc((sz))
#endif
#ifndef DROPUS_REALLOC
#define DROPUS_REALLOC(p, sz)               realloc((p), (sz))
#endif
#ifndef DROPUS_FREE
#define DROPUS_FREE(p)                      free((p))
#endif
#ifndef DROPUS_COPY_MEMORY
#define DROPUS_COPY_MEMORY(dst, src, sz)    memcpy((dst), (src), (sz))
#endif
#ifndef DROPUS_ZERO_MEMORY
#define DROPUS_ZERO_MEMORY(p, sz)           memset((p), 0, (sz))
#endif
#ifndef DROPUS_ZERO_OBJECT
#define DROPUS_ZERO_OBJECT(p)               DROPUS_ZERO_MEMORY((p), sizeof(*(p)))
#endif

#ifndef DROPUS_MIN
#define DROPUS_MIN(x, y)                    (((x) < (y)) ? (x) : (y))
#endif
#ifndef DROPUS_MAX
#define DROPUS_MAX(x, y)                    (((x) > (y)) ? (x) : (y))
#endif
#ifndef DROPUS_COUNTOF
#define DROPUS_COUNTOF(p)                   (sizeof(p) / sizeof((p)[0]))
#endif


/*********************************** 
Endian Management
************************************/
static DROPUS_INLINE dropus_bool32 dropus__is_little_endian()
{
#if defined(DROPUS_X86) || defined(DROPUS_X64)
    return DROPUS_TRUE;
#elif defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN
    return DROPUS_TRUE;
#else
    int n = 1;
    return (*(char*)&n) == 1;
#endif
}

static DROPUS_INLINE dropus_uint16 dropus__swap_endian_uint16(dropus_uint16 n)
{
#ifdef DROPUS_HAS_BYTESWAP16_INTRINSIC
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

static DROPUS_INLINE dropus_uint32 dropus__swap_endian_uint32(dropus_uint32 n)
{
#ifdef DROPUS_HAS_BYTESWAP32_INTRINSIC
    #if defined(_MSC_VER)
        return _byteswap_ulong(n);
    #elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap32(n);
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

static DROPUS_INLINE dropus_uint64 dropus__swap_endian_uint64(dropus_uint64 n)
{
#ifdef DROPUS_HAS_BYTESWAP64_INTRINSIC
    #if defined(_MSC_VER)
        return _byteswap_uint64(n);
    #elif defined(__GNUC__) || defined(__clang__)
        return __builtin_bswap64(n);
    #else
        #error "This compiler does not support the byte swap intrinsic."
    #endif
#else
    return ((n & (dropus_uint64)0xFF00000000000000) >> 56) |
           ((n & (dropus_uint64)0x00FF000000000000) >> 40) |
           ((n & (dropus_uint64)0x0000FF0000000000) >> 24) |
           ((n & (dropus_uint64)0x000000FF00000000) >>  8) |
           ((n & (dropus_uint64)0x00000000FF000000) <<  8) |
           ((n & (dropus_uint64)0x0000000000FF0000) << 24) |
           ((n & (dropus_uint64)0x000000000000FF00) << 40) |
           ((n & (dropus_uint64)0x00000000000000FF) << 56);
#endif
}


static DROPUS_INLINE dropus_uint16 dropus__be2host_16(dropus_uint16 n)
{
    if (dropus__is_little_endian()) {
        return dropus__swap_endian_uint16(n);
    }

    return n;
}

static DROPUS_INLINE dropus_uint32 dropus__be2host_32(dropus_uint32 n)
{
    if (dropus__is_little_endian()) {
        return dropus__swap_endian_uint32(n);
    }

    return n;
}

static DROPUS_INLINE dropus_uint64 dropus__be2host_64(dropus_uint64 n)
{
    if (dropus__is_little_endian()) {
        return dropus__swap_endian_uint64(n);
    }

    return n;
}


static DROPUS_INLINE dropus_uint16 dropus__le2host_16(dropus_uint16 n)
{
    if (!dropus__is_little_endian()) {
        return dropus__swap_endian_uint16(n);
    }

    return n;
}

static DROPUS_INLINE dropus_uint32 dropus__le2host_32(dropus_uint32 n)
{
    if (!dropus__is_little_endian()) {
        return dropus__swap_endian_uint32(n);
    }

    return n;
}

static DROPUS_INLINE dropus_uint64 dropus__le2host_64(dropus_uint64 n)
{
    if (!dropus__is_little_endian()) {
        return dropus__swap_endian_uint64(n);
    }

    return n;
}


/***********************************************************************************************************************************************************

Low-Level Opus Stream API

************************************************************************************************************************************************************/
#define DROPUS_MAX_FRAME_SIZE_IN_BYTES  1275    /* RFC 6716 - Section 3.4 [R2] */
#define DROPUS_MAX_PACKET_SIZE_IN_BYTES DROPUS_MAX_FRAME_SIZE_IN_BYTES*DROPUS_MAX_OPUS_FRAMES_PER_PACKET


/*********************************** 
RFC 6716 - Section 3.1 The TOC Byte
************************************/
static DROPUS_INLINE dropus_uint8 dropus_toc_config(dropus_uint8 toc)
{
    return (toc & 0xF8) >> 3;
}

static DROPUS_INLINE dropus_uint8 dropus_toc_s(dropus_uint8 toc)
{
    return (toc & 0x04) >> 2;
}

static DROPUS_INLINE dropus_uint8 dropus_toc_c(dropus_uint8 toc)
{
    return (toc & 0x03);
}

static DROPUS_INLINE dropus_mode dropus_toc_config_mode(dropus_uint8 config)
{
    /* Table 2 in RFC 6716 */
    static dropus_mode modes[32] = {
        dropus_mode_silk,   dropus_mode_silk,   dropus_mode_silk, dropus_mode_silk, /*  0...3  */
        dropus_mode_silk,   dropus_mode_silk,   dropus_mode_silk, dropus_mode_silk, /*  4...7  */
        dropus_mode_silk,   dropus_mode_silk,   dropus_mode_silk, dropus_mode_silk, /*  8...11 */
        dropus_mode_hybrid, dropus_mode_hybrid,                                     /* 12...13 */
        dropus_mode_hybrid, dropus_mode_hybrid,                                     /* 14...15 */
        dropus_mode_celt,   dropus_mode_celt,   dropus_mode_celt, dropus_mode_celt, /* 16...19 */
        dropus_mode_celt,   dropus_mode_celt,   dropus_mode_celt, dropus_mode_celt, /* 20...23 */
        dropus_mode_celt,   dropus_mode_celt,   dropus_mode_celt, dropus_mode_celt, /* 24...27 */
        dropus_mode_celt,   dropus_mode_celt,   dropus_mode_celt, dropus_mode_celt  /* 28...31 */
    };

    DROPUS_ASSERT(config < 32);
    return modes[config];
}

static DROPUS_INLINE dropus_mode dropus_toc_mode(dropus_uint8 toc)
{
    return dropus_toc_config_mode(dropus_toc_config(toc));
}

static DROPUS_INLINE dropus_uint32 dropus_toc_config_sample_rate(dropus_uint8 config)
{
    /* Table 2 with Table 1 in RFC 6716 */
    static dropus_uint32 rates[32] = {
        8000,  8000,  8000,  8000,  /*  0...3  */
        12000, 12000, 12000, 12000, /*  4...7  */
        16000, 16000, 16000, 16000, /*  8...11 */
        24000, 24000,               /* 12...13 */
        48000, 48000,               /* 14...15 */
        8000,  8000,  8000,  8000,  /* 16...19 */
        16000, 16000, 16000, 16000, /* 20...23 */
        24000, 24000, 24000, 24000, /* 24...27 */
        48000, 48000, 48000, 48000  /* 28...31 */
    };

    DROPUS_ASSERT(config < 32);
    return rates[config];
}

static DROPUS_INLINE dropus_uint32 dropus_toc_sample_rate(dropus_uint8 toc)
{
    return dropus_toc_config_sample_rate(dropus_toc_config(toc));
}

static DROPUS_INLINE dropus_uint32 dropus_toc_sample_rate_ms(dropus_uint8 toc)
{
    return dropus_toc_sample_rate(toc) / 1000;
}

static DROPUS_INLINE dropus_uint32 dropus_toc_config_frame_size_in_pcm_frames(dropus_uint8 config)
{
    /* Table 2 with Table 1 in RFC 6716 */
    static dropus_uint32 sizes[32] = {
        80,  160, 320, 480, /*  0...3  */
        120, 240, 480, 720, /*  4...7  */
        160, 320, 640, 960, /*  8...11 */
        240, 480,           /* 12...13 */
        480, 960,           /* 14...15 */
        20,  40,  80,  160, /* 16...19 */
        40,  80,  160, 320, /* 20...23 */
        60,  120, 240, 480, /* 24...27 */
        120, 240, 480, 960  /* 28...31 */
    };

    DROPUS_ASSERT(config < 32);
    return sizes[config];
}

static DROPUS_INLINE dropus_uint32 dropus_toc_frame_size_in_pcm_frames(dropus_uint8 toc)
{
    return dropus_toc_config_frame_size_in_pcm_frames(dropus_toc_config(toc));
}

static DROPUS_INLINE dropus_uint8 dropus_toc_config_silk_frame_count(dropus_uint8 config)
{
    /* Table 2 in RFC 6716 */
    static dropus_uint8 counts[32] = {
        1, 1, 2, 3, /*  0...3  */
        1, 1, 2, 3, /*  4...7  */
        1, 1, 2, 3, /*  8...11 */
        1, 1,       /* 12...13 */
        1, 1,       /* 14...15 */
        0, 0, 0, 0, /* 16...19 */   /* Here down is CELT-only. */
        0, 0, 0, 0, /* 20...23 */
        0, 0, 0, 0, /* 24...27 */
        0, 0, 0, 0  /* 28...31 */
    };

    DROPUS_ASSERT(config < 32);
    return counts[config];
}

static DROPUS_INLINE dropus_uint8 dropus_toc_silk_frame_count(dropus_uint8 toc)
{
    return dropus_toc_config_silk_frame_count(dropus_toc_config(toc));
}

static DROPUS_INLINE dropus_int32 dropus_Q13(dropus_uint16 index)
{
    /* Table 7 in RFC 6716 */
    static dropus_int32 Q13[16] = {
        -13732, -10050, -8266, -7526,
        -6500,  -5000,  -2950, -820,
         820,    2950,   5000,  6500,
         7526,   8266,   10050, 13732
    };

    DROPUS_ASSERT(index < DROPUS_COUNTOF(Q13));
    return Q13[index];
}


typedef struct
{
    const dropus_uint8* pData;
    dropus_uint16 dataSize;
    dropus_uint16 readPointer;
    dropus_uint8  b0;
    dropus_uint32 rng;      /* RFC 6716 - Section 4.1 - Both val and rng are 32-bit unsigned integer values. */
    dropus_uint32 val;      /* ^^^ */
} dropus_range_decoder;

static DROPUS_INLINE void dropus_range_decoder_normalize(dropus_range_decoder* pRangeDecoder)
{
    dropus_uint8 sym;
    dropus_uint8 b1;

    DROPUS_ASSERT(pRangeDecoder != NULL);

    while (pRangeDecoder->rng <= 0x800000) {
        pRangeDecoder->rng = (pRangeDecoder->rng << 8);   /* RFC 6716 - Section 4.1.2.1 - First, it sets rng to (rng<<8). */

        /*  RFC 6716 - Section 4.1.2.1 - If no more input bytes remain, it uses zero bits instead. */
        if (pRangeDecoder->dataSize > pRangeDecoder->readPointer) {
            b1 = pRangeDecoder->pData[pRangeDecoder->readPointer++];
        } else {
            b1 = 0;
        }
        
        /* RFC 6716 - Section 4.1.2.1 - ... using the leftover bit buffered from the previous byte as the high bit and the top 7 bits of the byte just read as the other 7 bits of sym. */
        sym = ((pRangeDecoder->b0 & 0x01) << 7) | (b1 >> 7);

        /* RFC 6716 - Section 4.1.2.1 - The remaining bit in the byte just read is buffered for use in the next iteration. */
        pRangeDecoder->b0 = (b1 & 0x01);

        /* val */
        pRangeDecoder->val = ((pRangeDecoder->val << 8) + (255 - sym)) & 0x7FFFFFFF;
    }

    DROPUS_ASSERT(pRangeDecoder->rng > 0x800000);
}

static DROPUS_INLINE void dropus_range_decoder_init(const dropus_uint8* pData, dropus_uint16 dataSize, dropus_range_decoder* pRangeDecoder)
{
    DROPUS_ASSERT(pRangeDecoder != NULL);

    pRangeDecoder->pData       = pData;
    pRangeDecoder->dataSize    = dataSize;
    pRangeDecoder->readPointer = 0;

    pRangeDecoder->b0 = 0;                                  /* RFC 6716 - Section 4.1.1 - Let b0 be an 8-bit unsigned integer containing first input byte (or containing zero if there are no bytes in this Opus frame). */
    if (dataSize > 0) {
        pRangeDecoder->b0 = pData[pRangeDecoder->readPointer++];
    }

    pRangeDecoder->rng = 128;                               /* RFC 6716 - Section 4.1.1 - The decoder initializes rng to 128 ... */
    pRangeDecoder->val = 127 - (pRangeDecoder->b0 >> 1);    /*                            ... and initializes val to (127 - (b0>>1)) ...*/

    /*
    It saves the remaining bit, (b0&1), for use in the renormalization
    procedure described in Section 4.1.2.1, which the decoder invokes
    immediately after initialization to read additional bits and
    establish the invariant that rng > 2**23.
    */
    dropus_range_decoder_normalize(pRangeDecoder);
}

static DROPUS_INLINE dropus_uint16 dropus_range_decoder_fs(dropus_range_decoder* pRangeDecoder, dropus_uint16 ft)
{
    /* Implements RFC 6716 - Section 4.1.2 (first step) */

    DROPUS_ASSERT(pRangeDecoder != NULL);
    /*DROPUS_ASSERT(ft <= 65535);*/ /* RFC 6716 - Section 4.1 */ /* Always true due to range limit of `ft`. */

    return (dropus_uint16)(ft - DROPUS_MIN((pRangeDecoder->val / (pRangeDecoder->rng/ft)) + 1, ft));
}

static DROPUS_INLINE dropus_uint16 dropus_range_decoder_k(dropus_uint16* f, dropus_uint16 n, dropus_uint16 fs, dropus_uint16* flOut, dropus_uint16* fhOut)
{
    dropus_uint8 i;
    dropus_uint16 k = 0;
    dropus_uint16 fl = 0;
    dropus_uint16 fh = 0;
    for (i = 0; i < n; ++i) {
        fh = fl + f[i];
        if (fl <= fs && fs < fh) {
            k = i;
            break;
        }

        fl += f[i];
    }

    *flOut = fl;
    *fhOut = fh;
    return k;
}

static DROPUS_INLINE dropus_uint16 dropus_range_decoder_update(dropus_range_decoder* pRangeDecoder, dropus_uint16* f, dropus_uint16 n, dropus_uint16 ft, dropus_uint16 fs)
{
    /* Implements RFC 6716 - Section 4.1.2 (second step) */
    dropus_uint16 k;
    dropus_uint16 fl;
    dropus_uint16 fh;

    DROPUS_ASSERT(pRangeDecoder != NULL);
    DROPUS_ASSERT(f  != NULL);
    DROPUS_ASSERT(n  >  0);
    DROPUS_ASSERT(ft >  0);

    k = dropus_range_decoder_k(f, n, fs, &fl, &fh);

    /*DROPUS_ASSERT(0  <= fl);*/    /* RFC 6716 - Section 4.1 */ /* Always true due to `fl` being unsigned. */
    DROPUS_ASSERT(fl <  fh);        /* RFC 6716 - Section 4.1 */
    DROPUS_ASSERT(fh <= ft);        /* RFC 6716 - Section 4.1 */
    /*DROPUS_ASSERT(ft <= 65535);*/ /* RFC 6716 - Section 4.1 */ /* Always true due to range limit of `ft`. */

    /* val */
    pRangeDecoder->val = pRangeDecoder->val - (pRangeDecoder->rng/ft) * (ft - fh);

    /* rng */
    if (fl > 0) {   /* RFC 6716 - Section 4.1.2 - If fl[k] is greater than zero, then the decoder updates rng using... */
        pRangeDecoder->rng = (pRangeDecoder->rng/ft) * (fh - fl);
    } else {        /* RFC 6716 - Section 4.1.2 - Otherwise, it updates rng using... */
        pRangeDecoder->rng = pRangeDecoder->rng - (pRangeDecoder->rng/ft) * (ft - fh);
    }

    /*
    RFC 6716 - Section 4.1.2

        After the updates, implemented by ec_dec_update() (entdec.c), the
        decoder normalizes the range using the procedure in the next section,
        and returns the index k.
    */
    dropus_range_decoder_normalize(pRangeDecoder);

    return k;
}

static DROPUS_INLINE dropus_uint16 dropus_range_decoder_decode(dropus_range_decoder* pRangeDecoder, dropus_uint16* f, dropus_uint16 n, dropus_uint16 ft)
{
    dropus_uint16 fs;

    DROPUS_ASSERT(pRangeDecoder != NULL);
    DROPUS_ASSERT(f  != NULL);
    DROPUS_ASSERT(n  >  0);
    DROPUS_ASSERT(ft >  0);

    /* Step 1 from RFC 6716 - Section 4.1.2. */
    fs = dropus_range_decoder_fs(pRangeDecoder, ft);

    /* Step 2 from RFC 6716 - Section 4.1.2. */
    return dropus_range_decoder_update(pRangeDecoder, f, n, ft, fs);
}


DROPUS_API dropus_result dropus_stream_init(dropus_stream* pOpusStream)
{
    if (pOpusStream == NULL) {
        return DROPUS_INVALID_ARGS;
    }

    DROPUS_ZERO_OBJECT(pOpusStream);

    return DROPUS_SUCCESS;
}

static dropus_result dropus_stream_decode_frame__silk(dropus_stream* pOpusStream, dropus_stream_frame* pOpusFrame, const dropus_uint8* pData, size_t dataSize)
{
    dropus_range_decoder rd;

    DROPUS_ASSERT(pOpusStream != NULL);
    DROPUS_ASSERT(pOpusFrame  != NULL);
    DROPUS_ASSERT(pData       != NULL);
    DROPUS_ASSERT(dataSize    <= DROPUS_MAX_FRAME_SIZE_IN_BYTES);

    pOpusFrame->sizeInBytes = (dropus_uint16)dataSize;  /* Safe cast because dataSize <= DROPUS_MAX_FRAME_SIZE_IN_BYTES <= 1275. */

    /* Everything is fed through the range decoder. */
    dropus_range_decoder_init(pData, pOpusFrame->sizeInBytes, &rd);
    {
        dropus_uint16 f_Flags[2] = {1, 1}, ft_Flags = 2;

        dropus_uint8  iChannel;
        dropus_uint8  iFrameSILK;
        dropus_uint8  frameCountSILK;
        dropus_uint8  channels;
        dropus_uint16 k;
        dropus_uint8  flagsVAD[2]  = {0, 0};        /* One for each channel. */
        dropus_uint8  flagsLBRR[2] = {0, 0};        /* One for each channel. */
        dropus_uint32 w0_Q13[3] = {0, 0, 0};        /* One for each SILK frame (max 3). */
        dropus_uint32 w1_Q13[3] = {0, 0, 0};        /* One for each SILK frame (max 3). */
        dropus_uint8  midOnlyFlag[3] = {0, 0, 0};   /* One for each SILK frame (max 3). */
        
        frameCountSILK = dropus_toc_silk_frame_count(pOpusStream->packet.toc);    /* SILK frame count. Between 1 and 3. Either 1 10ms SILK frame, or between 1 and 3 20ms frames (20ms, 40ms, 60ms). */
        if (frameCountSILK == 0) {
            return DROPUS_BAD_DATA;
        }

        channels = dropus_toc_s(pOpusStream->packet.toc) + 1;

        /* Header flags. */
        for (iChannel = 0; iChannel < channels; ++iChannel) {
            for (iFrameSILK = 0; iFrameSILK < frameCountSILK; ++iFrameSILK) {
                k = dropus_range_decoder_decode(&rd, f_Flags, DROPUS_COUNTOF(f_Flags), ft_Flags);
                flagsVAD[iChannel] |= (k << iFrameSILK);
            }

            k = dropus_range_decoder_decode(&rd, f_Flags, DROPUS_COUNTOF(f_Flags), ft_Flags);
            DROPUS_ASSERT(k <= 1);
            flagsLBRR[iChannel] = (dropus_uint8)k;
        }

        /*
        Per-Frame LBRR Frames.
        
        RFC 6716 - 4.2.4 - ... packed in order from the LSB to the MSB.
        */
        if (frameCountSILK > 1) {
            dropus_uint8 iChannel;
            for (iChannel = 0; iChannel < channels; iChannel += 1) {
                dropus_uint16 f_40[4] = {0, 53, 53, 150},                ft_40 = 256;
                dropus_uint16 f_60[8] = {0, 41, 20, 29, 41, 15, 28, 82}, ft_60 = 256;
                if (flagsLBRR[iChannel] != 0) {
                    if (frameCountSILK == 2) {
                        k = dropus_range_decoder_decode(&rd, f_40, DROPUS_COUNTOF(f_40), ft_40);
                        DROPUS_ASSERT(k <= 0b11);
                    } else {
                        DROPUS_ASSERT(frameCountSILK == 3);
                        k = dropus_range_decoder_decode(&rd, f_60, DROPUS_COUNTOF(f_60), ft_60);
                        DROPUS_ASSERT(k <= 0b111);
                    }

                    flagsLBRR[iChannel] = (dropus_uint8)k;
                }
            }
        }

        /* LBRR frames. Only do this if the relevant flag is set. */
        for (iFrameSILK = 0; iFrameSILK < frameCountSILK; ++iFrameSILK) {
            for (iChannel = 0; iChannel < channels; ++iChannel) {
                /*
                RFC 6716 - Section 4.2.7.1

                ... these weights are coded if and only if
                    -  This is a stereo Opus frame (Section 3.1), and
                    -  The current SILK frame corresponds to the mid channel.
                */
                if (channels == 2 && iChannel == 0) {
                    dropus_uint16 f_Stage1[] = {7, 2, 1, 1, 1, 10, 24, 8, 1, 1, 3, 23, 92, 23, 3, 1, 1, 8, 24, 10, 1, 1, 1, 2, 7}, ft_Stage1 = 256;
                    dropus_uint16 f_Stage2[] = {85, 86, 85},                                                                       ft_Stage2 = 256;
                    dropus_uint16 f_Stage3[] = {51, 51, 52, 51, 51},                                                               ft_Stage3 = 256;
                    dropus_uint16 n;
                    dropus_uint16 i0, i1, i2, i3;
                    dropus_uint16 wi0, wi1;

                    n  = dropus_range_decoder_decode(&rd, f_Stage1, DROPUS_COUNTOF(f_Stage1), ft_Stage1);
                    i0 = dropus_range_decoder_decode(&rd, f_Stage2, DROPUS_COUNTOF(f_Stage2), ft_Stage2);
                    i1 = dropus_range_decoder_decode(&rd, f_Stage3, DROPUS_COUNTOF(f_Stage3), ft_Stage3);
                    i2 = dropus_range_decoder_decode(&rd, f_Stage2, DROPUS_COUNTOF(f_Stage2), ft_Stage2);
                    i3 = dropus_range_decoder_decode(&rd, f_Stage3, DROPUS_COUNTOF(f_Stage3), ft_Stage3);

                    wi0 = i0 + 3 * (n / 5);
                    wi1 = i2 + 3 * (n % 5);

                    /* Note that w0_Q13 depends on w1_Q13 so must be calculated afterwards. */
                    w1_Q13[iFrameSILK] = dropus_Q13(wi1) + (((dropus_Q13((dropus_uint16)(wi1 + 1)) - dropus_Q13(wi1)) * 6554) >> 16) * ((2 * i3) + 1);
                    w0_Q13[iFrameSILK] = dropus_Q13(wi0) + (((dropus_Q13((dropus_uint16)(wi0 + 1)) - dropus_Q13(wi0)) * 6554) >> 16) * ((2 * i1) + 1) - w1_Q13[iFrameSILK];

                    /* RFC 6716 - Section 4.2.7.2 - Mid-Only Flag */
                    if ((flagsLBRR[iChannel] & (1 << iFrameSILK)) != 0) {
                        dropus_uint16 f_MOF[] = {192, 64}, ft_MOF = 256;
                        midOnlyFlag[iFrameSILK] = (dropus_uint8)dropus_range_decoder_decode(&rd, f_MOF, DROPUS_COUNTOF(f_MOF), ft_MOF);
                    }
                }
            }
        }

        /* TODO: Don't forget to set the previous stereo weights. Don't just blindly set it without first checking the rules in RFC 6716 - Section 4.2.7.1. */
        /* RFC 6716 - Section 4.2.7.1 - These prediction weights are never included in a mono Opus frame, and the previous weights are reset to zeros on any transition from mono to stereo. */
        if (channels == 1) {
            pOpusStream->silk.w0_Q13_prev = 0;
            pOpusStream->silk.w1_Q13_prev = 0;
        }


    }

    return DROPUS_SUCCESS;
}

static dropus_result dropus_stream_decode_frame__celt(dropus_stream* pOpusStream, dropus_stream_frame* pOpusFrame, const dropus_uint8* pData, size_t dataSize)
{
    dropus_range_decoder rd;

    DROPUS_ASSERT(pOpusStream != NULL);
    DROPUS_ASSERT(pOpusFrame  != NULL);
    DROPUS_ASSERT(pData       != NULL);
    DROPUS_ASSERT(dataSize    <= DROPUS_MAX_FRAME_SIZE_IN_BYTES);

    pOpusFrame->sizeInBytes = (dropus_uint16)dataSize;  /* Safe cast because dataSize <= DROPUS_MAX_FRAME_SIZE_IN_BYTES <= 1275. */

    /* Everything is fed through the range decoder. */
    dropus_range_decoder_init(pData, pOpusFrame->sizeInBytes, &rd);
    {

    }

    return DROPUS_SUCCESS;
}

static dropus_result dropus_stream_decode_frame__hybrid(dropus_stream* pOpusStream, dropus_stream_frame* pOpusFrame, const dropus_uint8* pData, size_t dataSize)
{
    dropus_range_decoder rd;

    DROPUS_ASSERT(pOpusStream != NULL);
    DROPUS_ASSERT(pOpusFrame  != NULL);
    DROPUS_ASSERT(pData       != NULL);
    DROPUS_ASSERT(dataSize    <= DROPUS_MAX_FRAME_SIZE_IN_BYTES);

    pOpusFrame->sizeInBytes = (dropus_uint16)dataSize;  /* Safe cast because dataSize <= DROPUS_MAX_FRAME_SIZE_IN_BYTES <= 1275. */

    /* Everything is fed through the range decoder. */
    dropus_range_decoder_init(pData, pOpusFrame->sizeInBytes, &rd);
    {

    }

    return DROPUS_SUCCESS;
}

DROPUS_API dropus_result dropus_stream_decode_packet(dropus_stream* pOpusStream, const void* pData, size_t dataSize)
{
    const dropus_uint8* pRunningData8 = (const dropus_uint8*)pData;
    dropus_uint8  toc; /* Table of Contents byte. */
    dropus_uint8  opusFrameCount;
    dropus_uint16 opusFrameSizes[DROPUS_MAX_OPUS_FRAMES_PER_PACKET];
    dropus_uint32 code; /* Determines the structure of the Opus packet. */

    if (pOpusStream == NULL || pData == NULL) {
        return DROPUS_INVALID_ARGS;
    }

    DROPUS_ASSERT(DROPUS_MAX_PACKET_SIZE_IN_BYTES < 65536);
    if (dataSize > DROPUS_MAX_PACKET_SIZE_IN_BYTES) {
        return DROPUS_BAD_DATA;
    }

    /* RFC 6716 - Section 3.4 [R1] Packets are at least one byte. */
    if (dataSize < 1) {
        return DROPUS_BAD_DATA;
    }

    /* The TOC byte specifies the structure of the packet. */
    toc = pRunningData8[0];
    pRunningData8 += 1;

    /*
    We need to look at the code to know the frames making up the packet are structured. We will do a pre-processing step to
    extract basic information about each frame in the packet.
    */
    code = dropus_toc_c(toc);
    switch (code) {
        case 0: /* RFC 6716 - Section 3.2.2. Code 0: One Frame in the Packet */
        {
            dropus_uint16 opusFrameSize = (dropus_uint16)(dataSize-1);

            /* RFC 6716 - Section 3.4 [R2] No implicit frame length is larger than 1275 bytes. */
            if (opusFrameSize > DROPUS_MAX_FRAME_SIZE_IN_BYTES) {
                return DROPUS_BAD_DATA;
            }

            opusFrameCount = 1;
            opusFrameSizes[0] = opusFrameSize;
        } break;

        case 1: /* RFC 6716 - Section 3.2.3. Code 1: Two Frames in the Packet, Each with Equal Compressed Size */
        {
            dropus_uint16 opusFrameSize;

            /* RFC 6716 - Section 3.4 [R3] Code 1 packets have an odd total length, N, so that (N-1)/2 is an integer. */
            if ((dataSize & 1) == 0) {
                return DROPUS_BAD_DATA;
            }

            opusFrameSize = (dropus_uint16)(dataSize-1)/2;

            /* RFC 6716 - Section 3.4 [R2] No implicit frame length is larger than 1275 bytes. */
            if (opusFrameSize > DROPUS_MAX_FRAME_SIZE_IN_BYTES) {
                return DROPUS_BAD_DATA;
            }

            opusFrameCount = 2;
            opusFrameSizes[0] = opusFrameSize;
            opusFrameSizes[1] = opusFrameSize;
        } break;

        case 2: /* RFC 6716 - Section 3.2.4. Code 2: Two Frames in the Packet, with Different Compressed Sizes */
        {
            dropus_uint8 byte0;
            dropus_uint8 byte1;
            dropus_uint16 opusFrameSize0 = 0;
            dropus_uint16 opusFrameSize1 = 0;
            dropus_uint16 headerByteCount;

            /* RFC 6716 - Section 3.4 [R4] Code 2 packets have enough bytes after the TOC for a valid frame length, and that length is no larger than the number of bytes remaining in the packet. */
            if (dataSize < 2) {
                return DROPUS_BAD_DATA;
            }

            /* RFC 6716 - Section 3.2.1. Frame Length Coding */
            byte0 = pRunningData8[0]; pRunningData8 += 1;
            if (byte0 == 0) {
                /*
                Section 3.2.1 of RFC 6716 says the following:

                    "Any Opus frame in any mode MAY have a length of 0.
                
                This implies to me that this is a valid case. dr_opus is going to handle this by setting the PCM frame count to 0 for this packet.
                */
                opusFrameSize0 = 0;
                opusFrameSize1 = 0;
            } else {
                if (byte0 >= 1 && byte0 <= 251) {
                    opusFrameSize0 = byte0;
                }
                if (byte0 >= 252 /*&& byte0 <= 255*/) {
                    /* RFC 6716 - Section 3.4 [R4] Code 2 packets have enough bytes after the TOC for a valid frame length, and that length is no larger than the number of bytes remaining in the packet. */
                    if (dataSize < 3) {
                        return DROPUS_BAD_DATA;
                    }

                    byte1 = pRunningData8[0]; pRunningData8 += 1;
                    opusFrameSize0 = (byte1*4) + byte0;
                }

                headerByteCount = (dropus_uint16)(pRunningData8 - (const dropus_uint8*)pData);   /* This is a safe case because the maximum difference will be 3. */

                /* RFC 6716 - Section 3.4 [R2] No implicit frame length is larger than 1275 bytes. */
                if (opusFrameSize0 > DROPUS_MAX_FRAME_SIZE_IN_BYTES) {
                    return DROPUS_BAD_DATA;
                }

                /* RFC 6716 - Section 3.4 [R4] Code 2 packets have enough bytes after the TOC for a valid frame length, and that length is no larger than the number of bytes remaining in the packet. */
                if ((dataSize-headerByteCount) > dataSize) {
                    return DROPUS_BAD_DATA;
                }

                opusFrameSize1 = (dropus_uint16)(dataSize-headerByteCount-opusFrameSize0);    /* Safe cast because dataSize is guaranteed to be < 65536 at this point since it was checked at the top of this function. */

                /* RFC 6716 - Section 3.4 [R2] No implicit frame length is larger than 1275 bytes. */
                if (opusFrameSize1 > DROPUS_MAX_FRAME_SIZE_IN_BYTES) {
                    return DROPUS_BAD_DATA;
                }

                /* RFC 6716 - Section 3.4 [R4] Code 2 packets have enough bytes after the TOC for a valid frame length, and that length is no larger than the number of bytes remaining in the packet. */
                if ((size_t)(headerByteCount + opusFrameSize0 + opusFrameSize1) > dataSize) {
                    return DROPUS_BAD_DATA;
                }
            }
            
            opusFrameCount = 2;
            opusFrameSizes[0] = opusFrameSize0;
            opusFrameSizes[1] = opusFrameSize1;
        } break;

        case 3: /* RFC 6716 - Section 3.2.5. Code 3: A Signaled Number of Frames in the Packet */
        {
            dropus_uint16 iFrame;
            dropus_uint8  frameCountByte;
            dropus_uint8  v;                /* Is VBR? */
            dropus_uint8  p;                /* Has padding? */
            dropus_uint8  M;                /* Frame count. */
            dropus_uint16 P;                /* The size of the padding. Must never be more than dataSize-2. */
            dropus_uint16 R;                /* The number of bytes remaining in the packet after subtracting the TOC, frame count byte and padding. */
            dropus_uint32 ms;               /* Total length in milliseconds. */
            dropus_uint32 paddingByteCount; /* The number of bytes making up the size of the padding. Only used for validation. */
            
            /*
            RFC 6716 - Section 3.2.5:
                "Code 3 packets MUST have at least 2 bytes [R6,R7]."
            */
            if (dataSize < 2) {
                return DROPUS_BAD_DATA;
            }

            frameCountByte = pRunningData8[0]; pRunningData8 += 1;
            v = (frameCountByte & 0x80) >> 7;
            p = (frameCountByte & 0x40) >> 6;
            M = (frameCountByte & 0x3F);

            /* RFC 6716 - Section 3.4 [R5] Code 3 packets contain at least one frame, but no more than 120 ms of audio total. */
            ms = (M * dropus_toc_frame_size_in_pcm_frames(toc)) / dropus_toc_sample_rate_ms(toc);
            if (M < 1 || ms > 120) {
                return DROPUS_BAD_DATA;
            }

            /* Sanity check to ensure the frame count is never greather than the maximum allowed. */
            if (M > DROPUS_MAX_OPUS_FRAMES_PER_PACKET)  {
                return DROPUS_BAD_DATA;
            }

            /* Padding bytes. Need to run this in a loop. */
            P = 0;
            paddingByteCount = 0;
            if (p != 0) {
                size_t iPaddingByte;
                for (iPaddingByte = 0; iPaddingByte < dataSize-2; ++iPaddingByte) {
                    dropus_uint8 paddingByte = pRunningData8[0]; pRunningData8 += 1;
                    P += paddingByte;
                    paddingByteCount += 1;

                    /* A padding byte not equal to 255 signals the last padding byte. */
                    if (paddingByte == 255) {
                        /* There must be an additional byte available in this case. */
                        if (iPaddingByte+1 >= dataSize-2) {
                            return DROPUS_BAD_DATA;
                        }
                    } else {
                        break;  /* Reached the end of the padding bytes. */
                    }
                }
            }

            /* Safety check. */
            if (P > dataSize-2) {
                return DROPUS_BAD_DATA;
            }

            /* R = bytes remaining. */
            R = (dropus_uint16)(dataSize-2-P);

            if (v == 0) {
                /* CBR */
                dropus_uint16 frameSize = R/M;

                /* RFC 6716 - Section 3.4 [R2] No implicit frame length is larger than 1275 bytes. */
                if (frameSize > DROPUS_MAX_FRAME_SIZE_IN_BYTES) {
                    return DROPUS_BAD_DATA;
                }

                /* RFC 6716 - Section 3.4 [R6] ... */
                if (dataSize < 2) {                                     /* ... The length of a CBR code 3 packet, N, is at least two bytes ... */
                    return DROPUS_BAD_DATA;
                }
                if (paddingByteCount+P > dataSize-2) {                  /* ... the number of bytes added to indicate the padding size plus the trailing padding bytes themselves, P, is no more than N-2 ... */
                    return DROPUS_BAD_DATA;
                }
                if (frameSize*M != (dropus_uint16)(dataSize-2-P)) {     /* ... the frame count, M, satisfies the constraint that (N-2-P) is a non-negative integer multiple of M ... */
                    return DROPUS_BAD_DATA;
                }

                opusFrameCount = M;
                for (iFrame = 0; iFrame < opusFrameCount; ++iFrame) {
                    opusFrameSizes[iFrame] = frameSize;
                }
            } else {
                /* VBR */
                dropus_uint16 totalFrameSizeExceptLast = 0; /* Used later for checking [R7]. */
                dropus_uintptr headerSizeInBytes;           /* For validation and deriving the size of the last frame. */

                opusFrameCount = M;
                for (iFrame = 0; iFrame < opusFrameCount-1; ++iFrame) {
                    dropus_uint8 byte0;
                    dropus_uint8 byte1;

                    if (pRunningData8 >= ((const dropus_uint8*)pData) + dataSize) {
                        return DROPUS_BAD_DATA; /* Ran out of data in the packet. Implicitly handles part of [R7]. */
                    }

                    byte0 = pRunningData8[0]; pRunningData8 += 1;
                    if (byte0 == 0) {
                        opusFrameSizes[iFrame] = 0;
                    } else {
                        if (byte0 >= 1 && byte0 <= 251) {
                            opusFrameSizes[iFrame] = byte0;
                        }
                        if (byte0 >= 252 /*&& byte0 <= 255*/) {
                            if (pRunningData8 >= ((const dropus_uint8*)pData) + dataSize) {
                                return DROPUS_BAD_DATA; /* Ran out of data in the packet. Implicitly handles part of [R7]. */
                            }

                            byte1 = pRunningData8[0]; pRunningData8 += 1;
                            opusFrameSizes[iFrame] = (byte1*4) + byte0;

                            /* RFC 6716 - Section 3.4 [R2] No implicit frame length is larger than 1275 bytes. */
                            if (opusFrameSizes[iFrame] > DROPUS_MAX_FRAME_SIZE_IN_BYTES) {
                                return DROPUS_BAD_DATA;
                            }
                        }
                    }

                    totalFrameSizeExceptLast += opusFrameSizes[iFrame];
                }

                headerSizeInBytes = (dropus_uintptr)(pRunningData8 - (const dropus_uint8*)pData);

                /*
                RFC 6716 - Section 3.4 [R6]
                    VBR code 3 packets are large enough to contain all the header
                    bytes (TOC byte, frame count byte, any padding length bytes,
                    and any frame length bytes), plus the length of the first M-1
                    frames, plus any trailing padding bytes.
                */
                if ((headerSizeInBytes + totalFrameSizeExceptLast + P) > dataSize) {
                    return DROPUS_BAD_DATA;
                }

                /* The size of the last frame is derived. */
                opusFrameSizes[opusFrameCount-1] = (dropus_uint16)(dataSize - headerSizeInBytes - totalFrameSizeExceptLast - P); /* Safe cast thanks to the myriad of validation done beforehand. */

                /* RFC 6716 - Section 3.4 [R2] No implicit frame length is larger than 1275 bytes. */
                if (opusFrameSizes[opusFrameCount-1] > DROPUS_MAX_FRAME_SIZE_IN_BYTES) {
                    return DROPUS_BAD_DATA;
                }
            }
        } break;

        /* Will never hit this, but need the default to keep some compilers quiet. */
        default: return DROPUS_BAD_DATA;
    }

    pOpusStream->packet.toc = toc;

    /* Make sure the frame count is within a valid range. */
    if (opusFrameCount > DROPUS_MAX_OPUS_FRAMES_PER_PACKET) {
        return DROPUS_BAD_DATA;
    }

    /* At this point, pRunningData8 should be sitting on the first byte of the first frame in the packet. */

    /* Decoding. */
    {
        dropus_result result;
        dropus_uint16 iFrame;
        dropus_mode mode;

        mode = dropus_toc_mode(pOpusStream->packet.toc);

        /* Would be slightly more optimal to pull the mode checks out of the loop, but it's more compact, readable and maintainable to do it inside. We can let the compiler decide what to do with it. */
        for (iFrame = 0; iFrame < opusFrameCount; ++iFrame) {
            /*  */ if (mode == dropus_mode_silk) {
                result = dropus_stream_decode_frame__silk(pOpusStream, &pOpusStream->packet.frames[iFrame], pRunningData8, opusFrameSizes[iFrame]);
            } else if (mode == dropus_mode_celt) {
                result = dropus_stream_decode_frame__celt(pOpusStream, &pOpusStream->packet.frames[iFrame], pRunningData8, opusFrameSizes[iFrame]);
            } else {
                DROPUS_ASSERT(mode == dropus_mode_hybrid);
                result = dropus_stream_decode_frame__hybrid(pOpusStream, &pOpusStream->packet.frames[iFrame], pRunningData8, opusFrameSizes[iFrame]);
            }

            if (result != DROPUS_SUCCESS) {
                return result;  /* Probably a corrupt frame. */
            }

            pRunningData8 += opusFrameSizes[iFrame];
        }
    }

    return DROPUS_SUCCESS;
}



/***********************************************************************************************************************************************************

High-Level Opus Decoding API

************************************************************************************************************************************************************/
static void* dropus__malloc_default(size_t sz, void* pUserData)
{
    (void)pUserData;
    return DROPUS_MALLOC(sz);
}

static void* dropus__realloc_default(void* p, size_t sz, void* pUserData)
{
    (void)pUserData;
    return DROPUS_REALLOC(p, sz);
}

static void dropus__free_default(void* p, void* pUserData)
{
    (void)pUserData;
    DROPUS_FREE(p);
}


static void* dropus__malloc_from_callbacks(size_t sz, const dropus_allocation_callbacks* pAllocationCallbacks)
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

static void* dropus__realloc_from_callbacks(void* p, size_t szNew, size_t szOld, const dropus_allocation_callbacks* pAllocationCallbacks)
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
            DROPUS_COPY_MEMORY(p2, p, szOld);
            pAllocationCallbacks->onFree(p, pAllocationCallbacks->pUserData);
        }

        return p2;
    }

    return NULL;
}

static void dropus__free_from_callbacks(void* p, const dropus_allocation_callbacks* pAllocationCallbacks)
{
    if (p == NULL || pAllocationCallbacks == NULL) {
        return;
    }

    if (pAllocationCallbacks->onFree != NULL) {
        pAllocationCallbacks->onFree(p, pAllocationCallbacks->pUserData);
    }
}


static dropus_allocation_callbacks dropus__copy_allocation_callbacks_or_defaults(const dropus_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks != NULL) {
        /* Copy. */
        return *pAllocationCallbacks;
    } else {
        /* Defaults. */
        dropus_allocation_callbacks allocationCallbacks;
        allocationCallbacks.pUserData = NULL;
        allocationCallbacks.onMalloc  = dropus__malloc_default;
        allocationCallbacks.onRealloc = dropus__realloc_default;
        allocationCallbacks.onFree    = dropus__free_default;
        return allocationCallbacks;
    }
}

static dropus_result dropus_init_internal(dropus* pOpus, dropus_read_proc onRead, dropus_seek_proc onSeek, void* pUserData, const dropus_allocation_callbacks* pAllocationCallbacks)
{
    DROPUS_ASSERT(pOpus != NULL);
    DROPUS_ASSERT(onRead != NULL);

    /* Must always have an onRead callback. */
    if (onRead == NULL) {
        return DROPUS_INVALID_ARGS;
    }

    pOpus->onRead              = onRead;
    pOpus->onSeek              = onSeek;
    pOpus->pUserData           = pUserData;
    pOpus->allocationCallbacks = dropus__copy_allocation_callbacks_or_defaults(pAllocationCallbacks);

    /* Basic validation for allocation callbacks. free() and at least one of malloc() or realloc() must be set. */
    if (pOpus->allocationCallbacks.onFree == NULL || (pOpus->allocationCallbacks.onMalloc == NULL && pOpus->allocationCallbacks.onRealloc == NULL)) {
        return DROPUS_INVALID_ARGS;
    }

    /* TODO: Implement me. */
    
    return DROPUS_SUCCESS;
}

DROPUS_API dropus_result dropus_init(dropus* pOpus, dropus_read_proc onRead, dropus_seek_proc onSeek, void* pUserData, const dropus_allocation_callbacks* pAllocationCallbacks)
{
    if (pOpus == NULL) {
        return DROPUS_INVALID_ARGS;
    }

    DROPUS_ZERO_OBJECT(pOpus);

    return dropus_init_internal(pOpus, onRead, onSeek, pUserData, pAllocationCallbacks);
}

#ifndef DR_OPUS_NO_STDIO
#include <errno.h>
static dropus_result dropus_result_from_errno(int e)
{
    switch (e)
    {
        case 0: return DROPUS_SUCCESS;
    #ifdef EPERM
        case EPERM: return DROPUS_INVALID_OPERATION;
    #endif
    #ifdef ENOENT
        case ENOENT: return DROPUS_DOES_NOT_EXIST;
    #endif
    #ifdef ESRCH
        case ESRCH: return DROPUS_DOES_NOT_EXIST;
    #endif
    #ifdef EINTR
        case EINTR: return DROPUS_INTERRUPT;
    #endif
    #ifdef EIO
        case EIO: return DROPUS_IO_ERROR;
    #endif
    #ifdef ENXIO
        case ENXIO: return DROPUS_DOES_NOT_EXIST;
    #endif
    #ifdef E2BIG
        case E2BIG: return DROPUS_INVALID_ARGS;
    #endif
    #ifdef ENOEXEC
        case ENOEXEC: return DROPUS_INVALID_FILE;
    #endif
    #ifdef EBADF
        case EBADF: return DROPUS_INVALID_FILE;
    #endif
    #ifdef ECHILD
        case ECHILD: return DROPUS_ERROR;
    #endif
    #ifdef EAGAIN
        case EAGAIN: return DROPUS_UNAVAILABLE;
    #endif
    #ifdef ENOMEM
        case ENOMEM: return DROPUS_OUT_OF_MEMORY;
    #endif
    #ifdef EACCES
        case EACCES: return DROPUS_ACCESS_DENIED;
    #endif
    #ifdef EFAULT
        case EFAULT: return DROPUS_BAD_ADDRESS;
    #endif
    #ifdef ENOTBLK
        case ENOTBLK: return DROPUS_ERROR;
    #endif
    #ifdef EBUSY
        case EBUSY: return DROPUS_BUSY;
    #endif
    #ifdef EEXIST
        case EEXIST: return DROPUS_ALREADY_EXISTS;
    #endif
    #ifdef EXDEV
        case EXDEV: return DROPUS_ERROR;
    #endif
    #ifdef ENODEV
        case ENODEV: return DROPUS_DOES_NOT_EXIST;
    #endif
    #ifdef ENOTDIR
        case ENOTDIR: return DROPUS_NOT_DIRECTORY;
    #endif
    #ifdef EISDIR
        case EISDIR: return DROPUS_IS_DIRECTORY;
    #endif
    #ifdef EINVAL
        case EINVAL: return DROPUS_INVALID_ARGS;
    #endif
    #ifdef ENFILE
        case ENFILE: return DROPUS_TOO_MANY_OPEN_FILES;
    #endif
    #ifdef EMFILE
        case EMFILE: return DROPUS_TOO_MANY_OPEN_FILES;
    #endif
    #ifdef ENOTTY
        case ENOTTY: return DROPUS_INVALID_OPERATION;
    #endif
    #ifdef ETXTBSY
        case ETXTBSY: return DROPUS_BUSY;
    #endif
    #ifdef EFBIG
        case EFBIG: return DROPUS_TOO_BIG;
    #endif
    #ifdef ENOSPC
        case ENOSPC: return DROPUS_NO_SPACE;
    #endif
    #ifdef ESPIPE
        case ESPIPE: return DROPUS_BAD_SEEK;
    #endif
    #ifdef EROFS
        case EROFS: return DROPUS_ACCESS_DENIED;
    #endif
    #ifdef EMLINK
        case EMLINK: return DROPUS_TOO_MANY_LINKS;
    #endif
    #ifdef EPIPE
        case EPIPE: return DROPUS_BAD_PIPE;
    #endif
    #ifdef EDOM
        case EDOM: return DROPUS_OUT_OF_RANGE;
    #endif
    #ifdef ERANGE
        case ERANGE: return DROPUS_OUT_OF_RANGE;
    #endif
    #ifdef EDEADLK
        case EDEADLK: return DROPUS_DEADLOCK;
    #endif
    #ifdef ENAMETOOLONG
        case ENAMETOOLONG: return DROPUS_PATH_TOO_LONG;
    #endif
    #ifdef ENOLCK
        case ENOLCK: return DROPUS_ERROR;
    #endif
    #ifdef ENOSYS
        case ENOSYS: return DROPUS_NOT_IMPLEMENTED;
    #endif
    #ifdef ENOTEMPTY
        case ENOTEMPTY: return DROPUS_DIRECTORY_NOT_EMPTY;
    #endif
    #ifdef ELOOP
        case ELOOP: return DROPUS_TOO_MANY_LINKS;
    #endif
    #ifdef ENOMSG
        case ENOMSG: return DROPUS_NO_MESSAGE;
    #endif
    #ifdef EIDRM
        case EIDRM: return DROPUS_ERROR;
    #endif
    #ifdef ECHRNG
        case ECHRNG: return DROPUS_ERROR;
    #endif
    #ifdef EL2NSYNC
        case EL2NSYNC: return DROPUS_ERROR;
    #endif
    #ifdef EL3HLT
        case EL3HLT: return DROPUS_ERROR;
    #endif
    #ifdef EL3RST
        case EL3RST: return DROPUS_ERROR;
    #endif
    #ifdef ELNRNG
        case ELNRNG: return DROPUS_OUT_OF_RANGE;
    #endif
    #ifdef EUNATCH
        case EUNATCH: return DROPUS_ERROR;
    #endif
    #ifdef ENOCSI
        case ENOCSI: return DROPUS_ERROR;
    #endif
    #ifdef EL2HLT
        case EL2HLT: return DROPUS_ERROR;
    #endif
    #ifdef EBADE
        case EBADE: return DROPUS_ERROR;
    #endif
    #ifdef EBADR
        case EBADR: return DROPUS_ERROR;
    #endif
    #ifdef EXFULL
        case EXFULL: return DROPUS_ERROR;
    #endif
    #ifdef ENOANO
        case ENOANO: return DROPUS_ERROR;
    #endif
    #ifdef EBADRQC
        case EBADRQC: return DROPUS_ERROR;
    #endif
    #ifdef EBADSLT
        case EBADSLT: return DROPUS_ERROR;
    #endif
    #ifdef EBFONT
        case EBFONT: return DROPUS_INVALID_FILE;
    #endif
    #ifdef ENOSTR
        case ENOSTR: return DROPUS_ERROR;
    #endif
    #ifdef ENODATA
        case ENODATA: return DROPUS_NO_DATA_AVAILABLE;
    #endif
    #ifdef ETIME
        case ETIME: return DROPUS_TIMEOUT;
    #endif
    #ifdef ENOSR
        case ENOSR: return DROPUS_NO_DATA_AVAILABLE;
    #endif
    #ifdef ENONET
        case ENONET: return DROPUS_NO_NETWORK;
    #endif
    #ifdef ENOPKG
        case ENOPKG: return DROPUS_ERROR;
    #endif
    #ifdef EREMOTE
        case EREMOTE: return DROPUS_ERROR;
    #endif
    #ifdef ENOLINK
        case ENOLINK: return DROPUS_ERROR;
    #endif
    #ifdef EADV
        case EADV: return DROPUS_ERROR;
    #endif
    #ifdef ESRMNT
        case ESRMNT: return DROPUS_ERROR;
    #endif
    #ifdef ECOMM
        case ECOMM: return DROPUS_ERROR;
    #endif
    #ifdef EPROTO
        case EPROTO: return DROPUS_ERROR;
    #endif
    #ifdef EMULTIHOP
        case EMULTIHOP: return DROPUS_ERROR;
    #endif
    #ifdef EDOTDOT
        case EDOTDOT: return DROPUS_ERROR;
    #endif
    #ifdef EBADMSG
        case EBADMSG: return DROPUS_BAD_MESSAGE;
    #endif
    #ifdef EOVERFLOW
        case EOVERFLOW: return DROPUS_TOO_BIG;
    #endif
    #ifdef ENOTUNIQ
        case ENOTUNIQ: return DROPUS_NOT_UNIQUE;
    #endif
    #ifdef EBADFD
        case EBADFD: return DROPUS_ERROR;
    #endif
    #ifdef EREMCHG
        case EREMCHG: return DROPUS_ERROR;
    #endif
    #ifdef ELIBACC
        case ELIBACC: return DROPUS_ACCESS_DENIED;
    #endif
    #ifdef ELIBBAD
        case ELIBBAD: return DROPUS_INVALID_FILE;
    #endif
    #ifdef ELIBSCN
        case ELIBSCN: return DROPUS_INVALID_FILE;
    #endif
    #ifdef ELIBMAX
        case ELIBMAX: return DROPUS_ERROR;
    #endif
    #ifdef ELIBEXEC
        case ELIBEXEC: return DROPUS_ERROR;
    #endif
    #ifdef EILSEQ
        case EILSEQ: return DROPUS_INVALID_DATA;
    #endif
    #ifdef ERESTART
        case ERESTART: return DROPUS_ERROR;
    #endif
    #ifdef ESTRPIPE
        case ESTRPIPE: return DROPUS_ERROR;
    #endif
    #ifdef EUSERS
        case EUSERS: return DROPUS_ERROR;
    #endif
    #ifdef ENOTSOCK
        case ENOTSOCK: return DROPUS_NOT_SOCKET;
    #endif
    #ifdef EDESTADDRREQ
        case EDESTADDRREQ: return DROPUS_NO_ADDRESS;
    #endif
    #ifdef EMSGSIZE
        case EMSGSIZE: return DROPUS_TOO_BIG;
    #endif
    #ifdef EPROTOTYPE
        case EPROTOTYPE: return DROPUS_BAD_PROTOCOL;
    #endif
    #ifdef ENOPROTOOPT
        case ENOPROTOOPT: return DROPUS_PROTOCOL_UNAVAILABLE;
    #endif
    #ifdef EPROTONOSUPPORT
        case EPROTONOSUPPORT: return DROPUS_PROTOCOL_NOT_SUPPORTED;
    #endif
    #ifdef ESOCKTNOSUPPORT
        case ESOCKTNOSUPPORT: return DROPUS_SOCKET_NOT_SUPPORTED;
    #endif
    #ifdef EOPNOTSUPP
        case EOPNOTSUPP: return DROPUS_INVALID_OPERATION;
    #endif
    #ifdef EPFNOSUPPORT
        case EPFNOSUPPORT: return DROPUS_PROTOCOL_FAMILY_NOT_SUPPORTED;
    #endif
    #ifdef EAFNOSUPPORT
        case EAFNOSUPPORT: return DROPUS_ADDRESS_FAMILY_NOT_SUPPORTED;
    #endif
    #ifdef EADDRINUSE
        case EADDRINUSE: return DROPUS_ALREADY_IN_USE;
    #endif
    #ifdef EADDRNOTAVAIL
        case EADDRNOTAVAIL: return DROPUS_ERROR;
    #endif
    #ifdef ENETDOWN
        case ENETDOWN: return DROPUS_NO_NETWORK;
    #endif
    #ifdef ENETUNREACH
        case ENETUNREACH: return DROPUS_NO_NETWORK;
    #endif
    #ifdef ENETRESET
        case ENETRESET: return DROPUS_NO_NETWORK;
    #endif
    #ifdef ECONNABORTED
        case ECONNABORTED: return DROPUS_NO_NETWORK;
    #endif
    #ifdef ECONNRESET
        case ECONNRESET: return DROPUS_CONNECTION_RESET;
    #endif
    #ifdef ENOBUFS
        case ENOBUFS: return DROPUS_NO_SPACE;
    #endif
    #ifdef EISCONN
        case EISCONN: return DROPUS_ALREADY_CONNECTED;
    #endif
    #ifdef ENOTCONN
        case ENOTCONN: return DROPUS_NOT_CONNECTED;
    #endif
    #ifdef ESHUTDOWN
        case ESHUTDOWN: return DROPUS_ERROR;
    #endif
    #ifdef ETOOMANYREFS
        case ETOOMANYREFS: return DROPUS_ERROR;
    #endif
    #ifdef ETIMEDOUT
        case ETIMEDOUT: return DROPUS_TIMEOUT;
    #endif
    #ifdef ECONNREFUSED
        case ECONNREFUSED: return DROPUS_CONNECTION_REFUSED;
    #endif
    #ifdef EHOSTDOWN
        case EHOSTDOWN: return DROPUS_NO_HOST;
    #endif
    #ifdef EHOSTUNREACH
        case EHOSTUNREACH: return DROPUS_NO_HOST;
    #endif
    #ifdef EALREADY
        case EALREADY: return DROPUS_IN_PROGRESS;
    #endif
    #ifdef EINPROGRESS
        case EINPROGRESS: return DROPUS_IN_PROGRESS;
    #endif
    #ifdef ESTALE
        case ESTALE: return DROPUS_INVALID_FILE;
    #endif
    #ifdef EUCLEAN
        case EUCLEAN: return DROPUS_ERROR;
    #endif
    #ifdef ENOTNAM
        case ENOTNAM: return DROPUS_ERROR;
    #endif
    #ifdef ENAVAIL
        case ENAVAIL: return DROPUS_ERROR;
    #endif
    #ifdef EISNAM
        case EISNAM: return DROPUS_ERROR;
    #endif
    #ifdef EREMOTEIO
        case EREMOTEIO: return DROPUS_IO_ERROR;
    #endif
    #ifdef EDQUOT
        case EDQUOT: return DROPUS_NO_SPACE;
    #endif
    #ifdef ENOMEDIUM
        case ENOMEDIUM: return DROPUS_DOES_NOT_EXIST;
    #endif
    #ifdef EMEDIUMTYPE
        case EMEDIUMTYPE: return DROPUS_ERROR;
    #endif
    #ifdef ECANCELED
        case ECANCELED: return DROPUS_CANCELLED;
    #endif
    #ifdef ENOKEY
        case ENOKEY: return DROPUS_ERROR;
    #endif
    #ifdef EKEYEXPIRED
        case EKEYEXPIRED: return DROPUS_ERROR;
    #endif
    #ifdef EKEYREVOKED
        case EKEYREVOKED: return DROPUS_ERROR;
    #endif
    #ifdef EKEYREJECTED
        case EKEYREJECTED: return DROPUS_ERROR;
    #endif
    #ifdef EOWNERDEAD
        case EOWNERDEAD: return DROPUS_ERROR;
    #endif
    #ifdef ENOTRECOVERABLE
        case ENOTRECOVERABLE: return DROPUS_ERROR;
    #endif
    #ifdef ERFKILL
        case ERFKILL: return DROPUS_ERROR;
    #endif
    #ifdef EHWPOISON
        case EHWPOISON: return DROPUS_ERROR;
    #endif
        default: return DROPUS_ERROR;
    }
}

static dropus_result dropus_fopen(FILE** ppFile, const char* pFilePath, const char* pOpenMode)
{
#if _MSC_VER && _MSC_VER >= 1400
    errno_t err;
#endif

    if (ppFile != NULL) {
        *ppFile = NULL;  /* Safety. */
    }

    if (pFilePath == NULL || pOpenMode == NULL || ppFile == NULL) {
        return DROPUS_INVALID_ARGS;
    }

#if _MSC_VER && _MSC_VER >= 1400
    err = fopen_s(ppFile, pFilePath, pOpenMode);
    if (err != 0) {
        return dropus_result_from_errno(err);
    }
#else
#if defined(_WIN32) || defined(__APPLE__)
    *ppFile = fopen(pFilePath, pOpenMode);
#else
    #if defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64 && defined(_LARGEFILE64_SOURCE)
        *ppFile = fopen64(pFilePath, pOpenMode);
    #else
        *ppFile = fopen(pFilePath, pOpenMode);
    #endif
#endif
    if (*ppFile == NULL) {
        dropus_result result = dropus_result_from_errno(errno);
        if (result == DROPUS_SUCCESS) {
            result = DROPUS_ERROR;   /* Just a safety check to make sure we never ever return success when pFile == NULL. */
        }

        return result;
    }
#endif

    return DROPUS_SUCCESS;
}

/*
_wfopen() isn't always available in all compilation environments.

    * Windows only.
    * MSVC seems to support it universally as far back as VC6 from what I can tell (haven't checked further back).
    * MinGW-64 (both 32- and 64-bit) seems to support it.
    * MinGW wraps it in !defined(__STRICT_ANSI__).

This can be reviewed as compatibility issues arise. The preference is to use _wfopen_s() and _wfopen() as opposed to the wcsrtombs()
fallback, so if you notice your compiler not detecting this properly I'm happy to look at adding support.
*/
#if defined(_WIN32)
    #if defined(_MSC_VER) || defined(__MINGW64__) || !defined(__STRICT_ANSI__)
        #define DROPUS_HAS_WFOPEN
    #endif
#endif

static dropus_result dropus_wfopen(FILE** ppFile, const wchar_t* pFilePath, const wchar_t* pOpenMode, const dropus_allocation_callbacks* pAllocationCallbacks)
{
    if (ppFile != NULL) {
        *ppFile = NULL;  /* Safety. */
    }

    if (pFilePath == NULL || pOpenMode == NULL || ppFile == NULL) {
        return DROPUS_INVALID_ARGS;
    }

#if defined(DROPUS_HAS_WFOPEN)
    {
        /* Use _wfopen() on Windows. */
    #if defined(_MSC_VER) && _MSC_VER >= 1400
        errno_t err = _wfopen_s(ppFile, pFilePath, pOpenMode);
        if (err != 0) {
            return dropus_result_from_errno(err);
        }
    #else
        *ppFile = _wfopen(pFilePath, pOpenMode);
        if (*ppFile == NULL) {
            return dropus_result_from_errno(errno);
        }
    #endif
        (void)pAllocationCallbacks;
    }
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
        char pOpenModeMB[32] = {0};

        /* Get the length first. */
        DROPUS_ZERO_OBJECT(&mbs);
        lenMB = wcsrtombs(NULL, &pFilePathTemp, 0, &mbs);
        if (lenMB == (size_t)-1) {
            return dropus_result_from_errno(errno);
        }

        pFilePathMB = (char*)dropus__malloc_from_callbacks(lenMB + 1, pAllocationCallbacks);
        if (pFilePathMB == NULL) {
            return DROPUS_OUT_OF_MEMORY;
        }

        pFilePathTemp = pFilePath;
        DROPUS_ZERO_OBJECT(&mbs);
        wcsrtombs(pFilePathMB, &pFilePathTemp, lenMB + 1, &mbs);

        /* The open mode should always consist of ASCII characters so we should be able to do a trivial conversion. */
        {
            size_t i = 0;
            for (;;) {
                if (pOpenMode[i] == 0) {
                    pOpenModeMB[i] = '\0';
                    break;
                }

                pOpenModeMB[i] = (char)pOpenMode[i];
                i += 1;
            }
        }

        *ppFile = fopen(pFilePathMB, pOpenModeMB);

        dropus__free_from_callbacks(pFilePathMB, pAllocationCallbacks);
    }

    if (*ppFile == NULL) {
        return DROPUS_ERROR;
    }
#endif

    return DROPUS_SUCCESS;
}


static size_t dropus_on_read_stdio(void* pUserData, void* pBufferOut, size_t bytesToRead)
{
    return fread(pBufferOut, 1, bytesToRead, (FILE*)pUserData);
}

static dropus_bool32 dropus_on_seek_stdio(void* pUserData, int offset, dropus_seek_origin origin)
{
    return fseek((FILE*)pUserData, offset, (origin == dropus_seek_origin_current) ? SEEK_CUR : SEEK_SET) == 0;
}

DROPUS_API dropus_result dropus_init_file(dropus* pOpus, const char* pFilePath, const dropus_allocation_callbacks* pAllocationCallbacks)
{
    dropus_result result;
    FILE* pFile;

    if (pOpus == NULL) {
        return DROPUS_INVALID_ARGS;
    }

    DROPUS_ZERO_OBJECT(pOpus);

    if (pFilePath == NULL || pFilePath[0] == '\0') {
        return DROPUS_INVALID_ARGS;
    }

    result = dropus_fopen(&pFile, pFilePath, "rb");
    if (result != DROPUS_SUCCESS) {
        return result;
    }

    pOpus->pFile = (void*)pFile;

    result = dropus_init_internal(pOpus, dropus_on_read_stdio, dropus_on_seek_stdio, NULL, pAllocationCallbacks);
    if (result != DROPUS_SUCCESS) {
        fclose(pFile);
        return result;
    }
    
    return DROPUS_SUCCESS;
}

DROPUS_API dropus_result dropus_init_file_w(dropus* pOpus, const wchar_t* pFilePath, const dropus_allocation_callbacks* pAllocationCallbacks)
{
    dropus_result result;
    FILE* pFile;

    if (pOpus == NULL) {
        return DROPUS_INVALID_ARGS;
    }

    DROPUS_ZERO_OBJECT(pOpus);

    if (pFilePath == NULL || pFilePath[0] == '\0') {
        return DROPUS_INVALID_ARGS;
    }

    result = dropus_wfopen(&pFile, pFilePath, L"rb", pAllocationCallbacks);
    if (result != DROPUS_SUCCESS) {
        return result;
    }

    pOpus->pFile = (void*)pFile;

    result = dropus_init_internal(pOpus, dropus_on_read_stdio, dropus_on_seek_stdio, NULL, pAllocationCallbacks);
    if (result != DROPUS_SUCCESS) {
        fclose(pFile);
        return result;
    }
    
    return DROPUS_SUCCESS;
}
#endif

static size_t dropus_on_read_memory(void* pUserData, void* pBufferOut, size_t bytesToRead)
{
    size_t bytesRemaining;
    dropus* pOpus = (dropus*)pUserData;

    DROPUS_ASSERT(pOpus != NULL);
    DROPUS_ASSERT(pOpus->memory.dataSize >= pOpus->memory.currentReadPos);

    bytesRemaining = pOpus->memory.dataSize - pOpus->memory.currentReadPos;
    if (bytesToRead > bytesRemaining) {
        bytesToRead = bytesRemaining;
    }

    if (bytesToRead > 0) {
        DROPUS_COPY_MEMORY(pBufferOut, pOpus->memory.pData + pOpus->memory.currentReadPos, bytesToRead);
        pOpus->memory.currentReadPos += bytesToRead;
    }

    return bytesToRead;
}

static dropus_bool32 dropus_on_seek_memory(void* pUserData, int byteOffset, dropus_seek_origin origin)
{
    dropus* pOpus = (dropus*)pUserData;
    DROPUS_ASSERT(pOpus != NULL);

    if (origin == dropus_seek_origin_current) {
        if (byteOffset > 0) {
            if (pOpus->memory.currentReadPos + byteOffset > pOpus->memory.dataSize) {
                byteOffset = (int)(pOpus->memory.dataSize - pOpus->memory.currentReadPos);  /* Trying to seek too far forward. */
            }
        } else {
            if (pOpus->memory.currentReadPos < (size_t)-byteOffset) {
                byteOffset = -(int)pOpus->memory.currentReadPos;  /* Trying to seek too far backwards. */
            }
        }

        /* This will never underflow thanks to the clamps above. */
        pOpus->memory.currentReadPos += byteOffset;
    } else {
        if ((dropus_uint32)byteOffset <= pOpus->memory.dataSize) {
            pOpus->memory.currentReadPos = byteOffset;
        } else {
            pOpus->memory.currentReadPos = pOpus->memory.dataSize;  /* Trying to seek too far forward. */
        }
    }

    return DROPUS_TRUE;
}

DROPUS_API dropus_result dropus_init_memory(dropus* pOpus, const void* pData, size_t dataSize, const dropus_allocation_callbacks* pAllocationCallbacks)
{
    if (pOpus == NULL) {
        return DROPUS_INVALID_ARGS;
    }

    DROPUS_ZERO_OBJECT(pOpus);

    if (pData == NULL || dataSize == 0) {
        return DROPUS_INVALID_ARGS;
    }

    pOpus->memory.pData = (const dropus_uint8*)pData;
    pOpus->memory.dataSize = dataSize;
    pOpus->memory.currentReadPos = 0;

    return dropus_init_internal(pOpus, dropus_on_read_memory, dropus_on_seek_memory, NULL, pAllocationCallbacks);
}


DROPUS_API void dropus_uninit(dropus* pOpus)
{
    if (pOpus == NULL) {
        return;
    }

#ifndef DR_OPUS_NO_STDIO
    /* Since dr_opus manages the stdio FILE object make sure it's closed on uninitialization. */
    if (pOpus->pFile != NULL) {
        fclose((FILE*)pOpus->pFile);
    }
#endif
}



DROPUS_API const char* dropus_result_description(dropus_result result)
{
    switch (result)
    {
        case DROPUS_SUCCESS:                       return "No error";
        case DROPUS_ERROR:                         return "Unknown error";
        case DROPUS_INVALID_ARGS:                  return "Invalid argument";
        case DROPUS_INVALID_OPERATION:             return "Invalid operation";
        case DROPUS_OUT_OF_MEMORY:                 return "Out of memory";
        case DROPUS_OUT_OF_RANGE:                  return "Out of range";
        case DROPUS_ACCESS_DENIED:                 return "Permission denied";
        case DROPUS_DOES_NOT_EXIST:                return "Resource does not exist";
        case DROPUS_ALREADY_EXISTS:                return "Resource already exists";
        case DROPUS_TOO_MANY_OPEN_FILES:           return "Too many open files";
        case DROPUS_INVALID_FILE:                  return "Invalid file";
        case DROPUS_TOO_BIG:                       return "Too large";
        case DROPUS_PATH_TOO_LONG:                 return "Path too long";
        case DROPUS_NAME_TOO_LONG:                 return "Name too long";
        case DROPUS_NOT_DIRECTORY:                 return "Not a directory";
        case DROPUS_IS_DIRECTORY:                  return "Is a directory";
        case DROPUS_DIRECTORY_NOT_EMPTY:           return "Directory not empty";
        case DROPUS_END_OF_FILE:                   return "End of file";
        case DROPUS_NO_SPACE:                      return "No space available";
        case DROPUS_BUSY:                          return "Device or resource busy";
        case DROPUS_IO_ERROR:                      return "Input/output error";
        case DROPUS_INTERRUPT:                     return "Interrupted";
        case DROPUS_UNAVAILABLE:                   return "Resource unavailable";
        case DROPUS_ALREADY_IN_USE:                return "Resource already in use";
        case DROPUS_BAD_ADDRESS:                   return "Bad address";
        case DROPUS_BAD_SEEK:                      return "Illegal seek";
        case DROPUS_BAD_PIPE:                      return "Broken pipe";
        case DROPUS_DEADLOCK:                      return "Deadlock";
        case DROPUS_TOO_MANY_LINKS:                return "Too many links";
        case DROPUS_NOT_IMPLEMENTED:               return "Not implemented";
        case DROPUS_NO_MESSAGE:                    return "No message of desired type";
        case DROPUS_BAD_MESSAGE:                   return "Invalid message";
        case DROPUS_NO_DATA_AVAILABLE:             return "No data available";
        case DROPUS_INVALID_DATA:                  return "Invalid data";
        case DROPUS_TIMEOUT:                       return "Timeout";
        case DROPUS_NO_NETWORK:                    return "Network unavailable";
        case DROPUS_NOT_UNIQUE:                    return "Not unique";
        case DROPUS_NOT_SOCKET:                    return "Socket operation on non-socket";
        case DROPUS_NO_ADDRESS:                    return "Destination address required";
        case DROPUS_BAD_PROTOCOL:                  return "Protocol wrong type for socket";
        case DROPUS_PROTOCOL_UNAVAILABLE:          return "Protocol not available";
        case DROPUS_PROTOCOL_NOT_SUPPORTED:        return "Protocol not supported";
        case DROPUS_PROTOCOL_FAMILY_NOT_SUPPORTED: return "Protocol family not supported";
        case DROPUS_ADDRESS_FAMILY_NOT_SUPPORTED:  return "Address family not supported";
        case DROPUS_SOCKET_NOT_SUPPORTED:          return "Socket type not supported";
        case DROPUS_CONNECTION_RESET:              return "Connection reset";
        case DROPUS_ALREADY_CONNECTED:             return "Already connected";
        case DROPUS_NOT_CONNECTED:                 return "Not connected";
        case DROPUS_CONNECTION_REFUSED:            return "Connection refused";
        case DROPUS_NO_HOST:                       return "No host";
        case DROPUS_IN_PROGRESS:                   return "Operation in progress";
        case DROPUS_CANCELLED:                     return "Operation cancelled";
        case DROPUS_MEMORY_ALREADY_MAPPED:         return "Memory already mapped";
        case DROPUS_AT_END:                        return "Reached end of collection";

        /* dr_opus specific result codes. */
        case DROPUS_CRC_MISMATCH:                  return "CRC mismatch";
        case DROPUS_BAD_DATA:                      return "Bad data";

        default:                                   return "Unknown error";
    }
}

DROPUS_API void* dropus_malloc(size_t sz, const dropus_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks == NULL) {
        return dropus__malloc_default(sz, NULL);
    } else {
        return dropus__malloc_from_callbacks(sz, pAllocationCallbacks);
    }
}

DROPUS_API void* dropus_realloc(void* p, size_t sz, const dropus_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks == NULL) {
        return dropus__realloc_default(p, sz, NULL);
    } else {
        /*
        We need to do an explicit check for onRealloc because dropus__realloc_from_callbacks() will fall back to a malloc based emulation if onRealloc is missing
        which we do not want to do dropus_realloc().
        */
        if (pAllocationCallbacks->onRealloc == NULL) {
            return NULL;
        }

        return dropus__realloc_from_callbacks(p, sz, /*szOld*/ 0, pAllocationCallbacks);  /* Safe to pass 0 for szOld thanks to the onRealloc check above. */
    }
}

DROPUS_API void dropus_free(void* p, const dropus_allocation_callbacks* pAllocationCallbacks)
{
    if (pAllocationCallbacks == NULL) {
        dropus__free_default(p, NULL);
    } else {
        dropus__free_from_callbacks(p, pAllocationCallbacks);
    }
}
