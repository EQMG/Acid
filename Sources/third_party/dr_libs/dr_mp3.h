/*
MP3 audio decoder. Choice of public domain or MIT-0. See license statements at the end of this file.
dr_mp3 - v0.5.5 - 2020-01-29

David Reid - mackron@gmail.com

Based off minimp3 (https://github.com/lieff/minimp3) which is where the real work was done. See the bottom of this file for
differences between minimp3 and dr_mp3.
*/

/*
RELEASE NOTES - v0.5.0
=======================
Version 0.5.0 has breaking API changes.

Improved Client-Defined Memory Allocation
-----------------------------------------
The main change with this release is the addition of a more flexible way of implementing custom memory allocation routines. The
existing system of DRMP3_MALLOC, DRMP3_REALLOC and DRMP3_FREE are still in place and will be used by default when no custom
allocation callbacks are specified.

To use the new system, you pass in a pointer to a drmp3_allocation_callbacks object to drmp3_init() and family, like this:

    void* my_malloc(size_t sz, void* pUserData)
    {
        return malloc(sz);
    }
    void* my_realloc(void* p, size_t sz, void* pUserData)
    {
        return realloc(p, sz);
    }
    void my_free(void* p, void* pUserData)
    {
        free(p);
    }

    ...

    drmp3_allocation_callbacks allocationCallbacks;
    allocationCallbacks.pUserData = &myData;
    allocationCallbacks.onMalloc  = my_malloc;
    allocationCallbacks.onRealloc = my_realloc;
    allocationCallbacks.onFree    = my_free;
    drmp3_init_file(&mp3, "my_file.mp3", NULL, &allocationCallbacks);

The advantage of this new system is that it allows you to specify user data which will be passed in to the allocation routines.

Passing in null for the allocation callbacks object will cause dr_mp3 to use defaults which is the same as DRMP3_MALLOC,
DRMP3_REALLOC and DRMP3_FREE and the equivalent of how it worked in previous versions.

Every API that opens a drmp3 object now takes this extra parameter. These include the following:

    drmp3_init()
    drmp3_init_file()
    drmp3_init_memory()
    drmp3_open_and_read_pcm_frames_f32()
    drmp3_open_and_read_pcm_frames_s16()
    drmp3_open_memory_and_read_pcm_frames_f32()
    drmp3_open_memory_and_read_pcm_frames_s16()
    drmp3_open_file_and_read_pcm_frames_f32()
    drmp3_open_file_and_read_pcm_frames_s16()

Renamed APIs
------------
The following APIs have been renamed for consistency with other dr_* libraries and to make it clear that they return PCM frame
counts rather than sample counts.

    drmp3_open_and_read_f32()        -> drmp3_open_and_read_pcm_frames_f32()
    drmp3_open_and_read_s16()        -> drmp3_open_and_read_pcm_frames_s16()
    drmp3_open_memory_and_read_f32() -> drmp3_open_memory_and_read_pcm_frames_f32()
    drmp3_open_memory_and_read_s16() -> drmp3_open_memory_and_read_pcm_frames_s16()
    drmp3_open_file_and_read_f32()   -> drmp3_open_file_and_read_pcm_frames_f32()
    drmp3_open_file_and_read_s16()   -> drmp3_open_file_and_read_pcm_frames_s16()
*/

/*
USAGE
=====
dr_mp3 is a single-file library. To use it, do something like the following in one .c file.
    #define DR_MP3_IMPLEMENTATION
    #include "dr_mp3.h"

You can then #include this file in other parts of the program as you would with any other header file. To decode audio data,
do something like the following:

    drmp3 mp3;
    if (!drmp3_init_file(&mp3, "MySong.mp3", NULL)) {
        // Failed to open file
    }

    ...

    drmp3_uint64 framesRead = drmp3_read_pcm_frames_f32(pMP3, framesToRead, pFrames);

The drmp3 object is transparent so you can get access to the channel count and sample rate like so:

    drmp3_uint32 channels = mp3.channels;
    drmp3_uint32 sampleRate = mp3.sampleRate;

The third parameter of drmp3_init_file() in the example above allows you to control the output channel count and sample rate. It
is a pointer to a drmp3_config object. Setting any of the variables of this object to 0 will cause dr_mp3 to use defaults.

The example above initializes a decoder from a file, but you can also initialize it from a block of memory and read and seek
callbacks with drmp3_init_memory() and drmp3_init() respectively.

You do not need to do any annoying memory management when reading PCM frames - this is all managed internally. You can request
any number of PCM frames in each call to drmp3_read_pcm_frames_f32() and it will return as many PCM frames as it can, up to the
requested amount.

You can also decode an entire file in one go with drmp3_open_and_read_pcm_frames_f32(), drmp3_open_memory_and_read_pcm_frames_f32() and
drmp3_open_file_and_read_pcm_frames_f32().


OPTIONS
=======
#define these options before including this file.

#define DR_MP3_NO_STDIO
  Disable drmp3_init_file(), etc.

#define DR_MP3_NO_SIMD
  Disable SIMD optimizations.
*/

#ifndef dr_mp3_h
#define dr_mp3_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef   signed char    drmp3_int8;
typedef unsigned char    drmp3_uint8;
typedef   signed short   drmp3_int16;
typedef unsigned short   drmp3_uint16;
typedef   signed int     drmp3_int32;
typedef unsigned int     drmp3_uint32;
typedef   signed __int64 drmp3_int64;
typedef unsigned __int64 drmp3_uint64;
#else
#include <stdint.h>
typedef int8_t           drmp3_int8;
typedef uint8_t          drmp3_uint8;
typedef int16_t          drmp3_int16;
typedef uint16_t         drmp3_uint16;
typedef int32_t          drmp3_int32;
typedef uint32_t         drmp3_uint32;
typedef int64_t          drmp3_int64;
typedef uint64_t         drmp3_uint64;
#endif
typedef drmp3_uint8      drmp3_bool8;
typedef drmp3_uint32     drmp3_bool32;
#define DRMP3_TRUE       1
#define DRMP3_FALSE      0

#define DRMP3_MAX_PCM_FRAMES_PER_MP3_FRAME  1152
#define DRMP3_MAX_SAMPLES_PER_FRAME         (DRMP3_MAX_PCM_FRAMES_PER_MP3_FRAME*2)

#ifdef _MSC_VER
    #define DRMP3_INLINE __forceinline
#elif defined(__GNUC__)
    /*
    I've had a bug report where GCC is emitting warnings about functions possibly not being inlineable. This warning happens when
    the __attribute__((always_inline)) attribute is defined without an "inline" statement. I think therefore there must be some
    case where "__inline__" is not always defined, thus the compiler emitting these warnings. When using -std=c89 or -ansi on the
    command line, we cannot use the "inline" keyword and instead need to use "__inline__". In an attempt to work around this issue
    I am using "__inline__" only when we're compiling in strict ANSI mode.
    */
    #if defined(__STRICT_ANSI__)
        #define DRMP3_INLINE __inline__ __attribute__((always_inline))
    #else
        #define DRMP3_INLINE inline __attribute__((always_inline))
    #endif
#else
    #define DRMP3_INLINE
#endif

/*
Low Level Push API
==================
*/
typedef struct
{
    int frame_bytes, channels, hz, layer, bitrate_kbps;
} drmp3dec_frame_info;

typedef struct
{
    float mdct_overlap[2][9*32], qmf_state[15*2*32];
    int reserv, free_format_bytes;
    unsigned char header[4], reserv_buf[511];
} drmp3dec;

/* Initializes a low level decoder. */
void drmp3dec_init(drmp3dec *dec);

/* Reads a frame from a low level decoder. */
int drmp3dec_decode_frame(drmp3dec *dec, const unsigned char *mp3, int mp3_bytes, void *pcm, drmp3dec_frame_info *info);

/* Helper for converting between f32 and s16. */
void drmp3dec_f32_to_s16(const float *in, drmp3_int16 *out, int num_samples);



/*
Main API (Pull API)
===================
*/
#ifndef DR_MP3_DEFAULT_CHANNELS
#define DR_MP3_DEFAULT_CHANNELS         2
#endif
#ifndef DR_MP3_DEFAULT_SAMPLE_RATE
#define DR_MP3_DEFAULT_SAMPLE_RATE      44100
#endif

typedef struct drmp3_src drmp3_src;
typedef drmp3_uint64 (* drmp3_src_read_proc)(drmp3_src* pSRC, drmp3_uint64 frameCount, void* pFramesOut, void* pUserData); /* Returns the number of frames that were read. */

typedef enum
{
    drmp3_src_algorithm_none,
    drmp3_src_algorithm_linear
} drmp3_src_algorithm;

#define DRMP3_SRC_CACHE_SIZE_IN_FRAMES    512
typedef struct
{
    drmp3_src* pSRC;
    float pCachedFrames[2 * DRMP3_SRC_CACHE_SIZE_IN_FRAMES];
    drmp3_uint32 cachedFrameCount;
    drmp3_uint32 iNextFrame;
} drmp3_src_cache;

typedef struct
{
    drmp3_uint32 sampleRateIn;
    drmp3_uint32 sampleRateOut;
    drmp3_uint32 channels;
    drmp3_src_algorithm algorithm;
    drmp3_uint32 cacheSizeInFrames;  /* The number of frames to read from the client at a time. */
} drmp3_src_config;

struct drmp3_src
{
    drmp3_src_config config;
    drmp3_src_read_proc onRead;
    void* pUserData;
    float bin[256];
    drmp3_src_cache cache;    /* <-- For simplifying and optimizing client -> memory reading. */
    union
    {
        struct
        {
            double alpha;
            drmp3_bool32 isPrevFramesLoaded : 1;
            drmp3_bool32 isNextFramesLoaded : 1;
        } linear;
    } algo;
};

typedef enum
{
    drmp3_seek_origin_start,
    drmp3_seek_origin_current
} drmp3_seek_origin;

typedef struct
{
    drmp3_uint64 seekPosInBytes;        /* Points to the first byte of an MP3 frame. */
    drmp3_uint64 pcmFrameIndex;         /* The index of the PCM frame this seek point targets. */
    drmp3_uint16 mp3FramesToDiscard;    /* The number of whole MP3 frames to be discarded before pcmFramesToDiscard. */
    drmp3_uint16 pcmFramesToDiscard;    /* The number of leading samples to read and discard. These are discarded after mp3FramesToDiscard. */
} drmp3_seek_point;

/*
Callback for when data is read. Return value is the number of bytes actually read.

pUserData   [in]  The user data that was passed to drmp3_init(), drmp3_open() and family.
pBufferOut  [out] The output buffer.
bytesToRead [in]  The number of bytes to read.

Returns the number of bytes actually read.

A return value of less than bytesToRead indicates the end of the stream. Do _not_ return from this callback until
either the entire bytesToRead is filled or you have reached the end of the stream.
*/
typedef size_t (* drmp3_read_proc)(void* pUserData, void* pBufferOut, size_t bytesToRead);

/*
Callback for when data needs to be seeked.

pUserData [in] The user data that was passed to drmp3_init(), drmp3_open() and family.
offset    [in] The number of bytes to move, relative to the origin. Will never be negative.
origin    [in] The origin of the seek - the current position or the start of the stream.

Returns whether or not the seek was successful.

Whether or not it is relative to the beginning or current position is determined by the "origin" parameter which
will be either drmp3_seek_origin_start or drmp3_seek_origin_current.
*/
typedef drmp3_bool32 (* drmp3_seek_proc)(void* pUserData, int offset, drmp3_seek_origin origin);

typedef struct
{
    void* pUserData;
    void* (* onMalloc)(size_t sz, void* pUserData);
    void* (* onRealloc)(void* p, size_t sz, void* pUserData);
    void  (* onFree)(void* p, void* pUserData);
} drmp3_allocation_callbacks;

typedef struct
{
    drmp3_uint32 outputChannels;
    drmp3_uint32 outputSampleRate;
} drmp3_config;

typedef struct
{
    drmp3dec decoder;
    drmp3dec_frame_info frameInfo;
    drmp3_uint32 channels;
    drmp3_uint32 sampleRate;
    drmp3_read_proc onRead;
    drmp3_seek_proc onSeek;
    void* pUserData;
    drmp3_allocation_callbacks allocationCallbacks;
    drmp3_uint32 mp3FrameChannels;      /* The number of channels in the currently loaded MP3 frame. Internal use only. */
    drmp3_uint32 mp3FrameSampleRate;    /* The sample rate of the currently loaded MP3 frame. Internal use only. */
    drmp3_uint32 pcmFramesConsumedInMP3Frame;
    drmp3_uint32 pcmFramesRemainingInMP3Frame;
    drmp3_uint8 pcmFrames[sizeof(float)*DRMP3_MAX_SAMPLES_PER_FRAME];  /* <-- Multipled by sizeof(float) to ensure there's enough room for DR_MP3_FLOAT_OUTPUT. */
    drmp3_uint64 currentPCMFrame;       /* The current PCM frame, globally, based on the output sample rate. Mainly used for seeking. */
    drmp3_uint64 streamCursor;          /* The current byte the decoder is sitting on in the raw stream. */
    drmp3_src src;
    drmp3_seek_point* pSeekPoints;      /* NULL by default. Set with drmp3_bind_seek_table(). Memory is owned by the client. dr_mp3 will never attempt to free this pointer. */
    drmp3_uint32 seekPointCount;        /* The number of items in pSeekPoints. When set to 0 assumes to no seek table. Defaults to zero. */
    size_t dataSize;
    size_t dataCapacity;
    drmp3_uint8* pData;
    drmp3_bool32 atEnd : 1;
    struct
    {
        const drmp3_uint8* pData;
        size_t dataSize;
        size_t currentReadPos;
    } memory;   /* Only used for decoders that were opened against a block of memory. */
} drmp3;

/*
Initializes an MP3 decoder.

onRead    [in]           The function to call when data needs to be read from the client.
onSeek    [in]           The function to call when the read position of the client data needs to move.
pUserData [in, optional] A pointer to application defined data that will be passed to onRead and onSeek.

Returns true if successful; false otherwise.

Close the loader with drmp3_uninit().

See also: drmp3_init_file(), drmp3_init_memory(), drmp3_uninit()
*/
drmp3_bool32 drmp3_init(drmp3* pMP3, drmp3_read_proc onRead, drmp3_seek_proc onSeek, void* pUserData, const drmp3_config* pConfig, const drmp3_allocation_callbacks* pAllocationCallbacks);

/*
Initializes an MP3 decoder from a block of memory.

This does not create a copy of the data. It is up to the application to ensure the buffer remains valid for
the lifetime of the drmp3 object.

The buffer should contain the contents of the entire MP3 file.
*/
drmp3_bool32 drmp3_init_memory(drmp3* pMP3, const void* pData, size_t dataSize, const drmp3_config* pConfig, const drmp3_allocation_callbacks* pAllocationCallbacks);

#ifndef DR_MP3_NO_STDIO
/*
Initializes an MP3 decoder from a file.

This holds the internal FILE object until drmp3_uninit() is called. Keep this in mind if you're caching drmp3
objects because the operating system may restrict the number of file handles an application can have open at
any given time.
*/
drmp3_bool32 drmp3_init_file(drmp3* pMP3, const char* filePath, const drmp3_config* pConfig, const drmp3_allocation_callbacks* pAllocationCallbacks);
#endif

/*
Uninitializes an MP3 decoder.
*/
void drmp3_uninit(drmp3* pMP3);

/*
Reads PCM frames as interleaved 32-bit IEEE floating point PCM.

Note that framesToRead specifies the number of PCM frames to read, _not_ the number of MP3 frames.
*/
drmp3_uint64 drmp3_read_pcm_frames_f32(drmp3* pMP3, drmp3_uint64 framesToRead, float* pBufferOut);

/*
Reads PCM frames as interleaved signed 16-bit integer PCM.

Note that framesToRead specifies the number of PCM frames to read, _not_ the number of MP3 frames.
*/
drmp3_uint64 drmp3_read_pcm_frames_s16(drmp3* pMP3, drmp3_uint64 framesToRead, drmp3_int16* pBufferOut);

/*
Seeks to a specific frame.

Note that this is _not_ an MP3 frame, but rather a PCM frame.
*/
drmp3_bool32 drmp3_seek_to_pcm_frame(drmp3* pMP3, drmp3_uint64 frameIndex);

/*
Calculates the total number of PCM frames in the MP3 stream. Cannot be used for infinite streams such as internet
radio. Runs in linear time. Returns 0 on error.
*/
drmp3_uint64 drmp3_get_pcm_frame_count(drmp3* pMP3);

/*
Calculates the total number of MP3 frames in the MP3 stream. Cannot be used for infinite streams such as internet
radio. Runs in linear time. Returns 0 on error.
*/
drmp3_uint64 drmp3_get_mp3_frame_count(drmp3* pMP3);

/*
Calculates the total number of MP3 and PCM frames in the MP3 stream. Cannot be used for infinite streams such as internet
radio. Runs in linear time. Returns 0 on error.

This is equivalent to calling drmp3_get_mp3_frame_count() and drmp3_get_pcm_frame_count() except that it's more efficient.
*/
drmp3_bool32 drmp3_get_mp3_and_pcm_frame_count(drmp3* pMP3, drmp3_uint64* pMP3FrameCount, drmp3_uint64* pPCMFrameCount);

/*
Calculates the seekpoints based on PCM frames. This is slow.

pSeekpoint count is a pointer to a uint32 containing the seekpoint count. On input it contains the desired count.
On output it contains the actual count. The reason for this design is that the client may request too many
seekpoints, in which case dr_mp3 will return a corrected count.

Note that seektable seeking is not quite sample exact when the MP3 stream contains inconsistent sample rates.
*/
drmp3_bool32 drmp3_calculate_seek_points(drmp3* pMP3, drmp3_uint32* pSeekPointCount, drmp3_seek_point* pSeekPoints);

/*
Binds a seek table to the decoder.

This does _not_ make a copy of pSeekPoints - it only references it. It is up to the application to ensure this
remains valid while it is bound to the decoder.

Use drmp3_calculate_seek_points() to calculate the seek points.
*/
drmp3_bool32 drmp3_bind_seek_table(drmp3* pMP3, drmp3_uint32 seekPointCount, drmp3_seek_point* pSeekPoints);


/*
Opens an decodes an entire MP3 stream as a single operation.

pConfig is both an input and output. On input it contains what you want. On output it contains what you got.

Free the returned pointer with drmp3_free().
*/
float* drmp3_open_and_read_pcm_frames_f32(drmp3_read_proc onRead, drmp3_seek_proc onSeek, void* pUserData, drmp3_config* pConfig, drmp3_uint64* pTotalFrameCount, const drmp3_allocation_callbacks* pAllocationCallbacks);
drmp3_int16* drmp3_open_and_read_pcm_frames_s16(drmp3_read_proc onRead, drmp3_seek_proc onSeek, void* pUserData, drmp3_config* pConfig, drmp3_uint64* pTotalFrameCount, const drmp3_allocation_callbacks* pAllocationCallbacks);

float* drmp3_open_memory_and_read_pcm_frames_f32(const void* pData, size_t dataSize, drmp3_config* pConfig, drmp3_uint64* pTotalFrameCount, const drmp3_allocation_callbacks* pAllocationCallbacks);
drmp3_int16* drmp3_open_memory_and_read_pcm_frames_s16(const void* pData, size_t dataSize, drmp3_config* pConfig, drmp3_uint64* pTotalFrameCount, const drmp3_allocation_callbacks* pAllocationCallbacks);

#ifndef DR_MP3_NO_STDIO
float* drmp3_open_file_and_read_pcm_frames_f32(const char* filePath, drmp3_config* pConfig, drmp3_uint64* pTotalFrameCount, const drmp3_allocation_callbacks* pAllocationCallbacks);
drmp3_int16* drmp3_open_file_and_read_pcm_frames_s16(const char* filePath, drmp3_config* pConfig, drmp3_uint64* pTotalFrameCount, const drmp3_allocation_callbacks* pAllocationCallbacks);
#endif

/*
Frees any memory that was allocated by a public drmp3 API.
*/
void drmp3_free(void* p, const drmp3_allocation_callbacks* pAllocationCallbacks);

#ifdef __cplusplus
}
#endif
#endif  /* dr_mp3_h */


/*
DIFFERENCES BETWEEN minimp3 AND dr_mp3
======================================
- First, keep in mind that minimp3 (https://github.com/lieff/minimp3) is where all the real work was done. All of the
  code relating to the actual decoding remains mostly unmodified, apart from some namespacing changes.
- dr_mp3 adds a pulling style API which allows you to deliver raw data via callbacks. So, rather than pushing data
  to the decoder, the decoder _pulls_ data from your callbacks.
- In addition to callbacks, a decoder can be initialized from a block of memory and a file.
- The dr_mp3 pull API reads PCM frames rather than whole MP3 frames.
- dr_mp3 adds convenience APIs for opening and decoding entire files in one go.
- dr_mp3 is fully namespaced, including the implementation section, which is more suitable when compiling projects
  as a single translation unit (aka unity builds). At the time of writing this, a unity build is not possible when
  using minimp3 in conjunction with stb_vorbis. dr_mp3 addresses this.
*/

/*
REVISION HISTORY
================
v0.5.5 - 2020-01-29
  - Fix a memory allocation bug in high level s16 decoding APIs.

v0.5.4 - 2019-12-02
  - Fix a possible null pointer dereference when using custom memory allocators for realloc().

v0.5.3 - 2019-11-14
  - Fix typos in documentation.

v0.5.2 - 2019-11-02
  - Bring up to date with minimp3.

v0.5.1 - 2019-10-08
  - Fix a warning with GCC.

v0.5.0 - 2019-10-07
  - API CHANGE: Add support for user defined memory allocation routines. This system allows the program to specify their own memory allocation
    routines with a user data pointer for client-specific contextual data. This adds an extra parameter to the end of the following APIs:
    - drmp3_init()
    - drmp3_init_file()
    - drmp3_init_memory()
    - drmp3_open_and_read_pcm_frames_f32()
    - drmp3_open_and_read_pcm_frames_s16()
    - drmp3_open_memory_and_read_pcm_frames_f32()
    - drmp3_open_memory_and_read_pcm_frames_s16()
    - drmp3_open_file_and_read_pcm_frames_f32()
    - drmp3_open_file_and_read_pcm_frames_s16()
  - API CHANGE: Renamed the following APIs:
    - drmp3_open_and_read_f32()        -> drmp3_open_and_read_pcm_frames_f32()
    - drmp3_open_and_read_s16()        -> drmp3_open_and_read_pcm_frames_s16()
    - drmp3_open_memory_and_read_f32() -> drmp3_open_memory_and_read_pcm_frames_f32()
    - drmp3_open_memory_and_read_s16() -> drmp3_open_memory_and_read_pcm_frames_s16()
    - drmp3_open_file_and_read_f32()   -> drmp3_open_file_and_read_pcm_frames_f32()
    - drmp3_open_file_and_read_s16()   -> drmp3_open_file_and_read_pcm_frames_s16()

v0.4.7 - 2019-07-28
  - Fix a compiler error.

v0.4.6 - 2019-06-14
  - Fix a compiler error.

v0.4.5 - 2019-06-06
  - Bring up to date with minimp3.

v0.4.4 - 2019-05-06
  - Fixes to the VC6 build.

v0.4.3 - 2019-05-05
  - Use the channel count and/or sample rate of the first MP3 frame instead of DR_MP3_DEFAULT_CHANNELS and
    DR_MP3_DEFAULT_SAMPLE_RATE when they are set to 0. To use the old behaviour, just set the relevant property to
    DR_MP3_DEFAULT_CHANNELS or DR_MP3_DEFAULT_SAMPLE_RATE.
  - Add s16 reading APIs
    - drmp3_read_pcm_frames_s16
    - drmp3_open_memory_and_read_pcm_frames_s16
    - drmp3_open_and_read_pcm_frames_s16
    - drmp3_open_file_and_read_pcm_frames_s16
  - Add drmp3_get_mp3_and_pcm_frame_count() to the public header section.
  - Add support for C89.
  - Change license to choice of public domain or MIT-0.

v0.4.2 - 2019-02-21
  - Fix a warning.

v0.4.1 - 2018-12-30
  - Fix a warning.

v0.4.0 - 2018-12-16
  - API CHANGE: Rename some APIs:
    - drmp3_read_f32 -> to drmp3_read_pcm_frames_f32
    - drmp3_seek_to_frame -> drmp3_seek_to_pcm_frame
    - drmp3_open_and_decode_f32 -> drmp3_open_and_read_pcm_frames_f32
    - drmp3_open_and_decode_memory_f32 -> drmp3_open_memory_and_read_pcm_frames_f32
    - drmp3_open_and_decode_file_f32 -> drmp3_open_file_and_read_pcm_frames_f32
  - Add drmp3_get_pcm_frame_count().
  - Add drmp3_get_mp3_frame_count().
  - Improve seeking performance.

v0.3.2 - 2018-09-11
  - Fix a couple of memory leaks.
  - Bring up to date with minimp3.

v0.3.1 - 2018-08-25
  - Fix C++ build.

v0.3.0 - 2018-08-25
  - Bring up to date with minimp3. This has a minor API change: the "pcm" parameter of drmp3dec_decode_frame() has
    been changed from short* to void* because it can now output both s16 and f32 samples, depending on whether or
    not the DR_MP3_FLOAT_OUTPUT option is set.

v0.2.11 - 2018-08-08
  - Fix a bug where the last part of a file is not read.

v0.2.10 - 2018-08-07
  - Improve 64-bit detection.

v0.2.9 - 2018-08-05
  - Fix C++ build on older versions of GCC.
  - Bring up to date with minimp3.

v0.2.8 - 2018-08-02
  - Fix compilation errors with older versions of GCC.

v0.2.7 - 2018-07-13
  - Bring up to date with minimp3.

v0.2.6 - 2018-07-12
  - Bring up to date with minimp3.

v0.2.5 - 2018-06-22
  - Bring up to date with minimp3.

v0.2.4 - 2018-05-12
  - Bring up to date with minimp3.

v0.2.3 - 2018-04-29
  - Fix TCC build.

v0.2.2 - 2018-04-28
  - Fix bug when opening a decoder from memory.

v0.2.1 - 2018-04-27
  - Efficiency improvements when the decoder reaches the end of the stream.

v0.2 - 2018-04-21
  - Bring up to date with minimp3.
  - Start using major.minor.revision versioning.

v0.1d - 2018-03-30
  - Bring up to date with minimp3.

v0.1c - 2018-03-11
  - Fix C++ build error.

v0.1b - 2018-03-07
  - Bring up to date with minimp3.

v0.1a - 2018-02-28
  - Fix compilation error on GCC/Clang.
  - Fix some warnings.

v0.1 - 2018-02-xx
  - Initial versioned release.
*/

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

/*
    https://github.com/lieff/minimp3
    To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide.
    This software is distributed without any warranty.
    See <http://creativecommons.org/publicdomain/zero/1.0/>.
*/
