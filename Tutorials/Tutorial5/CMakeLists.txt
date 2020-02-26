file(GLOB_RECURSE TUTORIAL5_HEADER_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.h" "*.hpp" "*.inl"
		)
file(GLOB_RECURSE TUTORIAL5_SOURCE_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.c" "*.cpp" "*.rc"
		)

add_executable(TestTutorial5 ${TUTORIAL5_HEADER_FILES} ${TUTORIAL5_SOURCE_FILES})

target_compile_features(TestTutorial5 PUBLIC cxx_std_17)
target_include_directories(TestTutorial5 PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(TestTutorial5 PRIVATE Acid::Acid)

set_target_properties(TestTutorial5 PROPERTIES
		FOLDER "Acid"
		)
if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestTutorial5 PROPERTIES 
			LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
			)
elseif(UNIX AND APPLE)
	set_target_properties(TestTutorial5 PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Tutorial5"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/Info.plist.in"
			)
endif()

add_test(NAME "Tutorial5" COMMAND "TestTutorial5")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestTutorial5
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()

include(AcidGroupSources)
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TUTORIAL5_HEADER_FILES}")
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TUTORIAL5_SOURCE_FILES}")

