file(GLOB_RECURSE TESTPBR_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE TESTPBR_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Header Files" FILES ${TESTPBR_HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" FILES ${TESTPBR_SOURCE_FILES})

add_executable(TestPBR ${TESTPBR_HEADER_FILES} ${TESTPBR_SOURCE_FILES})

target_compile_features(TestPBR PUBLIC cxx_std_17)
target_include_directories(TestPBR PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(TestPBR PRIVATE Acid::Acid)

set_target_properties(TestPBR PROPERTIES
		FOLDER "Acid"
		)
if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestPBR PROPERTIES 
			LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
			)
elseif(UNIX AND APPLE)
	set_target_properties(TestPBR PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test PBR"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "PBR" COMMAND "TestPBR")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestPBR
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
