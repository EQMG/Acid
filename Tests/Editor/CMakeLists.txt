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

add_executable(Editor ${EDITOR_SOURCES})

target_compile_features(Editor PUBLIC cxx_std_17)
set_target_properties(Editor PROPERTIES
		FOLDER "Acid"
		)

target_include_directories(Editor PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(Editor PRIVATE Acid::Acid)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(Editor PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(Editor PROPERTIES
			#RESOURCE Icon.icns
			MACOSX_BUNDLE_BUNDLE_NAME "Editor"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			#MACOSX_BUNDLE_ICON_FILE Icon.icns
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "Editor" COMMAND "Editor")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS Editor
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
