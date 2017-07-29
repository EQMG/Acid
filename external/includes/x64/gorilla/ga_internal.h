/** Gorilla Internal Interface.
 *
 *  Internal data structures and functions.
 *
 *  \file ga_internal.h
 */

/** \defgroup internal Internal Interface
 * Internal data structures and functions used by Gorilla Audio.
 */

#ifndef _GORILLA_GA_INTERNAL_H
#define _GORILLA_GA_INTERNAL_H

#include "gorilla/common/gc_common.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/************/
/*  Device  */
/************/
/** Internal device object definition.
 *
 *  \ingroup internal
 *  \defgroup intDevice Device
 */

/** Header of shared-data for all concrete device implementations.
 *  
 *  Stores the device type, number of buffers, number of samples, and device PCM format.
 *
 *  \ingroup intDevice
 */
#define GA_DEVICE_HEADER gc_int32 devType; gc_int32 numBuffers; gc_int32 numSamples; ga_Format format;

/** Hardware device abstract data structure [\ref SINGLE_CLIENT].
 *
 *  Abstracts the platform-specific details of presenting audio buffers to sound playback hardware.
 *
 *  \ingroup intDevice
 *  \warning You can only have one device open at-a-time.
 *  \warning Never instantiate a ga_Device directly, unless you are implementing a new concrete
 *           device implementation. Instead, you should use ga_device_open().
 */
struct ga_Device {
  GA_DEVICE_HEADER
};

/*****************/
/*  Data Source  */
/*****************/
/** Internal data source object definition.
 *
 *  \ingroup internal
 *  \defgroup intDataSource Data Source
 */

/** Data source read callback prototype.
 *  
 *  \ingroup intDataSource
 *  \param in_context User context (pointer to the first byte after the data source).
 *  \param in_dst Destination buffer into which bytes should be read. Guaranteed to
 *                be at least (in_size * in_count) bytes in size.
 *  \param in_size Size of a single element (in bytes).
 *  \param in_count Number of elements to read.
 *  \return Total number of bytes read into the destination buffer.
 */
typedef gc_int32 (*tDataSourceFunc_Read)(void* in_context, void* in_dst, gc_int32 in_size, gc_int32 in_count);

/** Data source seek callback prototype.
 *  
 *  \ingroup intDataSource
 *  \param in_context User context (pointer to the first byte after the data source).
 *  \param in_offset Offset (in bytes) from the specified seek origin.
 *  \param in_origin Seek origin (see [\ref globDefs]).
 *  \return If seek succeeds, the callback should return 0, otherwise it should return -1.
 *  \warning Data sources with GA_FLAG_SEEKABLE should always provide a seek callback.
 *  \warning Data sources with GA_FLAG_SEEKABLE set should only return -1 in the case of
 *           an invalid seek request.
 *  \todo Define a less-confusing contract for extending/defining this function.
 */
typedef gc_int32 (*tDataSourceFunc_Seek)(void* in_context, gc_int32 in_offset, gc_int32 in_origin);

/** Data source tell callback prototype.
 *
 *  \ingroup intDataSource
 *  \param in_context User context (pointer to the first byte after the data source).
 *  \return The current data source read position.
 */
typedef gc_int32 (*tDataSourceFunc_Tell)(void* in_context);

/** Data source close callback prototype.
 *
 *  \ingroup intDataSource
 *  \param in_context User context (pointer to the first byte after the data source).
 */
typedef void (*tDataSourceFunc_Close)(void* in_context);

/** Abstract data source data structure [\ref MULTI_CLIENT].
 *
 *  A data source is a source of binary data, such as a file or socket, that 
 *  generates bytes of binary data. This data is usually piped through a sample 
 *  source to generate actual PCM audio data.
 *
 *  \ingroup intDataSource
 *  \todo Design a clearer/better system for easily extending this data type.
 */
struct ga_DataSource {
  tDataSourceFunc_Read readFunc; /**< Internal read callback. */
  tDataSourceFunc_Seek seekFunc; /**< Internal seek callback (optional). */
  tDataSourceFunc_Tell tellFunc; /**< Internal tell callback (optional). */
  tDataSourceFunc_Close closeFunc; /**< Internal close callback (optional). */
  gc_int32 refCount; /**< Reference count. */
  gc_Mutex* refMutex; /**< Mutex to protect reference count manipulations. */
  gc_int32 flags; /**< Flags defining which functionality this data source supports (see [\ref globDefs]). */
};

/** Initializes the reference count and other default values.
 *
 *  Because ga_DataSource is an abstract data type, this function should not be 
 *  called except when implement a concrete data source implementation.
 *
 *  \ingroup intDataSource
 */
void ga_data_source_init(ga_DataSource* in_dataSrc);

/*******************/
/*  Sample Source  */
/*******************/
typedef gc_int32 (*tSampleSourceFunc_Read)(void* in_context, void* in_dst, gc_int32 in_numSamples,
                                           tOnSeekFunc in_onSeekFunc, void* in_seekContext);
typedef gc_int32 (*tSampleSourceFunc_End)(void* in_context);
typedef gc_int32 (*tSampleSourceFunc_Ready)(void* in_context, gc_int32 in_numSamples);
typedef gc_int32 (*tSampleSourceFunc_Seek)(void* in_context, gc_int32 in_sampleOffset);
typedef gc_int32 (*tSampleSourceFunc_Tell)(void* in_context, gc_int32* out_totalSamples);
typedef void (*tSampleSourceFunc_Close)(void* in_context);

struct ga_SampleSource {
  tSampleSourceFunc_Read readFunc;
  tSampleSourceFunc_End endFunc;
  tSampleSourceFunc_Ready readyFunc;
  tSampleSourceFunc_Seek seekFunc; /* OPTIONAL */
  tSampleSourceFunc_Tell tellFunc; /* OPTIONAL */
  tSampleSourceFunc_Close closeFunc; /* OPTIONAL */
  ga_Format format;
  gc_int32 refCount;
  gc_Mutex* refMutex;
  gc_int32 flags;
};

void ga_sample_source_init(ga_SampleSource* in_sampleSrc);

/************/
/*  Memory  */
/************/
struct ga_Memory {
  void* data;
  gc_uint32 size;
  gc_int32 refCount;
  gc_Mutex* refMutex;
};

/***********/
/*  Sound  */
/***********/
struct ga_Sound {
  ga_Memory* memory;
  ga_Format format;
  gc_int32 numSamples;
  gc_int32 refCount;
  gc_Mutex* refMutex;
};

/************/
/*  Handle  */
/************/
#define GA_HANDLE_STATE_UNKNOWN 0
#define GA_HANDLE_STATE_INITIAL 1
#define GA_HANDLE_STATE_PLAYING 2
#define GA_HANDLE_STATE_STOPPED 3
#define GA_HANDLE_STATE_FINISHED 4
#define GA_HANDLE_STATE_DESTROYED 5

struct ga_Handle {
  ga_Mixer* mixer;
  ga_FinishCallback callback;
  void* context;
  gc_int32 state;
  gc_float32 gain;
  gc_float32 pitch;
  gc_float32 pan;
  gc_Link dispatchLink;
  gc_Link mixLink;
  gc_Mutex* handleMutex;
  ga_SampleSource* sampleSrc;
  volatile gc_int32 finished;
};

/************/
/*  Mixer  */
/************/
struct ga_Mixer {
  ga_Format format;
  ga_Format mixFormat;
  gc_int32 numSamples;
  gc_int32* mixBuffer;
  gc_Link dispatchList;
  gc_Mutex* dispatchMutex;
  gc_Link mixList;
  gc_Mutex* mixMutex;
};


struct ga_StreamManager {
  gc_Link streamList;
  gc_Mutex* streamListMutex;
};

struct ga_BufferedStream {
  gc_Link* streamLink;
  ga_SampleSource* innerSrc;
  gc_CircBuffer* buffer;
  gc_Mutex* produceMutex;
  gc_Mutex* seekMutex;
  gc_Mutex* readMutex;
  gc_Mutex* refMutex;
  gc_int32 refCount;
  gc_Link tellJumps;
  ga_Format format;
  gc_int32 seek;
  gc_int32 tell;
  gc_int32 nextSample;
  gc_int32 end;
  gc_int32 flags;
  gc_int32 bufferSize;
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GORILLA_GA_INTERNAL_H */
