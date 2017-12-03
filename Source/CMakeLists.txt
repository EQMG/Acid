cmake_minimum_required (VERSION 3.9)
project (Flounder)

# Platform
set(_CRT_SECURE_NO_WARNINGS 1)

#if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(FLOUNDER_CONFIG_DEBUG TRUE)
#else()
#	set(FLOUNDER_CONFIG_RELEASE TRUE)
#endif()

if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
	set(FLOUNDER_64BIT true)
endif()

if(WIN32)
	set(FLOUNDER_PLATFORM_WINDOWS TRUE)
elseif(UNIX AND NOT APPLE)
	set(FLOUNDER_PLATFORM_LINUX TRUE)
else(UNIX AND APPLE)
	set(FLOUNDER_PLATFORM_MACOS TRUE)
endif()

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
	set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS")
	
	if(FLOUNDER_64BIT) # Debug edit and continue for 64-bit
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
	else() # Normal debug for 32-bit
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(BS_COMPILER_FLAGS_COMMON "-Wall -fPIC -fno-exceptions -fno-strict-aliasing -fno-rtti -fno-ms-compatibility
	-fms-extensions -Wl,-rpath=$ORIGIN")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(BS_COMPILER_FLAGS_COMMON "-Wall -fPIC -fno-exceptions -fno-strict-aliasing -fno-rtti -Wl,-rpath=$ORIGIN")
else()

endif()

# Sub-directories
add_subdirectory(FlounderEngine)
# add_subdirectory(FlounderEditor)

# Executables
add_subdirectory(ExampleStarting)
