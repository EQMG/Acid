file(GLOB_RECURSE TESTFONT_HEADER_FILES
	"*.h"
	"*.hpp"
	)
file(GLOB_RECURSE TESTFONT_SOURCE_FILES
	"*.c"
	"*.cpp"
	"*.rc"
	)
set(TESTFONT_SOURCES
	${TESTFONT_HEADER_FILES}
	${TESTFONT_SOURCE_FILES}
	)
set(TESTFONT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/TestFont/")

add_executable(TestFont ${TESTFONT_SOURCES})
add_dependencies(TestFont Acid)

target_compile_features(TestFont PUBLIC cxx_std_17)
set_target_properties(TestFont PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	FOLDER "Acid"
)

target_include_directories(TestFont PRIVATE ${ACID_INCLUDE_DIR} ${TESTFONT_INCLUDE_DIR})
target_link_libraries(TestFont PRIVATE Acid)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestFont PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(TestFont PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "Test Font"
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
		MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
		MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/Scripts/MacOSXBundleInfo.plist.in"
		)
endif()

add_test(NAME "Font" COMMAND "TestFont")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestFont
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
