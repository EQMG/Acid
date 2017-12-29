cmake_minimum_required (VERSION 3.1)
project (Flounder)

# Preprocessor
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DFLOUNDER_CONFIG_DEBUG)
	add_definitions(-DFLOUNDER_VERBOSE)
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
message(STATUS "System: ${CMAKE_SYSTEM}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /Ox /Ob2 /Oi /Ot /GT /GF /GS- /fp:fast /fp:except- /MP")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall") # -Werror -pedantic
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_definitions(-D__USE_MINGW_ANSI_STDIO=0)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall") # -Werror -pedantic
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
endif()

# Output-directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/bin)

file(COPY "${PROJECT_SOURCE_DIR}/Resources/" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources/")

# Libraries
set(GLFW_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/glfw/include")
find_library(GLFW_LIBRARY NAMES glfw3 PATHS "${PROJECT_SOURCE_DIR}/Libraries/glfw/lib/")
message(STATUS "Glfw: ${GLFW_LIBRARY}")

set(OPENAL_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/openal/include")
find_library(OPENAL_LIBRARY NAMES OpenAL32 PATHS "${PROJECT_SOURCE_DIR}/Libraries/openal/lib/")
message(STATUS "OpenAL: ${OPENAL_LIBRARY}")

set(VULKAN_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/vulkan/include")
find_library(VULKAN_LIBRARY NAMES vulkan vulkan-1 PATHS "${PROJECT_SOURCE_DIR}/Libraries/vulkan/lib/")
message(STATUS "Vulkan: ${VULKAN_LIBRARY}")

set(LIBRARIES_INCLUDES ${GLFW_INCLUDES} ${OPENAL_INCLUDES} ${VULKAN_INCLUDES})

# Sources
add_subdirectory(Sources/FlounderEngine)
add_subdirectory(Sources/FlounderEditor)
add_subdirectory(Sources/Kosmos)
