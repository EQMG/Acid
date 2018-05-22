include(CMakeSources.cmake)
#project(TestTerrain)

set(TESTTERRAIN_INCLUDES "${PROJECT_SOURCE_DIR}/Tests/TestTerrain/")

#if(FL_BUILD_RELEASE AND WIN32)
#	add_executable(TestTerrain WIN32 ${TESTTERRAIN_SOURCES})
#else()
add_executable(TestTerrain ${TESTTERRAIN_SOURCES})
#endif()

set_target_properties(TestTerrain PROPERTIES
                      POSITION_INDEPENDENT_CODE ON
                      FOLDER "Flounder")

add_dependencies(TestTerrain Flounder)

target_include_directories(TestTerrain PUBLIC ${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/" ${TESTTERRAIN_INCLUDES})
target_link_libraries(TestTerrain PRIVATE ${LIBRARIES_LINKS} Flounder)

# Install
if (FLOUNDER_INSTALL)
    install(DIRECTORY .
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
            PATTERN "Private" EXCLUDE
            )

    install(TARGETS TestTerrain
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif ()