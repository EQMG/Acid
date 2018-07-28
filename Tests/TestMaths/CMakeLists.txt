include(CMakeSources.cmake)
#project(TestMaths)

set(TESTMATHS_INCLUDES "${PROJECT_SOURCE_DIR}/Tests/TestMaths/")

add_executable(TestMaths "TestMaths.rc" ${TESTMATHS_SOURCES})

set_target_properties(TestMaths PROPERTIES
                      POSITION_INDEPENDENT_CODE ON
                      FOLDER "Acid")

add_dependencies(TestMaths Acid)

target_include_directories(TestMaths PUBLIC ${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/" ${TESTMATHS_INCLUDES})
target_link_libraries(TestMaths PRIVATE ${LIBRARIES_LINKS} Acid)

# Install
if(ACID_INSTALL)
    install(DIRECTORY .
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
            PATTERN "Private" EXCLUDE
            )

    install(TARGETS TestMaths
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif()