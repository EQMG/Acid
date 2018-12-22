# 3.8.0 required for cxx_std_17
cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)
project(Acid VERSION 0.11.1 LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(BUILD_TESTS "Build test applications" ON)
option(ACID_INSTALL_EXAMPLES "Installs the examples." ON)
option(ACID_INSTALL_RESOURCES "Installs the Resources directory." ON)

# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# Under some compilers CMAKE_DEBUG_POSTFIX is set to "d", removed to clean dll names
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Adds a postfix for debug-built libraries.")
# Removes any dll prefix name on windows, unix will keep a prefix set as "lib"
if(WIN32)
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

# Sets the install directories defined by GNU
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Use ccache if it is installed, will cach previous compilations
find_program(CCACHE_FOUND "ccache")
if(CCACHE_FOUND)
	message(STATUS "Using ccache for building.")
	set_property(GLOBAL PROPERTY LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

# Looks for a appropriate threads package for this platform
find_package(Threads REQUIRED)
# Finds and loads Vulkan, env "VULKAN_SDK" must be set
find_package(Vulkan REQUIRED)

# OpenAL must be installed on the system, env "OPENALDIR" must be set
find_package(OpenAL REQUIRED)
if(OPENAL_FOUND AND NOT TARGET OpenAL::OpenAL)
	add_library(OpenAL::OpenAL UNKNOWN IMPORTED)
	set_target_properties(OpenAL::OpenAL PROPERTIES
			IMPORTED_LOCATION "${OPENAL_LIBRARY}"
			INTERFACE_INCLUDE_DIRECTORIES "${OPENAL_INCLUDE_DIR}")
endif()

# Used to track if we're using ONLY system libs
# Prevents errors with EXPORT
set(_ACID_ALL_SYSTEM_LIBS true)

# PhysFS is arranged before GLFW to prevent "uninstall" being defined twice
find_package(PhysFS QUIET)
if(NOT PHYSFS_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	foreach(_physfs_option "PHYSFS_BUILD_STATIC" "PHYSFS_BUILD_TEST")
		set(${_physfs_option} OFF CACHE INTERNAL "")
	endforeach()

	add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/physfs)
	target_include_directories(physfs PUBLIC "${PROJECT_SOURCE_DIR}/Libraries/physfs/src")
	# Set here so we can use it in Sources/CMakeLists.txt
	if(PHYSFS_BUILD_STATIC)
		set(PHYSFS_LIBRARY "physfs-static")
	else()
		set(PHYSFS_LIBRARY "physfs")
	endif()
endif()

find_package(Freetype QUIET)
if(NOT FREETYPE_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	foreach(_freetype_option "CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz")
		set(${_freetype_option} TRUE CACHE INTERNAL "")
	endforeach()

	add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/freetype2)
	set(FREETYPE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Libraries/freetype2/include")
	# Used in target_link_libraries()
	set(FREETYPE_LIBRARIES "freetype")
endif()

find_package(glfw3 QUIET)
if(NOT TARGET glfw)
	set(_ACID_ALL_SYSTEM_LIBS false)
	# Used later to define as a pre-build dependency
	# The glfw3Config.cmake file doesn't define it automatically
	set(glfw_FOUND false)
	foreach(_glfw3_option "GLFW_BUILD_TESTS" "GLFW_BUILD_EXAMPLES" "GLFW_BUILD_DOCS" "GLFW_INSTALL" "GLFW_VULKAN_STATIC")
		set(${_glfw3_option} OFF CACHE INTERNAL "")
	endforeach()

	add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/glfw)
else()
	set(glfw_FOUND true)
endif()

# SPIRV is needed (from Glslang)
# NOTE: End-users can pass -DSPIRV_ROOT=/some/path to find the lib
set(SPIRV_ROOT CACHE PATH "An optional path to the system's SPIRV root dir to help find it. Ignore if building Glslang locally.")
find_library(SPIRV_LIBRARY
		NAMES "SPIRV" "libSPIRV"
		HINTS "${SPIRV_ROOT}"
		)
find_path(SPIRV_INCLUDE_DIR
		NAMES "GlslangToSpv.h"
		PATH_SUFFIXES "SPIRV"
		HINTS "${SPIRV_ROOT}"
		)

if(NOT SPIRV_LIBRARY OR NOT SPIRV_INCLUDE_DIR)
	set(_ACID_ALL_SYSTEM_LIBS false)
	# Used later to define as a pre-build dependencies
	# Have to manually define because we manually searched for SPIRV
	set(SPIRV_FOUND false)
	foreach(_glslang_option "BUILD_TESTING" "ENABLE_GLSLANG_BINARIES" "ENABLE_SPVREMAPPER" "ENABLE_HLSL" "ENABLE_AMD_EXTENSIONS" "ENABLE_NV_EXTENSIONS" "ENABLE_OPT")
		set(${_glslang_option} OFF CACHE INTERNAL "")
	endforeach()
	foreach(_glslang_option "SKIP_GLSLANG_INSTALL")
		set(${_glslang_option} ON CACHE INTERNAL "")
	endforeach()

	add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/glslang)
	# Used in target_link_libraries()
	set(SPIRV_LIBRARY "SPIRV")
else()
	set(SPIRV_FOUND true)
endif()

find_package(Bullet QUIET)
if(NOT BULLET_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	foreach(_bullet_option "BUILD_BULLET3" "BUILD_PYBULLET" "BUILD_BULLET2_DEMOS" "BUILD_OPENGL3_DEMOS" "BUILD_CPU_DEMOS" "BUILD_EXTRAS" "BUILD_UNIT_TESTS" "USE_GRAPHICAL_BENCHMARK" "USE_GLUT" "INSTALL_LIBS" "INSTALL_CMAKE_FILES")
		set(${_bullet_option} OFF CACHE INTERNAL "")
	endforeach()

	# On MSVC shared mode must be disabled with bullet currently
	set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")

	if(MSVC)
		set(BUILD_SHARED_LIBS OFF)
	endif()

	add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/bullet3)
	target_include_directories(BulletDynamics PUBLIC "${PROJECT_SOURCE_DIR}/Libraries/bullet3/src")
	# Used in target_link_libraries()
	set(BULLET_LIBRARIES "BulletSoftBody" "BulletDynamics" "BulletCollision" "LinearMath")
	# Reset back to value before MSVC fix
	set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")
endif()

# Acid sources directory
add_subdirectory(Sources)

if(ACID_INSTALL_RESOURCES)
	# Install resources for end-user usage
	# because many source files use these
	install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
			# Example: this will install the Resources dir to /usr/share/Acid/Resources on Linux
			DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}"
		)
endif()

# Allows automation of "BUILD_TESTING"
include(CTest)
if(BUILD_TESTS)
	add_subdirectory(Tests/TestFont)
	add_subdirectory(Tests/TestGUI)
	add_subdirectory(Tests/TestMaths)
	add_subdirectory(Tests/TestNetwork)
	add_subdirectory(Tests/TestPBR)
	add_subdirectory(Tests/TestPhysics)
	add_subdirectory(Tests/TestVoxel)
endif()
