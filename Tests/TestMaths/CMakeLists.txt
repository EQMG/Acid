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

add_executable(TestMaths ${TESTMATHS_SOURCES})
add_dependencies(TestMaths Acid)

target_compile_features(TestMaths PUBLIC cxx_std_17)
set_target_properties(TestMaths PROPERTIES
		POSITION_INDEPENDENT_CODE ON
		FOLDER "Acid"
		)

target_include_directories(TestMaths PRIVATE ${ACID_INCLUDE_DIR} ${TESTMATHS_INCLUDE_DIR})
target_link_libraries(TestMaths PRIVATE Acid)

if(UNIX AND APPLE)
	set_target_properties(TestMaths PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Maths"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "Maths" COMMAND "TestMaths")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestMaths
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
