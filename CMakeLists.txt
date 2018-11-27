cmake_minimum_required(VERSION 3.5.2 FATAL_ERROR)
project(Acid VERSION 0.10.7 LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(BUILD_TESTS "Build test applications" ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Adds a postfix for debug-built libraries.")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DACID_VERBOSE)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
	add_definitions(-DACID_VERBOSE)
endif()

if(WIN32)
	add_definitions(-DACID_BUILD_WINDOWS)
	add_definitions(-DWIN32_LEAN_AND_MEAN)
	add_definitions(-DNOMINMAX)
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
elseif(UNIX AND NOT APPLE)
	add_definitions(-DACID_BUILD_LINUX)
elseif(UNIX AND APPLE)
	add_definitions(-DACID_BUILD_MACOS)
endif()

if(MSVC)
	add_definitions(-DACID_BUILD_MSVC)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_definitions(-DACID_BUILD_CLANG)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_definitions(-DACID_BUILD_GNU)
	add_definitions(-D__USE_MINGW_ANSI_STDIO=0)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/Lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/Bin)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/Lib32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/Bin32)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_INSTALL_LIBDIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_INSTALL_BINDIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_INSTALL_INCLUDEDIR ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/include)

find_program(CCACHE_FOUND "ccache")

if(CCACHE_FOUND)
	message(STATUS "Using ccache for building.")
	set_property(GLOBAL PROPERTY LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

find_package(Threads REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenAL REQUIRED)

if(OPENAL_FOUND AND NOT TARGET OpenAL::OpenAL)
	add_library(OpenAL::OpenAL UNKNOWN IMPORTED)
	set_target_properties(OpenAL::OpenAL PROPERTIES
			IMPORTED_LOCATION "${OPENAL_LIBRARY}"
			INTERFACE_INCLUDE_DIRECTORIES "${OPENAL_INCLUDE_DIR}")
endif()

find_package(physfs QUIET)

if(NOT TARGET physfs)
	foreach(_physfs_option "PHYSFS_BUILD_STATIC" "PHYSFS_BUILD_TEST" "PHYSFS_BUILD_DOCS")
		set(${_physfs_option} false)
	endforeach()

	add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/physfs)
	target_include_directories(physfs PUBLIC "${PROJECT_SOURCE_DIR}/Libraries/physfs/src")
endif()

find_package(glfw3 QUIET)

if(NOT TARGET glfw)
	foreach(_glfw3_option "GLFW_BUILD_TESTS" "GLFW_BUILD_EXAMPLES" "GLFW_BUILD_DOCS" "GLFW_INSTALL")
		set(${_glfw3_option} false)
	endforeach()

	add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/glfw)
endif()

find_package(glsllang QUIET)

if(NOT TARGET glsllang)
	foreach(_glslang_option "BUILD_TESTING" "ENABLE_GLSLANG_BINARIES" "ENABLE_SPVREMAPPER" "ENABLE_HLSL")
		set(${_glslang_option} false)
	endforeach()

	foreach(_glslang_option "SKIP_GLSLANG_INSTALL")
		set(${_glslang_option} true)
	endforeach()

	add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/glslang)
endif()

find_package(bullet QUIET)

if(NOT TARGET bullet)
	foreach(_bullet_option "BUILD_BULLET3" "BUILD_PYBULLET" "BUILD_BULLET2_DEMOS" "BUILD_OPENGL3_DEMOS" "BUILD_CPU_DEMOS" "BUILD_EXTRAS" "BUILD_UNIT_TESTS" "USE_GRAPHICAL_BENCHMARK" "USE_GLUT" "INSTALL_LIBS" "INSTALL_CMAKE_FILES")
		set(${_bullet_option} false)
	endforeach()

	# TODO: Compile bullet dlls in MSVC.
	set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")

	if(MSVC)
		set(BUILD_SHARED_LIBS OFF)
	endif()

	add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/bullet3)
	target_include_directories(BulletDynamics PUBLIC "${PROJECT_SOURCE_DIR}/Libraries/bullet3/src")

	set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")
endif()

add_subdirectory(Sources)

if(BUILD_TESTS)
	add_subdirectory(Tests/TestGUI)
	add_subdirectory(Tests/TestMaths)
	add_subdirectory(Tests/TestNetwork)
	add_subdirectory(Tests/TestPBR)
	add_subdirectory(Tests/TestPhysics)
	add_subdirectory(Tests/TestVoxel)
endif()
