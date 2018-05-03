#pragma once

//
// Flounder exports (prerequisites) file.
//

#if FL_BUILD_WINDOWS
#  if FL_BUILD_MSVC
#    define FL_ALIGN(x) _declspec(align(x))
#    if defined(FL_STATICLIB)
#      define FL_EXPORT
#    else
#      if defined(FL_EXPORTS)
#        define FL_EXPORT __declspec(dllexport)
#      else
#        define FL_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    define FL_ALIGN(x) __attribute__ ((aligned(x)))
#    if defined(FL_STATICLIB)
#      define FL_EXPORT
#    else
#      if defined(FL_EXPORTS)
#        define FL_EXPORT __attribute__ ((dllexport))
#      else
#        define FL_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define FL_HIDDEN
#else
#  define FL_ALIGN(x) __attribute__ ((aligned(x)))
#  define FL_EXPORT __attribute__ ((visibility ("default")))
#  define FL_HIDDEN __attribute__ ((visibility ("hidden")))
#endif
