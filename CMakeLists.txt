cmake_minimum_required(VERSION 3.1)
project(Acid)

# Options
option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(ACID_INSTALL "Generate installation target" OFF)
option(ACID_BUILD_TESTING "Build the Acid test programs" ON)
option(ACID_SETUP_COMPILER "If Acid will set it's own compiler settings" ON)
option(ACID_SETUP_OUTPUT "If Acid will set it's own outputs" ON)

set(LIB_TYPE STATIC)

if(BUILD_SHARED_LIBS)
	set(LIB_TYPE SHARED)
endif()

# Preprocessor
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DACID_VERBOSE)
	add_definitions(-DACID_BUILD_DEBUG)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	add_definitions(-DACID_BUILD_RELEASE)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
	add_definitions(-DACID_VERBOSE)
	add_definitions(-DACID_BUILD_RELEASE)
endif()

if(WIN32)
	add_definitions(-DACID_BUILD_WINDOWS)
elseif(UNIX AND NOT APPLE)
	add_definitions(-DACID_BUILD_LINUX)
elseif(UNIX AND APPLE)
	add_definitions(-DACID_BUILD_MACOS)
endif()

# Compiler Options
if(ACID_SETUP_COMPILER)
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_EXTENSIONS ON)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_C_STANDARD 11)
	set(CMAKE_C_STANDARD_REQUIRED ON)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Adds a postfix for debug-built libraries.")

	if(WIN32)
		set(CMAKE_SHARED_LIBRARY_PREFIX "")
	endif()

	if(MSVC)
		add_definitions(-DACID_BUILD_MSVC)
		add_definitions(-D_SCL_SECURE_NO_WARNINGS)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /std:c++17 /INCREMENTAL:NO /Oi /Ot /GT /GF /GS- /fp:fast /fp:except- /MP /W3 /wd4251")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Ob0 /RTC1 /MDd /DEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ob2 /MD")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Od /Ob2 /MDd /DEBUG")

		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
			set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /OPT:ICF")
		else()
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
			set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
		endif()
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		add_definitions(-DACID_BUILD_CLANG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -msse3 -mfpmath=sse") # -Wextra -Werror -pedantic
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG -Wno-unused-variable")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_definitions(-DACID_BUILD_GNU)
		add_definitions(-D__USE_MINGW_ANSI_STDIO=0)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -mavx -msse3 -mfpmath=sse") # -Wextra -Werror -pedantic
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s -DNDEBUG -Wno-unused-variable")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
	endif()
endif()

# Output Directories
if(ACID_SETUP_OUTPUT)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/bin)

	set(CMAKE_INSTALL_LIBDIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	set(CMAKE_INSTALL_BINDIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	set(CMAKE_INSTALL_INCLUDEDIR ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/include)
endif()

# Dependencies
find_package(Threads REQUIRED)
add_subdirectory(Libraries)

# Engine Sources
add_subdirectory(Sources)

# Testing Sources
if(ACID_BUILD_TESTING)
	add_subdirectory(Tests/TestPhysics)
	add_subdirectory(Tests/TestGuis)
	add_subdirectory(Tests/TestMaths)
endif()
