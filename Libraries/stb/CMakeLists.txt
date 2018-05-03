include(CMakeSources.cmake)
project(STB)

option(STB_INSTALL "Generate installation target" ON)

add_library(stb ${LIB_TYPE} ${STB_SOURCES})

target_include_directories(stb PUBLIC ${STB_INCLUDES})

if(WIN32 AND BUILD_SHARED_LIBS)
	set_target_properties(stb PROPERTIES PREFIX "")
endif()

# Install
if (STB_INSTALL)
	install(TARGETS stb
			EXPORT stbTargets
			ARCHIVE DESTINATION "lib"
			LIBRARY DESTINATION "lib"
	)

    foreach(file ${STB_HEADERS_})
        get_filename_component(dir ${file} DIRECTORY)
        install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/stb/${dir})
    endforeach()
endif()
