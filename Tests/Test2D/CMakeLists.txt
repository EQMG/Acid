file(GLOB_RECURSE TEST2D_HEADER_FILES
	"*.h"
	"*.hpp"
)
file(GLOB_RECURSE TEST2D_SOURCE_FILES
	"*.c"
	"*.cpp"
	"*.rc"
)
set(TEST2D_SOURCES
	${TEST2D_HEADER_FILES}
	${TEST2D_SOURCE_FILES}
)
set(TEST2D_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/Test2D/")

if(ACID_BUILD_RELEASE AND WIN32)
	add_executable(Test2D WIN32 ${TEST2D_SOURCES})
else()
	add_executable(Test2D ${TEST2D_SOURCES})
endif()

set_target_properties(Test2D PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	FOLDER "Acid"
)

add_dependencies(Test2D Acid)

target_include_directories(Test2D PUBLIC ${ACID_INCLUDE_DIR} ${TEST2D_INCLUDE_DIR})
target_link_libraries(Test2D PUBLIC Acid)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(Test2D PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(Test2D PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "Test 2D"
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

	install(TARGETS Test2D
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
endif()