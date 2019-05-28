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

add_library(EditorTest ${EDITORTEST_SOURCES})

target_compile_definitions(EditorTest
		PUBLIC
		ACID_RELOAD
		)
target_compile_features(EditorTest PUBLIC cxx_std_17)
set_target_properties(EditorTest PROPERTIES
		FOLDER "Acid"
		)

target_include_directories(EditorTest PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(EditorTest PRIVATE Acid::Acid)
