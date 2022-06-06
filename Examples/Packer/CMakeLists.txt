set(_packer_headers
		)
set(_packer_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp
		)
set(_packer_third_party_headers
		${THIRD_PARTY_SOURCE_DIR}/Zippy/ZipArchive.hpp
		${THIRD_PARTY_SOURCE_DIR}/Zippy/ZipEntry.hpp
		${THIRD_PARTY_SOURCE_DIR}/Zippy/ZipException.hpp
		${THIRD_PARTY_SOURCE_DIR}/Zippy/Zippy.hpp
		${THIRD_PARTY_SOURCE_DIR}/Zippy/ZipUtilities.hpp
		)

add_executable(TestPacker
		${_packer_headers}
		${_packer_sources}
		${_packer_third_party_headers}
		)

target_compile_features(TestPacker PUBLIC c_std_11 cxx_std_17)
target_include_directories(TestPacker
		PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/Zippy>
		)
target_link_libraries(TestPacker
		PRIVATE
		${ACID_LIBRARIES}
		)
set_target_properties(TestPacker PROPERTIES FOLDER "Acid/Tests")

if(UNIX AND APPLE)
	set_target_properties(TestPacker PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Test Packer"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/Info.plist.in"
			)
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_packer_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_packer_sources})
source_group(TREE "${THIRD_PARTY_SOURCE_DIR}" PREFIX "ThirdParty" FILES ${_packer_third_party_headers})

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS TestPacker
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
