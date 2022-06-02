set(_acid_input_headers
		Inputs/Axes/ButtonInputAxis.hpp
		Inputs/Axes/CompoundInputAxis.hpp
		Inputs/Axes/JoystickHatInput.hpp
		Inputs/Axes/JoystickInputAxis.hpp
		Inputs/Axes/MouseInputAxis.hpp
		Inputs/Buttons/AxisInputButton.hpp
		Inputs/Buttons/CompoundInputButton.hpp
		Inputs/Buttons/JoystickInputButton.hpp
		Inputs/Buttons/KeyboardInputButton.hpp
		Inputs/Buttons/MouseInputButton.hpp
		Inputs/Inputs.hpp
		Inputs/InputAxis.hpp
		Inputs/InputButton.hpp
		Inputs/InputScheme.hpp
		)
set(_acid_input_sources
		Inputs/Axes/ButtonInputAxis.cpp
		Inputs/Axes/CompoundInputAxis.cpp
		Inputs/Axes/JoystickHatInput.cpp
		Inputs/Axes/JoystickInputAxis.cpp
		Inputs/Axes/MouseInputAxis.cpp
		Inputs/Buttons/AxisInputButton.cpp
		Inputs/Buttons/CompoundInputButton.cpp
		Inputs/Buttons/JoystickInputButton.cpp
		Inputs/Buttons/KeyboardInputButton.cpp
		Inputs/Buttons/MouseInputButton.cpp
		Inputs/Inputs.cpp
		Inputs/InputScheme.cpp
		)

set(_acid_input_third_party_headers
		)
set(_acid_input_third_party_sources
		)
set(_acid_input_third_party_includes
		)

add_library(Acid.Input
        ${_acid_input_headers}
        ${_acid_input_sources}
        ${_acid_input_third_party_headers}
        ${_acid_input_third_party_sources}
        )

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Input EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Input/Export.hpp")

target_compile_features(Acid.Input PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Input
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_input_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		)
target_link_libraries(Acid.Input
		PUBLIC
		Acid.Core
		Acid.Context
		)

set_target_properties(Acid.Input PROPERTIES
		#INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "Acid"
		)

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Input/Export.hpp"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Input"
		)
foreach(_header IN LISTS _acid_input_headers _acid_input_third_party_headers)
    get_filename_component(_header_dir ${_header} DIRECTORY)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_header}
			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Input/${_header_dir}"
			)
endforeach()
# Install without an export since we're using 1 (or more) non-system libs
install(TARGETS Acid.Input
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
