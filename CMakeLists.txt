cmake_minimum_required (VERSION 3.1)
project (Flounder)

# Options
option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(FLOUNDER_INSTALL "Generate installation target" OFF)
option(FLOUNDER_BUILD_TESTING "Build the Flounder test programs" ON)
option(FLOUNDER_SETUP_COMPILER "If Flounder will set it's own compiler settings" ON)
option(FLOUNDER_SETUP_OUTPUT "If Flounder will set it's own outputs" ON)

set(LIB_TYPE STATIC)

if(BUILD_SHARED_LIBS)
	set(LIB_TYPE SHARED)
endif()

message(STATUS "Shared Libs: ${BUILD_SHARED_LIBS}")

# Preprocessor
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DFL_VERBOSE)
	add_definitions(-DFL_BUILD_DEBUG)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	add_definitions(-DFL_BUILD_RELEASE)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
	add_definitions(-DFL_VERBOSE)
	add_definitions(-DFL_BUILD_RELEASE)
endif()

if(WIN32)
	message(STATUS "Platform: Windows")
	add_definitions(-DFL_BUILD_WINDOWS)
elseif(UNIX AND NOT APPLE)
	message(STATUS "Platform: Linux")
	add_definitions(-DFL_BUILD_LINUX)
elseif(UNIX AND APPLE)
	message(STATUS "Platform: Apple")
	add_definitions(-DFL_BUILD_MACOS)
elseif(ANDROID)
	message(STATUS "Platform: Android")
	add_definitions(-DFL_BUILD_ANDROID)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(FLOUNDER_64BIT true)
    add_definitions(-DFL_64BIT)
else()
    set(FLOUNDER_32BIT true)
    add_definitions(-DFL_32BIT)
endif()

# Compiler Options
if (FLOUNDER_SETUP_COMPILER)
	message(STATUS "System: ${CMAKE_SYSTEM}")
	message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
	message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
	message(STATUS "Sizeof Void: ${CMAKE_SIZEOF_VOID_P}")

	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_EXTENSIONS ON)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_C_STANDARD 11)
	set(CMAKE_C_STANDARD_REQUIRED ON)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Adds a postfix for debug-built libraries.")

	if(MSVC)
		add_definitions(-DFL_BUILD_MSVC)
		add_definitions(-D_SCL_SECURE_NO_WARNINGS)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /std:c++17 /Oi /Ot /GT /GF /GS- /fp:fast /fp:except- /arch:SSE2 /MP /W3 /wd4251")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Ob0 /RTC1 /MDd /DEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ob2 /MD")

		if (FLOUNDER_64BIT)
			set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
			set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /OPT:ICF")
		else()
			set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
		endif()
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		add_definitions(-DFL_BUILD_CLANG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -msse3 -mfpmath=sse") # -Wextra -Werror -pedantic
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG -Wno-unused-variable")
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_definitions(-DFL_BUILD_GNU)
		add_definitions(-D__USE_MINGW_ANSI_STDIO=0)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -mavx -msse3 -mfpmath=sse") # -Wextra -Werror -pedantic
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s -DNDEBUG -Wno-unused-variable")
	endif()

	if(ANDROID)
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
		list(APPEND LIBRARIES_INCLUDES ${ANDROID_NDK}/sources/android/native_app_glue/)
		list(APPEND LIBRARIES_LINKS "android log")
	endif()
endif()

# Output Directories
if (FLOUNDER_SETUP_OUTPUT)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/bin)

	set(CMAKE_INSTALL_LIBDIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	set(CMAKE_INSTALL_BINDIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	set(CMAKE_INSTALL_INCLUDEDIR ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/include)
endif()

# Dependencies
include(Libraries/FindVulkan.cmake)
include(Libraries/FindVWSI.cmake)
include(Libraries/FindGlslang.cmake)
include(Libraries/FindOpenAL.cmake)
include(Libraries/FindSTB.cmake)
include(Libraries/FindBullet.cmake)

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} PARENT_SCOPE)
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} PARENT_SCOPE)
message(${LIBRARIES_INCLUDES})
message(${LIBRARIES_LINKS})

# Engine Sources
add_subdirectory(Sources)

# Example Sources
if (FLOUNDER_BUILD_TESTING)
	add_subdirectory(Tests/TestDeferred)
	add_subdirectory(Tests/TestGuis)
	add_subdirectory(Tests/TestMaths)
endif()
