/** Gorilla Audio Utility API.
 *
 *  Utility data structures and functions to enhance Gorilla Audio's functionality.
 *
 *  \file gau.h
 */

#ifndef _GORILLA_GAU_H
#define _GORILLA_GAU_H

#include "gorilla/common/gc_common.h"
#include "gorilla/ga.h"
#include "gorilla/ga_internal.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** Data structures and functions.
 *
 *  \defgroup utility Utility API (GAU)
 */

/*************/
/*  Manager  */
/*************/
/** High-level audio manager and associated functions.
 *
 *  \ingroup utility
 *  \defgroup gau_Manager Manager
 */
  
/** High-level audio manager.
 *
 *  \ingroup gau_Manager
 */
typedef struct gau_Manager gau_Manager;

/** Manager thread policies.
 *
 *  \ingroup gau_Manager
 *  \defgroup threadPolicy Thread Policies
 */
#define GAU_THREAD_POLICY_UNKNOWN 0 /**< Unknown thread policy. \ingroup threadPolicy */
#define GAU_THREAD_POLICY_SINGLE 1 /**< Single-threaded policy (does not use background threads). \ingroup threadPolicy */
#define GAU_THREAD_POLICY_MULTI 2 /**< Multi-threaded mode (uses background threads). \ingroup threadPolicy */

/** Creates an audio manager.
 *
 *  \ingroup gau_Manager
 */
gau_Manager* gau_manager_create();

/** Creates an audio manager (customizable).
*
*  \ingroup gau_Manager
*/
gau_Manager* gau_manager_create_custom(gc_int32 in_devType,
                                       gc_int32 in_threadPolicy,
                                       gc_int32 in_numBuffers,
                                       gc_int32 in_bufferSamples);

/** Updates an audio manager.
 *
 *  \ingroup gau_Manager
 */
void gau_manager_update(gau_Manager* in_mgr);

/** Retrieves the internal mixer object from an audio manager.
 *
 *  \ingroup gau_Manager
 */
ga_Mixer* gau_manager_mixer(gau_Manager* in_mgr);

/** Retrieves the internal buffered stream manager object from an audio manager.
 *
 *  \ingroup gau_Manager
 */
ga_StreamManager* gau_manager_streamManager(gau_Manager* in_mgr);

/** Retrieves the internal device object from an audio manager.
 *
 *  \ingroup gau_Manager
 */
ga_Device* gau_manager_device(gau_Manager* in_mgr);

/** Destroys an audio manager.
 *
 *  \ingroup gau_Manager
 */
void gau_manager_destroy(gau_Manager* in_mgr);

/*****************************/
/**  Concrete Data Sources  **/
/*****************************/
/** Concrete data source implementations.
 *
 *  \ingroup utility
 *  \defgroup concreteData Concrete Data Sources
 */

/** Creates a data source of bytes from a file-on-disk.
 *
 *  \ingroup concreteData
 */
ga_DataSource* gau_data_source_create_file(const char* in_filename);

/** Creates a data source of bytes from a subregion of a file-on-disk.
 *
 *  \ingroup concreteData
 */
ga_DataSource* gau_data_source_create_file_arc(const char* in_filename, gc_int32 in_offset, gc_int32 in_size);

/** Creates a data source of bytes from a block of shared memory.
 *
 *  \ingroup concreteData
 */
ga_DataSource* gau_data_source_create_memory(ga_Memory* in_memory);

/*******************************/
/**  Concrete Sample Sources  **/
/*******************************/
/** Concrete sample source implementations.
 *
 *  \ingroup utility
 *  \defgroup concreteSample Concrete Sample Sources
 */

/** Creates a sample source of PCM samples from a WAVE file.
 *
 *  \ingroup concreteSample
 */
ga_SampleSource* gau_sample_source_create_wav(ga_DataSource* in_dataSrc);

/** Creates a sample source of PCM samples from an Ogg/Vorbis file.
 *
 *  \ingroup concreteSample
 */
ga_SampleSource* gau_sample_source_create_ogg(ga_DataSource* in_dataSrc);

/** Creates a buffered sample source of PCM samples from another sample source.
 *
 *  \ingroup concreteSample
 */
ga_SampleSource* gau_sample_source_create_buffered(ga_StreamManager* in_mgr, ga_SampleSource* in_sampleSrc, gc_int32 in_bufferSamples);

/** Creates a sample source of PCM samples from a cached sound object.
 *
 *  \ingroup concreteSample
 */
ga_SampleSource* gau_sample_source_create_sound(ga_Sound* in_sound);

/**************************/
/**  Loop Sample Source  **/
/**************************/
/** Loop sample source.
 *
 *  \ingroup concreteSample
 *  \defgroup loopSample Loop Sample Source
 */

/** Loop sample source.
 *
 *  Sample source that controls looping behavior of a contained sample source.
 *
 *  \ingroup loopSample
 */
typedef struct gau_SampleSourceLoop gau_SampleSourceLoop;

/** Create a loop sample source.
 *
 *  \ingroup loopSample
 */
gau_SampleSourceLoop* gau_sample_source_create_loop(ga_SampleSource* in_sampleSrc);

/** Set loop points on a loop sample source.
 *
 *  \ingroup loopSample
 */
void gau_sample_source_loop_set(gau_SampleSourceLoop* in_sampleSrc, gc_int32 in_triggerSample, gc_int32 in_targetSample);

/** Clear loop points on a loop sample source.
 *
 *  \ingroup loopSample
 */
void gau_sample_source_loop_clear(gau_SampleSourceLoop* in_sampleSrc);

/** Count number of times a loop sample source has looped.
 *
 *  \ingroup loopSample
 */
gc_int32 gau_sample_source_loop_count(gau_SampleSourceLoop* in_sampleSrc);

/***************************/
/**  On-Finish Callbacks  **/
/***************************/
/** Generic on-finish callbacks.
*
*  \ingroup utility
*  \defgroup onFinish On-Finish Callbacks
*/

/** On-finish callback that destroys the handle.
 *
 *  \ingroup onFinish
 */
void gau_on_finish_destroy(ga_Handle* in_finishedHandle, void* in_context);

/********************/
/**  Load Helpers  **/
/********************/
/** Functions that help load common sources of data into cached memory.
 *
 *  \ingroup utility
 *  \defgroup loadHelper Load Helpers
 */

/** Load a file's raw binary data into a memory object.
 *
 *  \ingroup loadHelper
 */
ga_Memory* gau_load_memory_file(const char* in_filename);

/** Load a file's PCM data into a sound object.
 *
 *  \ingroup loadHelper
 */
ga_Sound* gau_load_sound_file(const char* in_filename, const char* in_format);


/**********************/
/**  Create Helpers  **/
/**********************/
/** Functions that help to create common handle configurations.
*
*  \ingroup utility
*  \defgroup createHelper Create Helpers
*/

/** Create a handle to play a memory object in a given data format.
 *
 *  \ingroup createHelper
 */
ga_Handle* gau_create_handle_memory(ga_Mixer* in_mixer, ga_Memory* in_memory, const char* in_format,
                                    ga_FinishCallback in_callback, void* in_context,
                                    gau_SampleSourceLoop** out_loopSrc);

/** Create a handle to play a sound object.
 *
 *  \ingroup createHelper
 */
ga_Handle* gau_create_handle_sound(ga_Mixer* in_mixer, ga_Sound* in_sound,
                                   ga_FinishCallback in_callback, void* in_context,
                                   gau_SampleSourceLoop** out_loopSrc);

/** Create a handle to play a background-buffered stream from a data source.
 *
 *  \ingroup createHelper
 */
ga_Handle* gau_create_handle_buffered_data(ga_Mixer* in_mixer, ga_StreamManager* in_streamMgr,
                                           ga_DataSource* in_dataSrc, const char* in_format,
                                           ga_FinishCallback in_callback, void* in_context,
                                           gau_SampleSourceLoop** out_loopSrc);

/** Create a handle to play a background-buffered stream from a file.
 *
 *  \ingroup createHelper
 */
ga_Handle* gau_create_handle_buffered_file(ga_Mixer* in_mixer, ga_StreamManager* in_streamMgr,
                                           const char* in_filename, const char* in_format,
                                           ga_FinishCallback in_callback, void* in_context,
                                           gau_SampleSourceLoop** out_loopSrc);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GORILLA_GAU_H */
