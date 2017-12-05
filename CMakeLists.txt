cmake_minimum_required (VERSION 3.8)
project (Flounder)

# Preprocessor
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DFLOUNDER_CONFIG_DEBUG)
else()
	add_definitions(-DFLOUNDER_CONFIG_RELEASE)
endif()

if(WIN32)
	add_definitions(-DFLOUNDER_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
	add_definitions(-DFLOUNDER_PLATFORM_LINUX)
else(UNIX AND APPLE)
	add_definitions(-DFLOUNDER_PLATFORM_MACOS)
endif()

# Compiler-options
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	set(relFlags "/Ox /Ob2 /Oi /Ot /GT /GF /GS- /fp:fast /fp:except- /MP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS -std=c++17")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
else(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall") # -Werror -pedantic
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -municode")
endif()

# Output-directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/bin)

# Sub-directories
add_subdirectory(Libraries)
add_subdirectory(Sources)
