include(CMakeSources.cmake)
#project(TestDeferred)

set(TESTDEFERED_INCLUDES "${PROJECT_SOURCE_DIR}/Tests/TestDeferred/")

#if(FL_BUILD_RELEASE AND WIN32)
#	add_executable(TestDeferred WIN32 ${TESTDEFERED_SOURCES})
#else()
add_executable(TestDeferred ${TESTDEFERED_SOURCES})
#endif()

add_dependencies(TestDeferred Flounder)

target_include_directories(TestDeferred PUBLIC ${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/" ${TESTDEFERED_INCLUDES})
target_link_libraries(TestDeferred PRIVATE ${LIBRARIES_LINKS} Flounder)

# Install
if (FLOUNDER_INSTALL)
    install(DIRECTORY .
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
            PATTERN "Private" EXCLUDE
            )

    install(TARGETS TestDeferred
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif ()