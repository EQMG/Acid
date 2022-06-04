file(GLOB_RECURSE TUTORIAL1_HEADER_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.h" "*.hpp" "*.inl"
		)
file(GLOB_RECURSE TUTORIAL1_SOURCE_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.c" "*.cpp" "*.rc"
		)

add_executable(Tutorial1 ${TUTORIAL1_HEADER_FILES} ${TUTORIAL1_SOURCE_FILES})

# Changes how sources appear in IDE virtual file structures
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${TUTORIAL1_HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${TUTORIAL1_SOURCE_FILES})

target_compile_features(Tutorial1 PUBLIC cxx_std_17)
target_include_directories(Tutorial1 PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(Tutorial1 PRIVATE Acid::Acid)

set_target_properties(Tutorial1 PROPERTIES
		FOLDER "Acid/Tutorials"
		)
if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(Tutorial1 PROPERTIES 
			LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
			)
elseif(UNIX AND APPLE)
	set_target_properties(Tutorial1 PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Tutorial1"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/Info.plist.in"
			)
endif()

# add_test(NAME "Tutorial1" COMMAND "Tutorial1")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS Tutorial1
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
