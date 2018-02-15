include(CMakeSources.cmake)
#project(FlounderEngine)

set(FLOUNDERENGINE_INCLUDES "${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/")

include_directories(${LIBRARIES_INCLUDES} ${FLOUNDERENGINE_INCLUDES})

add_library(FlounderEngine ${LIB_TYPE} ${FLOUNDERENGINE_SOURCES})

if(BUILD_SHARED_LIBS)
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

target_link_libraries(FlounderEngine PRIVATE ${LIBRARIES_LINKS})
add_dependencies(FlounderEngine glfw stb)
