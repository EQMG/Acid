include(CMakeSources.cmake)
#project(TestPhysics)

set(TESTPHYSICS_INCLUDES "${PROJECT_SOURCE_DIR}/Tests/TestPhysics/")

#if(ACID_BUILD_RELEASE AND WIN32)
#add_executable(TestPhysics WIN32 ${TESTPHYSICS_SOURCES})
#else()
add_executable(TestPhysics ${TESTPHYSICS_SOURCES})
#endif()

set_target_properties(TestPhysics PROPERTIES
                      POSITION_INDEPENDENT_CODE ON
                      FOLDER "Acid")

add_dependencies(TestPhysics Acid)

target_include_directories(TestPhysics PUBLIC ${ACID_INCLUDES} ${TESTPHYSICS_INCLUDES})
target_link_libraries(TestPhysics PRIVATE Acid)

# Install
if(ACID_INSTALL)
    install(DIRECTORY .
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
            PATTERN "Private" EXCLUDE
            )

    install(TARGETS TestPhysics
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif()