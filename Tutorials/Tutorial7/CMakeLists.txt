file(GLOB_RECURSE TUTORIAL7_HEADER_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.h" "*.hpp" "*.inl"
		)
file(GLOB_RECURSE TUTORIAL7_SOURCE_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.c" "*.cpp" "*.rc"
		)

add_executable(TestTutorial7 ${TUTORIAL7_HEADER_FILES} ${TUTORIAL7_SOURCE_FILES})

target_compile_features(TestTutorial7 PUBLIC cxx_std_17)
target_include_directories(TestTutorial7 PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(TestTutorial7 PRIVATE Acid::Acid)

set_target_properties(TestTutorial7 PROPERTIES
		FOLDER "Acid"
		)
if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestTutorial7 PROPERTIES 
			LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
			)
elseif(UNIX AND APPLE)
	set_target_properties(TestTutorial7 PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Tutorial7"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/Info.plist.in"
			)
endif()

add_test(NAME "Tutorial7" COMMAND "TestTutorial7")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestTutorial7
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()

include(AcidGroupSources)
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TUTORIAL7_HEADER_FILES}")
acid_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${TUTORIAL7_SOURCE_FILES}")
