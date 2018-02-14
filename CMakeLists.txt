cmake_minimum_required (VERSION 3.1)
project (Flounder)

# Options
option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)

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
	set(CMAKE_BUILD_TYPE "Release")
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

if(MSVC)
    add_definitions(-DFLOUNDER_COMPILER_MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /Ox /Ob2 /Oi /Ot /GT /GF /GS- /fp:fast /fp:except- /MP")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-DFLOUNDER_COMPILER_CLANG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall") # -Werror -pedantic
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") # -s
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_definitions(-DFLOUNDER_COMPILER_GNU)
	add_definitions(-D__USE_MINGW_ANSI_STDIO=0)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall") # -Werror -pedantic
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
endif()

# Output Directories
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/bin)

# Linked Resources
#Windows: mklink /J C:\Users\mattp\Documents\Flounder\Build\Debug\bin\Resources C:\Users\mattp\Documents\Flounder\Resources
#MacOS: ln -s /Users/albrmat/Desktop/Flounder/Resources /Users/albrmat/Desktop/Flounder/Build/Debug/bin/Resorces
#Linux: ln -s /home/mattp/Documents/Flounder/Resources /home/mattp/Documents/Flounder/Build/Debug/bin/Resorces

# Dependencies
include(cmake/FindVulkan.cmake)
include(cmake/FindOpenAL.cmake)
include(cmake/FindGLFW.cmake)
include(cmake/FindSTB.cmake)

# Engine Sources
add_subdirectory(Sources/FlounderEngine)
add_subdirectory(Sources/FlounderEditor)

# Example Sources
add_subdirectory(Sources/ExampleStarting)
