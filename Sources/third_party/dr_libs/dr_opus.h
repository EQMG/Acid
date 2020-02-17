/*
Opus audio decoder. Choice of public domain or MIT-0. See license statements at the end of this file.
dr_opus - v0.0.0 (Unreleased) - xxxx-xx-xx

David Reid - mackron@gmail.com
*/

/* ====== WORK-IN-PROGRESSS ====== */

#ifndef dr_opus_h
#define dr_opus_h

#include <stddef.h> /* For size_t. */

/* Sized types. Prefer built-in types. Fall back to stdint. */
#ifdef _MSC_VER
    #if defined(__clang__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wlanguage-extension-token"
        #pragma GCC diagnostic ignored "-Wc++11-long-long"
    #endif
    typedef   signed __int8  dropus_int8;
    typedef unsigned __int8  dropus_uint8;
    typedef   signed __int16 dropus_int16;
    typedef unsigned __int16 dropus_uint16;
    typedef   signed __int32 dropus_int32;
    typedef unsigned __int32 dropus_uint32;
    typedef   signed __int64 dropus_int64;
    typedef unsigned __int64 dropus_uint64;
    #if defined(__clang__)
        #pragma GCC diagnostic pop
    #endif
#else
    #define DROPUS_HAS_STDINT
    #include <stdint.h>
    typedef int8_t   dropus_int8;
    typedef uint8_t  dropus_uint8;
    typedef int16_t  dropus_int16;
    typedef uint16_t dropus_uint16;
    typedef int32_t  dropus_int32;
    typedef uint32_t dropus_uint32;
    typedef int64_t  dropus_int64;
    typedef uint64_t dropus_uint64;
#endif

#ifdef DROPUS_HAS_STDINT
    typedef uintptr_t dropus_uintptr;
#else
    #if defined(_WIN32)
        #if defined(_WIN64)
            typedef dropus_uint64 dropus_uintptr;
        #else
            typedef dropus_uint32 dropus_uintptr;
        #endif
    #elif defined(__GNUC__)
        #if defined(__LP64__)
            typedef dropus_uint64 dropus_uintptr;
        #else
            typedef dropus_uint32 dropus_uintptr;
        #endif
    #else
        typedef dropus_uint64 dropus_uintptr;   /* Fallback. */
    #endif
#endif

typedef dropus_uint8  dropus_bool8;
typedef dropus_uint32 dropus_bool32;
#define DROPUS_TRUE   1
#define DROPUS_FALSE  0

typedef void* dropus_handle;
typedef void* dropus_ptr;
typedef void (* dropus_proc)(void);

#ifndef NULL
#define NULL 0
#endif

#if defined(SIZE_MAX)
    #define DROPUS_SIZE_MAX SIZE_MAX
#else
    #define DROPUS_SIZE_MAX 0xFFFFFFFF  /* When SIZE_MAX is not defined by the standard library just default to the maximum 32-bit unsigned integer. */
#endif


#ifdef _MSC_VER
#define DROPUS_INLINE __forceinline
#else
#ifdef __GNUC__
#define DROPUS_INLINE __inline__ __attribute__((always_inline))
#else
#define DROPUS_INLINE inline
#endif
#endif

typedef int dropus_result;
#define DROPUS_SUCCESS           0
#define DROPUS_ERROR            -1  /* Generic or unknown error. */
#define DROPUS_INVALID_ARGS     -2
#define DROPUS_BAD_DATA         -100

/***********************************************************************************************************************************************************

Low-Level Opus Stream API

This API is where the low-level decoding takes place. In order to use this, you must know the packet structure of the Opus stream. This is usually the job of
encapsulations such as Ogg and Matroska.

************************************************************************************************************************************************************/
#define DROPUS_MAX_OPUS_FRAMES_PER_PACKET   48      /* RFC 6716 - Section 3.2.5 */
#define DROPUS_MAX_PCM_FRAMES_PER_PACKET    6144    /* RFC 6716 - Section 3.2.5. Maximum of 120ms. Maximum rate is 48kHz. 6144 = 120*48. */

typedef enum
{
    dropus_mode_silk,
    dropus_mode_celt,
    dropus_mode_hybrid
} dropus_mode;

typedef struct
{
    dropus_uint16 sizeInBytes;
} dropus_stream_frame;

typedef struct
{
    dropus_uint8 toc;               /* TOC byte. RFC 6716 - Section 3.1 */
    dropus_stream_frame frames[DROPUS_MAX_OPUS_FRAMES_PER_PACKET];
} dropus_stream_packet;

typedef struct
{
    dropus_stream_packet packet;    /* The current packet. */
    struct
    {
        dropus_int32 w0_Q13_prev;   /* Previous stereo weights. */
        dropus_int32 w1_Q13_prev;
    } silk;
} dropus_stream;

/*
Initializes a new low-level Opus stream object.
*/
dropus_result dropus_stream_init(dropus_stream* pOpusStream);

/*
Decodes a packet from the given compressed data.
*/
dropus_result dropus_stream_decode_packet(dropus_stream* pOpusStream, const void* pData, size_t dataSize);



/***********************************************************************************************************************************************************

High-Level Opus Decoding API

************************************************************************************************************************************************************/

typedef enum
{
    dropus_seek_origin_start,
    dropus_seek_origin_current
} dropus_seek_origin;

typedef size_t (* dropus_read_proc)(void* pUserData, void* pBufferOut, size_t bytesToRead);
typedef dropus_bool32 (* dropus_seek_proc)(void* pUserData, int offset, dropus_seek_origin origin);

typedef struct
{
    dropus_read_proc onRead;
    dropus_seek_proc onSeek;
    void* pUserData;
    void* pFile;    /* Only used for decoders that were opened against a file. */
    struct
    {
        const dropus_uint8* pData;
        size_t dataSize;
        size_t currentReadPos;
    } memory;       /* Only used for decoders that were opened against a block of memory. */
} dropus;

/*
Initializes a pre-allocated decoder object from callbacks.
*/
dropus_bool32 dropus_init(dropus* pOpus, dropus_read_proc onRead, dropus_seek_proc onSeek, void* pUserData);

#ifndef DR_OPUS_NO_STDIO
/*
Initializes a pre-allocated decoder object from a file.

This keeps hold of the file handle throughout the lifetime of the decoder and closes it in dropus_uninit().
*/
dropus_bool32 dropus_init_file(dropus* pOpus, const char* pFilePath);
#endif

/*
Initializes a pre-allocated decoder object from a block of memory.

This does not make a copy of the memory.
*/
dropus_bool32 dropus_init_memory(dropus* pOpus, const void* pData, size_t dataSize);

/*
Uninitializes an Opus decoder.
*/
void dropus_uninit(dropus* pOpus);


#endif  /* dr_opus_h */


/*
This software is available as a choice of the following licenses. Choose
whichever you prefer.

===============================================================================
ALTERNATIVE 1 - Public Domain (www.unlicense.org)
===============================================================================
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>

===============================================================================
ALTERNATIVE 2 - MIT No Attribution
===============================================================================
Copyright 2019 David Reid

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
