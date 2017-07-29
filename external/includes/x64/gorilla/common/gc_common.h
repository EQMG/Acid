/** Gorilla Common API.
 *
 *  A collection of non-audio-specific classes that are common to most libraries.
 *
 *  \file gc_common.h
 */

/** Common data structures and functions.
 *
 *  \defgroup common Common API (GC)
 */

#ifndef _GORILLA_GC_COMMON_H
#define _GORILLA_GC_COMMON_H

#include "gc_types.h"
#include "gc_thread.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*************************/
/**  System Operations  **/
/*************************/
/** System operations.
 *
 *  \ingroup common
 *  \defgroup gc_SystemOps System Operations
 */

/** System allocation policies [\ref POD].
 *
 *  \ingroup gc_SystemOps
 */
typedef struct gc_SystemOps {
  void* (*allocFunc)(gc_uint32 in_size);
  void* (*reallocFunc)(void* in_ptr, gc_uint32 in_size);
  void (*freeFunc)(void* in_ptr);
} gc_SystemOps;
extern gc_SystemOps* gcX_ops;

/** Initialize the Gorilla library.
 *
 *  This must be called before any other functions in the library.
 *
 *  \ingroup gc_SystemOps
 *  \param in_callbacks You may (optionally) pass in a gc_SystemOps structure
 *                      to define custom allocation functions.  If you do not,
 *                      Gorilla will use standard ANSI C malloc/realloc/free
 *                      functions.
 *  \return GC_SUCCESS if library initialized successfully. GC_ERROR_GENERIC
 *          if not.
 */
gc_result gc_initialize(gc_SystemOps* in_callbacks);

/** Shutdown the Gorilla library.
 *
 *  Call this once you are finished using the library. After calling it,
 *  do not call any functions in the library.
 *
 *  \ingroup gc_SystemOps
 *  \return GC_SUCCESS if the library shut down successfully. GC_ERROR_GENERIC
 *          if not.
 */
gc_result gc_shutdown();

/***********************/
/**  Circular Buffer  **/
/***********************/
/** Circular Buffer.
 *
 *  \ingroup common
 *  \defgroup gc_CircBuffer Circular Buffer
 */

/** Circular buffer object [\ref SINGLE_CLIENT].
 *
 *  A circular buffer object that is thread-safe for single producer/single
 *  consumer use cases. It assumes a single thread producing (writing)
 *  data, and a single thread consuming (reading) data. The producer and 
 *  consumer threads may be the same thread.
 *
 *  \ingroup gc_CircBuffer
 *  \warning While it can be read/written from two different threads, the
 *           object’s memory management policy is Single Client, since there
 *           is only one owner responsible for creation/destruction of the
 *           thread.
 */
typedef struct gc_CircBuffer {
  gc_uint8* data;
  gc_uint32 dataSize;
  volatile gc_uint32 nextAvail;
  volatile gc_uint32 nextFree;
} gc_CircBuffer;

/** Create a circular buffer object.
 *
 *  \ingroup gc_CircBuffer
 */
gc_CircBuffer* gc_buffer_create(gc_uint32 in_size);

/** Destroy a circular buffer object.
 *
 *  \ingroup gc_CircBuffer
 */
gc_result gc_buffer_destroy(gc_CircBuffer* in_buffer);

/** Retrieve number of available bytes to read from a circular buffer object.
 *
 *  \ingroup gc_CircBuffer
 */
gc_uint32 gc_buffer_bytesAvail(gc_CircBuffer* in_buffer);

/** Retrieve number of free bytes to write to a circular buffer object.
 *
 *  \ingroup gc_CircBuffer
 */
gc_uint32 gc_buffer_bytesFree(gc_CircBuffer* in_buffer);

/** Retrieve write buffer(s) of free data in a circular buffer object.
 *
 *  \ingroup gc_CircBuffer
 *  \warning You must call gc_buffer_produce() to tell the buffer how many
 *           bytes you wrote to it.
 */
gc_result gc_buffer_getFree(gc_CircBuffer* in_buffer, gc_uint32 in_numBytes,
                            void** out_dataA, gc_uint32* out_sizeA,
                            void** out_dataB, gc_uint32* out_sizeB);

/** Write data to the circular buffer.
 *
 *  Easier-to-use than gc_buffer_getFree(), but less flexible.
 *
 *  \ingroup gc_CircBuffer
 *  \warning You must call gc_buffer_produce() to tell the buffer how many
 *           bytes you wrote to it.
 */
gc_result gc_buffer_write(gc_CircBuffer* in_buffer, void* in_data,
                          gc_uint32 in_numBytes);

/** Retrieve read buffer(s) of available data in a circular buffer object.
 *
 *  \ingroup gc_CircBuffer
 *  \warning You must call gc_buffer_consume() to tell the buffer how many
 *           bytes you read from it.
 */
gc_result gc_buffer_getAvail(gc_CircBuffer* in_buffer, gc_uint32 in_numBytes,
                             void** out_dataA, gc_uint32* out_sizeA,
                             void** out_dataB, gc_uint32* out_sizeB);

/** Read data from the circular buffer.
 *
 *  Easier-to-use than gc_buffer_getAvail(), but less flexible.
 *
 *  \ingroup gc_CircBuffer
 *  \warning You must call gc_buffer_consume() to tell the buffer how many
 *           bytes you read from it.
 */
void gc_buffer_read(gc_CircBuffer* in_buffer, void* in_data,
                    gc_uint32 in_numBytes);

/** Tell the buffer that bytes have been written to it.
 *
 *  \ingroup gc_CircBuffer
 */
void gc_buffer_produce(gc_CircBuffer* in_buffer, gc_uint32 in_numBytes);

/** Tell the buffer that bytes have been read from it.
 *
 *  \ingroup gc_CircBuffer
 */
void gc_buffer_consume(gc_CircBuffer* in_buffer, gc_uint32 in_numBytes);

/***********************/
/**  Linked List  **/
/***********************/
/** Linked list data structure and associated functions.
 *
 *  \ingroup common
 *  \defgroup gc_Link Linked List
 */

/** Linked list data structure [POD].
 *
 *  Intended usage: create a gc_Link head link. Add and remove additional links
 *  as needed. To iterate, start with it = head->next. Each loop, it = it->next.
 *  Terminate when it == &head.
 *
 *  \ingroup gc_Link
 */
typedef struct gc_Link {
  struct gc_Link* next;
  struct gc_Link* prev;
  void* data;
} gc_Link;


/** Initializes a linked list head element.
 *
 *  \ingroup gc_Link
 */
void gc_list_head(gc_Link* in_head);

/** Adds a link to a linked list (initializes the link).
 *
 *  \ingroup gc_Link
 */
void gc_list_link(gc_Link* in_head, gc_Link* in_link, void* in_data);

/** Removes a link from the linked list.
 *
 *  \ingroup gc_Link
 */
void gc_list_unlink(gc_Link* in_link);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GORILLA_GC_COMMON_H */
