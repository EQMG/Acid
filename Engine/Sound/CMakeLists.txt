set(_acid_sound_headers
		Audio/Audio.hpp
		Audio/Flac/FlacSoundBuffer.hpp
		Audio/Mp3/Mp3SoundBuffer.hpp
		Audio/Ogg/OggSoundBuffer.hpp
		Audio/Opus/OpusSoundBuffer.hpp
		Audio/Sound.hpp
		Audio/SoundBuffer.hpp
		Audio/Wave/WaveSoundBuffer.hpp
		)
set(_acid_sound_sources
		Audio/Audio.cpp
		Audio/Flac/FlacSoundBuffer.cpp
		Audio/Mp3/Mp3SoundBuffer.cpp
		Audio/Ogg/OggSoundBuffer.cpp
		Audio/Opus/OpusSoundBuffer.cpp
		Audio/Sound.cpp
		Audio/SoundBuffer.cpp
		Audio/Wave/WaveSoundBuffer.cpp
		)

set(_acid_sound_third_party_headers
		../../External/dr_libs/dr_flac.h
		../../External/dr_libs/dr_mp3.h
		../../External/dr_libs/dr_opus.h
		../../External/dr_libs/dr_wav.h
		../../External/stb/stb_vorbis.h
		)
set(_acid_sound_third_party_sources
		../../External/dr_libs/dr_flac.c
		../../External/dr_libs/dr_mp3.c
		../../External/dr_libs/dr_opus.c
		../../External/dr_libs/dr_wav.c
		../../External/stb/stb_vorbis.c
		)
set(_acid_sound_third_party_includes
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/dr_libs
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/stb
		)

add_library(Acid.Sound
        ${_acid_sound_headers}
        ${_acid_sound_sources}
        ${_acid_sound_third_party_headers}
        ${_acid_sound_third_party_sources}
        )

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Sound EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Sound/Export.hpp")

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

target_compile_features(Acid.Sound PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Sound
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_sound_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		)
target_link_libraries(Acid.Sound
		PUBLIC
		Acid.Core
		Acid.Scene
        PRIVATE
		OpenAL::OpenAL
		)

set_target_properties(Acid.Sound PROPERTIES
		#INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "Acid"
		)

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Sound/Export.hpp"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Sound"
		)
foreach(_header IN LISTS _acid_sound_headers _acid_sound_third_party_headers)
    get_filename_component(_header_dir ${_header} DIRECTORY)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_header}
			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Sound/${_header_dir}"
			)
endforeach()
# Install without an export since we're using 1 (or more) non-system libs
install(TARGETS Acid.Sound
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
