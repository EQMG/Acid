include(CMakeSources.cmake)
#project(STB)

set(STB_INCLUDES "${PROJECT_SOURCE_DIR}/include/")

add_library(stb ${LIB_TYPE} ${STB_SOURCES})

if(WIN32 AND BUILD_SHARED_LIBS)
	set_target_properties(stb PROPERTIES PREFIX "")
endif()

target_include_directories(stb PUBLIC ${STB_INCLUDES})

# Install
if (FLOUNDER_INSTALL)
	install(TARGETS stb
			EXPORT stbTargets
			RUNTIME DESTINATION "bin"
			ARCHIVE DESTINATION "lib"
			LIBRARY DESTINATION "lib"
	)
endif()
