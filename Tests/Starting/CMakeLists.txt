include(CMakeSources.cmake)
#project(Starting)

set(STARTING_INCLUDES "${PROJECT_SOURCE_DIR}/Tests/Starting/")

#if(FL_BUILD_RELEASE AND WIN32)
#	add_executable(Starting WIN32 ${STARTING_SOURCES})
#else()
add_executable(Starting ${STARTING_SOURCES})
#endif()

add_dependencies(Starting Flounder)

target_include_directories(Starting PUBLIC ${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/" ${STARTING_INCLUDES})
target_link_libraries(Starting PRIVATE ${LIBRARIES_LINKS} Flounder)

# Install
if (FLOUNDER_INSTALL)
    install(DIRECTORY .
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
            PATTERN "Private" EXCLUDE
            )

    install(TARGETS Starting
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif ()