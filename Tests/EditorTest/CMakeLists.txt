file(GLOB_RECURSE EDITORTEST_HEADER_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.h" "*.hpp" "*.inl"
		)
file(GLOB_RECURSE EDITORTEST_SOURCE_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.c" "*.cpp" "*.rc"
		)

add_library(EditorTest ${EDITORTEST_HEADER_FILES} ${EDITORTEST_SOURCE_FILES})

target_compile_features(EditorTest PUBLIC cxx_std_17)
target_compile_definitions(EditorTest
		PUBLIC
		ACID_RELOAD
		)
target_include_directories(EditorTest PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(EditorTest PRIVATE Acid::Acid)

set_target_properties(EditorTest PROPERTIES
		FOLDER "Acid"
		)

include(AcidGroupSources)
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${EDITORTEST_HEADER_FILES}")
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${EDITORTEST_SOURCE_FILES}")
