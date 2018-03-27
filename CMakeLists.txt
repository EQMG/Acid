cmake_minimum_required (VERSION 3.1)
project (Flounder)

# Options
option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(FLOUNDER_INSTALL "Generate installation target" ON)
option(FLOUNDER_BUILD_EXAMPLES "Build the Flounder example programs" ON)
option(FLOUNDER_BUILD_TESTS "Build the Flounder test programs" ON)
option(FLOUNDER_SET_OUTPUT "If Flounder will set it's own outputs" ON)

set(LIB_TYPE STATIC)

if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
endif()

message(STATUS "Shared Libs: ${BUILD_SHARED_LIBS}")

# Preprocessor
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DFLOUNDER_VERBOSE)
    add_definitions(-DFLOUNDER_CONFIG_DEBUG)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	add_definitions(-DFLOUNDER_CONFIG_RELEASE)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    add_definitions(-DFLOUNDER_VERBOSE)
	add_definitions(-DFLOUNDER_CONFIG_RELEASE)
endif()

if(WIN32)
	add_definitions(-DFLOUNDER_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
	add_definitions(-DFLOUNDER_PLATFORM_LINUX)
elseif(UNIX AND APPLE)
	add_definitions(-DFLOUNDER_PLATFORM_MACOS)
endif()

# Compiler Options
message(STATUS "System: ${CMAKE_SYSTEM}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(MSVC)
    add_definitions(-DFLOUNDER_COMPILER_MSVC)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /Ox /Ob2 /Oi /Ot /GT /GF /GS- /fp:fast /fp:except- /MP /W3 /wd4251")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-DFLOUNDER_COMPILER_CLANG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall") # -Wextra -Werror -pedantic
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_definitions(-DFLOUNDER_COMPILER_GNU)
	add_definitions(-D__USE_MINGW_ANSI_STDIO=0)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall") # -Wextra -Werror -pedantic
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s")
endif()

if(FLOUNDER_PLATFORM_ANDROID)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
    list(APPEND LIBRARIES_INCLUDES ${ANDROID_NDK}/sources/android/native_app_glue/)
    list(APPEND LIBRARIES_LINKS "android log")
endif()

# Output Directories
if (FLOUNDER_SET_OUTPUT)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/bin)
endif()

# Dependencies
include(Libraries/FindVulkan.cmake)
include(Libraries/FindGlslang.cmake)
include(Libraries/FindOpenAL.cmake)
include(Libraries/FindGLFW.cmake)
include(Libraries/FindSTB.cmake)

message(${LIBRARIES_INCLUDES})
message(${LIBRARIES_LINKS})

# Engine Sources
add_subdirectory(Sources/FlounderEngine)

# Example Sources
if (FLOUNDER_BUILD_EXAMPLES)
	add_subdirectory(Sources/ExampleStarting)
endif()
