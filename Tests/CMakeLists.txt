option(ACID_INSTALL_EXAMPLES "Installs the examples" ON)
option(ACID_ENABLE_COVERAGE "Enables the measurement of test coverages" OFF)
option(BUILD_TESTS_EDITOR "Build editor application" ON)
option(BUILD_TESTS_TUTORIAL "Build test tutorial applications" ON)
option(BUILD_TESTS_UNITS "Build unit test applications" ON)

if(BUILD_TESTS_EDITOR)
	add_subdirectory(Editor)
	add_subdirectory(EditorTest)
endif()

add_subdirectory(TestFont)
add_subdirectory(TestGUI)
add_subdirectory(TestMaths)
add_subdirectory(TestNetwork)
add_subdirectory(TestPacker)
add_subdirectory(TestPBR)
add_subdirectory(TestPhysics)
add_subdirectory(TestSerial)

if(BUILD_TESTS_TUTORIAL)
	add_subdirectory(Tutorial1)
	add_subdirectory(Tutorial2)
	add_subdirectory(Tutorial3)
	add_subdirectory(Tutorial4)
	add_subdirectory(Tutorial5)
	add_subdirectory(Tutorial6)
	add_subdirectory(Tutorial7)
endif()

if(BUILD_TESTS_UNITS)
	find_package(GTest 1.11.0 QUIET)
	if(NOT GTest_FOUND)
		add_subdirectory(../External/googletest googletest)
		set_target_properties(gmock PROPERTIES FOLDER "External/googletest")
		set_target_properties(gmock_main PROPERTIES FOLDER "External/googletest")
		set_target_properties(gtest PROPERTIES FOLDER "External/googletest")
		set_target_properties(gtest_main PROPERTIES FOLDER "External/googletest")
	
		set(GTEST_INCLUDE_DIRS
				"${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest/googlemock/include"
				"${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest/googletest/include"
				)
		set(GTEST_LIBRARIES "gtest")
		set(GTEST_MAIN_LIBRARIES "gtest_main")
		set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
	endif()

	add_subdirectory(Units)
endif()

if(BUILD_TESTS_UNITS AND ACID_ENABLE_COVERAGE)
	include(CodeCoverage)
	append_coverage_compiler_flags()
	SET(COVERAGE_EXCLUDES
			"/usr/*"
			"${CMAKE_SOURCE_DIR}/../Tests/*"
			)
	setup_target_for_coverage(NAME GX_COVERAGE EXECUTABLE ctest DEPENDENCIES UnitTests)
endif()
