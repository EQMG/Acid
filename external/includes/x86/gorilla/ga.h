/** Gorilla Audio API.
 *
 *  Data structures and functions for realtime audio playback.
 *
 *  \file ga.h
 */

/** \mainpage
 *  \htmlinclude manual.htm
 */

#ifndef _GORILLA_GA_H
#define _GORILLA_GA_H

#include "gorilla/common/gc_common.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** Data structures and functions.
 *
 *  \defgroup external Audio API (GA)
 */

/*************/
/*  Version  */
/*************/
/** API version and related functions.
 *
 *  \defgroup version Version
 *  \ingroup external
 */

/** Major version number.
 *
 *  Major version changes indicate a massive rewrite or refactor, and/or changes 
 *  where API backwards-compatibility is greatly compromised. \ingroup version
 *
 *  \ingroup version
 */
#define GA_VERSION_MAJOR 0

/** Minor version number.
 *
 *  Minor version changes indicate development milestones, large changes, and/or
 *  changes where API backwards-compatibility is somewhat compromised.
 *
 *  \ingroup version
 */
#define GA_VERSION_MINOR 3

/** Revision version number.
 *
 *  Revision version changes indicate a new version pushed to the trunk, and/or
 *  changes where API backwards-compatibility is trivially compromised.
 *
 *  \ingroup version
 */
#define GA_VERSION_REV 1

/** Compares the API version against the specified version.
 *
 *  \ingroup version
 *  \param in_major Major version to compare against
 *  \param in_minor Minor version to compare against
 *  \param in_rev Revision version to compare against
 *  \return 0 -> specified == api, 1 -> specified > api, -1 -> specified < api
 */
gc_int32 ga_version_check(gc_int32 in_major, gc_int32 in_minor, gc_int32 in_rev);


/************************/
/*  Global Definitions  */
/************************/
/** Global flags and definitions.
 *
 * \ingroup external
 * \defgroup globDefs Global Definitions
 */

/** Flags that define properties of data and sample source.
 *
 *  \ingroup globDefs
 *  \defgroup sourceFlags Source Flags
 */
#define GA_FLAG_SEEKABLE 1 /**< Flag for sources that supports seeking. \ingroup sourceFlags */
#define GA_FLAG_THREADSAFE 2 /**< Flag for sources with a thread-safe interface. \ingroup sourceFlags */


/***********************/
/*  Memory Management  */
/***********************/
/** Memory management policies for different types of objects.
 *
 *  \ingroup external
 *  \defgroup memManagement Memory Management
 */
#define POD /**< The object is POD (plain-ol'-data), and can be allocated/copied freely.
                 \ingroup memManagement */
#define SINGLE_CLIENT /**< The object has a single client (owner). The object should be
                           created/opened by its client, and then destroyed/closed when the
                           client is done with it. The object itself should never be copied.
                           Instead, a pointer to the object should be copied. The client 
                           must never use the object after destroying it. \ingroup memManagement */
#define MULTI_CLIENT /**< The object has multiple clients (owners), and is reference-counted.
                          The object should be created by its first client. Additional 
                          clients should call *_acquire() to add new references. Whenever a
                          client is done using the object, it should call *_release() to
                          remove its reference. When the last reference is removed, the 
                          object will be freed. A client must never use the object after 
                          releasing its reference. The object itself should never be copied.
                          Instead, a pointer to the object should be copied. \ingroup memManagement*/


/************/
/*  Format  */
/************/
/** Audio format definition data structure and associated functions.
 *
 *  \ingroup external
 *  \defgroup ga_Format Format
 */

/** Audio format data structure [\ref POD].
 *
 *  Stores the format (sample rate, bps, channels) for PCM audio data.
 *
 *  This object may be used on any thread.
 *
 *  \ingroup ga_Format
 */
typedef struct ga_Format {
  gc_int32 sampleRate; /**< Sample rate (usually 44100) */
  gc_int32 bitsPerSample; /**< Bits per PCM sample (usually 16) */
  gc_int32 numChannels; /**< Number of audio channels (1 for mono, 2 for stereo) */
} ga_Format;

/** Retrieves the sample size (in bytes) of a specified format.
 *
 *  \ingroup ga_Format
 *  \param in_format Format of the PCM data
 *  \return Sample size (in bytes) of the specified format
 */
gc_int32 ga_format_sampleSize(ga_Format* in_format);

/** Converts a discrete number of PCM samples into the duration (in seconds) it 
 *  will take to play back.
 *
 *  \ingroup ga_Format
 *  \param in_format Format of PCM sample data
 *  \param in_samples Number of PCM samples
 *  \return Duration (in seconds) it will take to play back
 */
gc_float32 ga_format_toSeconds(ga_Format* in_format, gc_int32 in_samples);

/** Converts a duration (in seconds) into the discrete number of PCM samples it 
 *  will take to play for that long.
 *
 *  \ingroup ga_Format
 *  \param in_format Format of PCM sample data
 *  \param in_seconds Duration (in seconds)
 *  \return Number of PCM samples it will take to play back for the given time
 */
gc_int32 ga_format_toSamples(ga_Format* in_format, gc_float32 in_seconds);


/************/
/*  Device  */
/************/
/** Abstract device data structure and associated functions.
 *
 *  \ingroup external
 *  \defgroup ga_Device Device
 */
#define GA_DEVICE_TYPE_DEFAULT -1 /**< Default device type (based on hard-coded priorities) \ingroup ga_Device */
#define GA_DEVICE_TYPE_UNKNOWN 0 /**< Unknown (invalid) device type \ingroup ga_Device */
#define GA_DEVICE_TYPE_OPENAL 1 /**< OpenAL playback device (Windows, Linux, Mac) \ingroup ga_Device */
#define GA_DEVICE_TYPE_DIRECTSOUND 2 /**< DirectSound playback device (Windows-only, disabled) \ingroup ga_Device */
#define GA_DEVICE_TYPE_XAUDIO2 3 /**< XAudio2 playback device (Windows-only) \ingroup ga_Device */

/** Hardware device abstract data structure [\ref SINGLE_CLIENT].
 *
 *  Abstracts the platform-specific details of presenting audio buffers to sound playback hardware.
 *
 *  If a new buffer is not queued prior to the last buffer finishing playback,
 *  the device has 'starved', and audible stuttering may occur.  This can be
 *  resolved by creating longer and/or more buffers when opening the device.
 *  The number of available presentation buffers is platform-specific, and is
 *  only guaranteed to be >= 2.  To find out how many buffers a device has,
 *  you can query it using ga_device_check() immediately after creating the
 *  device.
 *
 *  This object may only be used on the main thread.
 *
 *  \ingroup ga_Device
 *  \warning You can only have one device open at-a-time.
 *  \todo Create a way to query the actual buffers/samples/format of the opened device.
 *        ga_device_check() does not work in all use cases (such as gau_Manager))
 */
typedef struct ga_Device ga_Device;

/** Opens a concrete audio device.
 *
 *  \ingroup ga_Device
 *  \param in_type Requested device type (usually GA_DEVICE_TYPE_DEFAULT).
 *  \param in_numBuffers Requested number of buffers.
 *  \param in_numSamples Requested sample buffer size.
 *  \param in_format Requested device output format (usually 16-bit/44100/stereo).
 *  \return Concrete instance of the requested device type. 0 if a suitable device could not be opened.
 */
ga_Device* ga_device_open(gc_int32 in_type,
                          gc_int32 in_numBuffers,
                          gc_int32 in_numSamples,
                          ga_Format* in_format);

/** Checks the number of free (unqueued) buffers.
 *
 *  \ingroup ga_Device
 *  \param in_device Device to check.
 *  \return Number of free (unqueued) buffers.
 */
gc_int32 ga_device_check(ga_Device* in_device);

/** Adds a buffer to a device's presentation queue.
 *
 *  \ingroup ga_Device
 *  \param in_device Device in which to queue the buffer.
 *  \param in_buffer Buffer to add to the presentation queue.
 *  \return GC_SUCCESS if the buffer was queued successfully. GC_ERROR_GENERIC if not.
 *  \warning You should always call ga_device_check() prior to queueing a buffer! If 
 *           there isn't a free (unqueued) buffer, the operation will fail.
 */
gc_result ga_device_queue(ga_Device* in_device,
                          void* in_buffer);

/** Closes an open audio device.
 *
 *  \ingroup ga_Device
 *  \param in_device Device to close.
 *  \return GC_SUCCESS if the device was closed successfully. GC_ERROR_GENERIC if not.
 *  \warning The client must never use a device after calling ga_device_close().
 */
gc_result ga_device_close(ga_Device* in_device);


/*****************/
/*  Data Source  */
/*****************/
/** Abstract data source data structure and associated functions.
 *
 *  \ingroup external
 *  \defgroup ga_DataSource Data Source
 *  \todo Write an tutorial on how to write a ga_DataSource concrete implementation.
 *  \todo Design a clearer/better system for easily extending this data type.
 */

/** Abstract data source data structure [\ref MULTI_CLIENT].
 *
 *  A data source is a source of binary data, such as a file or socket, that 
 *  generates bytes of binary data. This data is usually piped through a sample 
 *  source to generate actual PCM audio data.
 *
 *  This object may only be used on the main thread.
 *
 *  \ingroup ga_DataSource
 */
typedef struct ga_DataSource ga_DataSource;

/** Enumerated seek origin values.
*
*  Used when seeking within data sources.
*
*  \ingroup ga_DataSource
*  \defgroup seekOrigins Seek Origins
*/
#define GA_SEEK_ORIGIN_SET 0 /**< Seek to an offset from the start of the source. \ingroup seekOrigins */
#define GA_SEEK_ORIGIN_CUR 1 /**< Seek to an offset from the current seek position. \ingroup seekOrigins */
#define GA_SEEK_ORIGIN_END 2 /**< Seek to an offset from the end of the source. \ingroup seekOrigins */

/** Reads binary data from the data source.
 *
 *  \ingroup ga_DataSource
 *  \param in_dataSrc Data source from which to read.
 *  \param in_dst Destination buffer into which bytes should be read. Guaranteed to
 *                be at least (in_size * in_count) bytes in size.
 *  \param in_size Size of a single element (in bytes).
 *  \param in_count Number of elements to read.
 *  \return Total number of bytes read into the destination buffer.
 */
gc_int32 ga_data_source_read(ga_DataSource* in_dataSrc, void* in_dst, gc_int32 in_size, gc_int32 in_count);

/** Seek to an offset within a data source.
 *
 *  \ingroup ga_DataSource
 *  \param in_dataSrc Data source to seek within.
 *  \param in_offset Offset (in bytes) from the specified seek origin.
 *  \param in_origin Seek origin (see [\ref seekOrigins]).
 *  \return If seek succeeds, returns 0, otherwise returns -1 (invalid seek request).
 *  \warning Only data sources with GA_FLAG_SEEKABLE can have ga_data_source_seek() called on them.
*/
gc_int32 ga_data_source_seek(ga_DataSource* in_dataSrc, gc_int32 in_offset, gc_int32 in_origin);

/** Tells the current read position of a data source.
 *
 *  \ingroup ga_DataSource
 *  \param in_dataSrc Data source to tell the read position of.
 *  \return The current data source read position.
 */
gc_int32 ga_data_source_tell(ga_DataSource* in_dataSrc);

/** Returns the bitfield of flags set for a data source (see \ref globDefs).
 *
 *  \ingroup ga_DataSource
 *  \param in_dataSrc Data source whose flags should be retrieved.
 *  \return The bitfield of flags set for the data source.
 */
gc_int32 ga_data_source_flags(ga_DataSource* in_dataSrc);

/** Acquires a reference for a data source.
 *
 *  Increments the data source's reference count by 1.
 *
 *  \ingroup ga_DataSource
 *  \param in_dataSrc Data source whose reference count should be incremented.
 */
void ga_data_source_acquire(ga_DataSource* in_dataSrc);

/** Releases a reference for a data source.
 *
 *  Decrements the data source's reference count by 1. When the last reference is
 *  released, the data source's resources will be deallocated.
 *
 *  \ingroup ga_DataSource
 *  \param in_dataSrc Data source whose reference count should be decremented.
 *  \warning A client must never use a data source after releasing its reference.
 */
void ga_data_source_release(ga_DataSource* in_dataSrc);


/*******************/
/*  Sample Source  */
/*******************/
/** Abstract sample source data structure and associated functions.
 *
 *  \ingroup external
 *  \defgroup ga_SampleSource Sample Source
 *  \todo Design a clearer/better system for easily extending this data type.
 */

/** Abstract sample source data structure [\ref MULTI_CLIENT].
 *
 *  A sample source is a source of PCM audio samples. These samples are usually 
 *  generated from a compatible data source or sample source, which is transformed
 *  or decoded into the resulting PCM audio data.
 *
 *  This object may only be used on the main thread.
 *
 *  \ingroup ga_SampleSource
 */
typedef struct ga_SampleSource ga_SampleSource;

/** On-seek callback function.
 *
 *  A callback that gets called while reading a sample source, if the sample source
 *  seeks as part of the read. This callback is used to implement gapless looping 
 *  features within the sample source pipeline.
 *
 *  \ingroup ga_SampleSource
 *  \param in_sample The sample the sample source was at when the seek happened.
 *  \param in_delta The signed distance from the old position to the new position.
 *  \param in_seekContext The user-specified context provided in ga_sample_source_read().
 */
typedef void (*tOnSeekFunc)(gc_int32 in_sample, gc_int32 in_delta, void* in_seekContext);

/** Reads samples from a samples source.
 *
 *  \ingroup ga_SampleSource
 *  \param in_sampleSrc Sample source from which to read.
 *  \param in_dst Destination buffer into which samples should be read. Must
 *                be at least (in_numSamples * sample-size) bytes in size.
 *  \param in_numSamples Number of samples to read.
 *  \param in_onSeekFunc The on-seek callback function for this read operation.
 *  \param in_seekContext User-specified context for the on-seek function.
 *  \return Total number of bytes read into the destination buffer.
 */
gc_int32 ga_sample_source_read(ga_SampleSource* in_sampleSrc, void* in_dst, gc_int32 in_numSamples,
                               tOnSeekFunc in_onSeekFunc, void* in_seekContext);

/** Checks whether a sample source has reached the end of the stream.
 *
 *  \ingroup ga_SampleSource
 *  \param in_sampleSrc Sample source to check.
 *  \return Whether the sample source has reached the end of the stream.
 */
gc_int32 ga_sample_source_end(ga_SampleSource* in_sampleSrc);

/** Checks whether a sample source has at least a given number of available
 *  samples.
 *
 *  If the sample source has fewer than in_numSamples samples left before it
 *  finishes, this function will returns GA_TRUE regardless of the number of
 *  samples.
 *
 *  \ingroup ga_SampleSource
 *  \param in_sampleSrc Sample source to check.
 *  \param in_numSamples The minimum number of samples required for the sample
 *                       source to be considered ready.
 *  \return Whether the sample source has at least a given number of available
 *          samples.
 */
gc_int32 ga_sample_source_ready(ga_SampleSource* in_sampleSrc, gc_int32 in_numSamples);

/** Seek to an offset (in samples) within a sample source.
 *
 *  \ingroup ga_SampleSource
 *  \param in_sampleSrc Sample source to seek within.
 *  \param in_sampleOffset Offset (in samples) from the start of the sample stream.
 *  \return If seek succeeds, returns 0, otherwise returns -1 (invalid seek request).
 *  \warning Only sample sources with GA_FLAG_SEEKABLE can have ga_sample_source_seek()
 *           called on them.
 */
gc_int32 ga_sample_source_seek(ga_SampleSource* in_sampleSrc, gc_int32 in_sampleOffset);

/** Tells the current sample number of a sample source.
 *
 *  \ingroup ga_SampleSource
 *  \param in_sampleSrc Sample source to tell the current sample number of.
 *  \param out_totalSamples If set, this value will be set to the total number of 
 *                          samples in the sample source. Output parameter.
 *  \return The current sample source sample number.
 */
gc_int32 ga_sample_source_tell(ga_SampleSource* in_sampleSrc, gc_int32* out_totalSamples);

/** Returns the bitfield of flags set for a sample source (see \ref globDefs).
 *
 *  \ingroup ga_SampleSource
 *  \param in_sampleSrc Sample source whose flags should be retrieved.
 *  \return The bitfield of flags set for the sample source.
 */
gc_int32 ga_sample_source_flags(ga_SampleSource* in_sampleSrc);

/** Retrieves the PCM sample format for a sample source.
 *
 *  \ingroup ga_SampleSource
 *  \param in_sampleSrc Sample source whose format should should be retrieved.
 *  \param out_format This value will be set to the same sample format 
 *                    as samples in the sample source. Output parameter.
 *  \todo Either return a copy of the format, or make it a const* return value.
 */
void ga_sample_source_format(ga_SampleSource* in_sampleSrc, ga_Format* out_format);

/** Acquires a reference for a sample source.
 *
 *  Increments the sample source's reference count by 1.
 *  
 *  \ingroup ga_SampleSource
 *  \param in_sampleSrc Sample source whose reference count should be incremented.
 */
void ga_sample_source_acquire(ga_SampleSource* in_sampleSrc);

/** Releases a reference for a sample source.
 *
 *  Decrements the sample source's reference count by 1. When the last reference is
 *  released, the sample source's resources will be deallocated.
 *
 *  \ingroup ga_SampleSource
 *  \param in_sampleSrc Sample source whose reference count should be decremented.
 *  \warning A client must never use a sample source after releasing its reference.
 */
void ga_sample_source_release(ga_SampleSource* in_sampleSrc);


/************/
/*  Memory  */
/************/
/** Shared (reference-counted) memory data structure and associated functions.
 *
 *  \ingroup external
 *  \defgroup ga_Memory Memory
 */

/** Shared memory object data structure [\ref MULTI_CLIENT].
 *
 *  As a way of sharing data between multiple client across multiple threads, 
 *  this data structure allows for a safe internal copy of the data. This is
 *  used in the internal implementation of ga_Sound, and can also be used to 
 *  play compressed audio directly from memory without having to read the data 
 *  source from a high-latency I/O interface or needlessly duplicate the data.
 *
 *  This object may be created on a secondary thread, but may otherwise only 
 *  be used on the main thread.
 *
 *  \ingroup ga_Memory
 */
typedef struct ga_Memory ga_Memory;

/** Create a shared memory object.
 *
 *  The buffer specified by in_data is copied into a newly-allocated internal 
 *  storage buffer. As such, you may safely free the data buffer passed into 
 *  ga_memory_create() as soon as the function returns.
 *  The returned memory object has an initial reference count of 1.
 *
 *  \ingroup ga_Memory
 *  \param in_data Data buffer to be copied into an internal data buffer.
 *  \param in_size Size (in bytes) of the provided data buffer.
 *  \return Newly-allocated memory object, containing an internal copy of the
 *          provided data buffer.
 */
ga_Memory* ga_memory_create(void* in_data, gc_int32 in_size);

/** Create a shared memory object from the full contents of a data source.
 *
 *  The full contents of the data source specified by in_dataSource are copied 
 *  into a newly-allocated internal storage buffer.
 *  The returned object has an initial reference count of 1.
 *
 *  \ingroup ga_Memory
 *  \param in_dataSource Data source to be read into an internal data buffer.
 *  \return Newly-allocated memory object, containing an internal copy of the
 *          full contents of the provided data source.
 */
ga_Memory* ga_memory_create_data_source(ga_DataSource* in_dataSource);

/** Retrieve the size (in bytes) of a memory object's stored data.
 *
 *  \ingroup ga_Memory
 *  \param in_mem Memory object whose stored data size should be retrieved.
 *  \return Size (in bytes) of the memory object's stored data.
 */
gc_int32 ga_memory_size(ga_Memory* in_mem);

/** Retrieve a pointer to a memory object's stored data.
 *
 *  \ingroup ga_Memory
 *  \param in_mem Memory object whose stored data pointer should be retrieved.
 *  \return Pointer to the memory object's stored data.
 *  \warning Never manually free the pointer returned by this function.
 */
void* ga_memory_data(ga_Memory* in_mem);

/** Acquires a reference for a memory object.
 *
 *  Increments the memory object's reference count by 1.
 *
 *  \ingroup ga_Memory
 *  \param in_mem Memory object whose reference count should be incremented.
 */
void ga_memory_acquire(ga_Memory* in_mem);

/** Releases a reference for a memory object.
 *
 *  Decrements the memory object's reference count by 1. When the last reference is
 *  released, the memory object's resources will be deallocated.
 *  
 *  \ingroup ga_Memory
 *  \param in_mem Memory object whose reference count should be decremented.
 *  \warning A client must never use a memory object after releasing its reference.
 */
void ga_memory_release(ga_Memory* in_mem);


/***********/
/*  Sound  */
/***********/
/** Shared (reference-counted) sound data structure and associated functions.
 *  
 *  \ingroup external
 *  \defgroup ga_Sound Sound
 */

/** Shared sound object data structure [\ref MULTI_CLIENT].
 *
 *  As a way of sharing sounds between multiple client across multiple threads, 
 *  this data structure allows for a safe internal copy of the PCM data. The 
 *  data buffer must contain only raw PCM data, not formatted or compressed 
 *  in any other way. To cache or share any other data, use a ga_Memory.
 *
 *  This object may be created on a secondary thread, but may otherwise only 
 *  be used on the main thread.
 * 
 *  \ingroup ga_Sound
 */
typedef struct ga_Sound ga_Sound;

/** Create a shared sound object.
 *
 *  The provided memory buffer must contain raw PCM data. The returned
 *  object has an initial reference count of 1. This function acquires a
 *  reference from the provided memory object.
 *
 *  \ingroup ga_Sound
 *  \param in_memory Shared memory object containing raw PCM data. This 
 *  function acquires a reference from the provided memory object.
 *  \param in_format Format of the raw PCM data contained by in_memory.
 *  \return Newly-allocated sound object.
 */
ga_Sound* ga_sound_create(ga_Memory* in_memory, ga_Format* in_format);

/** Create a shared memory object from the full contents of a sample source.
 *
 *  The full contents of the sample source specified by in_sampleSource are 
 *  streamed into a newly-allocated internal memory object.
 *  The returned sound object has an initial reference count of 1.
 *
 *  \ingroup ga_Sound
 *  \param in_sampleSrc Sample source to be read into an internal data buffer.
 *  \return Newly-allocated memory object, containing an internal copy of the
 *          full contents of the provided data source.
 */
ga_Sound* ga_sound_create_sample_source(ga_SampleSource* in_sampleSrc);

/** Retrieve a pointer to a sound object's stored data.
 *
 *  \ingroup ga_Sound
 *  \param in_sound Sound object whose stored data pointer should be retrieved.
 *  \return Pointer to the sound object's stored data.
 *  \warning Never manually free the pointer returned by this function.
 */
void* ga_sound_data(ga_Sound* in_sound);

/** Retrieve the size (in bytes) of a sound object's stored data.
 *
 *  \ingroup ga_Sound
 *  \param in_sound Sound object whose stored data size should be retrieved.
 *  \return Size (in bytes) of the sound object's stored data.
 */
gc_int32 ga_sound_size(ga_Sound* in_sound);

/** Retrieve the number of samples in a sound object's stored PCM data.
 *
 *  \ingroup ga_Sound
 *  \param in_sound Sound object whose number of samples should be retrieved.
 *  \return Number of samples in the sound object's stored PCM data.
 */
gc_int32 ga_sound_numSamples(ga_Sound* in_sound);

/** Retrieves the PCM sample format for a sound.
 *
 *  \ingroup ga_Sound
 *  \param in_sound Sound whose format should should be retrieved.
 *  \param out_format This value will be set to the same sample format 
 *                    as samples in the sound. Output parameter.
 *  \todo Either return a copy of the format, or make it a const* return value.
 */
void ga_sound_format(ga_Sound* in_sound, ga_Format* out_format);

/** Acquires a reference for a sound object.
 *
 *  Increments the sound object's reference count by 1.
 *
 *  \ingroup ga_Sound
 *  \param in_sound Sound object whose reference count should be incremented.
 *  \todo Either return a copy of the format, or make it a const* return value.
 */
void ga_sound_acquire(ga_Sound* in_sound);

/** Releases a reference for a sound object.
 *
 *  Decrements the sound object's reference count by 1. When the last reference is
 *  released, the sound object's resources will be deallocated.
 *  
 *  \ingroup ga_Sound
 *  \param in_sound Sound object whose reference count should be decremented.
 *  \warning A client must never use a sound object after releasing its reference.
 */
void ga_sound_release(ga_Sound* in_sound);


/************/
/*  Mixer  */
/************/
/** Multi-channel audio mixer data structure and associated functions.
 *
 *  \ingroup external
 *  \defgroup ga_Mixer Mixer
 */

/** Audio mixer data structure [\ref SINGLE_CLIENT].
 *
 *  The mixer mixes PCM samples from multiple audio handles into a single buffer
 *  of PCM samples. The mixer is responsible for applying handle parameters 
 *  such as gain, pan, and pitch. The mixer has a fixed sample size and format 
 *  that must be specified upon creation. Buffers passed in must be large enough
 *  to hold the specified number of samples of the specified format.
 *
 *  This object may only be used on the main thread.
 *
 *  \ingroup ga_Mixer
 */
typedef struct ga_Mixer ga_Mixer;

/** Creates a mixer object with the specified number and format of PCM samples.
 *
 *  \ingroup ga_Mixer
 *  \param in_format Format for the PCM samples produced by the buffer.
 *  \param in_numSamples Number of samples to be mixed at a time (must be a power-of-two).
 *  \return Newly-created mixer object.
 *  \warning The number of samples must be a power-of-two.
 *  \todo Remove the requirement that the buffer be a power-of-two in size.
 */
ga_Mixer* ga_mixer_create(ga_Format* in_format, gc_int32 in_numSamples);

/** Retrieves the PCM sample format for a mixer object.
 *
 *  \ingroup ga_Mixer
 *  \param in_mixer Mixer whose format should should be retrieved.
 *  \return Pointer to the internal format structure used by this object.
 *  \warning Do not modify the contents of this pointer
 *  \todo Either return a copy of the format, or make it a const*.
 */
ga_Format* ga_mixer_format(ga_Mixer* in_mixer);

/** Retrieve the number of samples in a mixer object's mix buffer.
 *
 *  \ingroup ga_Mixer
 *  \param in_mixer Mixer object whose number of samples should be retrieved.
 *  \return Number of samples in a mixer object's mix buffer.
 */
gc_int32 ga_mixer_numSamples(ga_Mixer* in_mixer);

/** Mixes samples from all ready handles into a single output buffer.
 *
 *  The output buffer is generally presented directly to the device queue
 *  for playback.
 *
 *  \ingroup ga_Mixer
 *  \param in_mixer Mixer object whose handles' samples should be mixed.
 *  \param out_buffer An empty buffer into which the mixed samples should be 
 *                    copied. The buffer must be large enough to hold the 
 *                    mixer's number of samples in the mixer's sample format.
 *  \return Whether the mixer successfully mixed the data. GA_SUCCESS if the 
 *          operation was successful, GA_ERROR_GENERIC if not.
 */
gc_result ga_mixer_mix(ga_Mixer* in_mixer, void* out_buffer);

/** Dispatches all pending finish callbacks.
 *
 *  This function should be called regularly. This function (like all other functions
 *  associated with this object) must be called from the main thread. All callbacks 
 *  will be called on the main thread.
 *
 *  \ingroup ga_Mixer
 *  \param in_mixer Mixer object whose handles' finish callbacks should be dispatched.
 *  \return Whether the mixer successfully dispatched the callbacks. GA_SUCCESS if the 
 *          operation was successful, GA_ERROR_GENERIC if not.
 */
gc_result ga_mixer_dispatch(ga_Mixer* in_mixer);

/** Destroys a mixer object.
 *
 *  \ingroup ga_Mixer
 *  \param in_mixer Mixer object to destroy.
 *  \return Whether the mixer was successfully destroyed. GA_SUCCESS if the
 *          operation was successful, GA_ERROR_GENERIC if not.
 *  \warning The client must never use a mixer after calling ga_mixer_destroy().
 */
gc_result ga_mixer_destroy(ga_Mixer* in_mixer);


/************/
/*  Handle  */
/************/
/** Audio playback control handle data structure and associated functions.
 *
 *  Handle objects let you control pitch/pan/gain on playing audio, as well
 *  as playback state (play/pause/stop).
 *
 *  \ingroup external
 *  \defgroup ga_Handle Handle
 */

/** Audio playback control handle data structure [\ref SINGLE_CLIENT].
 *
 *  A handle is a set of controls for manipulating the playback and mix 
 *  parameters of a sample source.
 *
 *  Handles track playback parameters such as pan, pitch, and gain (volume)
 *  which are taken into account during the mix. Additionally, each handle
 *  tracks its playback state (playing/stopped/finished).
 *
 *  This object may only be used on the main thread.
 *
 *  \ingroup ga_Handle
 */
typedef struct ga_Handle ga_Handle;

/** Enumerated handle parameter values.
 *
 *  Used when calling \ref ga_handle_setParamf() "ga_handle_setParam*()"
 *  or \ref ga_handle_getParamf() "ga_handle_getParam*()".
 *
 *  \ingroup ga_Handle
 *  \defgroup handleParams Handle Parameters
 */
#define GA_HANDLE_PARAM_UNKNOWN 0 /**< Unknown parameter. \ingroup handleParams */
#define GA_HANDLE_PARAM_PAN 1 /**< Left <-> right pan (center -> 0.0, left -> -1.0, right -> 1.0). Floating-point parameter. \ingroup handleParams */
#define GA_HANDLE_PARAM_PITCH 2 /**< Pitch/speed multiplier (normal -> 1.0). Floating-point parameter. \ingroup handleParams */
#define GA_HANDLE_PARAM_GAIN 3 /**< Gain/volume (silent -> 0.0, normal -> 1.0). Floating-point parameter. \ingroup handleParams */

/** Enumerated parameter values for ga_handle_tell().
 *
 *  Used in ga_handle_tell() to specify which value to return. 
 *
 *  \ingroup ga_Handle
 *  \defgroup tellParams Tell Parameters
 */
#define GA_TELL_PARAM_CURRENT 0 /**< Current playback position (in samples). \ingroup tellParams */
#define GA_TELL_PARAM_TOTAL 1 /**< Total samples in this handle's sample source. \ingroup tellParams */

/** Prototype for handle-finished-playback callback.
 *
 *  This callback will be called when the internal sampleSource ends. Stopping a handle
 *  does not generate this callback. Looping sample sources will never generate this
 *  callback.
 *
 *  \ingroup ga_Handle
 *  \param in_finishedHandle The handle that has finished playback.
 *  \param in_context The user-specified callback context.
 *  \warning This callback is thrown once the handle has finished playback, 
 *           after which the handle can no longer be used except to destroy it.
 *  \todo Allow handles with GA_FLAG_SEEKABLE to be rewound/reused once finished.
 */
typedef void (*ga_FinishCallback)(ga_Handle* in_finishedHandle, void* in_context);

/** Creates an audio playback control handle.
 *
 *  The sample source is not playing by default. To start playback, you must
 *  call ga_handle_play(). Default gain is 1.0. Default pan is 0.0. Default
 *  pitch is 1.0.
 *
 *  \ingroup ga_Handle
 *  \param in_mixer The mixer that should mix the handle's sample data.
 *  \param in_sampleSrc The sample source from which to stream samples.
 *  \todo Provide a way to query handles for flags.
 */
ga_Handle* ga_handle_create(ga_Mixer* in_mixer, ga_SampleSource* in_sampleSrc);

/** Destroys an audio playback handle.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle object to destroy.
 *  \return Whether the mixer was successfully destroyed. GA_SUCCESS if the
 *          operation was successful, GA_ERROR_GENERIC if not.
 *  \warning The client must never use a handle after calling ga_handle_destroy().
 */
gc_result ga_handle_destroy(ga_Handle* in_handle);

/** Starts playback on an audio playback handle.
 *
 *  It is valid to call ga_handle_play() on a handle that is already playing.
 *  
 *  \ingroup ga_Handle
 *  \param in_handle Handle object to play.
 *  \return Whether the handle played successfully. GA_SUCCESS if the
 *          operation was successful, GA_ERROR_GENERIC if not.
 *  \warning You cannot play a handle that has finished playing. When in doubt, check
 *           ga_handle_finished() to verify this state prior to calling play.
 */
gc_result ga_handle_play(ga_Handle* in_handle);

/** Stops playback of a playing audio playback handle.
 *
 *  It is valid to call ga_handle_stop() on a handle that is already stopped.
 *  
 *  \ingroup ga_Handle
 *  \param in_handle Handle object to stop.
 *  \return Whether the handle was stopped successfully. GA_SUCCESS if the
 *          operation was successful, GA_ERROR_GENERIC if not.
 *  \warning You cannot stop a handle that has finished playing. When in doubt, check
 *           ga_handle_finished() to verify this state prior to calling play.
 */
gc_result ga_handle_stop(ga_Handle* in_handle);

/** Checks whether a handle is currently playing.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle object to check.
 *  \return Whether the handle is currently playing.
 */
gc_int32 ga_handle_playing(ga_Handle* in_handle);

/** Checks whether a handle is currently stopped.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle object to check.
 *  \return Whether the handle is currently stopped.
 */
gc_int32 ga_handle_stopped(ga_Handle* in_handle);

/** Checks whether a handle is currently finished.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle object to check.
 *  \return Whether the handle is currently finished.
 */
gc_int32 ga_handle_finished(ga_Handle* in_handle);

/** Checks whether a handle is currently destroyed.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle object to check.
 *  \return Whether the handle is currently destroyed.
 */
gc_int32 ga_handle_destroyed(ga_Handle* in_handle);

/** Sets the handle-finished-playback callback for a handle.
 *
 *  This callback will be called right before the handle enters the 'finished'
 *  playback state. The callback value is set to 0 internally once it triggers.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle object to set the callback for.
 *  \param in_callback Callback function pointer.
 *  \param in_context User-specified callback context.
 *  \return Whether the handle's callback was set successfully. GA_SUCCESS if the
 *          operation was successful, GA_ERROR_GENERIC if not.
 */
gc_result ga_handle_setCallback(ga_Handle* in_handle,
                                ga_FinishCallback in_callback,
                                void* in_context);

/** Sets a floating-point parameter value on a handle.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle on which the parameter should be set
 *                   (see \ref handleParams).
 *  \param in_param Parameter to set (must be a floating-point value, see
 *                  \ref handleParams).
 *  \param in_value Value to set.
 *  \return Whether the parameter was set successfully. GA_SUCCESS if the
 *          operation was successful, GA_ERROR_GENERIC if not.
 */
gc_result ga_handle_setParamf(ga_Handle* in_handle,
                              gc_int32 in_param,
                              gc_float32 in_value);

/** Retrieves a floating-point parameter value from a handle.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle from which the parameter should be retrieved
 *                   (see \ref handleParams).
 *  \param in_param Parameter to retrieve (must be a floating-point value, see
 *                  \ref handleParams).
 *  \param out_value Output parameter. Retrieved parameter is copied into the
 *                   memory pointed at by this pointer.
 *  \return Whether the parameter was retrieved successfully. GA_SUCCESS if the
 *          operation was successful, GA_ERROR_GENERIC if not.
 */
gc_result ga_handle_getParamf(ga_Handle* in_handle,
                              gc_int32 in_param,
                              gc_float32* out_value);

/** Sets an integer parameter value on a handle.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle on which the parameter should be set (see \ref handleParams).
 *  \param in_param Parameter to set (must be an integer value, see
 *                  \ref handleParams).
 *  \param in_value Value to set. valid ranges vary depending by parameter
 *                  (see \ref handleParams).
 *  \return Whether the parameter was set successfully. GA_SUCCESS if the
 *          operation was successful, GA_ERROR_GENERIC if not.
 */
gc_result ga_handle_setParami(ga_Handle* in_handle,
                              gc_int32 in_param,
                              gc_int32 in_value);

/** Retrieves an integer parameter value from a handle.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle from which the parameter should be retrieved
 *                   (see \ref handleParams).
 *  \param in_param Parameter to retrieve (must be an integer value, see
 *                  \ref handleParams).
 *  \param out_value Output parameter. Retrieved parameter is copied into the
 *                   memory pointed at by this pointer.
 *  \return Whether the parameter was retrieved successfully. GA_SUCCESS if the
 *          operation was successful, GA_ERROR_GENERIC if not.
 */
gc_result ga_handle_getParami(ga_Handle* in_handle,
                              gc_int32 in_param,
                              gc_int32* out_value);

/** Seek to an offset (in samples) within a handle.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle to seek within.
 *  \param in_sampleOffset Offset (in samples) from the start of the handle.
 *  \return If seek succeeds, returns 0, otherwise returns -1 (invalid seek request).
 *  \warning Only handles containing sample sources with GA_FLAG_SEEKABLE can
 *           have ga_handle_seek() called on them.
 */
gc_result ga_handle_seek(ga_Handle* in_handle, gc_int32 in_sampleOffset);

/** Tells the current playback sample number or total samples of a handle.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle to query.
 *  \param in_param Tell value to retrieve (see \ref tellParams).
 *  \return The current handle playback sample number if in_param is set to 
 *          GA_TELL_PARAM_CURRENT. The total number of samples in the handle
 *          if in_param is set to GA_TELL_PARAM_TOTAL.
 */
gc_int32 ga_handle_tell(ga_Handle* in_handle, gc_int32 in_param);

/** Checks whether a handle has at least a given number of available samples.
 *
 *  If the handle has fewer than in_numSamples samples left before it finishes,
 *  this function will returns GA_TRUE regardless of the number of samples.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle to check.
 *  \param in_numSamples The minimum number of samples required for the handle
 *                       to be considered ready.
 *  \return Whether the handle has at least a given number of available samples.
 */
gc_int32 ga_handle_ready(ga_Handle* in_handle, gc_int32 in_numSamples);

/** Retrieves the PCM sample format for a handle.
 *
 *  \ingroup ga_Handle
 *  \param in_handle Handle whose format should should be retrieved.
 *  \param out_format This value will be set to the same sample format 
 *                    as samples streamed by the handle. Output parameter.
 *  \todo Either return a copy of the format, or make it a const* return value.
 */
void ga_handle_format(ga_Handle* in_handle, ga_Format* out_format);


/*****************************/
/*  Buffered-Stream Manager  */
/*****************************/
/** Buffered-stream manager data structure and related functions.
 *
 *  \defgroup ga_StreamManager Buffered Stream Manager
 *  \ingroup external
 */

/** Buffered-stream manager data structure [\ref SINGLE_CLIENT].
 *
 *  Manages a list of buffered streams, filling them. This class can be used
 *  on a background thread, to allow filling the streams without causing
 *  real-time applications to stutter.
 *
 *  \ingroup ga_StreamManager
 */
typedef struct ga_StreamManager ga_StreamManager;

/** Creates a buffered-stream manager.
 *
 *  \ingroup ga_StreamManager
 *  \return Newly-created stream manager.
 */
ga_StreamManager* ga_stream_manager_create();

/** Fills all buffers managed by a buffered-stream manager.
 *
 *  \ingroup ga_StreamManager
 *  \param in_mgr The buffered-stream manager whose buffers are to be filled.
 */
void ga_stream_manager_buffer(ga_StreamManager* in_mgr);

/** Destroys a buffered-stream manager.
 *
 *  \ingroup ga_StreamManager
 *  \param in_mgr The buffered-stream manager to be destroyed.
 *  \warning Never use a buffered-stream manager after it has been destroyed.
 */
void ga_stream_manager_destroy(ga_StreamManager* in_mgr);


/*********************/
/*  Buffered Stream  */
/*********************/
/** Buffered stream data structure and related functions.
 *
 *  \defgroup ga_BufferedStream Buffered Stream
 *  \ingroup external
 */

/** Buffered stream data structure [\ref MULTI_CLIENT].
 *
 *  Buffered streams read samples from a sample source into a buffer. They 
 *  support seeking, reading, and all other sample source functionality,
 *  albeit through a different interface. This is done to decouple the
 *  background-streaming logic from the audio-processing pipeline logic.
 *
 *  \ingroup ga_BufferedStream
 */
typedef struct ga_BufferedStream ga_BufferedStream;

/** Creates a buffered stream.
 *
 *  \ingroup ga_BufferedStream
 *  \param in_mgr Buffered-stream manager to manage the buffered stream
 *                (non-optional).
 *  \param in_sampleSrc Sample source to buffer samples from.
 *  \param in_bufferSize Size of the internal data buffer (in bytes). Must be
 *         a multiple of sample size.
 *  \return Newly-created buffered stream.
 *  \todo Change in_bufferSize to in_bufferSamples for a mre fault-resistant
 *        interface.
 */
ga_BufferedStream* ga_stream_create(ga_StreamManager* in_mgr, ga_SampleSource* in_sampleSrc, gc_int32 in_bufferSize);

/** Buffers samples from the sample source into the internal buffer (producer).
 *
 *  Can be called from a background thread.
 *
 *  \ingroup ga_BufferedStream
 *  \param in_stream Stream to produce samples.
 *  \warning This function should only ever be called by the buffered stream
 *           manager.
 */
void ga_stream_produce(ga_BufferedStream* in_stream); /* Can be called from a secondary thread */

/** Reads samples from a buffered stream.
 *
 *  \ingroup ga_BufferedStream
 *  \param in_stream Buffered stream from which to read.
 *  \param in_dst Destination buffer into which samples should be read. Must
 *                be at least (in_numSamples * sample size) bytes in size.
 *  \param in_numSamples Number of samples to read.
 *  \return Total number of bytes read into the destination buffer.
 */
gc_int32 ga_stream_read(ga_BufferedStream* in_stream, void* in_dst, gc_int32 in_numSamples);

/** Checks whether a buffered stream has reached the end of the stream.
 *
 *  \ingroup ga_BufferedStream
 *  \param in_stream Buffered stream to check.
 *  \return Whether the buffered stream has reached the end of the stream.
 */
gc_int32 ga_stream_end(ga_BufferedStream* in_stream);

/** Checks whether a buffered stream has at least a given number of available
 *  samples.
 *
 *  If the sample source has fewer than in_numSamples samples left before it
 *  finishes, this function will returns GA_TRUE regardless of the number of
 *  samples.
 *
 *  \ingroup ga_BufferedStream
 *  \param in_stream Buffered stream to check.
 *  \param in_numSamples The minimum number of samples required for the 
 *                       buffered stream to be considered ready.
 *  \return Whether the buffered stream has at least a given number of available
 *          samples.
 */
gc_int32 ga_stream_ready(ga_BufferedStream* in_stream, gc_int32 in_numSamples);

/** Seek to an offset (in samples) within a buffered stream.
 *
 *  \ingroup ga_BufferedStream
 *  \param in_stream Buffered stream to seek within.
 *  \param in_sampleOffset Offset (in samples) from the start of the contained
 *                         sample source.
 *  \return If seek succeeds, returns 0, otherwise returns -1 (invalid seek
 *          request).
 *  \warning Only buffered streams with GA_FLAG_SEEKABLE can have ga_stream_seek()
 *           called on them.
 */
gc_int32 ga_stream_seek(ga_BufferedStream* in_stream, gc_int32 in_sampleOffset);

/** Tells the current sample number of a buffered stream.
 *
 *  \ingroup ga_BufferedStream
 *  \param in_stream Buffered stream to tell the current sample number of.
 *  \param out_totalSamples If set, this value will be set to the total number of 
 *                          samples in the contained sample source. Output parameter.
 *  \return The current sample source sample number.
 */
gc_int32 ga_stream_tell(ga_BufferedStream* in_stream, gc_int32* out_totalSamples);

/** Returns the bitfield of flags set for a buffered stream (see \ref globDefs).
 *
 *  \ingroup ga_BufferedStream
 *  \param in_stream Buffered stream whose flags should be retrieved.
 *  \return The bitfield of flags set for the buffered stream.
 */
gc_int32 ga_stream_flags(ga_BufferedStream* in_stream);

/** Acquire a reference for a buffered stream.
 *
 *  Increments the buffered stream's reference count by 1.
 *
 *  \ingroup ga_BufferedStream
 *  \param in_stream Buffered stream whose reference count should be incremented.
 *  \todo Either return a copy of the format, or make it a const* return value.
 */
void ga_stream_acquire(ga_BufferedStream* in_stream);

/** Releases a reference for a buffered stream.
 *
 *  Decrements the buffered stream's reference count by 1. When the last reference is
 *  released, the buffered stream's resources will be deallocated.
 *  
 *  \ingroup ga_BufferedStream
 *  \param in_stream Buffered stream whose reference count should be decremented.
 *  \warning A client must never use a buffered stream after releasing its reference.
 */
void ga_stream_release(ga_BufferedStream* in_stream);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GORILLA_GA_H */
