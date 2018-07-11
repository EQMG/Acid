include(CMakeSources.cmake)
#project(Flounder)

find_package(Threads REQUIRED)

set(FLOUNDER_INCLUDES "${PROJECT_SOURCE_DIR}/Sources/")

add_library(Flounder ${LIB_TYPE} ${FLOUNDER_SOURCES})
add_dependencies(Flounder VWSI STB SPIRV LinearMath BulletCollision BulletDynamics)

set_target_properties(Flounder PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        FOLDER "Flounder")

if (WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(Flounder PROPERTIES PREFIX "")
    set_target_properties(Flounder PROPERTIES DEBUG_POSTFIX "")
endif ()

if (BUILD_SHARED_LIBS)
    target_compile_definitions(Flounder PUBLIC
            -DFL_EXPORTS
            )
else ()
    target_compile_definitions(Flounder PUBLIC
            -DFL_STATICLIB
            )
endif ()

target_include_directories(Flounder PUBLIC ${LIBRARIES_INCLUDES} ${FLOUNDER_INCLUDES})
target_link_libraries(Flounder PRIVATE ${LIBRARIES_LINKS})

# Install
if (FLOUNDER_INSTALL)
    install(TARGETS Flounder
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )

    foreach (file ${FLOUNDER_HEADERS_})
        get_filename_component(dir ${file} DIRECTORY)
        install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/${dir})
    endforeach ()
endif ()
