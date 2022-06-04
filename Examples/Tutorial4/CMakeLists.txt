file(GLOB_RECURSE TUTORIAL4_HEADER_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.h" "*.hpp" "*.inl"
		)
file(GLOB_RECURSE TUTORIAL4_SOURCE_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.c" "*.cpp" "*.rc"
		)

add_executable(Tutorial4 ${TUTORIAL4_HEADER_FILES} ${TUTORIAL4_SOURCE_FILES})

# Changes how sources appear in IDE virtual file structures
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${TUTORIAL4_HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${TUTORIAL4_SOURCE_FILES})

target_compile_features(Tutorial4 PUBLIC cxx_std_17)
target_include_directories(Tutorial4 PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(Tutorial4 PRIVATE Acid::Acid)

set_target_properties(Tutorial4 PROPERTIES
		FOLDER "Acid/Tutorials"
		)
if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(Tutorial4 PROPERTIES 
			LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
			)
elseif(UNIX AND APPLE)
	set_target_properties(Tutorial4 PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Tutorial4"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/Info.plist.in"
			)
endif()

# add_test(NAME "Tutorial4" COMMAND "Tutorial4")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS Tutorial4
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
