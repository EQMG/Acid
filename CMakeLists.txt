# 3.11.0 required for cxx_std_17 and FetchContent
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)
project(Acid VERSION 0.13.6 LANGUAGES CXX C)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(BUILD_TESTS "Build test applications" ON)
# BUILD_UNIT_TESTS conflicts with bullet unit tests.
option(ACID_BUILD_UNIT_TESTS "Build unit tests" ON)
option(ACID_INSTALL_EXAMPLES "Installs the examples" ON)
option(ACID_INSTALL_RESOURCES "Installs the Resources directory" ON)
option(ACID_LINK_RESOURCES "Passes local Resources directory into debug Confg" ON)

# Sets the install directories defined by GNU
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Add property to allow making project folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to ON
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# Under some compilers CMAKE_DEBUG_POSTFIX is set to "d", removed to clean dll names
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Set Debug library postfix")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "Set Release library postfix")
set(CMAKE_RELWITHDEBINFO_POSTFIX "" CACHE STRING "Set RelWithDebInfo library postfix")
set(CMAKE_MINSIZEREL_POSTFIX "" CACHE STRING "Set MinsizeRel library postfix")
# Removes any dll prefix name on windows, unix will keep a prefix set as "lib"
if(WIN32)
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

# Force all compilers to use the correct C and C++ standard versions.
set(CMAKE_REQUIRED_FLAGS "-std=c++17 -std=c11")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Assume built-in pthreads on macOS
if(APPLE)
	set(CMAKE_THREAD_LIBS_INIT "-lpthread")
	set(CMAKE_HAVE_THREADS_LIBRARY 1)
	set(CMAKE_USE_WIN32_THREADS_INIT 0)
	set(CMAKE_USE_PTHREADS_INIT 1)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	set(CMAKE_FIND_FRAMEWORK LAST)
	set(CMAKE_FIND_APPBUNDLE LAST)
endif()
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4592 /wd4018 /wd4101 /wd4244 /wd4267 /wd4756 /wd4309 -D_CRT_SECURE_NO_WARNINGS" CACHE STRING "" FORCE)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO" CACHE STRING "" FORCE)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO" CACHE STRING "" FORCE)
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO" CACHE STRING "" FORCE)
endif()

# Used to include Acid CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/CMake" CACHE STRING "Modules for CMake" FORCE)

# Uses Git to find the current git branch and commit.
execute_process(
		COMMAND git rev-parse --abbrev-ref HEAD
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE ACID_GIT_BRANCH
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
		)

execute_process(
		COMMAND git log -1 --format=%h
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE ACID_GIT_COMMIT_HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
		)

# Acid sources directory
add_subdirectory(Sources)

# Allows automation of "BUILD_TESTING"
include(CTest)
if(BUILD_TESTS)
	add_subdirectory(Tests/Editor)
	add_subdirectory(Tests/EditorTest)
	
	add_subdirectory(Tests/TestFont)
	add_subdirectory(Tests/TestGUI)
	add_subdirectory(Tests/TestMaths)
	add_subdirectory(Tests/TestNetwork)
	add_subdirectory(Tests/TestPacker)
	add_subdirectory(Tests/TestPBR)
	add_subdirectory(Tests/TestPhysics)
	add_subdirectory(Tests/TestSerial)
endif()
if(ACID_BUILD_UNIT_TESTS)
	add_subdirectory(Units)
endif()
