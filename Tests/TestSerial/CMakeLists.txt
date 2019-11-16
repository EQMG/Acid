file(GLOB_RECURSE TESTSERIAL_HEADER_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.h" "*.hpp" "*.inl"
		)
file(GLOB_RECURSE TESTSERIAL_SOURCE_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.c" "*.cpp" "*.rc"
		)

add_executable(TestSerial ${TESTSERIAL_HEADER_FILES} ${TESTSERIAL_SOURCE_FILES})

target_compile_features(TestSerial PUBLIC cxx_std_17)
target_include_directories(TestSerial PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(TestSerial PRIVATE Acid::Acid)

set_target_properties(TestSerial PROPERTIES
		FOLDER "Acid"
		)
if(UNIX AND APPLE)
	set_target_properties(TestSerial PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Serial"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "Serial" COMMAND "TestSerial")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestSerial
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()

include(AcidGroupSources)
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TESTSERIAL_HEADER_FILES}")
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TESTSERIAL_SOURCE_FILES}")
