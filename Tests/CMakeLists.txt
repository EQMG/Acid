set(_tests_headers
		)
set(_tests_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Test_Maths.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Test_Network.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Test_RingBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Test_Serial.cpp
		)

add_executable(Tests
		${_tests_headers}
		${_tests_sources}
		)
add_test(NAME Tests COMMAND Tests)

find_package(GTest 1.11.0 QUIET)
if(NOT GTest_FOUND)
	set(INSTALL_GTEST OFF CACHE BOOL "Enable installation of googletest")
	set(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
	add_subdirectory(${THIRD_PARTY_SOURCE_DIR}/googletest ${THIRD_PARTY_BINARY_DIR}/googletest)

	set_target_properties(gmock PROPERTIES FOLDER "External/googletest")
	set_target_properties(gmock_main PROPERTIES FOLDER "External/googletest")
	set_target_properties(gtest PROPERTIES FOLDER "External/googletest")
	set_target_properties(gtest_main PROPERTIES FOLDER "External/googletest")

	set(GTEST_INCLUDE_DIRS
			"${THIRD_PARTY_SOURCE_DIR}/googletest/googlemock/include"
			"${THIRD_PARTY_SOURCE_DIR}/googletest/googletest/include"
			)
	set(GTEST_LIBRARIES "gtest")
	set(GTEST_MAIN_LIBRARIES "gtest_main")
	set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
endif()

target_compile_features(Tests PUBLIC c_std_11 cxx_std_17)
target_include_directories(Tests
		PRIVATE 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		${GTEST_INCLUDE_DIRS}
		)
target_link_libraries(Tests
		PRIVATE
		${ACID_LIBRARIES}
		${GTEST_BOTH_LIBRARIES}
		)
set_target_properties(Tests
		PROPERTIES
		FOLDER "Acid/Tests"
		)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_tests_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_tests_sources})
