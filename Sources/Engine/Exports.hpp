#pragma once

//
// Acid exports (prerequisites) file.
//

#if ACID_BUILD_WINDOWS
#  if ACID_BUILD_MSVC
#	if ACID_STATICLIB
#	  define ACID_EXPORT
#	else
#	  if ACID_EXPORTS
#		define ACID_EXPORT __declspec(dllexport)
#	  else
#		define ACID_EXPORT __declspec(dllimport)
#	  endif
#	 endif
#  else
#	if ACID_STATICLIB
#	  define ACID_EXPORT
#	else
#	  if ACID_EXPORTS
#		define ACID_EXPORT __attribute__ ((dllexport))
#	  else
#		define ACID_EXPORT __attribute__ ((dllimport))
#	  endif
#	 endif
#  endif
#  define ACID_HIDDEN
#else
#  define ACID_EXPORT __attribute__ ((visibility ("default")))
#  define ACID_HIDDEN __attribute__ ((visibility ("hidden")))
#endif
