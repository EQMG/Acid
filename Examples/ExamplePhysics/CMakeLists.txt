file(GLOB_RECURSE _example_headers
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.h" "*.hpp" "*.inl"
		)
file(GLOB_RECURSE _example_sources
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.c" "*.cpp" "*.rc"
		)
set(_example_third_party_headers
		${THIRD_PARTY_SOURCE_DIR}/FastNoiseLite/FastNoiseLite.h
		)

add_executable(TestPhysics
		${_example_headers}
		${_example_sources}
		${_example_third_party_headers}
		)

target_compile_features(TestPhysics PUBLIC c_std_11 cxx_std_17)
target_include_directories(TestPhysics
		PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/FastNoiseLite>
		)
target_link_libraries(TestPhysics
		PRIVATE
		${ACID_LIBRARIES}
		)
set_target_properties(TestPhysics PROPERTIES FOLDER "Acid/Tests")

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
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/Info.plist.in"
			)
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_example_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_example_sources})

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestPhysics
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
