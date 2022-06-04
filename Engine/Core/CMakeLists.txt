set(_acid_core_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Engine/App.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Engine/Engine.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Engine/Log.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Engine/Module.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/File.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/FileObserver.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/Files.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/Json/Json.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/Node.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/Node.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Files/NodeConstView.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/NodeConstView.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Files/NodeFormat.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/NodeView.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/NodeView.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Files/Xml/Xml.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/Xml/XmlContainer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Colour.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Colour.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/ElapsedTime.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Maths.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Matrix2.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Matrix3.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Matrix4.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Quaternion.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Time.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Time.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Vector2.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Vector2.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Vector3.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Vector3.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Vector4.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Vector4.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Resources/Resource.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Resources/Resources.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Timers/Timers.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/ConstExpr.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/Enumerate.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/EnumValue.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/Factory.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/Future.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/NonCopyable.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/RingBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/StreamFactory.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/String.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/ThreadPool.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/TypeInfo.hpp
		)
set(_acid_core_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Engine/Engine.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Engine/Log.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/File.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/FileObserver.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/Files.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/Json/Json.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/Node.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/NodeConstView.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/NodeView.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Files/Xml/Xml.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Colour.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/ElapsedTime.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Maths.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Matrix2.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Matrix3.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Matrix4.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Quaternion.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Vector2.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Vector3.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Maths/Vector4.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Resources/Resources.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Timers/Timers.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/String.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils/ThreadPool.cpp
		)
set(_acid_core_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/CoreExport.hpp
		${CMAKE_CURRENT_BINARY_DIR}/include/CoreConfig.hpp
		)
set(_acid_core_third_party_headers
		${ACID_THIRD_PARTY_DIR}/bitmask/bitmask.hpp
		${ACID_THIRD_PARTY_DIR}/magic_enum/magic_enum.hpp
		${ACID_THIRD_PARTY_DIR}/rocket/rocket.hpp
		)
set(_acid_core_third_party_sources
		)

add_library(Acid.Core
        ${_acid_core_headers}
        ${_acid_core_sources}
		${_acid_core_generated_headers}
        ${_acid_core_third_party_headers}
        ${_acid_core_third_party_sources}
        )

include(GenerateExportHeader)
generate_export_header(Acid.Core EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/CoreExport.hpp")

configure_file(Config.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/include/CoreConfig.hpp" @ONLY)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_core_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_core_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_acid_core_generated_headers})
source_group(TREE "${ACID_THIRD_PARTY_DIR}" PREFIX "ThirdParty" FILES ${_acid_core_third_party_headers})
source_group(TREE "${ACID_THIRD_PARTY_DIR}" PREFIX "ThirdParty" FILES ${_acid_core_third_party_sources})

if(APPLE)
    set(OTHER_LDFLAGS ${OTHER_LDFLAGS} "-framework IOKit -framework Foundation -framework CoreFoundation")
endif()

#find_package(PhysFS 3.0.2 QUIET)
if(NOT PhysFS_FOUND)
	set(PHYSFS_BUILD_SHARED OFF CACHE INTERNAL "Build shared library.")
	set(PHYSFS_BUILD_TEST OFF CACHE INTERNAL "Build stdio test program.")
	set(PHYSFS_DISABLE_INSTALL ON CACHE INTERNAL "Disable installing PhysFS.")
	set(PHYSFS_BUILD_DOCS OFF CACHE INTERNAL "Build doxygen based documentation.")
	set(PHYSFS_TARGETNAME_DIST "physfs-dist" CACHE INTERNAL STRING)
	set(PHYSFS_TARGETNAME_UNINSTALL "physfs-uninstall" CACHE INTERNAL STRING)
	add_subdirectory(${ACID_THIRD_PARTY_DIR}/physfs physfs)

	set_target_properties(physfs-static PROPERTIES FOLDER "External/physfs")
	if(UNIX)
		set_target_properties(physfs-dist PROPERTIES FOLDER "External/physfs")
		set_target_properties(physfs-uninstall PROPERTIES FOLDER "External/physfs")
	endif()
	if(CMAKE_COMPILER_IS_GNUCC)
		set(CMAKE_SKIP_RPATH OFF CACHE BOOL "Skip RPATH" FORCE)
	endif()

	set(PHYSFS_INCLUDE_DIR "${ACID_THIRD_PARTY_DIR}/physfs/src")
	set(PHYSFS_LIBRARY physfs-static)
endif()

# Looks for a appropriate threads package for this platform
find_package(Threads REQUIRED)
# Check whether a SSE instructions can be used on the machine
include(FindSSE)

set_target_properties(Acid.Core PROPERTIES FOLDER "Acid")
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
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/bitmask>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/magic_enum>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/rocket>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Core>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party>
		PRIVATE
		$<$<BOOL:${PHYSFS_INCLUDE_DIR}>:${PHYSFS_INCLUDE_DIR}>
		)
target_link_libraries(Acid.Core
		PUBLIC
		$<$<CXX_COMPILER_ID:GNU>:stdc++fs> # std::filesystem
		$<$<PLATFORM_ID:Windows>:ws2_32> # Winsock 2.0
		$<$<PLATFORM_ID:Android>:log> # log support
		${OTHER_LDFLAGS}
		${CMAKE_DL_LIBS} # dlopen and dlclose
		Threads::Threads # pthread
        PRIVATE
		${PHYSFS_LIBRARY}
		)

install(TARGETS Acid.Core
		EXPORT AcidCoreExport
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
install(EXPORT AcidCoreExport
		FILE AcidCoreConfig.cmake
		#NAMESPACE acid::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
install_headers_with_dir("Core" "${CMAKE_CURRENT_SOURCE_DIR}" "${_acid_core_headers}")
install_headers_with_dir("Core" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_acid_core_generated_headers}")
install_headers_with_dir("third_party" "${ACID_THIRD_PARTY_DIR}" "${_acid_core_third_party_headers}")
