/*
FLAC audio decoder. Choice of public domain or MIT-0. See license statements at the end of this file.
dr_flac - v0.12.5 - 2020-01-30

David Reid - mackron@gmail.com
*/

/*
RELEASE NOTES - v0.12.0
=======================
Version 0.12.0 has breaking API changes including changes to the existing API and the removal of deprecated APIs.


Improved Client-Defined Memory Allocation
-----------------------------------------
The main change with this release is the addition of a more flexible way of implementing custom memory allocation routines. The
existing system of DRFLAC_MALLOC, DRFLAC_REALLOC and DRFLAC_FREE are still in place and will be used by default when no custom
allocation callbacks are specified.

To use the new system, you pass in a pointer to a drflac_allocation_callbacks object to drflac_open() and family, like this:

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

    drflac_allocation_callbacks allocationCallbacks;
    allocationCallbacks.pUserData = &myData;
    allocationCallbacks.onMalloc  = my_malloc;
    allocationCallbacks.onRealloc = my_realloc;
    allocationCallbacks.onFree    = my_free;
    drflac* pFlac = drflac_open_file("my_file.flac", &allocationCallbacks);

The advantage of this new system is that it allows you to specify user data which will be passed in to the allocation routines.

Passing in null for the allocation callbacks object will cause dr_flac to use defaults which is the same as DRFLAC_MALLOC,
DRFLAC_REALLOC and DRFLAC_FREE and the equivalent of how it worked in previous versions.

Every API that opens a drflac object now takes this extra parameter. These include the following:

    drflac_open()
    drflac_open_relaxed()
    drflac_open_with_metadata()
    drflac_open_with_metadata_relaxed()
    drflac_open_file()
    drflac_open_file_with_metadata()
    drflac_open_memory()
    drflac_open_memory_with_metadata()
    drflac_open_and_read_pcm_frames_s32()
    drflac_open_and_read_pcm_frames_s16()
    drflac_open_and_read_pcm_frames_f32()
    drflac_open_file_and_read_pcm_frames_s32()
    drflac_open_file_and_read_pcm_frames_s16()
    drflac_open_file_and_read_pcm_frames_f32()
    drflac_open_memory_and_read_pcm_frames_s32()
    drflac_open_memory_and_read_pcm_frames_s16()
    drflac_open_memory_and_read_pcm_frames_f32()



Optimizations
-------------
Seeking performance has been greatly improved. A new binary search based seeking algorithm has been introduced which significantly
improves performance over the brute force method which was used when no seek table was present. Seek table based seeking also takes
advantage of the new binary search seeking system to further improve performance there as well. Note that this depends on CRC which
means it will be disabled when DR_FLAC_NO_CRC is used.

The SSE4.1 pipeline has been cleaned up and optimized. You should see some improvements with decoding speed of 24-bit files in
particular. 16-bit streams should also see some improvement.

drflac_read_pcm_frames_s16() has been optimized. Previously this sat on top of drflac_read_pcm_frames_s32() and performed it's s32
to s16 conversion in a second pass. This is now all done in a single pass. This includes SSE2 and ARM NEON optimized paths.

A minor optimization has been implemented for drflac_read_pcm_frames_s32(). This will now use an SSE2 optimized pipeline for stereo
channel reconstruction which is the last part of the decoding process.

The ARM build has seen a few improvements. The CLZ (count leading zeroes) and REV (byte swap) instructions are now used when
compiling with GCC and Clang which is achieved using inline assembly. The CLZ instruction requires ARM architecture version 5 at
compile time and the REV instruction requires ARM architecture version 6.

An ARM NEON optimized pipeline has been implemented. To enable this you'll need to add -mfpu=neon to the command line when compiling.


Removed APIs
------------
The following APIs were deprecated in version 0.11.0 and have been completely removed in version 0.12.0:

    drflac_read_s32()                   -> drflac_read_pcm_frames_s32()
    drflac_read_s16()                   -> drflac_read_pcm_frames_s16()
    drflac_read_f32()                   -> drflac_read_pcm_frames_f32()
    drflac_seek_to_sample()             -> drflac_seek_to_pcm_frame()
    drflac_open_and_decode_s32()        -> drflac_open_and_read_pcm_frames_s32()
    drflac_open_and_decode_s16()        -> drflac_open_and_read_pcm_frames_s16()
    drflac_open_and_decode_f32()        -> drflac_open_and_read_pcm_frames_f32()
    drflac_open_and_decode_file_s32()   -> drflac_open_file_and_read_pcm_frames_s32()
    drflac_open_and_decode_file_s16()   -> drflac_open_file_and_read_pcm_frames_s16()
    drflac_open_and_decode_file_f32()   -> drflac_open_file_and_read_pcm_frames_f32()
    drflac_open_and_decode_memory_s32() -> drflac_open_memory_and_read_pcm_frames_s32()
    drflac_open_and_decode_memory_s16() -> drflac_open_memory_and_read_pcm_frames_s16()
    drflac_open_and_decode_memory_f32() -> drflac_open_memroy_and_read_pcm_frames_f32()

Prior versions of dr_flac operated on a per-sample basis whereas now it operates on PCM frames. The removed APIs all relate
to the old per-sample APIs. You now need to use the "pcm_frame" versions.
*/


/*
USAGE
=====
dr_flac is a single-file library. To use it, do something like the following in one .c file.

    #define DR_FLAC_IMPLEMENTATION
    #include "dr_flac.h"

You can then #include this file in other parts of the program as you would with any other header file. To decode audio data,
do something like the following:

    drflac* pFlac = drflac_open_file("MySong.flac", NULL);
    if (pFlac == NULL) {
        // Failed to open FLAC file
    }

    drflac_int32* pSamples = malloc(pFlac->totalPCMFrameCount * pFlac->channels * sizeof(drflac_int32));
    drflac_uint64 numberOfInterleavedSamplesActuallyRead = drflac_read_pcm_frames_s32(pFlac, pFlac->totalPCMFrameCount, pSamples);

The drflac object represents the decoder. It is a transparent type so all the information you need, such as the number of
channels and the bits per sample, should be directly accessible - just make sure you don't change their values. Samples are
always output as interleaved signed 32-bit PCM. In the example above a native FLAC stream was opened, however dr_flac has
seamless support for Ogg encapsulated FLAC streams as well.

You do not need to decode the entire stream in one go - you just specify how many samples you'd like at any given time and
the decoder will give you as many samples as it can, up to the amount requested. Later on when you need the next batch of
samples, just call it again. Example:

    while (drflac_read_pcm_frames_s32(pFlac, chunkSizeInPCMFrames, pChunkSamples) > 0) {
        do_something();
    }

You can seek to a specific sample with drflac_seek_to_sample(). The given sample is based on interleaving. So for example,
if you were to seek to the sample at index 0 in a stereo stream, you'll be seeking to the first sample of the left channel.
The sample at index 1 will be the first sample of the right channel. The sample at index 2 will be the second sample of the
left channel, etc.


If you just want to quickly decode an entire FLAC file in one go you can do something like this:

    unsigned int channels;
    unsigned int sampleRate;
    drflac_uint64 totalPCMFrameCount;
    drflac_int32* pSampleData = drflac_open_file_and_read_pcm_frames_s32("MySong.flac", &channels, &sampleRate, &totalPCMFrameCount, NULL);
    if (pSampleData == NULL) {
        // Failed to open and decode FLAC file.
    }

    ...

    drflac_free(pSampleData);


You can read samples as signed 16-bit integer and 32-bit floating-point PCM with the *_s16() and *_f32() family of APIs
respectively, but note that these should be considered lossy.


If you need access to metadata (album art, etc.), use drflac_open_with_metadata(), drflac_open_file_with_metdata() or
drflac_open_memory_with_metadata(). The rationale for keeping these APIs separate is that they're slightly slower than the
normal versions and also just a little bit harder to use.

dr_flac reports metadata to the application through the use of a callback, and every metadata block is reported before
drflac_open_with_metdata() returns.


The main opening APIs (drflac_open(), etc.) will fail if the header is not present. The presents a problem in certain
scenarios such as broadcast style streams or internet radio where the header may not be present because the user has
started playback mid-stream. To handle this, use the relaxed APIs: drflac_open_relaxed() and drflac_open_with_metadata_relaxed().

It is not recommended to use these APIs for file based streams because a missing header would usually indicate a
corrupt or perverse file. In addition, these APIs can take a long time to initialize because they may need to spend
a lot of time finding the first frame.



OPTIONS
=======
#define these options before including this file.

#define DR_FLAC_NO_STDIO
  Disable drflac_open_file() and family.

#define DR_FLAC_NO_OGG
  Disables support for Ogg/FLAC streams.

#define DR_FLAC_BUFFER_SIZE <number>
  Defines the size of the internal buffer to store data from onRead(). This buffer is used to reduce the number of calls
  back to the client for more data. Larger values means more memory, but better performance. My tests show diminishing
  returns after about 4KB (which is the default). Consider reducing this if you have a very efficient implementation of
  onRead(), or increase it if it's very inefficient. Must be a multiple of 8.

#define DR_FLAC_NO_CRC
  Disables CRC checks. This will offer a performance boost when CRC is unnecessary. This will disable binary search seeking.
  When seeking, the seek table will be used if available. Otherwise the seek will be performed using brute force.

#define DR_FLAC_NO_SIMD
  Disables SIMD optimizations (SSE on x86/x64 architectures, NEON on ARM architectures). Use this if you are having
  compatibility issues with your compiler.



QUICK NOTES
===========
- dr_flac does not currently support changing the sample rate nor channel count mid stream.
- This has not been tested on big-endian architectures.
- dr_flac is not thread-safe, but its APIs can be called from any thread so long as you do your own synchronization.
- When using Ogg encapsulation, a corrupted metadata block will result in drflac_open_with_metadata() and drflac_open()
  returning inconsistent samples.
*/

#ifndef dr_flac_h
#define dr_flac_h

#include <stddef.h>

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef   signed char    drflac_int8;
typedef unsigned char    drflac_uint8;
typedef   signed short   drflac_int16;
typedef unsigned short   drflac_uint16;
typedef   signed int     drflac_int32;
typedef unsigned int     drflac_uint32;
typedef   signed __int64 drflac_int64;
typedef unsigned __int64 drflac_uint64;
#else
#include <stdint.h>
typedef int8_t           drflac_int8;
typedef uint8_t          drflac_uint8;
typedef int16_t          drflac_int16;
typedef uint16_t         drflac_uint16;
typedef int32_t          drflac_int32;
typedef uint32_t         drflac_uint32;
typedef int64_t          drflac_int64;
typedef uint64_t         drflac_uint64;
#endif
typedef drflac_uint8     drflac_bool8;
typedef drflac_uint32    drflac_bool32;
#define DRFLAC_TRUE      1
#define DRFLAC_FALSE     0

#if defined(_MSC_VER) && _MSC_VER >= 1700   /* Visual Studio 2012 */
    #define DRFLAC_DEPRECATED       __declspec(deprecated)
#elif (defined(__GNUC__) && __GNUC__ >= 4)  /* GCC 4 */
    #define DRFLAC_DEPRECATED       __attribute__((deprecated))
#elif defined(__has_feature)                /* Clang */
    #if __has_feature(attribute_deprecated)
        #define DRFLAC_DEPRECATED   __attribute__((deprecated))
    #else
        #define DRFLAC_DEPRECATED
    #endif
#else
    #define DRFLAC_DEPRECATED
#endif

/*
As data is read from the client it is placed into an internal buffer for fast access. This controls the
size of that buffer. Larger values means more speed, but also more memory. In my testing there is diminishing
returns after about 4KB, but you can fiddle with this to suit your own needs. Must be a multiple of 8.
*/
#ifndef DR_FLAC_BUFFER_SIZE
#define DR_FLAC_BUFFER_SIZE   4096
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Check if we can enable 64-bit optimizations. */
#if defined(_WIN64) || defined(_LP64) || defined(__LP64__)
#define DRFLAC_64BIT
#endif

#ifdef DRFLAC_64BIT
typedef drflac_uint64 drflac_cache_t;
#else
typedef drflac_uint32 drflac_cache_t;
#endif

/* The various metadata block types. */
#define DRFLAC_METADATA_BLOCK_TYPE_STREAMINFO       0
#define DRFLAC_METADATA_BLOCK_TYPE_PADDING          1
#define DRFLAC_METADATA_BLOCK_TYPE_APPLICATION      2
#define DRFLAC_METADATA_BLOCK_TYPE_SEEKTABLE        3
#define DRFLAC_METADATA_BLOCK_TYPE_VORBIS_COMMENT   4
#define DRFLAC_METADATA_BLOCK_TYPE_CUESHEET         5
#define DRFLAC_METADATA_BLOCK_TYPE_PICTURE          6
#define DRFLAC_METADATA_BLOCK_TYPE_INVALID          127

/* The various picture types specified in the PICTURE block. */
#define DRFLAC_PICTURE_TYPE_OTHER                   0
#define DRFLAC_PICTURE_TYPE_FILE_ICON               1
#define DRFLAC_PICTURE_TYPE_OTHER_FILE_ICON         2
#define DRFLAC_PICTURE_TYPE_COVER_FRONT             3
#define DRFLAC_PICTURE_TYPE_COVER_BACK              4
#define DRFLAC_PICTURE_TYPE_LEAFLET_PAGE            5
#define DRFLAC_PICTURE_TYPE_MEDIA                   6
#define DRFLAC_PICTURE_TYPE_LEAD_ARTIST             7
#define DRFLAC_PICTURE_TYPE_ARTIST                  8
#define DRFLAC_PICTURE_TYPE_CONDUCTOR               9
#define DRFLAC_PICTURE_TYPE_BAND                    10
#define DRFLAC_PICTURE_TYPE_COMPOSER                11
#define DRFLAC_PICTURE_TYPE_LYRICIST                12
#define DRFLAC_PICTURE_TYPE_RECORDING_LOCATION      13
#define DRFLAC_PICTURE_TYPE_DURING_RECORDING        14
#define DRFLAC_PICTURE_TYPE_DURING_PERFORMANCE      15
#define DRFLAC_PICTURE_TYPE_SCREEN_CAPTURE          16
#define DRFLAC_PICTURE_TYPE_BRIGHT_COLORED_FISH     17
#define DRFLAC_PICTURE_TYPE_ILLUSTRATION            18
#define DRFLAC_PICTURE_TYPE_BAND_LOGOTYPE           19
#define DRFLAC_PICTURE_TYPE_PUBLISHER_LOGOTYPE      20

typedef enum
{
    drflac_container_native,
    drflac_container_ogg,
    drflac_container_unknown
} drflac_container;

typedef enum
{
    drflac_seek_origin_start,
    drflac_seek_origin_current
} drflac_seek_origin;

/* Packing is important on this structure because we map this directly to the raw data within the SEEKTABLE metadata block. */
#pragma pack(2)
typedef struct
{
    drflac_uint64 firstPCMFrame;
    drflac_uint64 flacFrameOffset;   /* The offset from the first byte of the header of the first frame. */
    drflac_uint16 pcmFrameCount;
} drflac_seekpoint;
#pragma pack()

typedef struct
{
    drflac_uint16 minBlockSizeInPCMFrames;
    drflac_uint16 maxBlockSizeInPCMFrames;
    drflac_uint32 minFrameSizeInPCMFrames;
    drflac_uint32 maxFrameSizeInPCMFrames;
    drflac_uint32 sampleRate;
    drflac_uint8  channels;
    drflac_uint8  bitsPerSample;
    drflac_uint64 totalPCMFrameCount;
    drflac_uint8  md5[16];
} drflac_streaminfo;

typedef struct
{
    /* The metadata type. Use this to know how to interpret the data below. */
    drflac_uint32 type;

    /*
    A pointer to the raw data. This points to a temporary buffer so don't hold on to it. It's best to
    not modify the contents of this buffer. Use the structures below for more meaningful and structured
    information about the metadata. It's possible for this to be null.
    */
    const void* pRawData;

    /* The size in bytes of the block and the buffer pointed to by pRawData if it's non-NULL. */
    drflac_uint32 rawDataSize;

    union
    {
        drflac_streaminfo streaminfo;

        struct
        {
            int unused;
        } padding;

        struct
        {
            drflac_uint32 id;
            const void* pData;
            drflac_uint32 dataSize;
        } application;

        struct
        {
            drflac_uint32 seekpointCount;
            const drflac_seekpoint* pSeekpoints;
        } seektable;

        struct
        {
            drflac_uint32 vendorLength;
            const char* vendor;
            drflac_uint32 commentCount;
            const void* pComments;
        } vorbis_comment;

        struct
        {
            char catalog[128];
            drflac_uint64 leadInSampleCount;
            drflac_bool32 isCD;
            drflac_uint8 trackCount;
            const void* pTrackData;
        } cuesheet;

        struct
        {
            drflac_uint32 type;
            drflac_uint32 mimeLength;
            const char* mime;
            drflac_uint32 descriptionLength;
            const char* description;
            drflac_uint32 width;
            drflac_uint32 height;
            drflac_uint32 colorDepth;
            drflac_uint32 indexColorCount;
            drflac_uint32 pictureDataSize;
            const drflac_uint8* pPictureData;
        } picture;
    } data;
} drflac_metadata;


/*
Callback for when data needs to be read from the client.

pUserData   [in]  The user data that was passed to drflac_open() and family.
pBufferOut  [out] The output buffer.
bytesToRead [in]  The number of bytes to read.

Returns the number of bytes actually read.

A return value of less than bytesToRead indicates the end of the stream. Do _not_ return from this callback until
either the entire bytesToRead is filled or you have reached the end of the stream.
*/
typedef size_t (* drflac_read_proc)(void* pUserData, void* pBufferOut, size_t bytesToRead);

/*
Callback for when data needs to be seeked.

pUserData [in] The user data that was passed to drflac_open() and family.
offset    [in] The number of bytes to move, relative to the origin. Will never be negative.
origin    [in] The origin of the seek - the current position or the start of the stream.

Returns whether or not the seek was successful.

The offset will never be negative. Whether or not it is relative to the beginning or current position is determined
by the "origin" parameter which will be either drflac_seek_origin_start or drflac_seek_origin_current.

When seeking to a PCM frame using drflac_seek_to_pcm_frame(), dr_flac may call this with an offset beyond the end of
the FLAC stream. This needs to be detected and handled by returning DRFLAC_FALSE.
*/
typedef drflac_bool32 (* drflac_seek_proc)(void* pUserData, int offset, drflac_seek_origin origin);

/*
Callback for when a metadata block is read.

pUserData [in] The user data that was passed to drflac_open() and family.
pMetadata [in] A pointer to a structure containing the data of the metadata block.

Use pMetadata->type to determine which metadata block is being handled and how to read the data.
*/
typedef void (* drflac_meta_proc)(void* pUserData, drflac_metadata* pMetadata);


typedef struct
{
    void* pUserData;
    void* (* onMalloc)(size_t sz, void* pUserData);
    void* (* onRealloc)(void* p, size_t sz, void* pUserData);
    void  (* onFree)(void* p, void* pUserData);
} drflac_allocation_callbacks;

/* Structure for internal use. Only used for decoders opened with drflac_open_memory. */
typedef struct
{
    const drflac_uint8* data;
    size_t dataSize;
    size_t currentReadPos;
} drflac__memory_stream;

/* Structure for internal use. Used for bit streaming. */
typedef struct
{
    /* The function to call when more data needs to be read. */
    drflac_read_proc onRead;

    /* The function to call when the current read position needs to be moved. */
    drflac_seek_proc onSeek;

    /* The user data to pass around to onRead and onSeek. */
    void* pUserData;


    /*
    The number of unaligned bytes in the L2 cache. This will always be 0 until the end of the stream is hit. At the end of the
    stream there will be a number of bytes that don't cleanly fit in an L1 cache line, so we use this variable to know whether
    or not the bistreamer needs to run on a slower path to read those last bytes. This will never be more than sizeof(drflac_cache_t).
    */
    size_t unalignedByteCount;

    /* The content of the unaligned bytes. */
    drflac_cache_t unalignedCache;

    /* The index of the next valid cache line in the "L2" cache. */
    drflac_uint32 nextL2Line;

    /* The number of bits that have been consumed by the cache. This is used to determine how many valid bits are remaining. */
    drflac_uint32 consumedBits;

    /*
    The cached data which was most recently read from the client. There are two levels of cache. Data flows as such:
    Client -> L2 -> L1. The L2 -> L1 movement is aligned and runs on a fast path in just a few instructions.
    */
    drflac_cache_t cacheL2[DR_FLAC_BUFFER_SIZE/sizeof(drflac_cache_t)];
    drflac_cache_t cache;

    /*
    CRC-16. This is updated whenever bits are read from the bit stream. Manually set this to 0 to reset the CRC. For FLAC, this
    is reset to 0 at the beginning of each frame.
    */
    drflac_uint16 crc16;
    drflac_cache_t crc16Cache;              /* A cache for optimizing CRC calculations. This is filled when when the L1 cache is reloaded. */
    drflac_uint32 crc16CacheIgnoredBytes;   /* The number of bytes to ignore when updating the CRC-16 from the CRC-16 cache. */
} drflac_bs;

typedef struct
{
    /* The type of the subframe: SUBFRAME_CONSTANT, SUBFRAME_VERBATIM, SUBFRAME_FIXED or SUBFRAME_LPC. */
    drflac_uint8 subframeType;

    /* The number of wasted bits per sample as specified by the sub-frame header. */
    drflac_uint8 wastedBitsPerSample;

    /* The order to use for the prediction stage for SUBFRAME_FIXED and SUBFRAME_LPC. */
    drflac_uint8 lpcOrder;

    /* A pointer to the buffer containing the decoded samples in the subframe. This pointer is an offset from drflac::pExtraData. */
    drflac_int32* pSamplesS32;
} drflac_subframe;

typedef struct
{
    /*
    If the stream uses variable block sizes, this will be set to the index of the first PCM frame. If fixed block sizes are used, this will
    always be set to 0. This is 64-bit because the decoded PCM frame number will be 36 bits.
    */
    drflac_uint64 pcmFrameNumber;

    /*
    If the stream uses fixed block sizes, this will be set to the frame number. If variable block sizes are used, this will always be 0. This
    is 32-bit because in fixed block sizes, the maximum frame number will be 31 bits.
    */
    drflac_uint32 flacFrameNumber;

    /* The sample rate of this frame. */
    drflac_uint32 sampleRate;

    /* The number of PCM frames in each sub-frame within this frame. */
    drflac_uint16 blockSizeInPCMFrames;

    /*
    The channel assignment of this frame. This is not always set to the channel count. If interchannel decorrelation is being used this
    will be set to DRFLAC_CHANNEL_ASSIGNMENT_LEFT_SIDE, DRFLAC_CHANNEL_ASSIGNMENT_RIGHT_SIDE or DRFLAC_CHANNEL_ASSIGNMENT_MID_SIDE.
    */
    drflac_uint8 channelAssignment;

    /* The number of bits per sample within this frame. */
    drflac_uint8 bitsPerSample;

    /* The frame's CRC. */
    drflac_uint8 crc8;
} drflac_frame_header;

typedef struct
{
    /* The header. */
    drflac_frame_header header;

    /*
    The number of PCM frames left to be read in this FLAC frame. This is initially set to the block size. As PCM frames are read,
    this will be decremented. When it reaches 0, the decoder will see this frame as fully consumed and load the next frame.
    */
    drflac_uint32 pcmFramesRemaining;

    /* The list of sub-frames within the frame. There is one sub-frame for each channel, and there's a maximum of 8 channels. */
    drflac_subframe subframes[8];
} drflac_frame;

typedef struct
{
    /* The function to call when a metadata block is read. */
    drflac_meta_proc onMeta;

    /* The user data posted to the metadata callback function. */
    void* pUserDataMD;

    /* Memory allocation callbacks. */
    drflac_allocation_callbacks allocationCallbacks;


    /* The sample rate. Will be set to something like 44100. */
    drflac_uint32 sampleRate;

    /*
    The number of channels. This will be set to 1 for monaural streams, 2 for stereo, etc. Maximum 8. This is set based on the
    value specified in the STREAMINFO block.
    */
    drflac_uint8 channels;

    /* The bits per sample. Will be set to something like 16, 24, etc. */
    drflac_uint8 bitsPerSample;

    /* The maximum block size, in samples. This number represents the number of samples in each channel (not combined). */
    drflac_uint16 maxBlockSizeInPCMFrames;

    /*
    The total number of PCM Frames making up the stream. Can be 0 in which case it's still a valid stream, but just means
    the total PCM frame count is unknown. Likely the case with streams like internet radio.
    */
    drflac_uint64 totalPCMFrameCount;


    /* The container type. This is set based on whether or not the decoder was opened from a native or Ogg stream. */
    drflac_container container;

    /* The number of seekpoints in the seektable. */
    drflac_uint32 seekpointCount;


    /* Information about the frame the decoder is currently sitting on. */
    drflac_frame currentFLACFrame;


    /* The index of the PCM frame the decoder is currently sitting on. This is only used for seeking. */
    drflac_uint64 currentPCMFrame;

    /* The position of the first FLAC frame in the stream. This is only ever used for seeking. */
    drflac_uint64 firstFLACFramePosInBytes;


    /* A hack to avoid a malloc() when opening a decoder with drflac_open_memory(). */
    drflac__memory_stream memoryStream;


    /* A pointer to the decoded sample data. This is an offset of pExtraData. */
    drflac_int32* pDecodedSamples;

    /* A pointer to the seek table. This is an offset of pExtraData, or NULL if there is no seek table. */
    drflac_seekpoint* pSeekpoints;

    /* Internal use only. Only used with Ogg containers. Points to a drflac_oggbs object. This is an offset of pExtraData. */
    void* _oggbs;

    /* Internal use only. Used for profiling and testing different seeking modes. */
    drflac_bool32 _noSeekTableSeek    : 1;
    drflac_bool32 _noBinarySearchSeek : 1;
    drflac_bool32 _noBruteForceSeek   : 1;

    /* The bit streamer. The raw FLAC data is fed through this object. */
    drflac_bs bs;

    /* Variable length extra data. We attach this to the end of the object so we can avoid unnecessary mallocs. */
    drflac_uint8 pExtraData[1];
} drflac;

/*
Opens a FLAC decoder.

onRead               [in]           The function to call when data needs to be read from the client.
onSeek               [in]           The function to call when the read position of the client data needs to move.
pUserData            [in, optional] A pointer to application defined data that will be passed to onRead and onSeek.
pAllocationCallbacks [in, optional] A pointer to application defined callbacks for managing memory allocations.

Returns a pointer to an object representing the decoder.

Close the decoder with drflac_close().

pAllocationCallbacks can be NULL in which case it will use DRFLAC_MALLOC, DRFLAC_REALLOC and DRFLAC_FREE.

This function will automatically detect whether or not you are attempting to open a native or Ogg encapsulated
FLAC, both of which should work seamlessly without any manual intervention. Ogg encapsulation also works with
multiplexed streams which basically means it can play FLAC encoded audio tracks in videos.

This is the lowest level function for opening a FLAC stream. You can also use drflac_open_file() and drflac_open_memory()
to open the stream from a file or from a block of memory respectively.

The STREAMINFO block must be present for this to succeed. Use drflac_open_relaxed() to open a FLAC stream where
the header may not be present.

See also: drflac_open_file(), drflac_open_memory(), drflac_open_with_metadata(), drflac_close()
*/
drflac* drflac_open(drflac_read_proc onRead, drflac_seek_proc onSeek, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);

/*
The same as drflac_open(), except attempts to open the stream even when a header block is not present.

Because the header is not necessarily available, the caller must explicitly define the container (Native or Ogg). Do
not set this to drflac_container_unknown - that is for internal use only.

Opening in relaxed mode will continue reading data from onRead until it finds a valid frame. If a frame is never
found it will continue forever. To abort, force your onRead callback to return 0, which dr_flac will use as an
indicator that the end of the stream was found.
*/
drflac* drflac_open_relaxed(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_container container, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);

/*
Opens a FLAC decoder and notifies the caller of the metadata chunks (album art, etc.).

onRead               [in]           The function to call when data needs to be read from the client.
onSeek               [in]           The function to call when the read position of the client data needs to move.
onMeta               [in]           The function to call for every metadata block.
pUserData            [in, optional] A pointer to application defined data that will be passed to onRead, onSeek and onMeta.
pAllocationCallbacks [in, optional] A pointer to application defined callbacks for managing memory allocations.

Returns a pointer to an object representing the decoder.

Close the decoder with drflac_close().

pAllocationCallbacks can be NULL in which case it will use DRFLAC_MALLOC, DRFLAC_REALLOC and DRFLAC_FREE.

This is slower than drflac_open(), so avoid this one if you don't need metadata. Internally, this will allocate and free
memory on the heap for every metadata block except for STREAMINFO and PADDING blocks.

The caller is notified of the metadata via the onMeta callback. All metadata blocks will be handled before the function
returns.

The STREAMINFO block must be present for this to succeed. Use drflac_open_with_metadata_relaxed() to open a FLAC
stream where the header may not be present.

Note that this will behave inconsistently with drflac_open() if the stream is an Ogg encapsulated stream and a metadata
block is corrupted. This is due to the way the Ogg stream recovers from corrupted pages. When drflac_open_with_metadata()
is being used, the open routine will try to read the contents of the metadata block, whereas drflac_open() will simply
seek past it (for the sake of efficiency). This inconsistency can result in different samples being returned depending on
whether or not the stream is being opened with metadata.

See also: drflac_open_file_with_metadata(), drflac_open_memory_with_metadata(), drflac_open(), drflac_close()
*/
drflac* drflac_open_with_metadata(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);

/*
The same as drflac_open_with_metadata(), except attempts to open the stream even when a header block is not present.

See also: drflac_open_with_metadata(), drflac_open_relaxed()
*/
drflac* drflac_open_with_metadata_relaxed(drflac_read_proc onRead, drflac_seek_proc onSeek, drflac_meta_proc onMeta, drflac_container container, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);

/*
Closes the given FLAC decoder.

pFlac [in] The decoder to close.

This will destroy the decoder object.
*/
void drflac_close(drflac* pFlac);


/*
Reads sample data from the given FLAC decoder, output as interleaved signed 32-bit PCM.

pFlac        [in]            The decoder.
framesToRead [in]            The number of PCM frames to read.
pBufferOut   [out, optional] A pointer to the buffer that will receive the decoded samples.

Returns the number of PCM frames actually read.

pBufferOut can be null, in which case the call will act as a seek, and the return value will be the number of frames
seeked.
*/
drflac_uint64 drflac_read_pcm_frames_s32(drflac* pFlac, drflac_uint64 framesToRead, drflac_int32* pBufferOut);

/*
Same as drflac_read_pcm_frames_s32(), except outputs samples as 16-bit integer PCM rather than 32-bit.

Note that this is lossy for streams where the bits per sample is larger than 16.
*/
drflac_uint64 drflac_read_pcm_frames_s16(drflac* pFlac, drflac_uint64 framesToRead, drflac_int16* pBufferOut);

/*
Same as drflac_read_pcm_frames_s32(), except outputs samples as 32-bit floating-point PCM.

Note that this should be considered lossy due to the nature of floating point numbers not being able to exactly
represent every possible number.
*/
drflac_uint64 drflac_read_pcm_frames_f32(drflac* pFlac, drflac_uint64 framesToRead, float* pBufferOut);

/*
Seeks to the PCM frame at the given index.

pFlac         [in] The decoder.
pcmFrameIndex [in] The index of the PCM frame to seek to. See notes below.

Returns DRFLAC_TRUE if successful; DRFLAC_FALSE otherwise.
*/
drflac_bool32 drflac_seek_to_pcm_frame(drflac* pFlac, drflac_uint64 pcmFrameIndex);



#ifndef DR_FLAC_NO_STDIO
/*
Opens a FLAC decoder from the file at the given path.

filename             [in]           The path of the file to open, either absolute or relative to the current directory.
pAllocationCallbacks [in, optional] A pointer to application defined callbacks for managing memory allocations.

Returns a pointer to an object representing the decoder.

Close the decoder with drflac_close().

This will hold a handle to the file until the decoder is closed with drflac_close(). Some platforms will restrict the
number of files a process can have open at any given time, so keep this mind if you have many decoders open at the
same time.

See also: drflac_open(), drflac_open_file_with_metadata(), drflac_close()
*/
drflac* drflac_open_file(const char* filename, const drflac_allocation_callbacks* pAllocationCallbacks);

/*
Opens a FLAC decoder from the file at the given path and notifies the caller of the metadata chunks (album art, etc.)

Look at the documentation for drflac_open_with_metadata() for more information on how metadata is handled.
*/
drflac* drflac_open_file_with_metadata(const char* filename, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);
#endif

/*
Opens a FLAC decoder from a pre-allocated block of memory

This does not create a copy of the data. It is up to the application to ensure the buffer remains valid for
the lifetime of the decoder.
*/
drflac* drflac_open_memory(const void* data, size_t dataSize, const drflac_allocation_callbacks* pAllocationCallbacks);

/*
Opens a FLAC decoder from a pre-allocated block of memory and notifies the caller of the metadata chunks (album art, etc.)

Look at the documentation for drflac_open_with_metadata() for more information on how metadata is handled.
*/
drflac* drflac_open_memory_with_metadata(const void* data, size_t dataSize, drflac_meta_proc onMeta, void* pUserData, const drflac_allocation_callbacks* pAllocationCallbacks);



/* High Level APIs */

/*
Opens a FLAC stream from the given callbacks and fully decodes it in a single operation. The return value is a
pointer to the sample data as interleaved signed 32-bit PCM. The returned data must be freed with drflac_free().

You can pass in custom memory allocation callbacks via the pAllocationCallbacks parameter. This can be NULL in which
case it will use DRFLAC_MALLOC, DRFLAC_REALLOC and DRFLAC_FREE.

Sometimes a FLAC file won't keep track of the total sample count. In this situation the function will continuously
read samples into a dynamically sized buffer on the heap until no samples are left.

Do not call this function on a broadcast type of stream (like internet radio streams and whatnot).
*/
drflac_int32* drflac_open_and_read_pcm_frames_s32(drflac_read_proc onRead, drflac_seek_proc onSeek, void* pUserData, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);

/* Same as drflac_open_and_read_pcm_frames_s32(), except returns signed 16-bit integer samples. */
drflac_int16* drflac_open_and_read_pcm_frames_s16(drflac_read_proc onRead, drflac_seek_proc onSeek, void* pUserData, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);

/* Same as drflac_open_and_read_pcm_frames_s32(), except returns 32-bit floating-point samples. */
float* drflac_open_and_read_pcm_frames_f32(drflac_read_proc onRead, drflac_seek_proc onSeek, void* pUserData, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);

#ifndef DR_FLAC_NO_STDIO
/* Same as drflac_open_and_read_pcm_frames_s32() except opens the decoder from a file. */
drflac_int32* drflac_open_file_and_read_pcm_frames_s32(const char* filename, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);

/* Same as drflac_open_file_and_read_pcm_frames_s32(), except returns signed 16-bit integer samples. */
drflac_int16* drflac_open_file_and_read_pcm_frames_s16(const char* filename, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);

/* Same as drflac_open_file_and_read_pcm_frames_s32(), except returns 32-bit floating-point samples. */
float* drflac_open_file_and_read_pcm_frames_f32(const char* filename, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);
#endif

/* Same as drflac_open_and_read_pcm_frames_s32() except opens the decoder from a block of memory. */
drflac_int32* drflac_open_memory_and_read_pcm_frames_s32(const void* data, size_t dataSize, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);

/* Same as drflac_open_memory_and_read_pcm_frames_s32(), except returns signed 16-bit integer samples. */
drflac_int16* drflac_open_memory_and_read_pcm_frames_s16(const void* data, size_t dataSize, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);

/* Same as drflac_open_memory_and_read_pcm_frames_s32(), except returns 32-bit floating-point samples. */
float* drflac_open_memory_and_read_pcm_frames_f32(const void* data, size_t dataSize, unsigned int* channels, unsigned int* sampleRate, drflac_uint64* totalPCMFrameCount, const drflac_allocation_callbacks* pAllocationCallbacks);

/*
Frees memory that was allocated internally by dr_flac.

Set pAllocationCallbacks to the same object that was passed to drflac_open_*_and_read_pcm_frames_*(). If you originally passed in NULL, pass in NULL for this.
*/
void drflac_free(void* p, const drflac_allocation_callbacks* pAllocationCallbacks);


/* Structure representing an iterator for vorbis comments in a VORBIS_COMMENT metadata block. */
typedef struct
{
    drflac_uint32 countRemaining;
    const char* pRunningData;
} drflac_vorbis_comment_iterator;

/*
Initializes a vorbis comment iterator. This can be used for iterating over the vorbis comments in a VORBIS_COMMENT
metadata block.
*/
void drflac_init_vorbis_comment_iterator(drflac_vorbis_comment_iterator* pIter, drflac_uint32 commentCount, const void* pComments);

/*
Goes to the next vorbis comment in the given iterator. If null is returned it means there are no more comments. The
returned string is NOT null terminated.
*/
const char* drflac_next_vorbis_comment(drflac_vorbis_comment_iterator* pIter, drflac_uint32* pCommentLengthOut);


/* Structure representing an iterator for cuesheet tracks in a CUESHEET metadata block. */
typedef struct
{
    drflac_uint32 countRemaining;
    const char* pRunningData;
} drflac_cuesheet_track_iterator;

/* Packing is important on this structure because we map this directly to the raw data within the CUESHEET metadata block. */
#pragma pack(4)
typedef struct
{
    drflac_uint64 offset;
    drflac_uint8 index;
    drflac_uint8 reserved[3];
} drflac_cuesheet_track_index;
#pragma pack()

typedef struct
{
    drflac_uint64 offset;
    drflac_uint8 trackNumber;
    char ISRC[12];
    drflac_bool8 isAudio;
    drflac_bool8 preEmphasis;
    drflac_uint8 indexCount;
    const drflac_cuesheet_track_index* pIndexPoints;
} drflac_cuesheet_track;

/*
Initializes a cuesheet track iterator. This can be used for iterating over the cuesheet tracks in a CUESHEET metadata
block.
*/
void drflac_init_cuesheet_track_iterator(drflac_cuesheet_track_iterator* pIter, drflac_uint32 trackCount, const void* pTrackData);

/* Goes to the next cuesheet track in the given iterator. If DRFLAC_FALSE is returned it means there are no more comments. */
drflac_bool32 drflac_next_cuesheet_track(drflac_cuesheet_track_iterator* pIter, drflac_cuesheet_track* pCuesheetTrack);


#ifdef __cplusplus
}
#endif
#endif  /* dr_flac_h */


/*
REVISION HISTORY
================
v0.12.5 - 2020-01-30
  - Silence some static analysis warnings.

v0.12.4 - 2020-01-29
  - Silence some static analysis warnings.

v0.12.3 - 2019-12-02
  - Fix some warnings when compiling with GCC and the -Og flag.
  - Fix a crash in out-of-memory situations.
  - Fix potential integer overflow bug.
  - Fix some static analysis warnings.
  - Fix a possible crash when using custom memory allocators without a custom realloc() implementation.
  - Fix a bug with binary search seeking where the bits per sample is not a multiple of 8.

v0.12.2 - 2019-10-07
  - Internal code clean up.

v0.12.1 - 2019-09-29
  - Fix some Clang Static Analyzer warnings.
  - Fix an unused variable warning.

v0.12.0 - 2019-09-23
  - API CHANGE: Add support for user defined memory allocation routines. This system allows the program to specify their own memory allocation
    routines with a user data pointer for client-specific contextual data. This adds an extra parameter to the end of the following APIs:
    - drflac_open()
    - drflac_open_relaxed()
    - drflac_open_with_metadata()
    - drflac_open_with_metadata_relaxed()
    - drflac_open_file()
    - drflac_open_file_with_metadata()
    - drflac_open_memory()
    - drflac_open_memory_with_metadata()
    - drflac_open_and_read_pcm_frames_s32()
    - drflac_open_and_read_pcm_frames_s16()
    - drflac_open_and_read_pcm_frames_f32()
    - drflac_open_file_and_read_pcm_frames_s32()
    - drflac_open_file_and_read_pcm_frames_s16()
    - drflac_open_file_and_read_pcm_frames_f32()
    - drflac_open_memory_and_read_pcm_frames_s32()
    - drflac_open_memory_and_read_pcm_frames_s16()
    - drflac_open_memory_and_read_pcm_frames_f32()
    Set this extra parameter to NULL to use defaults which is the same as the previous behaviour. Setting this NULL will use
    DRFLAC_MALLOC, DRFLAC_REALLOC and DRFLAC_FREE.
  - Remove deprecated APIs:
    - drflac_read_s32()
    - drflac_read_s16()
    - drflac_read_f32()
    - drflac_seek_to_sample()
    - drflac_open_and_decode_s32()
    - drflac_open_and_decode_s16()
    - drflac_open_and_decode_f32()
    - drflac_open_and_decode_file_s32()
    - drflac_open_and_decode_file_s16()
    - drflac_open_and_decode_file_f32()
    - drflac_open_and_decode_memory_s32()
    - drflac_open_and_decode_memory_s16()
    - drflac_open_and_decode_memory_f32()
  - Remove drflac.totalSampleCount which is now replaced with drflac.totalPCMFrameCount. You can emulate drflac.totalSampleCount
    by doing pFlac->totalPCMFrameCount*pFlac->channels.
  - Rename drflac.currentFrame to drflac.currentFLACFrame to remove ambiguity with PCM frames.
  - Fix errors when seeking to the end of a stream.
  - Optimizations to seeking.
  - SSE improvements and optimizations.
  - ARM NEON optimizations.
  - Optimizations to drflac_read_pcm_frames_s16().
  - Optimizations to drflac_read_pcm_frames_s32().

v0.11.10 - 2019-06-26
  - Fix a compiler error.

v0.11.9 - 2019-06-16
  - Silence some ThreadSanitizer warnings.

v0.11.8 - 2019-05-21
  - Fix warnings.

v0.11.7 - 2019-05-06
  - C89 fixes.

v0.11.6 - 2019-05-05
  - Add support for C89.
  - Fix a compiler warning when CRC is disabled.
  - Change license to choice of public domain or MIT-0.

v0.11.5 - 2019-04-19
  - Fix a compiler error with GCC.

v0.11.4 - 2019-04-17
  - Fix some warnings with GCC when compiling with -std=c99.

v0.11.3 - 2019-04-07
  - Silence warnings with GCC.

v0.11.2 - 2019-03-10
  - Fix a warning.

v0.11.1 - 2019-02-17
  - Fix a potential bug with seeking.

v0.11.0 - 2018-12-16
  - API CHANGE: Deprecated drflac_read_s32(), drflac_read_s16() and drflac_read_f32() and replaced them with
    drflac_read_pcm_frames_s32(), drflac_read_pcm_frames_s16() and drflac_read_pcm_frames_f32(). The new APIs take
    and return PCM frame counts instead of sample counts. To upgrade you will need to change the input count by
    dividing it by the channel count, and then do the same with the return value.
  - API_CHANGE: Deprecated drflac_seek_to_sample() and replaced with drflac_seek_to_pcm_frame(). Same rules as
    the changes to drflac_read_*() apply.
  - API CHANGE: Deprecated drflac_open_and_decode_*() and replaced with drflac_open_*_and_read_*(). Same rules as
    the changes to drflac_read_*() apply.
  - Optimizations.

v0.10.0 - 2018-09-11
  - Remove the DR_FLAC_NO_WIN32_IO option and the Win32 file IO functionality. If you need to use Win32 file IO you
    need to do it yourself via the callback API.
  - Fix the clang build.
  - Fix undefined behavior.
  - Fix errors with CUESHEET metdata blocks.
  - Add an API for iterating over each cuesheet track in the CUESHEET metadata block. This works the same way as the
    Vorbis comment API.
  - Other miscellaneous bug fixes, mostly relating to invalid FLAC streams.
  - Minor optimizations.

v0.9.11 - 2018-08-29
  - Fix a bug with sample reconstruction.

v0.9.10 - 2018-08-07
  - Improve 64-bit detection.

v0.9.9 - 2018-08-05
  - Fix C++ build on older versions of GCC.

v0.9.8 - 2018-07-24
  - Fix compilation errors.

v0.9.7 - 2018-07-05
  - Fix a warning.

v0.9.6 - 2018-06-29
  - Fix some typos.

v0.9.5 - 2018-06-23
  - Fix some warnings.

v0.9.4 - 2018-06-14
  - Optimizations to seeking.
  - Clean up.

v0.9.3 - 2018-05-22
  - Bug fix.

v0.9.2 - 2018-05-12
  - Fix a compilation error due to a missing break statement.

v0.9.1 - 2018-04-29
  - Fix compilation error with Clang.

v0.9 - 2018-04-24
  - Fix Clang build.
  - Start using major.minor.revision versioning.

v0.8g - 2018-04-19
  - Fix build on non-x86/x64 architectures.

v0.8f - 2018-02-02
  - Stop pretending to support changing rate/channels mid stream.

v0.8e - 2018-02-01
  - Fix a crash when the block size of a frame is larger than the maximum block size defined by the FLAC stream.
  - Fix a crash the the Rice partition order is invalid.

v0.8d - 2017-09-22
  - Add support for decoding streams with ID3 tags. ID3 tags are just skipped.

v0.8c - 2017-09-07
  - Fix warning on non-x86/x64 architectures.

v0.8b - 2017-08-19
  - Fix build on non-x86/x64 architectures.

v0.8a - 2017-08-13
  - A small optimization for the Clang build.

v0.8 - 2017-08-12
  - API CHANGE: Rename dr_* types to drflac_*.
  - Optimizations. This brings dr_flac back to about the same class of efficiency as the reference implementation.
  - Add support for custom implementations of malloc(), realloc(), etc.
  - Add CRC checking to Ogg encapsulated streams.
  - Fix VC++ 6 build. This is only for the C++ compiler. The C compiler is not currently supported.
  - Bug fixes.

v0.7 - 2017-07-23
  - Add support for opening a stream without a header block. To do this, use drflac_open_relaxed() / drflac_open_with_metadata_relaxed().

v0.6 - 2017-07-22
  - Add support for recovering from invalid frames. With this change, dr_flac will simply skip over invalid frames as if they
    never existed. Frames are checked against their sync code, the CRC-8 of the frame header and the CRC-16 of the whole frame.

v0.5 - 2017-07-16
  - Fix typos.
  - Change drflac_bool* types to unsigned.
  - Add CRC checking. This makes dr_flac slower, but can be disabled with #define DR_FLAC_NO_CRC.

v0.4f - 2017-03-10
  - Fix a couple of bugs with the bitstreaming code.

v0.4e - 2017-02-17
  - Fix some warnings.

v0.4d - 2016-12-26
  - Add support for 32-bit floating-point PCM decoding.
  - Use drflac_int* and drflac_uint* sized types to improve compiler support.
  - Minor improvements to documentation.

v0.4c - 2016-12-26
  - Add support for signed 16-bit integer PCM decoding.

v0.4b - 2016-10-23
  - A minor change to drflac_bool8 and drflac_bool32 types.

v0.4a - 2016-10-11
  - Rename drBool32 to drflac_bool32 for styling consistency.

v0.4 - 2016-09-29
  - API/ABI CHANGE: Use fixed size 32-bit booleans instead of the built-in bool type.
  - API CHANGE: Rename drflac_open_and_decode*() to drflac_open_and_decode*_s32().
  - API CHANGE: Swap the order of "channels" and "sampleRate" parameters in drflac_open_and_decode*(). Rationale for this is to
    keep it consistent with drflac_audio.

v0.3f - 2016-09-21
  - Fix a warning with GCC.

v0.3e - 2016-09-18
  - Fixed a bug where GCC 4.3+ was not getting properly identified.
  - Fixed a few typos.
  - Changed date formats to ISO 8601 (YYYY-MM-DD).

v0.3d - 2016-06-11
  - Minor clean up.

v0.3c - 2016-05-28
  - Fixed compilation error.

v0.3b - 2016-05-16
  - Fixed Linux/GCC build.
  - Updated documentation.

v0.3a - 2016-05-15
  - Minor fixes to documentation.

v0.3 - 2016-05-11
  - Optimizations. Now at about parity with the reference implementation on 32-bit builds.
  - Lots of clean up.

v0.2b - 2016-05-10
  - Bug fixes.

v0.2a - 2016-05-10
  - Made drflac_open_and_decode() more robust.
  - Removed an unused debugging variable

v0.2 - 2016-05-09
  - Added support for Ogg encapsulation.
  - API CHANGE. Have the onSeek callback take a third argument which specifies whether or not the seek
    should be relative to the start or the current position. Also changes the seeking rules such that
    seeking offsets will never be negative.
  - Have drflac_open_and_decode() fail gracefully if the stream has an unknown total sample count.

v0.1b - 2016-05-07
  - Properly close the file handle in drflac_open_file() and family when the decoder fails to initialize.
  - Removed a stale comment.

v0.1a - 2016-05-05
  - Minor formatting changes.
  - Fixed a warning on the GCC build.

v0.1 - 2016-05-03
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
