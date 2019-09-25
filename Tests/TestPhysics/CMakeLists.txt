file(GLOB_RECURSE TESTPHYSICS_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE TESTPHYSICS_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Header Files" FILES ${TESTPHYSICS_HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" FILES ${TESTPHYSICS_SOURCE_FILES})

add_executable(TestPhysics ${TESTPHYSICS_HEADER_FILES} ${TESTPHYSICS_SOURCE_FILES})

target_compile_features(TestPhysics PUBLIC cxx_std_17)
target_include_directories(TestPhysics PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(TestPhysics PRIVATE Acid::Acid)

set_target_properties(TestPhysics PROPERTIES
		FOLDER "Acid"
		)
if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestPhysics PROPERTIES 
			LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
			)
elseif(UNIX AND APPLE)
	set_target_properties(TestPhysics PROPERTIES
			#RESOURCE Icon.icns
			MACOSX_BUNDLE_BUNDLE_NAME "Test Physics"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			#MACOSX_BUNDLE_ICON_FILE Icon.icns
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
