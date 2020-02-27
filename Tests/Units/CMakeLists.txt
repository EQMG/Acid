include(FetchContent)

find_package(GTest 1.8.1 QUIET)
if(NOT GTest_FOUND)
	FetchContent_Declare(
			googletest
			URL https://github.com/google/googletest/archive/release-1.8.1.tar.gz
			URL_MD5 2e6fbeb6a91310a16efe181886c59596
			)
	FetchContent_GetProperties(googletest)
	if(NOT googletest_POPULATED)
		FetchContent_Populate(googletest)
		add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
		set_target_properties(gmock PROPERTIES FOLDER googletest)
		set_target_properties(gmock_main PROPERTIES FOLDER googletest)
		set_target_properties(gtest PROPERTIES FOLDER googletest)
		set_target_properties(gtest_main PROPERTIES FOLDER googletest)
	endif()
	set(GTEST_INCLUDE_DIRS "${bullet3_SOURCE_DIR}/googlemock/include" "${bullet3_SOURCE_DIR}/googletest/include")
	set(GTEST_LIBRARIES "gtest")
	set(GTEST_MAIN_LIBRARIES "gtest_main")
	set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
endif()

enable_testing()

file(GLOB_RECURSE TESTUNITS_HEADER_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.h" "*.hpp" "*.inl"
		)
file(GLOB_RECURSE TESTUNITS_SOURCE_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.c" "*.cpp" "*.rc"
		)

add_executable(UnitTests ${TESTUNITS_HEADER_FILES} ${TESTUNITS_SOURCE_FILES})

target_compile_features(UnitTests PUBLIC cxx_std_17)
target_include_directories(UnitTests PRIVATE 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		${GTEST_INCLUDE_DIRS}
		)
target_link_libraries(UnitTests PRIVATE Acid::Acid ${GTEST_BOTH_LIBRARIES})

set_target_properties(UnitTests PROPERTIES
		FOLDER "Acid/Tests"
		)

add_test(NAME unit
		COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit
		)
		
include(AcidGroupSources)
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TESTUNITS_HEADER_FILES}")
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TESTUNITS_SOURCE_FILES}")
