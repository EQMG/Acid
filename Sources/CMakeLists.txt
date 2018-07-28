include(CMakeSources.cmake)
#project(Acid)

set(ACID_INCLUDES "${PROJECT_SOURCE_DIR}/Sources/")

add_library(Acid ${LIB_TYPE} ${ACID_SOURCES})
add_dependencies(Acid SPIRV glslang BulletDynamics BulletCollision LinearMath)

set_target_properties(Acid PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        FOLDER "Acid")

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(Acid PROPERTIES PREFIX "")
    set_target_properties(Acid PROPERTIES DEBUG_POSTFIX "")
endif()

if(BUILD_SHARED_LIBS)
    target_compile_definitions(Acid PUBLIC
            -DACID_EXPORTS
            )
else()
    target_compile_definitions(Acid PUBLIC
            -DACID_STATICLIB
            )
endif()

target_include_directories(Acid PUBLIC ${LIBRARIES_INCLUDES} ${ACID_INCLUDES})
target_link_libraries(Acid PRIVATE ${LIBRARIES_LINKS})

# Install
if(ACID_INSTALL)
    install(TARGETS Acid
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )

    foreach(file ${ACID_HEADERS_})
        get_filename_component(dir ${file} DIRECTORY)
        install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/acid/${dir})
    endforeach()
endif()
