include(CMakeSources.cmake)
#project(TestGuis)

set(TESTGUIS_INCLUDES "${PROJECT_SOURCE_DIR}/Tests/TestGuis/")

#if(FL_BUILD_RELEASE AND WIN32)
#	add_executable(TestGuis WIN32 ${TESTGUIS_SOURCES})
#else()
add_executable(TestGuis ${TESTGUIS_SOURCES})
#endif()

add_dependencies(TestGuis Flounder)

target_include_directories(TestGuis PUBLIC ${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/" ${TESTGUIS_INCLUDES})
target_link_libraries(TestGuis PRIVATE ${LIBRARIES_LINKS} Flounder)

# Install
if (FLOUNDER_INSTALL)
    install(DIRECTORY .
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
            PATTERN "Private" EXCLUDE
            )

    install(TARGETS TestGuis
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif ()