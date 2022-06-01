# Used to track if we're using ONLY system libs
# Prevents errors with EXPORT
set(_ACID_ALL_SYSTEM_LIBS true)

# Looks for a appropriate threads package for this platform
find_package(Threads REQUIRED)
# Check whether a SSE instructions can be used on the machine
include(FindSSE)

# Finds Vulkan, env "VULKAN_SDK" must be set
#find_package(volk 121 QUIET)
#if(NOT volk_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	add_subdirectory(../External/volk volk)
	set_target_properties(volk PROPERTIES FOLDER "External")
#endif()

# OpenAL must be installed on the system, env "OPENALDIR" must be set
find_package(OpenALSoft)
find_package(OpenAL)
if(OPENALSOFT_FOUND)
	#if(OPENALSOFT_FOUND AND NOT TARGET OpenAL::OpenAL)
		add_library(OpenAL::OpenAL UNKNOWN IMPORTED)
		set_target_properties(OpenAL::OpenAL PROPERTIES
				IMPORTED_LOCATION "${OPENALSOFT_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${OPENALSOFT_INCLUDE_DIR}"
				)
	#endif()
elseif(OPENAL_FOUND)
	#if(OPENAL_FOUND AND NOT TARGET OpenAL::OpenAL)
		add_library(OpenAL::OpenAL UNKNOWN IMPORTED)
		set_target_properties(OpenAL::OpenAL PROPERTIES
				IMPORTED_LOCATION "${OPENAL_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${OPENAL_INCLUDE_DIR}"
				)
	#endif()
else()
	message(FATAL_ERROR "Could not find OpenAL or OpenAL-Soft")
endif()

find_package(glfw3 3.3.6 QUIET)
if(NOT TARGET glfw)
	set(_ACID_ALL_SYSTEM_LIBS false)
	foreach(_glfw3_option "GLFW_BUILD_TESTS" "GLFW_BUILD_EXAMPLES" "GLFW_BUILD_DOCS" "GLFW_INSTALL")
		set(${_glfw3_option} OFF CACHE INTERNAL "")
	endforeach()
	add_subdirectory(../External/glfw glfw)
	set_target_properties(glfw PROPERTIES FOLDER "External")
endif()

find_package(Freetype 2.11.1 QUIET)
if(NOT Freetype_FOUND)
	set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_BZip2 TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_PNG TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE)
	add_subdirectory(../External/freetype freetype)
	
	set_target_properties(freetype PROPERTIES FOLDER "External")
	
	set(FREETYPE_LIBRARIES "freetype")
endif()

# TODO: Prebuilt glslang binaries are unreliable between systems
#find_package(glslang 11.6.0 QUIET)
if(NOT glslang_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)

	# On MSVC shared mode must be disabled with glslang currently
	set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
	if(MSVC)
		set(BUILD_SHARED_LIBS OFF)
	endif()
	set(ENABLE_SPVREMAPPER OFF CACHE INTERNAL "Enables building of SPVRemapper")
	set(ENABLE_GLSLANG_BINARIES OFF CACHE INTERNAL "Builds glslangValidator and spirv-remap")
	set(ENABLE_HLSL OFF CACHE INTERNAL "Enables HLSL input support")
	set(ENABLE_RTTI ON CACHE INTERNAL "Enables RTTI")
	set(ENABLE_EXCEPTIONS ON CACHE INTERNAL "Enables Exceptions")
	set(ENABLE_CTEST OFF CACHE INTERNAL "Enables testing")
	set(SKIP_GLSLANG_INSTALL ON CACHE INTERNAL "Skip installation")
	add_subdirectory(../External/glslang glslang)
	# Reset back to value before MSVC fix
	set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")

	set_target_properties(glslang PROPERTIES FOLDER "External/glslang")
	#set_target_properties(glslang-build-info PROPERTIES FOLDER "External/glslang")
	set_target_properties(GenericCodeGen PROPERTIES FOLDER "External/glslang")
	set_target_properties(MachineIndependent PROPERTIES FOLDER "External/glslang")
	set_target_properties(OGLCompiler PROPERTIES FOLDER "External/glslang")
	set_target_properties(OSDependent PROPERTIES FOLDER "External/glslang")
	set_target_properties(SPIRV PROPERTIES FOLDER "External/glslang")

	# Used in target_link_libraries()
	set(GLSLANG_LIBRARIES glslang SPIRV)
else()
	set(GLSLANG_INCLUDE_DIRS "${GLSLANG_INCLUDE_DIR}" "${SPIRV_INCLUDE_DIR}")
	set(GLSLANG_LIBRARIES glslang::glslang glslang::SPIRV)
endif()

if(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
	set(CMAKE_DISABLE_FIND_PACKAGE_Bullet TRUE CACHE INTERNAL "")
endif()
find_package(Bullet 3.17 QUIET)
if(NOT BULLET_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)

	# On MSVC shared mode must be disabled with bullet currently
	set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
	if(MSVC)
		set(USE_MSVC_INCREMENTAL_LINKING ON CACHE INTERNAL "")
		set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE INTERNAL "")
		set(BUILD_SHARED_LIBS OFF)
	endif()
	set(USE_GRAPHICAL_BENCHMARK OFF CACHE INTERNAL "Use Graphical Benchmark")
	set(BUILD_CPU_DEMOS OFF CACHE INTERNAL "Build original Bullet CPU examples")
	set(USE_GLUT OFF CACHE INTERNAL "Use Glut")
	set(BUILD_BULLET3 OFF CACHE INTERNAL "Set when you want to build Bullet 3")
	set(BUILD_PYBULLET OFF CACHE INTERNAL "Set when you want to build pybullet (Python bindings for Bullet)")
	set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "Set when you want to build the Bullet 2 demos")
	set(BUILD_EXTRAS OFF CACHE INTERNAL "Set when you want to build the extras")
	set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "Build Unit Tests")
	add_subdirectory(../External/bullet3 bullet3)
	# Reset back to value before MSVC fix
	set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")

	set_target_properties(Bullet3Common PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletCollision PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletDynamics PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletInverseDynamics PROPERTIES FOLDER "External/bullet")
	set_target_properties(BulletSoftBody PROPERTIES FOLDER "External/bullet")
	set_target_properties(LinearMath PROPERTIES FOLDER "External/bullet")

	set(BULLET_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../External/bullet3/src")
	set(BULLET_LIBRARIES BulletSoftBody BulletDynamics BulletCollision LinearMath)
endif()

find_package(PhysFS 3.0.2 QUIET)
if(NOT PhysFS_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	
	if(BUILD_SHARED_LIBS)
		set(PHYSFS_BUILD_STATIC OFF CACHE INTERNAL "Build static library.")
	else()
		set(PHYSFS_BUILD_SHARED OFF CACHE INTERNAL "Build shared library.")
	endif()
	set(PHYSFS_BUILD_TEST OFF CACHE INTERNAL "Build stdio test program.")
	set(PHYSFS_BUILD_DOCS OFF CACHE INTERNAL "Build doxygen based documentation.")
	set(PHYSFS_TARGETNAME_DIST "physfs-dist" CACHE INTERNAL STRING)
	set(PHYSFS_TARGETNAME_UNINSTALL "physfs-uninstall" CACHE INTERNAL STRING)
	add_subdirectory(../External/physfs physfs)
	
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

	set(PHYSFS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../External/physfs/src")
endif()

add_library(Acid)
add_library(Acid::Acid ALIAS Acid)
# This file is quite big, so for readability we include instead...
# This uses target_sources(Acid) to define its headers & source files
include(CMakeSources.cmake)

if(APPLE)
    set(OTHER_LDFLAGS ${OTHER_LDFLAGS} "-framework IOKit -framework Foundation -framework CoreFoundation")
endif()

target_compile_features(Acid PUBLIC c_std_11 cxx_std_17)
target_compile_definitions(Acid
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
target_compile_options(Acid
		PUBLIC
		# Disables symbol warnings.
		$<$<CXX_COMPILER_ID:MSVC>:/wd4018 /wd4067 /wd4244 /wd4251 /wd4267 /wd4275 /wd4996>
		# Enabled SSE2 for MSVC for 32-bit.
		$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>>:/arch:SSE2>
		# Enables SSE4.1, it is also possible to use SSE2 with -msse2
		$<$<BOOL:${SSE4_1_FOUND}>:-msse4.1>
		)
target_include_directories(Acid
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_temp_acid_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		PRIVATE
		# Since building locally from the submodules won't always create these vars.
		# We have to do a simple check if they exist or they will cause errors
		$<$<BOOL:${FREETYPE_INCLUDE_DIRS}>:${FREETYPE_INCLUDE_DIRS}>
		$<$<BOOL:${GLSLANG_INCLUDE_DIRS}>:${GLSLANG_INCLUDE_DIRS}>
		$<$<BOOL:${BULLET_INCLUDE_DIRS}>:${BULLET_INCLUDE_DIRS}>
		$<$<BOOL:${PHYSFS_INCLUDE_DIR}>:${PHYSFS_INCLUDE_DIR}>
		)
target_link_libraries(Acid
		PUBLIC
		# All IMPORTED targets, which automatically handles includes
		$<$<CXX_COMPILER_ID:GNU>:stdc++fs> # std::filesystem
		$<$<PLATFORM_ID:Windows>:ws2_32> # Winsock 2.0
		$<$<PLATFORM_ID:Android>:log> # log support
		${OTHER_LDFLAGS}
		${CMAKE_DL_LIBS} # dlopen and dlclose
		Threads::Threads # pthread
		volk::volk
		PRIVATE
		# More IMPORTED
		OpenAL::OpenAL
		glfw
		${FREETYPE_LIBRARIES}
		${GLSLANG_LIBRARIES}
		${BULLET_LIBRARIES}
		${PHYSFS_LIBRARY}
		)

set_target_properties(Acid PROPERTIES
		#INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "Acid"
		)

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Config.hpp" 
		"${CMAKE_CURRENT_BINARY_DIR}/include/Export.hpp"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
		)
foreach(_acid_header IN LISTS _temp_acid_headers _temp_acid_third_party_headers)
    get_filename_component(_acid_header_dir ${_acid_header} DIRECTORY)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_acid_header}
			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_acid_header_dir}"
			)
endforeach()
# If strictly using system libs, we can generate an export & install it
if(_ACID_ALL_SYSTEM_LIBS)
	install(TARGETS Acid
			# Auto-generates an export to install
			EXPORT
			AcidTargets
			LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
	# Install the auto-generated export
	install(EXPORT AcidTargets
			# Makes the end-user imported library Acid::Acid instead of just Acid
			NAMESPACE Acid::
			DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
			)
	# Defines write_basic_package_version_file
	include(CMakePackageConfigHelpers)
	# Dynamically writes the path to the resources dir (if installed) into the AcidConfig.cmake file
	configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/AcidConfig.cmake.in"
			"${PROJECT_BINARY_DIR}/AcidConfig.cmake"
			INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
			PATH_VARS CMAKE_INSTALL_DATADIR
			NO_CHECK_REQUIRED_COMPONENTS_MACRO
			)
	# Auto-generates a version file based on the current version specified in project()
	write_basic_package_version_file("${PROJECT_BINARY_DIR}/AcidConfigVersion.cmake"
			COMPATIBILITY SameMajorVersion
			)
	# Install the auto-generated version file and AcidConfig
	install(FILES
			"${PROJECT_BINARY_DIR}/AcidConfig.cmake"
			"${PROJECT_BINARY_DIR}/AcidConfigVersion.cmake"
			DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
			)
else()
	# Install without an export since we're using 1 (or more) non-system libs
	install(TARGETS Acid
			LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			)
endif()
