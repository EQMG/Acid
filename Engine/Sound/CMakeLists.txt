set(_component_name Sound)
set(_component_library Acid.${_component_name})
set(_component_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Audio.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Flac/FlacSoundBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Mp3/Mp3SoundBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Ogg/OggSoundBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Opus/OpusSoundBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Sound.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/SoundBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Wave/WaveSoundBuffer.hpp
		)
set(_component_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Audio.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Flac/FlacSoundBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Mp3/Mp3SoundBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Ogg/OggSoundBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Opus/OpusSoundBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Sound.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/SoundBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Audio/Wave/WaveSoundBuffer.cpp
		)
set(_component_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp
		)
set(_component_third_party_headers
		${ACID_THIRD_PARTY_DIR}/dr_libs/dr_flac.h
		${ACID_THIRD_PARTY_DIR}/dr_libs/dr_mp3.h
		${ACID_THIRD_PARTY_DIR}/dr_libs/dr_opus.h
		${ACID_THIRD_PARTY_DIR}/dr_libs/dr_wav.h
		${ACID_THIRD_PARTY_DIR}/stb/stb_vorbis.h
		)
set(_component_third_party_sources
		${ACID_THIRD_PARTY_DIR}/dr_libs/dr_flac.c
		${ACID_THIRD_PARTY_DIR}/dr_libs/dr_mp3.c
		${ACID_THIRD_PARTY_DIR}/dr_libs/dr_opus.c
		${ACID_THIRD_PARTY_DIR}/dr_libs/dr_wav.c
		${ACID_THIRD_PARTY_DIR}/stb/stb_vorbis.c
		)

add_library(${_component_library}
        ${_component_headers}
        ${_component_sources}
		${_component_generated_headers}
        ${_component_third_party_headers}
        ${_component_third_party_sources}
        )

include(GenerateExportHeader)
generate_export_header(${_component_library} EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp")

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

set_target_properties(${_component_library} PROPERTIES FOLDER "Acid")
target_compile_features(${_component_library} PUBLIC c_std_11 cxx_std_17)
target_include_directories(${_component_library}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/dr_libs>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/stb>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/dr_libs>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/stb>
		)
target_link_libraries(${_component_library}
		PUBLIC
		Acid.Core
		Acid.Scene
        PRIVATE
		OpenAL::OpenAL
		)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_component_generated_headers})
source_group(TREE "${ACID_THIRD_PARTY_DIR}" PREFIX "ThirdParty" FILES ${_component_third_party_headers})
source_group(TREE "${ACID_THIRD_PARTY_DIR}" PREFIX "ThirdParty" FILES ${_component_third_party_sources})

install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_SOURCE_DIR}" "${_component_headers}")
install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_component_generated_headers}")
install_headers_with_dir("third_party" "${ACID_THIRD_PARTY_DIR}" "${_component_third_party_headers}")

set(ACID_INSTALL_TARGETS ${ACID_INSTALL_TARGETS} ";${_component_library}" PARENT_SCOPE)
