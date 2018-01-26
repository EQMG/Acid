cmake_minimum_required (VERSION 3.1)
project (Flounder)

# Preprocessor
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DFLOUNDER_CONFIG_DEBUG)
	add_definitions(-DFLOUNDER_VERBOSE)
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
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") # -s
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_definitions(-D__USE_MINGW_ANSI_STDIO=0)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall") # -Werror -pedantic
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
endif()

# Output-directories
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/bin)

#mklink /J C:\Users\mattp\Documents\Flounder\Build\Debug\bin\Resources C:\Users\mattp\Documents\Flounder\Resources
#ln -s /Volumes/albrmat/Flounder/Resources /Volumes/albrmat/Flounder/Build/Debug/bin/Resorces
#file(COPY "${PROJECT_SOURCE_DIR}/Resources/" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources/")
#ADD_CUSTOM_TARGET(link_target ALL COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources/" "${PROJECT_SOURCE_DIR}/Resources/")

# Libraries
set(GLFW_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/glfw/include")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(Libraries/glfw)
set(GLFW_LIBRARY "glfw")

set(STB_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/stb/include")
add_subdirectory(Libraries/stb)
set(STB_LIBRARY "stb")

if(APPLE)
	FIND_PATH(OPENAL_INCLUDES al.h)
else()
	set(OPENAL_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/openal/include")
endif()
	find_library(OPENAL_LIBRARY NAMES OpenAL32 OpenAL PATHS "${PROJECT_SOURCE_DIR}/Libraries/openal/lib/")
message(STATUS "OpenAL: ${OPENAL_LIBRARY}")

set(VULKAN_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/vulkan/include")
find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS "${PROJECT_SOURCE_DIR}/Libraries/vulkan/lib/")
message(STATUS "Vulkan: ${VULKAN_LIBRARY}")

set(LIBRARIES_INCLUDES ${STB_INCLUDES} ${GLFW_INCLUDES} ${OPENAL_INCLUDES} ${VULKAN_INCLUDES})
set(LIBRARIES_LINKS "${STB_LIBRARY}" "${GLFW_LIBRARY}" "${OPENAL_LIBRARY}" "${VULKAN_LIBRARY}")

# Engine Sources
add_subdirectory(Sources/FlounderEngine)
add_subdirectory(Sources/FlounderEditor)

# Example Sources
add_subdirectory(Sources/ExampleStarting)
