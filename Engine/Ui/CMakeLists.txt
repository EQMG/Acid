set(_component_name Ui)
set(_component_library Acid_${_component_name})
set(_component_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Fonts/FontsSubrender.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Fonts/FontType.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Fonts/Text.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Guis/Gui.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Guis/GuisSubrender.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Constraints/BestFitConstraint.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Constraints/PixelConstraint.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Constraints/RatioConstraint.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Constraints/RelativeConstraint.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Constraints/UiAnchor.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Constraints/UiConstraint.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Constraints/UiConstraints.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Drivers/BounceDriver.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Drivers/ConstantDriver.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Drivers/FadeDriver.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Drivers/LinearDriver.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Drivers/SinewaveDriver.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Drivers/SlideDriver.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Drivers/UiDriver.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/InputDelay.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiBooleanInput.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiButtonInput.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiDropdownInput.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiGrabberInput.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiRadioInput.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiSliderInput.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiTextInput.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/UiObject.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/UiPanel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Uis.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/UiScrollBar.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/UiSection.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/UiStartLogo.hpp
		)
set(_component_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Fonts/FontsSubrender.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Fonts/FontType.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Fonts/Text.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Guis/Gui.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Guis/GuisSubrender.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Constraints/UiAnchor.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Constraints/UiConstraints.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/InputDelay.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiBooleanInput.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiButtonInput.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiDropdownInput.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiGrabberInput.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiRadioInput.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiSliderInput.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Inputs/UiTextInput.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/UiObject.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/UiPanel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/Uis.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/UiScrollBar.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/UiSection.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/UiStartLogo.cpp
		)
set(_component_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp
		)
set(_component_third_party_headers
		${THIRD_PARTY_SOURCE_DIR}/tinymsdf/tiny_msdf.hpp
		)
set(_component_third_party_sources
		${THIRD_PARTY_SOURCE_DIR}/tinymsdf/tiny_msdf.cpp
		)

add_library(${_component_library}
        ${_component_headers}
        ${_component_sources}
		${_component_generated_headers}
        ${_component_third_party_headers}
        ${_component_third_party_sources}
        )
add_library(Acid::${_component_name} ALIAS ${_component_library})

include(GenerateExportHeader)
generate_export_header(${_component_library} EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp")

find_package(Freetype 2.11.1 QUIET)
if(NOT Freetype_FOUND)
	set(SKIP_INSTALL_ALL TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_BZip2 TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_PNG TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_BrotliDec TRUE)

	set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
	set(BUILD_SHARED_LIBS OFF)
	add_subdirectory(${THIRD_PARTY_SOURCE_DIR}/freetype freetype)
	set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")
	
	set_target_properties(freetype PROPERTIES FOLDER "External")
	
	set(FREETYPE_LIBRARIES "freetype")
endif()

target_compile_features(${_component_library} PUBLIC c_std_11 cxx_std_17)
target_include_directories(${_component_library}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/tinymsdf>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/tinymsdf>
		PRIVATE
		$<BUILD_INTERFACE:$<$<BOOL:${FREETYPE_INCLUDE_DIRS}>:${FREETYPE_INCLUDE_DIRS}>>
		)
target_link_libraries(${_component_library}
		PUBLIC
		Acid::Core
		Acid::Graphics
		PRIVATE
		${FREETYPE_LIBRARIES}
		)
set_target_properties(${_component_library}
		PROPERTIES
		FOLDER "Acid"
		EXPORT_NAME "${_component_name}"
		)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_component_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_component_generated_headers})
source_group(TREE "${THIRD_PARTY_SOURCE_DIR}" PREFIX "ThirdParty" FILES ${_component_third_party_headers})
source_group(TREE "${THIRD_PARTY_SOURCE_DIR}" PREFIX "ThirdParty" FILES ${_component_third_party_sources})

install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_SOURCE_DIR}" "${_component_headers}")
install_headers_with_dir("${_component_name}" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_component_generated_headers}")
install_headers_with_dir("third_party" "${THIRD_PARTY_SOURCE_DIR}" "${_component_third_party_headers}")

set(ACID_LIBRARIES "${ACID_LIBRARIES};${_component_library}" PARENT_SCOPE)
