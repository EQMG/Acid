include(CMakeSources.cmake)
#project(TestGuis)

set(TESTGUIS_INCLUDES "${PROJECT_SOURCE_DIR}/Tests/TestGuis/")

#if(FL_BUILD_RELEASE AND WIN32)
#	add_executable(TestGuis WIN32 ${TESTGUIS_SOURCES})
#else()
add_executable(TestGuis "TestGuis.rc" ${TESTGUIS_SOURCES})
#endif()

set_target_properties(TestGuis PROPERTIES
                      POSITION_INDEPENDENT_CODE ON
                      FOLDER "Acid")

add_dependencies(TestGuis Acid)

target_include_directories(TestGuis PUBLIC ${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/" ${TESTGUIS_INCLUDES})
target_link_libraries(TestGuis PRIVATE ${LIBRARIES_LINKS} Acid)

# Install
if (ACID_INSTALL)
    install(DIRECTORY .
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
            PATTERN "Private" EXCLUDE
            )

    install(TARGETS TestGuis
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif ()