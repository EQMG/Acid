add_library(Acid)
add_library(Acid::Acid ALIAS Acid)

# This file is quite big, so for readability we include instead...
# This uses target_sources(Acid) to define its headers & source files
include(CMakeSources.cmake)

# Used to track if we're using ONLY system libs
# Prevents errors with EXPORT
set(_ACID_ALL_SYSTEM_LIBS true)

# Allows usage of configure time content
include(FetchContent)

# Looks for a appropriate threads package for this platform
find_package(Threads REQUIRED)

# Finds Vulkan, env "VULKAN_SDK" must be set
find_package(volk 121 QUIET)
if(NOT volk_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(volk
			URL https://github.com/zeux/volk/tarball/ed4c168595bc7c5d36925f0036ea14d36b20c87c
			URL_MD5 1e5f7e3fedbaf5223751418ebbd43aa3
			)
	FetchContent_GetProperties(volk)
	if(NOT volk_POPULATED)
		FetchContent_Populate(volk)
		add_subdirectory(${volk_SOURCE_DIR} ${volk_BINARY_DIR})
	endif()
	set_target_properties(volk PROPERTIES FOLDER External)
	add_library(volk::volk ALIAS volk)
endif()

# OpenAL must be installed on the system, env "OPENALDIR" must be set
find_package(OpenALSoft)
find_package(OpenAL)
if(OPENALSOFT_FOUND)
	if(OPENALSOFT_FOUND AND NOT TARGET OpenAL::OpenAL)
		add_library(OpenAL::OpenAL UNKNOWN IMPORTED)
		set_target_properties(OpenAL::OpenAL PROPERTIES
				IMPORTED_LOCATION "${OPENALSOFT_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${OPENALSOFT_INCLUDE_DIR}"
				)
	endif()
elseif(OPENAL_FOUND)
	if(OPENAL_FOUND AND NOT TARGET OpenAL::OpenAL)
		add_library(OpenAL::OpenAL UNKNOWN IMPORTED)
		set_target_properties(OpenAL::OpenAL PROPERTIES
				IMPORTED_LOCATION "${OPENAL_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${OPENAL_INCLUDE_DIR}"
				)
	endif()
else()
	message(FATAL_ERROR "Could not find OpenAL or OpenAL-Soft")
endif()

find_package(glfw3 3.3 QUIET)
if(NOT TARGET glfw)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(glfw3
			URL https://github.com/glfw/glfw/archive/3.3.2.tar.gz
			URL_MD5 865e54ff0a100e9041a40429db98be0b
			)
	FetchContent_GetProperties(glfw3)
	if(NOT glfw3_POPULATED)
		foreach(_glfw3_option "GLFW_BUILD_TESTS" "GLFW_BUILD_EXAMPLES" "GLFW_BUILD_DOCS" "GLFW_INSTALL")
			set(${_glfw3_option} OFF CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(glfw3)
		add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
	endif()
	set_target_properties(glfw PROPERTIES FOLDER External)
endif()

find_package(glslang QUIET)
if(NOT glslang_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(glslang
			URL https://github.com/KhronosGroup/glslang/archive/SDK-candidate-2.tar.gz
			URL_MD5 f238d01ae6e1a59ce67bc79aec2f6b33
			)
	FetchContent_GetProperties(glslang)
	if(NOT glslang_POPULATED)
		foreach(_glslang_option "BUILD_TESTING" "ENABLE_GLSLANG_BINARIES" "ENABLE_SPVREMAPPER" "ENABLE_HLSL" 
				"ENABLE_AMD_EXTENSIONS" "ENABLE_NV_EXTENSIONS"
				)
			set(${_glslang_option} OFF CACHE INTERNAL "")
		endforeach()
		foreach(_glslang_option "SKIP_GLSLANG_INSTALL" "ENABLE_OPT")
			set(${_glslang_option} ON CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(glslang)
		add_subdirectory(${glslang_SOURCE_DIR} ${glslang_BINARY_DIR})
	endif()
	set_target_properties(glslang PROPERTIES FOLDER External/glslang)
	set_target_properties(OGLCompiler PROPERTIES FOLDER External/glslang)
	set_target_properties(OSDependent PROPERTIES FOLDER External/glslang)
	set_target_properties(SPIRV PROPERTIES FOLDER External/glslang)

	# Used in target_link_libraries()
	set(GLSLANG_LIBRARIES SPIRV)
else()
	set(GLSLANG_INCLUDE_DIRS "${GLSLANG_INCLUDE_DIR}" "${SPIRV_INCLUDE_DIR}")
	set(GLSLANG_LIBRARIES glslang::glslang glslang::SPIRV)
endif()

if(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
	set(CMAKE_DISABLE_FIND_PACKAGE_Bullet TRUE CACHE INTERNAL "")
endif()
find_package(Bullet 2.89 QUIET)
if(NOT BULLET_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(bullet3
			URL https://github.com/bulletphysics/bullet3/archive/2.89.tar.gz
			URL_MD5 d239b4800ec30513879834be6fcdc376
			)
	FetchContent_GetProperties(bullet3)
	if(NOT bullet3_POPULATED)
		# On MSVC shared mode must be disabled with bullet currently
		set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
		foreach(_bullet3_option "BUILD_BULLET3" "BUILD_PYBULLET" "BUILD_BULLET2_DEMOS" "BUILD_OPENGL3_DEMOS" 
				"BUILD_CPU_DEMOS" "BUILD_EXTRAS" "BUILD_UNIT_TESTS" "USE_GRAPHICAL_BENCHMARK" "USE_GLUT"
				)
			set(${_bullet3_option} OFF CACHE INTERNAL "")
		endforeach()
		if(MSVC)
			set(USE_MSVC_INCREMENTAL_LINKING ON CACHE INTERNAL "")
			set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE INTERNAL "")
			set(BUILD_SHARED_LIBS OFF)
		endif()
		FetchContent_Populate(bullet3)
		add_subdirectory(${bullet3_SOURCE_DIR} ${bullet3_BINARY_DIR})
		# Reset back to value before MSVC fix
		set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")
	endif()
	set_target_properties(Bullet3Common PROPERTIES FOLDER External/bullet)
	set_target_properties(BulletCollision PROPERTIES FOLDER External/bullet)
	set_target_properties(BulletDynamics PROPERTIES FOLDER External/bullet)
	set_target_properties(BulletInverseDynamics PROPERTIES FOLDER External/bullet)
	set_target_properties(BulletSoftBody PROPERTIES FOLDER External/bullet)
	set_target_properties(LinearMath PROPERTIES FOLDER External/bullet)

	set(BULLET_INCLUDE_DIRS "${bullet3_SOURCE_DIR}/src")
	set(BULLET_LIBRARIES BulletSoftBody BulletDynamics BulletCollision LinearMath)
endif()

find_package(PhysFS 3.0.1 QUIET)
if(NOT PhysFS_FOUND)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(physfs
			URL https://icculus.org/physfs/downloads/physfs-3.0.2.tar.bz2
			URL_MD5 dc751294aaf59d1359bbe34e693d1d87
			)
	FetchContent_GetProperties(physfs)
	if(NOT physfs_POPULATED)
		set(PHYSFS_BUILD_STATIC NOT${BUILD_SHARED_LIBS} CACHE INTERNAL "")
		set(PHYSFS_BUILD_SHARED ${BUILD_SHARED_LIBS} CACHE INTERNAL "")
		set(PHYSFS_BUILD_TEST OFF CACHE INTERNAL "")
		set(PHYSFS_TARGETNAME_DOCS "physfs-docs" CACHE INTERNAL STRING)
		set(PHYSFS_TARGETNAME_DIST "physfs-dist" CACHE INTERNAL STRING)
		set(PHYSFS_TARGETNAME_UNINSTALL "physfs-uninstall" CACHE INTERNAL STRING)
		set(CMAKE_DISABLE_FIND_PACKAGE_Doxygen TRUE)
		FetchContent_Populate(physfs)
		add_subdirectory(${physfs_SOURCE_DIR} ${physfs_BINARY_DIR})
	endif()
	set_target_properties(physfs-static PROPERTIES FOLDER External/physfs)
	if(TARGET physfs)
		set_target_properties(physfs PROPERTIES FOLDER External/physfs)
	endif()
	if(TARGET physfs-docs)
		set_target_properties(physfs-docs PROPERTIES FOLDER External/physfs)
	endif()
	if(TARGET physfs-uninstall)
		set_target_properties(physfs-uninstall PROPERTIES FOLDER External/physfs)
	endif()

	# PhysFS disables rpath on GNU, when disabled Acid will not run on Unix.
	if(CMAKE_COMPILER_IS_GNUCC)
		set(CMAKE_SKIP_RPATH OFF CACHE BOOL "Skip RPATH" FORCE)
	endif()

	set(PHYSFS_INCLUDE_DIR "${physfs_SOURCE_DIR}/src")
	# Set here so we can use it in Sources/CMakeLists.txt
	if(PHYSFS_BUILD_STATIC)
		set(PHYSFS_LIBRARY physfs-static)
	else()
		set(PHYSFS_LIBRARY physfs)
	endif()
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
		$<$<CXX_COMPILER_ID:MSVC>:/wd4251 /wd4592 /wd4018 /wd4101 /wd4244 /wd4267 /wd4756 /wd4309 /wd26812>
		PRIVATE
		# Enables SSE4.1, it is also possible to use SSE2 with -msse2
		$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-msse4.1>
		# Enabled SSE2 for MSVC for 32-bit.
		$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>>:/arch:SSE2>
		)
target_include_directories(Acid
		PUBLIC
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party>
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		PRIVATE
		# Since building locally from the submodules won't always create these vars.
		# We have to do a simple check if they exist or they will cause errors
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
		${CMAKE_DL_LIBS} # dlopen and dlclose
		Threads::Threads # pthread
		volk::volk
		PRIVATE
		# More IMPORTED
		OpenAL::OpenAL
		glfw
		${GLSLANG_LIBRARIES}
		${BULLET_LIBRARIES}
		${PHYSFS_LIBRARY}
		)

set_target_properties(Acid PROPERTIES
		#INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "Acid"
		)

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Config.hpp" 
		"${CMAKE_CURRENT_BINARY_DIR}/Export.hpp"
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
