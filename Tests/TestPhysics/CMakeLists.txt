file(GLOB_RECURSE TESTPHYSICS_HEADER_FILES
	"*.h"
	"*.hpp"
)
file(GLOB_RECURSE TESTPHYSICS_SOURCE_FILES
	"*.c"
	"*.cpp"
	"*.rc"
)
set(TESTPHYSICS_SOURCES
	${TESTPHYSICS_HEADER_FILES}
	${TESTPHYSICS_SOURCE_FILES}
)
set(TESTPHYSICS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/TestPhysics/")

if(ACID_BUILD_RELEASE AND WIN32)
	add_executable(TestPhysics WIN32 ${TESTPHYSICS_SOURCES})
else()
	add_executable(TestPhysics ${TESTPHYSICS_SOURCES})
endif()

set_target_properties(TestPhysics PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	FOLDER "Acid"
)

add_dependencies(TestPhysics Acid)

target_include_directories(TestPhysics PUBLIC ${ACID_INCLUDE_DIR} ${TESTPHYSICS_INCLUDE_DIR})
target_link_libraries(TestPhysics PUBLIC Acid)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestPhysics PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(TestPhysics PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "Test Physics"
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

	install(TARGETS TestPhysics
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
endif()