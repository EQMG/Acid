set(_component_name Input)
set(_component_library Acid.${_component_name})
set(_component_headers
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
set(_component_sources
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
set(_component_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp
		)

add_library(${_component_library}
        ${_component_headers}
        ${_component_sources}
		${_component_generated_headers}
        )

include(GenerateExportHeader)
generate_export_header(${_component_library} EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp")

set_target_properties(${_component_library} PROPERTIES FOLDER "Acid")
target_compile_features(${_component_library} PUBLIC c_std_11 cxx_std_17)
target_include_directories(${_component_library}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}>
		)
target_link_libraries(${_component_library}
		PUBLIC
		Acid.Core
		Acid.Context
		)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_component_generated_headers})

install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_SOURCE_DIR}" "${_component_headers}")
install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_component_generated_headers}")

set(ACID_INSTALL_TARGETS ${ACID_INSTALL_TARGETS} ";${_component_library}" PARENT_SCOPE)
