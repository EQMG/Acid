include(FetchContent)

FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG release-1.8.1
		)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
	FetchContent_Populate(googletest)
	add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

enable_testing()

file(GLOB_RECURSE TESTUNITS_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE TESTUNITS_SOURCE_FILES
		"*.c"
		"*.cpp"
		)
set(TESTUNITS_SOURCES
		${TESTUNITS_HEADER_FILES}
		${TESTUNITS_SOURCE_FILES}
		)

add_executable(UnitTests ${TESTUNITS_SOURCES})

target_compile_features(UnitTests PUBLIC cxx_std_17)
target_include_directories(UnitTests PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(UnitTests PRIVATE Acid::Acid gtest_main)

add_test(NAME unit
		COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/vat_test
		)
