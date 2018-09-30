#pragma once

//
// Acid exports (prerequisites) file.
//

#if defined(ACID_BUILD_WINDOWS)
#  ifdef ACID_BUILD_MSVC
#	ifdef ACID_STATICLIB
#	  define ACID_EXPORT
#	else
#	  ifdef ACID_EXPORTS
#		define ACID_EXPORT __declspec(dllexport)
#	  else
#		define ACID_EXPORT __declspec(dllimport)
#	  endif
#	 endif
#  else
#	ifdef ACID_STATICLIB
#	  define ACID_EXPORT
#	else
#	  ifdef ACID_EXPORTS
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
