file(GLOB_RECURSE TESTMATHS_HEADER_FILES
	"*.h"
	"*.hpp"
)
file(GLOB_RECURSE TESTMATHS_SOURCE_FILES
	"*.c"
	"*.cpp"
	"*.rc"
)
set(TESTMATHS_SOURCES
	${TESTMATHS_HEADER_FILES}
	${TESTMATHS_SOURCE_FILES}
)
set(TESTMATHS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/TestMaths/")

if(ACID_BUILD_RELEASE AND WIN32)
	add_executable(TestMaths WIN32 ${TESTMATHS_SOURCES})
else()
	add_executable(TestMaths ${TESTMATHS_SOURCES})
endif()

set_target_properties(TestMaths PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	FOLDER "Acid"
)

add_dependencies(TestMaths Acid)

target_include_directories(TestMaths PUBLIC ${ACID_INCLUDE_DIR} ${TESTMATHS_INCLUDE_DIR})
target_link_libraries(TestMaths PUBLIC Acid)

if(UNIX AND APPLE)
	set_target_properties(TestMaths PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "Test Maths"
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

	install(TARGETS TestMaths
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
endif()