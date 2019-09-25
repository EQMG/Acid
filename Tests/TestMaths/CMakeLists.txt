file(GLOB_RECURSE TESTMATHS_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE TESTMATHS_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Header Files" FILES ${TESTMATHS_HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" FILES ${TESTMATHS_SOURCE_FILES})

add_executable(TestMaths ${TESTMATHS_HEADER_FILES} ${TESTMATHS_SOURCE_FILES})

target_compile_features(TestMaths PUBLIC cxx_std_17)
target_include_directories(TestMaths PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(TestMaths PRIVATE Acid::Acid)

set_target_properties(TestMaths PROPERTIES
		FOLDER "Acid"
		)
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
