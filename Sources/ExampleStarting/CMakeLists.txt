include(CMakeSources.cmake)
#project(ExampleStarting)

set(EXAMPLE_STARTING_INCLUDES
    "${PROJECT_SOURCE_DIR}/Sources/ExampleStarting/"
)

include_directories(${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/" ${EXAMPLE_STARTING_INCLUDES}) # ${FLOUNDERENGINE_INCLUDES}

#if(FLOUNDER_CONFIG_RELEASE AND WIN32)
#	add_executable(ExampleStarting WIN32 ${EXAMPLE_STARTING_SOURCES})
#else()
	add_executable(ExampleStarting ${EXAMPLE_STARTING_SOURCES})
#endif()

target_compile_definitions(ExampleStarting PRIVATE -DEXAMPLE_STARTING_EXPORTS)

target_link_libraries(ExampleStarting FlounderEngine)
add_dependencies(ExampleStarting FlounderEngine)
