file(GLOB_RECURSE TESTNETWORK_HEADER_FILES
	"*.h"
	"*.hpp"
)
file(GLOB_RECURSE TESTNETWORK_SOURCE_FILES
	"*.c"
	"*.cpp"
	"*.rc"
)
set(TESTNETWORK_SOURCES
	${TESTNETWORK_HEADER_FILES}
	${TESTNETWORK_SOURCE_FILES}
)
set(TESTNETWORK_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/TestNetwork/")

if(ACID_BUILD_RELEASE AND WIN32)
	add_executable(TestNetwork WIN32 ${TESTNETWORK_SOURCES})
else()
	add_executable(TestNetwork ${TESTNETWORK_SOURCES})
endif()

set_target_properties(TestNetwork PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	FOLDER "Acid"
)

add_dependencies(TestNetwork Acid)

target_include_directories(TestNetwork PUBLIC ${ACID_INCLUDE_DIR} ${TESTNETWORK_INCLUDE_DIR})
target_link_libraries(TestNetwork PUBLIC Acid)

if(UNIX AND APPLE)
	set_target_properties(TestNetwork PROPERTIES
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

	install(TARGETS TestNetwork
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
endif()