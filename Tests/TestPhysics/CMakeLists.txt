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

add_executable(TestPhysics ${TESTPHYSICS_SOURCES})
add_dependencies(TestPhysics Acid)

target_compile_features(TestPhysics PUBLIC cxx_std_17)
set_target_properties(TestPhysics PROPERTIES
		POSITION_INDEPENDENT_CODE ON
		FOLDER "Acid"
		)

target_include_directories(TestPhysics PRIVATE ${ACID_INCLUDE_DIR} ${TESTPHYSICS_INCLUDE_DIR})
target_link_libraries(TestPhysics PRIVATE Acid)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestPhysics PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(TestPhysics PROPERTIES
			#			RESOURCE Icon.icns
			MACOSX_BUNDLE_BUNDLE_NAME "Test Physics"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			#			MACOSX_BUNDLE_ICON_FILE Icon.icns
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "Physics" COMMAND "TestPhysics")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestPhysics
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
