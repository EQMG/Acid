set(_example_executable Tutorial5)
set(_example_headers
		${CMAKE_CURRENT_SOURCE_DIR}/MainApp.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainRenderer.hpp
		)
set(_example_sources
		${CMAKE_CURRENT_SOURCE_DIR}/MainApp.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainRenderer.cpp
		)

add_executable(${_example_executable}
		${_example_headers}
		${_example_sources}
		)

target_compile_features(${_example_executable} PUBLIC cxx_std_20 c_std_11)
target_include_directories(${_example_executable}
		PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		)
target_link_libraries(${_example_executable}
		PRIVATE
		${ACID_LIBRARIES}
		)
set_target_properties(${_example_executable} PROPERTIES FOLDER "Acid/Tests")

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(${_example_executable} PROPERTIES 
			LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
			)
elseif(UNIX AND APPLE)
	set_target_properties(${_example_executable} PROPERTIES
			#RESOURCE Icon.icns
			MACOSX_BUNDLE_BUNDLE_NAME "${_example_executable}"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			#MACOSX_BUNDLE_ICON_FILE Icon.icns
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/Info.plist.in"
			)
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_example_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_example_sources})

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS ${_example_executable}
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
