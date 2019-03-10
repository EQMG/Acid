file(GLOB_RECURSE TESTSERIAL_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE TESTSERIAL_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
set(TESTSERIAL_SOURCES
		${TESTSERIAL_HEADER_FILES}
		${TESTSERIAL_SOURCE_FILES}
		)
set(TESTSERIAL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/TestSerial/")

add_executable(TestSerial ${TESTSERIAL_SOURCES})
add_dependencies(TestSerial Acid)

target_compile_features(TestSerial PUBLIC cxx_std_17)
set_target_properties(TestSerial PROPERTIES
		POSITION_INDEPENDENT_CODE ON
		FOLDER "Acid"
		)

target_include_directories(TestSerial PRIVATE ${ACID_INCLUDE_DIR} ${TESTSERIAL_INCLUDE_DIR})
target_link_libraries(TestSerial PRIVATE Acid)

if(UNIX AND APPLE)
	set_target_properties(TestSerial PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Serialization"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/Scripts/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "Serialization" COMMAND "TestSerial")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestSerial
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
