file(GLOB_RECURSE ACID_HEADER_FILES
	"*.h"
	"*.hpp"
)
file(GLOB_RECURSE ACID_SOURCE_FILES
	"*.c"
	"*.cpp"
)
set(ACID_SOURCES
	${ACID_HEADER_FILES}
	${ACID_SOURCE_FILES}
)
set(ACID_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Sources/")

add_library(Acid ${LIB_TYPE} ${ACID_SOURCES})
add_dependencies(Acid glfw SPIRV BulletDynamics)

set_target_properties(Acid PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	FOLDER "Acid"
	VERSION ${ACID_VERSION}
	SOVERSION ${ACID_VERSION}
)

if(BUILD_SHARED_LIBS)
	target_compile_definitions(Acid PUBLIC "ACID_EXPORTS")

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

target_include_directories(Acid PUBLIC ${VULKAN_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR} ${GLSLANG_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR} ${BULLET_INCLUDE_DIRS} ${ACID_INCLUDE_DIR})
target_link_libraries(Acid PUBLIC ${VULKAN_LIBRARY} ${OPENAL_LIBRARY} ${GLSLANG_LIBRARIES} ${GLFW_LIBRARY} ${BULLET_LIBRARIES})

# Install
if(ACID_INSTALL)
	install(TARGETS Acid
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

	foreach(file ${ACID_HEADER_FILES})
		get_filename_component(dir ${file} DIRECTORY)
		install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/acid/${dir})
	endforeach()
endif()
