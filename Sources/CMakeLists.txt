add_library(Acid)
# This file is quite big, so for readability we include instead...
# This uses target_sources(Acid) to define its headers & source files
include(acid_sources.cmake)

# Manually defined by us
if(NOT glfw_FOUND)
	add_dependencies(Acid glfw)
endif()
# Defined in find_package(PhysFS)
if(NOT PHYSFS_FOUND)
	add_dependencies(Acid physfs)
endif()
# Manually defined by us
if(NOT SPIRV_FOUND)
	add_dependencies(Acid SPIRV)
endif()
# Defined in find_package(Bullet)
if(NOT BULLET_FOUND)
	add_dependencies(Acid BulletDynamics)
endif()

set_target_properties(Acid PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	FOLDER "Acid"
	)
target_compile_features(Acid PUBLIC cxx_std_17)

if(BUILD_SHARED_LIBS)
	set_target_properties(Acid PROPERTIES DEFINE_SYMBOL "ACID_EXPORTS")

	if(WIN32)
		set_target_properties(Acid PROPERTIES PREFIX "")
		set_target_properties(Acid PROPERTIES DEBUG_POSTFIX "")
	elseif(UNIX AND APPLE)
		set_target_properties(Acid PROPERTIES INSTALL_NAME_DIR "lib${LIB_SUFFIX}")
	endif()
	target_compile_options(Acid
		PRIVATE
		$<$<CXX_COMPILER_ID:GNU>:-fvisibility=hidden>
		$<$<CXX_COMPILER_ID:AppleClang>:-fno-common>
		)
else()
	target_compile_definitions(Acid PUBLIC "ACID_STATICLIB")
endif()

target_compile_definitions(Acid
	PUBLIC
	# If the CONFIG is Debug or RelWithDebInfo, define ACID_VERBOSE
	# Works on both single & mutli configuration
	$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:ACID_VERBOSE>
	# Windows
	$<$<PLATFORM_ID:Windows>:ACID_BUILD_WINDOWS WIN32_LEAN_AND_MEAN NOMINMAX>
	# Linux
	$<$<PLATFORM_ID:Linux>:ACID_BUILD_LINUX>
	# macOS
	$<$<PLATFORM_ID:Darwin>:ACID_BUILD_MACOS>
	# MSVC
	$<$<CXX_COMPILER_ID:MSVC>:ACID_BUILD_MSVC _SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS>
	# Clang/AppleClang
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:ACID_BUILD_CLANG>
	# GNU/GCC
	$<$<CXX_COMPILER_ID:GNU>:ACID_BUILD_GNU __USE_MINGW_ANSI_STDIO=0>
	)

target_include_directories(Acid
	PUBLIC
	# This is needed so that non-relative include paths can find headers
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	PRIVATE
	$<$<BOOL:${BULLET_INCLUDE_DIRS}>:${BULLET_INCLUDE_DIRS}>
	$<$<BOOL:${PHYSFS_INCLUDE_DIR}>:${PHYSFS_INCLUDE_DIR}>
	$<$<BOOL:${SPIRV_INCLUDE_DIR}>:${SPIRV_INCLUDE_DIR}>
	)
target_link_libraries(Acid
	PUBLIC
	# All IMPORTED targets, which automatically handles includes
	Threads::Threads
	Vulkan::Vulkan
	PRIVATE
	# More IMPORTED
	glfw
	OpenAL::OpenAL
	# Not IMPORTED targets, requires manual includes handling
	${PHYSFS_LIBRARY}
	${BULLET_LIBRARIES}
	${SPIRV_LIBRARY}
	# Windows only
	$<$<PLATFORM_ID:Windows>:ws2_32>
	)

install(TARGETS Acid
	# Auto-generates an export to install
	EXPORT AcidConfig
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
	FILES_MATCHING
	PATTERN "*.h"
	PATTERN "*.hpp"
	)
# Install the auto-generated export
install(EXPORT AcidConfig
	# Makes the end-user imported library Acid::Acid instead of just Acid
	NAMESPACE Acid::
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
	)
# Defines write_basic_package_version_file
include(CMakePackageConfigHelpers)
# Auto-generates a version file based on the current version specified in project()
write_basic_package_version_file(
	"${PROJECT_BINARY_DIR}/AcidConfigVersion.cmake"
	COMPATIBILITY SameMajorVersion
)
install(
  FILES
	"${PROJECT_BINARY_DIR}/AcidConfigVersion.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
