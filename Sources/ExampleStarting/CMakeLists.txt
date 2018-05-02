include(CMakeSources.cmake)
#project(ExampleStarting)

set(EXAMPLE_STARTING_INCLUDES "${PROJECT_SOURCE_DIR}/Sources/ExampleStarting/")

#if(FL_BUILD_RELEASE AND WIN32)
#	add_executable(ExampleStarting WIN32 ${EXAMPLE_STARTING_SOURCES})
#else()
add_executable(ExampleStarting ${EXAMPLE_STARTING_SOURCES})
#endif()

add_dependencies(ExampleStarting FlounderEngine)

target_include_directories(ExampleStarting PUBLIC ${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/" ${EXAMPLE_STARTING_INCLUDES})
target_link_libraries(ExampleStarting PRIVATE ${LIBRARIES_LINKS} FlounderEngine)

# Install
if (FLOUNDER_INSTALL)
    install(DIRECTORY .
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
            PATTERN "Private" EXCLUDE
            )

    install(TARGETS ExampleStarting
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif ()