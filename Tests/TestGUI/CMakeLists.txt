file(GLOB_RECURSE TESTGUI_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE TESTGUI_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Header Files" FILES ${TESTGUI_HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" FILES ${TESTGUI_SOURCE_FILES})

add_executable(TestGUI ${TESTGUI_HEADER_FILES} ${TESTGUI_SOURCE_FILES})

target_compile_features(TestGUI PUBLIC cxx_std_17)
target_include_directories(TestGUI PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(TestGUI PRIVATE Acid::Acid)

set_target_properties(TestGUI PROPERTIES
		FOLDER "Acid"
		)
if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestGUI PROPERTIES 
			LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
			)
elseif(UNIX AND APPLE)
	set_target_properties(TestGUI PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test GUI"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "GUI" COMMAND "TestGUI")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestGUI
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
