# 3.11.0 required for cxx_std_17 and FetchContent
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)
project(Acid VERSION 0.13.3 LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(BUILD_TESTS "Build test applications" ON)
option(ACID_INSTALL_EXAMPLES "Installs the examples" ON)
option(ACID_INSTALL_RESOURCES "Installs the Resources directory" ON)
option(ACID_LINK_RESOURCES "Links the Resources to the bin directory" ON)

# Sets the install directories defined by GNU
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Add property to allow making project folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# Position-independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to ON
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# Under some compilers CMAKE_DEBUG_POSTFIX is set to "d", removed to clean dll names
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Set Debug library postfix")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "Set Release library postfix")
set(CMAKE_RELWITHDEBINFO_POSTFIX "" CACHE STRING "Set RelWithDebInfo library postfix")
# Removes any dll prefix name on windows, unix will keep a prefix set as "lib"
if(WIN32)
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

# Used to include Acid cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

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

# Allows usage of configure time content
include(FetchContent)

# PhysFS is arranged before GLFW to prevent "uninstall" being defined twice
find_package(PhysFS QUIET)
if(NOT PHYSFS_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(physfs
			GIT_REPOSITORY https://github.com/Didstopia/physfs.git
			GIT_TAG master
			)
	FetchContent_GetProperties(physfs)
	if(NOT physfs_POPULATED)
		set(PHYSFS_BUILD_STATIC NOT${BUILD_SHARED_LIBS} CACHE INTERNAL "")
		set(PHYSFS_BUILD_SHARED ${BUILD_SHARED_LIBS} CACHE INTERNAL "")
		foreach(_physfs_option "PHYSFS_BUILD_TEST")
			set(${_physfs_option} OFF CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(physfs)
		add_subdirectory(${physfs_SOURCE_DIR} ${physfs_BINARY_DIR})
	endif()

	set(PHYSFS_INCLUDE_DIR "${physfs_SOURCE_DIR}/src")
	# Set here so we can use it in Sources/CMakeLists.txt
	if(PHYSFS_BUILD_STATIC)
		set(PHYSFS_LIBRARY "physfs-static")
	else()
		set(PHYSFS_LIBRARY "physfs")
	endif()
endif()

find_package(Freetype QUIET)
if(NOT Freetype_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(freetype
			GIT_REPOSITORY https://git.savannah.gnu.org/git/freetype/freetype2.git
			GIT_TAG master
			)
	FetchContent_GetProperties(freetype)
	if(NOT freetype_POPULATED)
		foreach(_freetype_option "CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz")
			set(${_freetype_option} ON CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(freetype)
		add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR})
	endif()

	# Used in target_link_libraries()
	set(FREETYPE_LIBRARIES "freetype")
endif()

find_package(glfw3 QUIET)
if(NOT TARGET glfw)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(glfw3
			GIT_REPOSITORY https://github.com/glfw/glfw.git
			GIT_TAG master
			)
	FetchContent_GetProperties(glfw3)
	if(NOT glfw3_POPULATED)
		foreach(_glfw3_option "GLFW_BUILD_TESTS" "GLFW_BUILD_EXAMPLES" "GLFW_BUILD_DOCS")
			set(${_glfw3_option} OFF CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(glfw3)
		add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
	endif()

	# Used later to define as a pre-build dependency
	# The glfw3Config.cmake file doesn't define it automatically
	set(glfw_FOUND false)
else()
	set(glfw_FOUND true)
endif()

# SPIRV and other GLSLang libraries are needed.
# NOTE: End-users can pass -DSPIRV_ROOT=/some/path to find the lib
set(SPIRV_ROOT CACHE PATH "An optional path to the system's SPIRV root dir to help find it. Ignore if building Glslang locally.")
find_library(SPIRV_LIBRARY
		NAMES "SPIRV" "libSPIRV"
		HINTS "${SPIRV_ROOT}"
		)
find_library(GLSLANG_LIBRARY
		NAMES "glslang" "libglslang"
		HINTS "${SPIRV_ROOT}"
		)
find_library(OSDEPENDENT_LIBRARY
		NAMES "OSDependent" "libOSDependent"
		HINTS "${SPIRV_ROOT}"
		)
find_library(OGLCOMPILER_LIBRARY
		NAMES "OGLCompiler" "libOGLCompiler"
		HINTS "${SPIRV_ROOT}"
		)
find_library(HLSL_LIBRARY
		NAMES "HLSL" "libHLSL"
		HINTS "${SPIRV_ROOT}"
		)
find_path(SPIRV_INCLUDE_DIR
		NAMES "GlslangToSpv.h"
		PATH_SUFFIXES "SPIRV"
		HINTS "${SPIRV_ROOT}"
		)

if(NOT SPIRV_LIBRARY
		OR NOT GLSLANG_LIBRARY
		OR NOT OSDEPENDENT_LIBRARY
		OR NOT OGLCOMPILER_LIBRARY
		OR NOT HLSL_LIBRARY
		OR NOT SPIRV_INCLUDE_DIR)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(glslang
			GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
			GIT_TAG master
			)
	FetchContent_GetProperties(glslang)
	if(NOT glslang_POPULATED)
		foreach(_glslang_option "BUILD_TESTING" "ENABLE_GLSLANG_BINARIES" "ENABLE_SPVREMAPPER" "ENABLE_HLSL" "ENABLE_AMD_EXTENSIONS" "ENABLE_NV_EXTENSIONS")
			set(${_glslang_option} OFF CACHE INTERNAL "")
		endforeach()
		foreach(_glslang_option "ENABLE_OPT")
			set(${_glslang_option} ON CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(glslang)
		add_subdirectory(${glslang_SOURCE_DIR} ${glslang_BINARY_DIR})
	endif()

	# Used later to define as a pre-build dependencies
	# Have to manually define because we manually searched for SPIRV
	set(SPIRV_FOUND false)
	# Used in target_link_libraries()
	# Please note that SPIRV is now a CMake target, which means transitive dependencies are taken into account.
	set(SPIRV_LIBRARIES "SPIRV")
else()
	set(SPIRV_FOUND true)
	# glslang, hlsl and the others are transitive dependencies of libSPIRV, which are not detected
	# during linking (because the project might be a shared object).
	set(SPIRV_LIBRARIES
			"${SPIRV_LIBRARY}"
			"${GLSLANG_LIBRARY}"
			"${OSDEPENDENT_LIBRARY}"
			"${OGLCOMPILER_LIBRARY}"
			"${HLSL_LIBRARY}"
			)
endif()

find_package(Bullet QUIET)
if(NOT BULLET_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(bullet3
			GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
			GIT_TAG master
			)
	FetchContent_GetProperties(bullet3)
	if(NOT bullet3_POPULATED)
		# On MSVC shared mode must be disabled with bullet currently
		set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
		foreach(_bullet3_option "BUILD_BULLET3" "BUILD_PYBULLET" "BUILD_BULLET2_DEMOS" "BUILD_OPENGL3_DEMOS" "BUILD_CPU_DEMOS" "BUILD_EXTRAS" "BUILD_UNIT_TESTS" "USE_GRAPHICAL_BENCHMARK" "USE_GLUT")
			set(${_bullet3_option} OFF CACHE INTERNAL "")
		endforeach()
		if(MSVC)
			set(BUILD_SHARED_LIBS OFF)
		endif()
		FetchContent_Populate(bullet3)
		add_subdirectory(${bullet3_SOURCE_DIR} ${bullet3_BINARY_DIR})
		# Reset back to value before MSVC fix
		set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")
	endif()

	set(BULLET_INCLUDE_DIRS "${bullet3_SOURCE_DIR}/src")
	# Used in target_link_libraries()
	set(BULLET_LIBRARIES "BulletSoftBody" "BulletDynamics" "BulletCollision" "LinearMath")
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
	add_subdirectory(Tests/Editor)
	add_subdirectory(Tests/EditorTest)
	
	add_subdirectory(Tests/TestFont)
	add_subdirectory(Tests/TestGUI)
	add_subdirectory(Tests/TestMaths)
	add_subdirectory(Tests/TestNetwork)
	add_subdirectory(Tests/TestPBR)
	add_subdirectory(Tests/TestPhysics)
	add_subdirectory(Tests/TestSerial)
endif()

