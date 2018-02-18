include(CMakeSources.cmake)
#project(FlounderEngine)

set(FLOUNDERENGINE_INCLUDES "${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/")

add_library(FlounderEngine ${LIB_TYPE} ${FLOUNDERENGINE_SOURCES})
add_dependencies(FlounderEngine glfw stb LinearMath BulletCollision)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(FlounderEngine PROPERTIES PREFIX "")
endif()

if (BUILD_SHARED_LIBS)
    target_compile_definitions(FlounderEngine PUBLIC
            -DF_EXPORTS
    )
else()
    target_compile_definitions(FlounderEngine PUBLIC
            -DFLOUNDER_STATIC_LIB
    )
endif()

target_include_directories(stb PUBLIC ${LIBRARIES_INCLUDES} ${FLOUNDERENGINE_INCLUDES})
target_link_libraries(FlounderEngine PRIVATE ${LIBRARIES_LINKS})

# Install
if (FLOUNDER_INSTALL)
    install(TARGETS FlounderEngine
            EXPORT FlounderEngineTargets
            RUNTIME DESTINATION "bin"
            ARCHIVE DESTINATION "lib"
            LIBRARY DESTINATION "lib"
    )
endif()
