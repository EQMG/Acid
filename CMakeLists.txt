cmake_minimum_required (VERSION 3.1)
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
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall") # -Werror -pedantic
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall") # -Werror -pedantic
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
endif()

# Output-directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/bin)

# Libraries
set(GLFW_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/glfw/include")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(Libraries/glfw)

# set(ALSOFT_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/openal-soft/include")
# set(ALSOFT_UTILS OFF CACHE INTERNAL "Build and install utility programs")
# set(ALSOFT_EXAMPLES OFF CACHE INTERNAL "Build and install example programs")
# set(ALSOFT_TESTS OFF CACHE INTERNAL "Build and install test programs")
# set(ALSOFT_INSTALL OFF CACHE INTERNAL "Install headers and libraries")
# add_subdirectory(Libraries/openal-soft)

set(VULKAN_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/vulkan/include")
find_library(VULKAN_LIBRARY NAMES vulkan vulkan-1 PATHS "${PROJECT_SOURCE_DIR}/Libraries/vulkan/lib/")
# add_subdirectory(Libraries/vulkan)

set(LIBRARIES_INCLUDES ${GLFW_INCLUDES} ${VULKAN_INCLUDES}) # ${ALSOFT_INCLUDES} 

# Sources
add_subdirectory(Sources/FlounderEngine)
add_subdirectory(Sources/FlounderEditor)
add_subdirectory(Sources/ExampleStarting)
