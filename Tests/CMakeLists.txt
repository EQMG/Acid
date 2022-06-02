set(_tests_headers
		)
set(_tests_sources
		Main.cpp
		Test_Maths.cpp
		Test_Network.cpp
		Test_RingBuffer.cpp
		Test_Serial.cpp
		Test_Vector2.cpp
		)

add_executable(Tests ${_tests_headers} ${_tests_sources})

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

target_compile_features(Tests PUBLIC cxx_std_17)
target_include_directories(Tests PRIVATE 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		${GTEST_INCLUDE_DIRS}
		)
target_link_libraries(Tests PRIVATE Acid::Acid ${GTEST_BOTH_LIBRARIES})

set_target_properties(Tests PROPERTIES
		FOLDER "Acid/Tests"
		)

add_test(NAME Tests COMMAND Tests)
		
include(AcidGroupSources)
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${_tests_headers}")
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${_tests_sources}")
