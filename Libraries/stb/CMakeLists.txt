include(CMakeSources.cmake)
project(STB)

option(STB_INSTALL "Generate installation target" ON)

set(STB_INCLUDES "${PROJECT_SOURCE_DIR}/include")
add_library(STB ${LIB_TYPE} ${STB_SOURCES})

set_target_properties(STB PROPERTIES
                      POSITION_INDEPENDENT_CODE ON
                      FOLDER "STB")

target_include_directories(STB PUBLIC ${STB_INCLUDES})

if(WIN32 AND BUILD_SHARED_LIBS)
	set_target_properties(STB PROPERTIES PREFIX "")
endif()

# Install
if (STB_INSTALL)
	install(TARGETS STB
			EXPORT stbTargets
			ARCHIVE DESTINATION "lib"
			LIBRARY DESTINATION "lib"
	)

#    foreach(file ${STB_HEADERS_})
#        get_filename_component(dir ${file} DIRECTORY)
#        install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/STB/${dir})
#    endforeach()

	if (FLOUNDER_INSTALL)
		file(COPY "${STB_INCLUDES}/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/")
	endif()
endif()
