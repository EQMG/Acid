/** Common Types.
 *
 *  Cross-platform primitive type definitions.
 *
 *  \file gc_types.h
 */

#ifndef _GORILLA_GC_TYPES_H
#define _GORILLA_GC_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
  WARNING: Do not typedef char or bool!
  (also, note that char != signed char != unsigned char)
  typedef char         char;
  typedef bool         bool;
*/

/****************************/
/**  Primitive Data Types  **/
/****************************/
/** Data Types
 *
 *  \ingroup common
 *  \defgroup dataTypes Data Types
 */

#ifdef _WIN32
  typedef unsigned char     gc_uint8;
  typedef unsigned short    gc_uint16;
  typedef unsigned int      gc_uint32;
  typedef unsigned __int64  gc_uint64;
  typedef signed char       gc_int8;
  typedef signed short      gc_int16;
  typedef signed int        gc_int32;
  typedef signed __int64    gc_int64;
  typedef float             gc_float32;
  typedef double            gc_float64;

#elif __GNUC__ /* GCC */
  typedef unsigned char          gc_uint8;
  typedef unsigned short         gc_uint16;
  typedef unsigned int           gc_uint32;
  typedef unsigned long long int gc_uint64;
  typedef signed char            gc_int8;
  typedef signed short           gc_int16;
  typedef signed int             gc_int32;
  typedef signed long long int   gc_int64;
  typedef float                  gc_float32;
  typedef double                 gc_float64;

#include <stdint.h>
#include <stddef.h>
#else
#error Types not yet specified for this platform
  
/** 8-bit  unsigned integer. \ingroup dataTypes */
typedef unsigned char           gc_uint8;
/** 16-bit unsigned integer. \ingroup dataTypes */
typedef unsigned short          gc_uint16;
/** 32-bit unsigned integer. \ingroup dataTypes */
typedef unsigned int            gc_uint32;
/** 64-bit unsigned integer. \ingroup dataTypes */
typedef unsigned long long int  gc_uint64;
/** 8-bit  signed integer.   \ingroup dataTypes */
typedef signed char             gc_int8;
/** 16-bit signed integer.   \ingroup dataTypes */
typedef signed short            gc_int16;
/** 32-bit signed integer.   \ingroup dataTypes */
typedef signed int              gc_int32;
/** 64-bit signed integer.   \ingroup dataTypes */
typedef signed long long int    gc_int64;
/** 32-bit floating-point. \ingroup dataTypes */
typedef float                   gc_float32;
/** 64-bit floating-point. \ingroup dataTypes */
typedef double                  gc_float64;

#endif

/*********************/
/**  Result Values  **/
/*********************/
/** Result Values
 *
 *  \ingroup common
 *  \defgroup results Result Values
 */

typedef gc_int32 gc_result; /**< Return type for the result of an operation. \ingroup results */

#define GC_FALSE 0 /**< Result was false. \ingroup results */
#define GC_TRUE 1 /**< Result was true. \ingroup results */
#define GC_SUCCESS 1 /**< Operation completed successfully. \ingroup results */
#define GC_ERROR_GENERIC -1 /**< Operation failed with an unspecified error. \ingroup results */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GORILLA_GC_TYPES_H */
