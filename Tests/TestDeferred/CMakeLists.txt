include(CMakeSources.cmake)
#project(TestDeferred)

set(TESTDEFERED_INCLUDES "${PROJECT_SOURCE_DIR}/Tests/TestDeferred/")

#if(ACID_BUILD_RELEASE AND WIN32)
#add_executable(TestDeferred WIN32 ${TESTDEFERED_SOURCES})
#else()
add_executable(TestDeferred ${TESTDEFERED_SOURCES})
#endif()

set_target_properties(TestDeferred PROPERTIES
                      POSITION_INDEPENDENT_CODE ONif(UNIX AND NOT APPLE)
        find_package(XCB REQUIRED)
        endif()

        FOLDER "Acid")

add_dependencies(TestDeferred Acid)

target_include_directories(TestDeferred PUBLIC ${ACID_INCLUDES} ${TESTDEFERED_INCLUDES})
target_link_libraries(TestDeferred PRIVATE Acid)

# Install
if(ACID_INSTALL)
    install(DIRECTORY .
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
            PATTERN "Private" EXCLUDE
            )

    install(TARGETS TestDeferred
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif()