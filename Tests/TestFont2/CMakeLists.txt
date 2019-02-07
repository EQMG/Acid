file(GLOB_RECURSE TESTFONT2_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE TESTFONT2_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
set(TESTFONT2_SOURCES
		${TESTFONT2_HEADER_FILES}
		${TESTFONT2_SOURCE_FILES}
		)
set(TESTFONT2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/TestFont2/")

add_executable(TestFont2 ${TESTFONT2_SOURCES})
add_dependencies(TestFont2 Acid)

target_compile_features(TestFont2 PUBLIC cxx_std_17)
set_target_properties(TestFont2 PROPERTIES
		POSITION_INDEPENDENT_CODE ON
		FOLDER "Acid"
		)

target_include_directories(TestFont2 PRIVATE ${ACID_INCLUDE_DIR} ${TESTFONT2_INCLUDE_DIR} $<$<BOOL:${FREETYPE_INCLUDE_DIRS}>:${FREETYPE_INCLUDE_DIRS}>)
target_link_libraries(TestFont2 PRIVATE Acid glfw ${FREETYPE_LIBRARIES})

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestFont2 PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(TestFont2 PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Font"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/Scripts/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "Font2" COMMAND "TestFont2")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestFont2
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
