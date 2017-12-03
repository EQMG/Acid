cmake_minimum_required (VERSION 3.9)
project (Flounder)

# Platform
set(_CRT_SECURE_NO_WARNINGS 1)

#if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(FLOUNDER_CONFIG_DEBUG TRUE)
#else()
#	set(FLOUNDER_CONFIG_RELEASE TRUE)
#endif()

if(WIN32)
	set(FLOUNDER_PLATFORM_WINDOWS TRUE)
elseif(UNIX AND NOT APPLE)
	set(FLOUNDER_PLATFORM_LINUX TRUE)
else(UNIX AND APPLE)
	set(FLOUNDER_PLATFORM_MACOS TRUE)
endif()

set(CMAKE_CXX_STANDARD 17)

# Sub-directories
add_subdirectory(FlounderEngine)
# add_subdirectory(FlounderEditor)

# Executables
# add_subdirectory(ExamplesStarting)