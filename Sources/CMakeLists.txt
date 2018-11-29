add_library(Acid)
# This file is quite big, so for readability we include instead...
# This uses target_sources(Acid) to define its headers & source files
include(acid_sources.cmake)

add_library(Acid ${ACID_SOURCES})
add_dependencies(Acid glfw SPIRV physfs BulletDynamics)

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

if(WIN32)
	target_link_libraries(Acid PRIVATE ws2_32)
endif()

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
