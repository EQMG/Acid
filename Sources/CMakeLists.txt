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
	elseif(UNIX AND NOT APPLE)
		target_compile_options(Acid PRIVATE "-fvisibility=hidden")
	elseif(UNIX AND APPLE)
		target_compile_options(Acid PRIVATE "-fno-common")
		set_target_properties(Acid PROPERTIES INSTALL_NAME_DIR "lib${LIB_SUFFIX}")
	endif()
else()
	target_compile_definitions(Acid PUBLIC "ACID_STATICLIB")
endif()

target_include_directories(Acid PUBLIC ${ACID_INCLUDE_DIR})
target_link_libraries(Acid PUBLIC Threads::Threads Vulkan::Vulkan OpenAL::OpenAL glfw SPIRV physfs BulletSoftBody BulletDynamics BulletCollision LinearMath)

target_include_directories(Acid
	PUBLIC
	# This is needed so that non-relative include paths can find headers
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	# Having an INSTALL_INTERFACE tells EXPORT to auto-include the dir when Acid::Acid is linked against
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
	PRIVATE
	"${BULLET_INCLUDE_DIRS}"
	"${PHYSFS_INCLUDE_DIR}"
	"${SPIRV_INCLUDE_DIR}"
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
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
		FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp"
		)
