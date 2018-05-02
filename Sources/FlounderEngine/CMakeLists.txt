include(CMakeSources.cmake)
#project(FlounderEngine)

set(FLOUNDERENGINE_INCLUDES "${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/")

add_library(FlounderEngine ${LIB_TYPE} ${FLOUNDERENGINE_SOURCES})
add_dependencies(FlounderEngine SPIRV glfw stb)

if (WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(FlounderEngine PROPERTIES PREFIX "")
endif ()

if (BUILD_SHARED_LIBS)
    target_compile_definitions(FlounderEngine PUBLIC
            -DFL_EXPORTS
            )
else ()
    target_compile_definitions(FlounderEngine PUBLIC
            -DFL_STATICLIB
            )
endif ()

target_include_directories(stb PUBLIC ${LIBRARIES_INCLUDES} ${FLOUNDERENGINE_INCLUDES})
target_link_libraries(FlounderEngine PRIVATE ${LIBRARIES_LINKS})

# Install
if (FLOUNDER_INSTALL)
    install(TARGETS FlounderEngine
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )

    foreach(file ${FLOUNDERENGINE_HEADERS_})
        get_filename_component(dir ${file} DIRECTORY)
        install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Flounder/${dir})
    endforeach()
endif ()
