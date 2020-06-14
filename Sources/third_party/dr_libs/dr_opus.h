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
        #pragma GCC diagnostic ignored "-Wlong-long"
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
#else
    #include <stdint.h>
    typedef int8_t    dropus_int8;
    typedef uint8_t   dropus_uint8;
    typedef int16_t   dropus_int16;
    typedef uint16_t  dropus_uint16;
    typedef int32_t   dropus_int32;
    typedef uint32_t  dropus_uint32;
    typedef int64_t   dropus_int64;
    typedef uint64_t  dropus_uint64;
    typedef uintptr_t dropus_uintptr;
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
#elif defined(__GNUC__)
    /*
    I've had a bug report where GCC is emitting warnings about functions possibly not being inlineable. This warning happens when
    the __attribute__((always_inline)) attribute is defined without an "inline" statement. I think therefore there must be some
    case where "__inline__" is not always defined, thus the compiler emitting these warnings. When using -std=c89 or -ansi on the
    command line, we cannot use the "inline" keyword and instead need to use "__inline__". In an attempt to work around this issue
    I am using "__inline__" only when we're compiling in strict ANSI mode.
    */
    #if defined(__STRICT_ANSI__)
        #define DROPUS_INLINE __inline__ __attribute__((always_inline))
    #else
        #define DROPUS_INLINE inline __attribute__((always_inline))
    #endif
#else
    #define DROPUS_INLINE
#endif

#if !defined(DROPUS_API)
    #if defined(DROPUS_DLL)
        #if defined(_WIN32)
            #define DROPUS_DLL_IMPORT  __declspec(dllimport)
            #define DROPUS_DLL_EXPORT  __declspec(dllexport)
            #define DROPUS_DLL_PRIVATE static
        #else
            #if defined(__GNUC__) && __GNUC__ >= 4
                #define DROPUS_DLL_IMPORT  __attribute__((visibility("default")))
                #define DROPUS_DLL_EXPORT  __attribute__((visibility("default")))
                #define DROPUS_DLL_PRIVATE __attribute__((visibility("hidden")))
            #else
                #define DROPUS_DLL_IMPORT
                #define DROPUS_DLL_EXPORT
                #define DROPUS_DLL_PRIVATE static
            #endif
        #endif

        #if defined(DR_OPUS_IMPLEMENTATION) || defined(DROPUS_IMPLEMENTATION)
            #define DROPUS_API  DROPUS_DLL_EXPORT
        #else
            #define DROPUS_API  DROPUS_DLL_IMPORT
        #endif
        #define DROPUS_PRIVATE DROPUS_DLL_PRIVATE
    #else
        #define DROPUS_API extern
        #define DROPUS_PRIVATE static
    #endif
#endif

typedef int dropus_result;
#define DROPUS_SUCCESS                           0
#define DROPUS_ERROR                            -1   /* A generic error. */
#define DROPUS_INVALID_ARGS                     -2
#define DROPUS_INVALID_OPERATION                -3
#define DROPUS_OUT_OF_MEMORY                    -4
#define DROPUS_OUT_OF_RANGE                     -5
#define DROPUS_ACCESS_DENIED                    -6
#define DROPUS_DOES_NOT_EXIST                   -7
#define DROPUS_ALREADY_EXISTS                   -8
#define DROPUS_TOO_MANY_OPEN_FILES              -9
#define DROPUS_INVALID_FILE                     -10
#define DROPUS_TOO_BIG                          -11
#define DROPUS_PATH_TOO_LONG                    -12
#define DROPUS_NAME_TOO_LONG                    -13
#define DROPUS_NOT_DIRECTORY                    -14
#define DROPUS_IS_DIRECTORY                     -15
#define DROPUS_DIRECTORY_NOT_EMPTY              -16
#define DROPUS_END_OF_FILE                      -17
#define DROPUS_NO_SPACE                         -18
#define DROPUS_BUSY                             -19
#define DROPUS_IO_ERROR                         -20
#define DROPUS_INTERRUPT                        -21
#define DROPUS_UNAVAILABLE                      -22
#define DROPUS_ALREADY_IN_USE                   -23
#define DROPUS_BAD_ADDRESS                      -24
#define DROPUS_BAD_SEEK                         -25
#define DROPUS_BAD_PIPE                         -26
#define DROPUS_DEADLOCK                         -27
#define DROPUS_TOO_MANY_LINKS                   -28
#define DROPUS_NOT_IMPLEMENTED                  -29
#define DROPUS_NO_MESSAGE                       -30
#define DROPUS_BAD_MESSAGE                      -31
#define DROPUS_NO_DATA_AVAILABLE                -32
#define DROPUS_INVALID_DATA                     -33
#define DROPUS_TIMEOUT                          -34
#define DROPUS_NO_NETWORK                       -35
#define DROPUS_NOT_UNIQUE                       -36
#define DROPUS_NOT_SOCKET                       -37
#define DROPUS_NO_ADDRESS                       -38
#define DROPUS_BAD_PROTOCOL                     -39
#define DROPUS_PROTOCOL_UNAVAILABLE             -40
#define DROPUS_PROTOCOL_NOT_SUPPORTED           -41
#define DROPUS_PROTOCOL_FAMILY_NOT_SUPPORTED    -42
#define DROPUS_ADDRESS_FAMILY_NOT_SUPPORTED     -43
#define DROPUS_SOCKET_NOT_SUPPORTED             -44
#define DROPUS_CONNECTION_RESET                 -45
#define DROPUS_ALREADY_CONNECTED                -46
#define DROPUS_NOT_CONNECTED                    -47
#define DROPUS_CONNECTION_REFUSED               -48
#define DROPUS_NO_HOST                          -49
#define DROPUS_IN_PROGRESS                      -50
#define DROPUS_CANCELLED                        -51
#define DROPUS_MEMORY_ALREADY_MAPPED            -52
#define DROPUS_AT_END                           -53
#define DROPUS_CRC_MISMATCH                     -100
#define DROPUS_BAD_DATA                         -101

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
DROPUS_API dropus_result dropus_stream_init(dropus_stream* pOpusStream);

/*
Decodes a packet from raw compressed data.
*/
DROPUS_API dropus_result dropus_stream_decode_packet(dropus_stream* pOpusStream, const void* pData, size_t dataSize);



/***********************************************************************************************************************************************************

High-Level Opus Decoding API

************************************************************************************************************************************************************/
typedef enum
{
    dropus_seek_origin_start,
    dropus_seek_origin_current
} dropus_seek_origin;

typedef size_t        (* dropus_read_proc)(void* pUserData, void* pBufferOut, size_t bytesToRead);
typedef dropus_bool32 (* dropus_seek_proc)(void* pUserData, int offset, dropus_seek_origin origin);

typedef struct
{
    void* pUserData;
    void* (* onMalloc)(size_t sz, void* pUserData);
    void* (* onRealloc)(void* p, size_t sz, void* pUserData);
    void  (* onFree)(void* p, void* pUserData);
} dropus_allocation_callbacks;

typedef struct
{
    dropus_read_proc onRead;
    dropus_seek_proc onSeek;
    void* pUserData;
    dropus_allocation_callbacks allocationCallbacks;
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
DROPUS_API dropus_result dropus_init(dropus* pOpus, dropus_read_proc onRead, dropus_seek_proc onSeek, void* pUserData, const dropus_allocation_callbacks* pAllocationCallbacks);

#ifndef DR_OPUS_NO_STDIO
/*
Initializes a pre-allocated decoder object from a file.

This keeps hold of the file handle throughout the lifetime of the decoder and closes it in dropus_uninit().
*/
DROPUS_API dropus_result dropus_init_file(dropus* pOpus, const char* pFilePath, const dropus_allocation_callbacks* pAllocationCallbacks);
DROPUS_API dropus_result dropus_init_file_w(dropus* pOpus, const wchar_t* pFilePath, const dropus_allocation_callbacks* pAllocationCallbacks);
#endif

/*
Initializes a pre-allocated decoder object from a block of memory.

This does not make a copy of the memory.
*/
DROPUS_API dropus_result dropus_init_memory(dropus* pOpus, const void* pData, size_t dataSize, const dropus_allocation_callbacks* pAllocationCallbacks);

/*
Uninitializes an Opus decoder.
*/
DROPUS_API void dropus_uninit(dropus* pOpus);



/************************************************************************************************************************************************************

Utilities

************************************************************************************************************************************************************/
/*
Retrieves a human readable description of the given result code.
*/
DROPUS_API const char* dropus_result_description(dropus_result result);

/*
Allocates memory from callbacks.


Parameters
----------
sz (in)
    The size of the allocation in bytes.

pAllocationCallbacks (in, optional)
    A pointer to the `dropus_allocation_callbacks` object containing pointers to the allocation routines.


Return Value
------------
A pointer to the allocated block of memory. NULL if an error occurs.


Remarks
-------
`pAllocationCallbacks` can be NULL, in which case DROPUS_MALLOC() will be used. Otherwise, if `pAllocationCallbacks` is not null, either `onMalloc` or
`onRealloc` must be set. If `onMalloc` is NULL, it will fall back to `onRealloc`. If both `onMalloc` and `onRealloc` are NULL, NULL will be returned.
*/
DROPUS_API void* dropus_malloc(size_t sz, const dropus_allocation_callbacks* pAllocationCallbacks);

/*
Reallocates memory from callbacks.


Parameters
----------
p (in)
    A pointer to the memory being reallocated.

sz (in)
    The size of the allocation in bytes.

pAllocationCallbacks (in, optional)
    A pointer to the `dropus_allocation_callbacks` object containing pointers to the allocation routines.


Return Value
------------
A pointer to the allocated block of memory. NULL if an error occurs.


Remarks
-------
`pAllocationCallbacks` can be NULL, in which case DROPUS_REALLOC() will be used. If `onRealloc` is NULL, this will fail and NULL will be returned.
*/
DROPUS_API void* dropus_realloc(void* p, size_t sz, const dropus_allocation_callbacks* pAllocationCallbacks);

/*
Frees memory allocated by `dropus_malloc()` or `dropus_realloc()`.


Parameters
----------
p (in)
    A pointer to the memory being freed.


Remarks
-------
`pAllocationCallbacks` can be NULL in which case DROPUS_FREE() will be used. If `onFree` is NULL, this will be a no-op.
*/
DROPUS_API void dropus_free(void* p, const dropus_allocation_callbacks* pAllocationCallbacks);


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
Copyright 2020 David Reid

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