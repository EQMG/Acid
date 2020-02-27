file(GLOB_RECURSE TESTPACKER_HEADER_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.h" "*.hpp" "*.inl"
		)
file(GLOB_RECURSE TESTPACKER_SOURCE_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.c" "*.cpp" "*.rc"
		)

add_executable(TestPacker ${TESTPACKER_HEADER_FILES} ${TESTPACKER_SOURCE_FILES})

target_compile_features(TestPacker PUBLIC cxx_std_17)
target_include_directories(TestPacker PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(TestPacker PRIVATE Acid::Acid)

set_target_properties(TestPacker PROPERTIES
		FOLDER "Acid/Tests"
		)
if(UNIX AND APPLE)
	set_target_properties(TestPacker PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Packer"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/Info.plist.in"
			)
endif()

add_test(NAME "Packer" COMMAND "TestPacker")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestPacker
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()

include(AcidGroupSources)
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TESTPACKER_HEADER_FILES}")
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TESTPACKER_SOURCE_FILES}")
