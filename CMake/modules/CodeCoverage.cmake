include(CMakeParseArguments)

find_program( GCOV_PATH gcov )
find_program( LCOV_PATH  NAMES lcov lcov.bat lcov.exe lcov.perl)
find_program( GENHTML_PATH NAMES genhtml genhtml.perl genhtml.bat )
find_program( GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/scripts/test)
find_program( SIMPLE_PYTHON_EXECUTABLE python )

if(NOT GCOV_PATH)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
        message(FATAL_ERROR "Clang version must be 3.0.0 or greater! Aborting...")
    endif()
elseif(NOT CMAKE_COMPILER_IS_GNUCXX)
    message(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
endif()

set(COVERAGE_COMPILER_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage"
    CACHE INTERNAL "")

set(CMAKE_CXX_FLAGS_COVERAGE
    ${COVERAGE_COMPILER_FLAGS}
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
set(CMAKE_C_FLAGS_COVERAGE
    ${COVERAGE_COMPILER_FLAGS}
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE )
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE )
mark_as_advanced(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "Code coverage results with an optimised (non-Debug) build may be misleading")
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    link_libraries(gcov)
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

function(SETUP_TARGET_FOR_COVERAGE)

    set(options NONE)
    set(oneValueArgs NAME)
    set(multiValueArgs EXECUTABLE EXECUTABLE_ARGS DEPENDENCIES)
    cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT LCOV_PATH)
        message(FATAL_ERROR "lcov not found! Aborting...")
    endif()

    if(NOT GENHTML_PATH)
        message(FATAL_ERROR "genhtml not found! Aborting...")
    endif()

    add_custom_target(${Coverage_NAME}

        COMMAND ${LCOV_PATH} --directory . --zerocounters
        COMMAND ${LCOV_PATH} -c -i -d . -o ${Coverage_NAME}.base
        COMMAND ${Coverage_EXECUTABLE}
        COMMAND ${LCOV_PATH} --directory . --capture --output-file ${Coverage_NAME}.info
        COMMAND ${LCOV_PATH} -a ${Coverage_NAME}.base -a ${Coverage_NAME}.info --output-file ${Coverage_NAME}.total
        COMMAND ${LCOV_PATH} --remove ${Coverage_NAME}.total ${COVERAGE_EXCLUDES} --output-file ${PROJECT_BINARY_DIR}/${Coverage_NAME}.info.cleaned
        COMMAND ${GENHTML_PATH} -o ${Coverage_NAME} ${PROJECT_BINARY_DIR}/${Coverage_NAME}.info.cleaned
        COMMAND ${CMAKE_COMMAND} -E remove ${Coverage_NAME}.base ${Coverage_NAME}.total ${PROJECT_BINARY_DIR}/${Coverage_NAME}.info.cleaned

        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${Coverage_DEPENDENCIES}
        COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
    )
    
    add_custom_command(TARGET ${Coverage_NAME} POST_BUILD
        COMMAND ;
        COMMENT "Lcov code coverage info report saved in ${Coverage_NAME}.info."
    )

    add_custom_command(TARGET ${Coverage_NAME} POST_BUILD
        COMMAND ;
        COMMENT "Open ./${Coverage_NAME}/index.html in your browser to view the coverage report."
    )

endfunction()

function(SETUP_TARGET_FOR_COVERAGE_COBERTURA)

    set(options NONE)
    set(oneValueArgs NAME)
    set(multiValueArgs EXECUTABLE EXECUTABLE_ARGS DEPENDENCIES)
    cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT SIMPLE_PYTHON_EXECUTABLE)
        message(FATAL_ERROR "python not found! Aborting...")
    endif()

    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "gcovr not found! Aborting...")
    endif()

    set(COBERTURA_EXCLUDES "")
    foreach(EXCLUDE ${COVERAGE_EXCLUDES})
        set(COBERTURA_EXCLUDES "-e ${EXCLUDE} ${COBERTURA_EXCLUDES}")
    endforeach()

    add_custom_target(${Coverage_NAME}

        ${Coverage_EXECUTABLE}

        COMMAND ${GCOVR_PATH} -x -r ${CMAKE_SOURCE_DIR} ${COBERTURA_EXCLUDES}
            -o ${Coverage_NAME}.xml
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${Coverage_DEPENDENCIES}
        COMMENT "Running gcovr to produce Cobertura code coverage report."
    )

    add_custom_command(TARGET ${Coverage_NAME} POST_BUILD
        COMMAND ;
        COMMENT "Cobertura code coverage report saved in ${Coverage_NAME}.xml."
    )

endfunction()

function(APPEND_COVERAGE_COMPILER_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}" PARENT_SCOPE)
    message(STATUS "Appending code coverage compiler flags: ${COVERAGE_COMPILER_FLAGS}")
endfunction()