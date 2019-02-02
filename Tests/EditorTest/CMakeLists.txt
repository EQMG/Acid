file(GLOB_RECURSE EDITORTEST_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE EDITORTEST_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
set(EDITORTEST_SOURCES
		${EDITORTEST_HEADER_FILES}
		${EDITORTEST_SOURCE_FILES}
		)
set(EDITORTEST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/EditorTest/")

add_library(EditorTest ${EDITORTEST_SOURCES})
add_dependencies(EditorTest Acid)

target_compile_definitions(EditorTest
		PUBLIC
		ACID_RELOAD
		)
target_compile_features(EditorTest PUBLIC cxx_std_17)
set_target_properties(EditorTest PROPERTIES
		POSITION_INDEPENDENT_CODE ON
		FOLDER "Acid"
		)

target_include_directories(EditorTest PRIVATE ${ACID_INCLUDE_DIR} ${EDITORTEST_INCLUDE_DIR})
target_link_libraries(EditorTest PRIVATE Acid)
