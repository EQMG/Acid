set(_acid_core_headers
		Engine/App.hpp
		Engine/Engine.hpp
		Engine/Log.hpp
		Engine/Module.hpp
		Files/File.hpp
		Files/FileObserver.hpp
		Files/Files.hpp
		Files/Json/Json.hpp
		Files/Node.hpp
		Files/Node.inl
		Files/NodeConstView.hpp
		Files/NodeConstView.inl
		Files/NodeFormat.hpp
		Files/NodeView.hpp
		Files/NodeView.inl
		Files/Xml/Xml.hpp
		Files/Xml/XmlContainer.hpp
		Maths/Colour.hpp
		Maths/Colour.inl
		Maths/ElapsedTime.hpp
		Maths/Maths.hpp
		Maths/Matrix2.hpp
		Maths/Matrix3.hpp
		Maths/Matrix4.hpp
		Maths/Quaternion.hpp
		Maths/Time.hpp
		Maths/Time.inl
		Maths/Vector2.hpp
		Maths/Vector2.inl
		Maths/Vector3.hpp
		Maths/Vector3.inl
		Maths/Vector4.hpp
		Maths/Vector4.inl
		Resources/Resource.hpp
		Resources/Resources.hpp
		Timers/Timers.hpp
		Utils/ConstExpr.hpp
		Utils/Enumerate.hpp
		Utils/EnumValue.hpp
		Utils/Factory.hpp
		Utils/Future.hpp
		Utils/NonCopyable.hpp
		Utils/RingBuffer.hpp
		Utils/StreamFactory.hpp
		Utils/String.hpp
		Utils/ThreadPool.hpp
		Utils/TypeInfo.hpp
		)
set(_acid_core_sources
		Engine/Engine.cpp
		Engine/Log.cpp
		Files/File.cpp
		Files/FileObserver.cpp
		Files/Files.cpp
		Files/Json/Json.cpp
		Files/Node.cpp
		Files/NodeConstView.cpp
		Files/NodeView.cpp
		Files/Xml/Xml.cpp
		Maths/Colour.cpp
		Maths/ElapsedTime.cpp
		Maths/Maths.cpp
		Maths/Matrix2.cpp
		Maths/Matrix3.cpp
		Maths/Matrix4.cpp
		Maths/Quaternion.cpp
		Maths/Vector2.cpp
		Maths/Vector3.cpp
		Maths/Vector4.cpp
		Resources/Resources.cpp
		Timers/Timers.cpp
		Utils/String.cpp
		Utils/ThreadPool.cpp
		)

set(_acid_core_third_party_headers
		../../External/bitmask/bitmask.hpp
		../../External/magic_enum/magic_enum.hpp
		../../External/rocket/rocket.hpp
		)
set(_acid_core_third_party_sources
		)
set(_acid_core_third_party_includes
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/bitmask
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/magic_enum
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/rocket
		)

add_library(Acid.Core
        ${_acid_core_headers}
        ${_acid_core_sources}
        ${_acid_core_third_party_headers}
        ${_acid_core_third_party_sources}
        )

get_filename_component(CURRENT_PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. ABSOLUTE)
if(ACID_LINK_RESOURCES)
	# Directory that Acid resources can be found.
	set(ACID_RESOURCES_DIR "${CURRENT_PARENT_DIR}/Resources")
endif()
if(ACID_INSTALL_RESOURCES)
	# Install resources for end-user usage because many source files use these
	install(DIRECTORY "${CURRENT_PARENT_DIR}/Resources"
			# Example: this will install the Resources dir to /usr/share/Acid/Resources on Linux
			DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}"
			)
endif()

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Core EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Core/Export.hpp")

# Adds a CMake generated config file
configure_file(Config.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/include/Core/Config.hpp" @ONLY)

if(APPLE)
    set(OTHER_LDFLAGS ${OTHER_LDFLAGS} "-framework IOKit -framework Foundation -framework CoreFoundation")
endif()

find_package(PhysFS 3.0.2 QUIET)
if(NOT PhysFS_FOUND)
	if(BUILD_SHARED_LIBS)
		set(PHYSFS_BUILD_STATIC OFF CACHE INTERNAL "Build static library.")
	else()
		set(PHYSFS_BUILD_SHARED OFF CACHE INTERNAL "Build shared library.")
	endif()
	set(PHYSFS_BUILD_TEST OFF CACHE INTERNAL "Build stdio test program.")
	set(PHYSFS_DISABLE_INSTALL ON CACHE INTERNAL "Disable installing PhysFS.")
	set(PHYSFS_BUILD_DOCS OFF CACHE INTERNAL "Build doxygen based documentation.")
	set(PHYSFS_TARGETNAME_DIST "physfs-dist" CACHE INTERNAL STRING)
	set(PHYSFS_TARGETNAME_UNINSTALL "physfs-uninstall" CACHE INTERNAL STRING)
	add_subdirectory(../../External/physfs physfs)
	
	if(TARGET physfs)
		set_target_properties(physfs PROPERTIES FOLDER "External/physfs")
		set(PHYSFS_LIBRARY physfs)
	endif()
	if(TARGET physfs-static)
		set_target_properties(physfs-static PROPERTIES FOLDER "External/physfs")
		set(PHYSFS_LIBRARY physfs-static)
	endif()
	if(UNIX)
		set_target_properties(physfs-dist PROPERTIES FOLDER "External/physfs")
		set_target_properties(physfs-uninstall PROPERTIES FOLDER "External/physfs")
	endif()
	if(CMAKE_COMPILER_IS_GNUCC)
		set(CMAKE_SKIP_RPATH OFF CACHE BOOL "Skip RPATH" FORCE)
	endif()

	set(PHYSFS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../External/physfs/src")
endif()

# Looks for a appropriate threads package for this platform
find_package(Threads REQUIRED)
# Check whether a SSE instructions can be used on the machine
include(FindSSE)

target_compile_features(Acid.Core PUBLIC c_std_11 cxx_std_17)
target_compile_definitions(Acid.Core
		PUBLIC
		# If the CONFIG is Debug or RelWithDebInfo, define ACID_DEBUG
		# Works on both single and mutli configuration
		$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:DEBUG ACID_DEBUG>
		# 32-bit
		$<$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>:ACID_BUILD_32BIT>
		# 64-bit
		$<$<EQUAL:8,${CMAKE_SIZEOF_VOID_P}>:ACID_BUILD_64BIT>
		# Windows
		$<$<PLATFORM_ID:Windows>:ACID_BUILD_WINDOWS WIN32_LEAN_AND_MEAN NOMINMAX>
		# Linux
		$<$<PLATFORM_ID:Linux>:ACID_BUILD_LINUX>
		# macOS
		$<$<PLATFORM_ID:Darwin>:ACID_BUILD_MACOS>
		# MSVC
		$<$<CXX_COMPILER_ID:MSVC>:ACID_BUILD_MSVC _SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS>
		# Clang/AppleClang
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:ACID_BUILD_CLANG>
		# GNU/GCC
		$<$<CXX_COMPILER_ID:GNU>:ACID_BUILD_GNU __USE_MINGW_ANSI_STDIO=0>
		)
target_compile_options(Acid.Core
		PUBLIC
		# Disables symbol warnings.
		$<$<CXX_COMPILER_ID:MSVC>:/wd4018 /wd4067 /wd4244 /wd4251 /wd4267 /wd4275 /wd4996>
		# Enabled SSE2 for MSVC for 32-bit.
		$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>>:/arch:SSE2>
		# Enables SSE4.1, it is also possible to use SSE2 with -msse2
		$<$<BOOL:${SSE4_1_FOUND}>:-msse4.1>
		)
target_include_directories(Acid.Core
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_core_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		PRIVATE
		# Since building locally from the submodules won't always create these vars.
		# We have to do a simple check if they exist or they will cause errors
		$<$<BOOL:${PHYSFS_INCLUDE_DIR}>:${PHYSFS_INCLUDE_DIR}>
		)
target_link_libraries(Acid.Core
		PUBLIC
		# All IMPORTED targets, which automatically handles includes
		$<$<CXX_COMPILER_ID:GNU>:stdc++fs> # std::filesystem
		$<$<PLATFORM_ID:Windows>:ws2_32> # Winsock 2.0
		$<$<PLATFORM_ID:Android>:log> # log support
		${OTHER_LDFLAGS}
		${CMAKE_DL_LIBS} # dlopen and dlclose
		Threads::Threads # pthread
        PRIVATE
		${PHYSFS_LIBRARY}
		)

set_target_properties(Acid.Core PROPERTIES
		#INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "Acid"
		)

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Core/Config.hpp" 
		"${CMAKE_CURRENT_BINARY_DIR}/include/Core/Export.hpp"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Core"
		)
foreach(_header IN LISTS _acid_core_headers _acid_core_third_party_headers)
    get_filename_component(_header_dir ${_header} DIRECTORY)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_header}
			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Core/${_header_dir}"
			)
endforeach()
# Install without an export since we're using 1 (or more) non-system libs
install(TARGETS Acid.Core
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
