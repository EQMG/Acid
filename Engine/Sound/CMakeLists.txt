set(_component_name Sound)
set(_component_library Acid_${_component_name})
set(_component_sources
		${CMAKE_CURRENT_SOURCE_DIR}/AudioModule.ixx
		${CMAKE_CURRENT_SOURCE_DIR}/SoundBuffer.ixx
		${CMAKE_CURRENT_SOURCE_DIR}/FlacSoundBuffer.ixx
		${CMAKE_CURRENT_SOURCE_DIR}/Mp3SoundBuffer.ixx
		${CMAKE_CURRENT_SOURCE_DIR}/OggSoundBuffer.ixx
		${CMAKE_CURRENT_SOURCE_DIR}/OpusSoundBuffer.ixx
		${CMAKE_CURRENT_SOURCE_DIR}/WaveSoundBuffer.ixx
		${CMAKE_CURRENT_SOURCE_DIR}/SoundComponent.ixx
		)
set(_component_third_party_headers
		${THIRD_PARTY_SOURCE_DIR}/dr_libs/dr_flac.h
		${THIRD_PARTY_SOURCE_DIR}/dr_libs/dr_mp3.h
		${THIRD_PARTY_SOURCE_DIR}/dr_libs/dr_opus.h
		${THIRD_PARTY_SOURCE_DIR}/dr_libs/dr_wav.h
		${THIRD_PARTY_SOURCE_DIR}/stb/stb_vorbis.h
		)
set(_component_third_party_sources
		${THIRD_PARTY_SOURCE_DIR}/dr_libs/dr_flac.c
		${THIRD_PARTY_SOURCE_DIR}/dr_libs/dr_mp3.c
		${THIRD_PARTY_SOURCE_DIR}/dr_libs/dr_opus.c
		${THIRD_PARTY_SOURCE_DIR}/dr_libs/dr_wav.c
		${THIRD_PARTY_SOURCE_DIR}/stb/stb_vorbis.c
		)

add_library(${_component_library} STATIC
        ${_component_sources}
        ${_component_third_party_headers}
        ${_component_third_party_sources}
        )
#target_sources(${_component_library}
#		PUBLIC
#			FILE_SET modules TYPE CXX_MODULES
#			BASE_DIRS
#				"${CMAKE_CURRENT_SOURCE_DIR}"
#			FILES
#				${_component_sources}
#		)
add_library(Acid::${_component_name} ALIAS ${_component_library})

# OpenAL must be installed on the system, env "OPENALDIR" must be set
find_package(OpenALSoft)
find_package(OpenAL)
if(OPENALSOFT_FOUND)
	#if(OPENALSOFT_FOUND AND NOT TARGET OpenAL::OpenAL)
		add_library(OpenAL::OpenAL UNKNOWN IMPORTED)
		set_target_properties(OpenAL::OpenAL PROPERTIES
				IMPORTED_LOCATION "${OPENALSOFT_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${OPENALSOFT_INCLUDE_DIR}"
				)
	#endif()
elseif(OPENAL_FOUND)
	#if(OPENAL_FOUND AND NOT TARGET OpenAL::OpenAL)
		add_library(OpenAL::OpenAL UNKNOWN IMPORTED)
		set_target_properties(OpenAL::OpenAL PROPERTIES
				IMPORTED_LOCATION "${OPENAL_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${OPENAL_INCLUDE_DIR}"
				)
	#endif()
else()
	message(FATAL_ERROR "Could not find OpenAL or OpenAL-Soft")
endif()

target_compile_features(${_component_library} PUBLIC cxx_std_20 c_std_11)
target_include_directories(${_component_library}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/dr_libs>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/stb>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/dr_libs>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/stb>
		)
target_link_libraries(${_component_library}
		PUBLIC
		Acid::Scene
        PRIVATE
		OpenAL::OpenAL
		)
set_target_properties(${_component_library}
		PROPERTIES
		FOLDER "Acid"
		EXPORT_NAME "${_component_name}"
		)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_sources})
source_group(TREE "${THIRD_PARTY_SOURCE_DIR}" PREFIX "ThirdParty" FILES ${_component_third_party_headers})
source_group(TREE "${THIRD_PARTY_SOURCE_DIR}" PREFIX "ThirdParty" FILES ${_component_third_party_sources})

install_headers_with_dir("third_party" "${THIRD_PARTY_SOURCE_DIR}" "${_component_third_party_headers}")

set(ACID_LIBRARIES "${ACID_LIBRARIES};${_component_library}" CACHE INTERNAL "")
