#pragma once

//
// Flounder exports (prerequisites) file.
//

#if FLOUNDER_PLATFORM_WINDOWS
#  if FLOUNDER_COMPILER_MSVC
#    if defined(FLOUNDER_STATIC_LIB)
#      define F_EXPORT
#    else
#      if defined(F_EXPORTS)
#        define F_EXPORT __declspec(dllexport)
#      else
#        define F_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(FLOUNDER_STATIC_LIB)
#      define F_EXPORT
#    else
#      if defined(F_EXPORTS)
#        define F_EXPORT __attribute__ ((dllexport))
#      else
#        define F_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define F_HIDDEN
#else
#  define F_EXPORT __attribute__ ((visibility ("default")))
#  define F_HIDDEN __attribute__ ((visibility ("hidden")))
#endif
