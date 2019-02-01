file(GLOB_RECURSE EDITOR_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE EDITOR_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
set(EDITOR_SOURCES
		${EDITOR_HEADER_FILES}
		${EDITOR_SOURCE_FILES}
		)
set(EDITOR_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/Editor/")

add_executable(Editor ${EDITOR_SOURCES})
add_dependencies(Editor Acid TestGUI)

target_compile_features(Editor PUBLIC cxx_std_17)
set_target_properties(Editor PROPERTIES
		POSITION_INDEPENDENT_CODE ON
		FOLDER "Acid"
		)

target_include_directories(Editor PRIVATE ${ACID_INCLUDE_DIR} ${EDITOR_INCLUDE_DIR})
target_link_libraries(Editor PRIVATE Acid)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(Editor PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(Editor PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Editor"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/Scripts/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "Editor" COMMAND "Editor")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS Editor
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
