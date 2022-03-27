include(FetchContent)

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

add_test(NAME UnitTests COMMAND UnitTests)
		
include(AcidGroupSources)
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TESTUNITS_HEADER_FILES}")
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TESTUNITS_SOURCE_FILES}")
