file(GLOB_RECURSE TESTGUI_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE TESTGUI_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
set(TESTGUI_SOURCES
		${TESTGUI_HEADER_FILES}
		${TESTGUI_SOURCE_FILES}
		)
set(TESTGUI_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/TestGUI/")

add_library(TestGUI ${TESTGUI_SOURCES})
add_dependencies(TestGUI Acid)

target_compile_definitions(TestGUI
		PUBLIC
		ACID_RELOAD
		)
target_compile_features(TestGUI PUBLIC cxx_std_17)
set_target_properties(TestGUI PROPERTIES
		POSITION_INDEPENDENT_CODE ON
		FOLDER "Acid"
		)

target_include_directories(TestGUI PRIVATE ${ACID_INCLUDE_DIR} ${TESTGUI_INCLUDE_DIR})
target_link_libraries(TestGUI PRIVATE Acid)

#if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
#	set_target_properties(TestGUI PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
#elseif(UNIX AND APPLE)
#	set_target_properties(TestGUI PROPERTIES
#		    MACOSX_BUNDLE_BUNDLE_NAME "Test GUI"
#		    MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
#		    MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
#		    MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/Scripts/MacOSXBundleInfo.plist.in"
#		    )
#endif()

#add_test(NAME "GUI" COMMAND "TestGUI")

if(ACID_INSTALL_EXAMPLES)
#	install(TARGETS TestGUI
#			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
#			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#			)
endif()
