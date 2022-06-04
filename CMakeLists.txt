# 3.11.0 required for cxx_std_17 and FetchContent
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)
project(Acid VERSION 0.15.0 LANGUAGES CXX C)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(BUILD_EDITOR "Build editor" ON)
option(BUILD_EXAMPLES "Build example applications" ON)
option(BUILD_TESTS "Build unit tests" ON)
option(ACID_INSTALL_RESOURCES "Installs the Resources directory" ON)
option(ACID_LINK_RESOURCES "Passes local Resources directory into debug Confg" ON)

# Used to include Acid CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/CMake" CACHE STRING "Modules for CMake" FORCE)

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

# Installs headers, preserving their file structure
macro(install_headers_with_dir _component_name _source_dir _header_list)
	foreach(_header IN ITEMS ${_header_list})
		file(RELATIVE_PATH _rel_header "${_source_dir}" ${_header})
	    get_filename_component(_header_dir ${_rel_header} DIRECTORY)
		install(FILES ${_header}
				DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}/${_header_dir}"
				)
	endforeach()
endmacro()

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
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP${N}" CACHE STRING "" FORCE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MP${N}" CACHE STRING "" FORCE)
		set(CMAKE_CSharp_FLAGS "${CMAKE_CSharp_FLAGS} /m:${N}" CACHE STRING "" FORCE)
	endif()
endif()

# Force all compilers to use the correct C and C++ standard versions
# Needed to allow for CheckCXXSourceCompiles to work correctly
set(CMAKE_REQUIRED_FLAGS "-std=c++17 -std=c11")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Uses Git to find the current git branch and commit
find_package(Git REQUIRED)

#execute_process(
#		COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#		COMMAND_ERROR_IS_FATAL ANY
#		)
execute_process(
		COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE ACID_GIT_BRANCH
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
		)
execute_process(
		COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE ACID_GIT_COMMIT_HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
		)

# Directory where external third_party sources are located
get_filename_component(ACID_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External ABSOLUTE)

if(ACID_INSTALL_RESOURCES)
	# Directory that Acid resources can be found.
	get_filename_component(ACID_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources ABSOLUTE)
	# Install resources for end-user usage because many source files use these
	install(DIRECTORY "${ACID_RESOURCES_DIR}"
			# Example: this will install the Resources dir to /usr/share/Acid/Resources on Linux
			DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}"
			)
endif()

# Acid components
add_subdirectory(Engine/Core)
add_subdirectory(Engine/Network)
add_subdirectory(Engine/Context)
add_subdirectory(Engine/Input)
add_subdirectory(Engine/Graphics)
add_subdirectory(Engine/Ui)
add_subdirectory(Engine/Scene)
add_subdirectory(Engine/Sound)
add_subdirectory(Engine/Shim)
add_subdirectory(Engine/Animation)
add_subdirectory(Engine/Physics)

#if(BUILD_EDITOR)
#	add_subdirectory(Editor)
#endif

#if(BUILD_EXAMPLES)
#	add_subdirectory(Examples)
#endif()

# Must be a top level before add_subdirectory to make ctest pick up the individual add_test commands
enable_testing()

# Allows automation of "BUILD_TESTING"
#include(CTest)
#if(BUILD_TESTS)
#	add_subdirectory(Tests)
#endif()
