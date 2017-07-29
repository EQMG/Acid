/** OpenAL Device Implementation.
 *
 *  \file ga_openal.h
 */

#ifndef _GORILLA_GA_OPENAL_H
#define _GORILLA_GA_OPENAL_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "gorilla/ga_internal.h"

typedef struct ga_DeviceImpl_OpenAl
{
  GA_DEVICE_HEADER
  struct ALCdevice_struct* dev;
  struct ALCcontext_struct* context;
  gc_uint32* hwBuffers;
  gc_uint32 hwSource;
  gc_uint32 nextBuffer;
  gc_uint32 emptyBuffers;
} ga_DeviceImpl_OpenAl;

ga_DeviceImpl_OpenAl* gaX_device_open_openAl(gc_int32 in_numBuffers,
                                             gc_int32 in_numSamples,
                                             ga_Format* in_format);
gc_int32 gaX_device_check_openAl(ga_DeviceImpl_OpenAl* in_device);
gc_result gaX_device_queue_openAl(ga_DeviceImpl_OpenAl* in_device,
                                  void* in_buffer);
gc_result gaX_device_close_openAl(ga_DeviceImpl_OpenAl* in_device);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GORILLA_GA_OPENAL_H */
