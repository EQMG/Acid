file(GLOB_RECURSE TESTGUIS_HEADER_FILES
	"*.h"
	"*.hpp"
)
file(GLOB_RECURSE TESTGUIS_SOURCE_FILES
	"*.c"
	"*.cpp"
	"*.rc"
)
set(TESTGUIS_SOURCES
	${TESTGUIS_HEADER_FILES}
	${TESTGUIS_SOURCE_FILES}
)
set(TESTGUIS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/TestGuis/")

if(ACID_BUILD_RELEASE AND WIN32)
	add_executable(TestGuis WIN32 ${TESTGUIS_SOURCES})
else()
	add_executable(TestGuis ${TESTGUIS_SOURCES})
endif()

set_target_properties(TestGuis PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	FOLDER "Acid"
)

add_dependencies(TestGuis Acid)

target_include_directories(TestGuis PUBLIC ${ACID_INCLUDE_DIR} ${TESTGUIS_INCLUDE_DIR})
target_link_libraries(TestGuis PUBLIC Acid)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestGuis PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(TestGuis PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "Test Guis"
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
		MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
		MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/Scripts/MacOSXBundleInfo.plist.in"
	)
endif()

# Install
if(ACID_INSTALL)
	install(DIRECTORY .
		DESTINATION include
		FILES_MATCHING PATTERN "*.h"
		PATTERN "Private" EXCLUDE
	)

	install(TARGETS TestGuis
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
endif()