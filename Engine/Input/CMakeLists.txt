set(_acid_input_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Axes/ButtonInputAxis.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Axes/CompoundInputAxis.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Axes/JoystickHatInput.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Axes/JoystickInputAxis.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Axes/MouseInputAxis.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Buttons/AxisInputButton.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Buttons/CompoundInputButton.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Buttons/JoystickInputButton.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Buttons/KeyboardInputButton.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Buttons/MouseInputButton.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Inputs.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/InputAxis.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/InputButton.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/InputScheme.hpp
		)
set(_acid_input_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Axes/ButtonInputAxis.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Axes/CompoundInputAxis.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Axes/JoystickHatInput.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Axes/JoystickInputAxis.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Axes/MouseInputAxis.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Buttons/AxisInputButton.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Buttons/CompoundInputButton.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Buttons/JoystickInputButton.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Buttons/KeyboardInputButton.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Buttons/MouseInputButton.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/Inputs.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Inputs/InputScheme.cpp
		)
set(_acid_input_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/InputExport.hpp
		)

add_library(Acid.Input
        ${_acid_input_headers}
        ${_acid_input_sources}
		${_acid_input_generated_headers}
        )

include(GenerateExportHeader)
generate_export_header(Acid.Input EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/InputExport.hpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_input_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_input_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_acid_input_generated_headers})

set_target_properties(Acid.Input PROPERTIES FOLDER "Acid")
target_compile_features(Acid.Input PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Input
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Input>
		)
target_link_libraries(Acid.Input
		PUBLIC
		Acid.Core
		Acid.Context
		)

install(TARGETS Acid.Input
		EXPORT AcidInputExport
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
install(EXPORT AcidInputExport
		FILE AcidInputConfig.cmake
		#NAMESPACE acid::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
install_headers_with_dir("Input" "${CMAKE_CURRENT_SOURCE_DIR}" "${_acid_input_headers}")
install_headers_with_dir("Input" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_acid_input_generated_headers}")
