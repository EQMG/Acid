# 3.12.0 required for cxx_std_20 and FetchContent
cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)
project(Acid VERSION 0.16.0 LANGUAGES CXX C)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(BUILD_EDITOR "Build editor" ON)
option(BUILD_EXAMPLES "Build example applications" ON)
option(BUILD_TESTS "Build unit tests" ON)
option(ACID_LINK_RESOURCES "Passes local Resources directory into debug Confg" ON)
option(ACID_INSTALL_RESOURCES "Installs the Resources directory" ON)

# Used to include Acid CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/CMake" CACHE STRING "Modules for CMake")

# Sets the install directories defined by GNU
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
# Visual Studio will default to Build/x64-Debug/bin/Debug, this removed the Debug postfix folder
# https://stackoverflow.com/questions/7229571/cmake-visual-studio-debug-folder
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
endforeach()

# Add property to allow making project folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON CACHE BOOL "")
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON CACHE BOOL "")
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Whether to create a position-independent target")
if(BUILD_SHARED_LIBS)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON CACHE BOOL "Export all symbols")
endif()
# Under some compilers CMAKE_DEBUG_POSTFIX is set to "d", removed to clean dll names
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Set Debug library postfix")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "Set Release library postfix")
set(CMAKE_RELWITHDEBINFO_POSTFIX "" CACHE STRING "Set RelWithDebInfo library postfix")
set(CMAKE_MINSIZEREL_POSTFIX "" CACHE STRING "Set MinsizeRel library postfix")
# Removes any dll prefix name on windows, unix will keep a prefix set as "lib"
if(WIN32)
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

# Multithreaded MSVC builds
if(MSVC_VERSION GREATER 1500 AND ${CMAKE_VERSION} VERSION_GREATER "2.8.6")
	include(ProcessorCount)
	ProcessorCount(N)
	if(NOT N EQUAL 0)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP${N}" CACHE STRING "")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MP${N}" CACHE STRING "")
		set(CMAKE_CSharp_FLAGS "${CMAKE_CSharp_FLAGS} /m:${N}" CACHE STRING "")
	endif()
endif()

# Force all compilers to use the correct C and C++ standard versions
# Needed to allow for CheckCXXSourceCompiles to work correctly
set(CMAKE_REQUIRED_FLAGS "-std=c++20 -std=c11")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
string(CONCAT CMAKE_EXPERIMENTAL_CXX_SCANDEP_SOURCE
  "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> <SOURCE>"
  " -MT <DYNDEP_FILE> -MD -MF <DEP_FILE>"
  " ${flags_to_scan_deps} -fdep-file=<DYNDEP_FILE> -fdep-output=<OBJECT>"
  )

# Directory that Acid resources can be found.
get_filename_component(ACID_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources ABSOLUTE)
# Directory where external third_party sources are located
get_filename_component(THIRD_PARTY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External ABSOLUTE)
get_filename_component(THIRD_PARTY_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/External ABSOLUTE)

add_subdirectory(Engine)

#if(BUILD_EDITOR)
#	add_subdirectory(Editor)
#endif

if(BUILD_EXAMPLES)
	add_subdirectory(Examples)
endif()

# Must be a top level before add_subdirectory to make ctest pick up the individual add_test commands
enable_testing()

# Allows automation of "BUILD_TESTING"
include(CTest)
if(BUILD_TESTS)
	add_subdirectory(Tests)
endif()
