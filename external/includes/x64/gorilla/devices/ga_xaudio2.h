/** XAudio2 Device Implementation.
 *
 *  \file ga_xaudio2.h
 */

#ifndef _GORILLA_GA_XAUDIO2_H
#define _GORILLA_GA_XAUDIO2_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "gorilla/ga_internal.h"

typedef struct ga_DeviceImpl_XAudio2
{
  GA_DEVICE_HEADER
  struct IXAudio2* xa;
  struct IXAudio2MasteringVoice* master;
  struct IXAudio2SourceVoice* source;
  gc_int32 sampleSize;
  gc_uint32 nextBuffer;
  void** buffers;
} ga_DeviceImpl_XAudio2;

ga_DeviceImpl_XAudio2* gaX_device_open_xaudio2(gc_int32 in_numBuffers,
                                               gc_int32 in_numSamples,
                                               ga_Format* in_format);
gc_int32 gaX_device_check_xaudio2(ga_DeviceImpl_XAudio2* in_device);
gc_result gaX_device_queue_xaudio2(ga_DeviceImpl_XAudio2* in_device,
                                   void* in_buffer);
gc_result gaX_device_close_xaudio2(ga_DeviceImpl_XAudio2* in_dev);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GORILLA_GA_XAUDIO2_H */
