file(GLOB_RECURSE TESTPACKER_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE TESTPACKER_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Header Files" FILES ${TESTPACKER_HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" FILES ${TESTPACKER_SOURCE_FILES})

add_executable(TestPacker ${TESTPACKER_HEADER_FILES} ${TESTPACKER_SOURCE_FILES})

target_compile_features(TestPacker PUBLIC cxx_std_17)
target_include_directories(TestPacker PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(TestPacker PRIVATE Acid::Acid)

set_target_properties(TestPacker PROPERTIES
		FOLDER "Acid"
		)
if(UNIX AND APPLE)
	set_target_properties(TestPacker PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Packer"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "Packer" COMMAND "TestPacker")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestPacker
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
