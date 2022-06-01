set(_acid_ui_headers
		Fonts/FontsSubrender.hpp
		Fonts/FontType.hpp
		Fonts/Text.hpp
		Guis/Gui.hpp
		Guis/GuisSubrender.hpp
		Uis/Constraints/BestFitConstraint.hpp
		Uis/Constraints/PixelConstraint.hpp
		Uis/Constraints/RatioConstraint.hpp
		Uis/Constraints/RelativeConstraint.hpp
		Uis/Constraints/UiAnchor.hpp
		Uis/Constraints/UiConstraint.hpp
		Uis/Constraints/UiConstraints.hpp
		Uis/Drivers/BounceDriver.hpp
		Uis/Drivers/ConstantDriver.hpp
		Uis/Drivers/FadeDriver.hpp
		Uis/Drivers/LinearDriver.hpp
		Uis/Drivers/SinewaveDriver.hpp
		Uis/Drivers/SlideDriver.hpp
		Uis/Drivers/UiDriver.hpp
		Uis/Inputs/InputDelay.hpp
		Uis/Inputs/UiBooleanInput.hpp
		Uis/Inputs/UiButtonInput.hpp
		Uis/Inputs/UiDropdownInput.hpp
		Uis/Inputs/UiGrabberInput.hpp
		Uis/Inputs/UiRadioInput.hpp
		Uis/Inputs/UiSliderInput.hpp
		Uis/Inputs/UiTextInput.hpp
		Uis/UiObject.hpp
		Uis/UiPanel.hpp
		Uis/Uis.hpp
		Uis/UiScrollBar.hpp
		Uis/UiSection.hpp
		Uis/UiStartLogo.hpp
		)
set(_acid_ui_sources
		Fonts/FontsSubrender.cpp
		Fonts/FontType.cpp
		Fonts/Text.cpp
		Guis/Gui.cpp
		Guis/GuisSubrender.cpp
		Uis/Constraints/UiAnchor.cpp
		Uis/Constraints/UiConstraints.cpp
		Uis/Inputs/InputDelay.cpp
		Uis/Inputs/UiBooleanInput.cpp
		Uis/Inputs/UiButtonInput.cpp
		Uis/Inputs/UiDropdownInput.cpp
		Uis/Inputs/UiGrabberInput.cpp
		Uis/Inputs/UiRadioInput.cpp
		Uis/Inputs/UiSliderInput.cpp
		Uis/Inputs/UiTextInput.cpp
		Uis/UiObject.cpp
		Uis/UiPanel.cpp
		Uis/Uis.cpp
		Uis/UiScrollBar.cpp
		Uis/UiSection.cpp
		Uis/UiStartLogo.cpp
		)

set(_acid_ui_third_party_headers
		../../External/tinymsdf/tiny_msdf.hpp
		)
set(_acid_ui_third_party_sources
		../../External/tinymsdf/tiny_msdf.cpp
		)
set(_acid_ui_third_party_includes
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/tinymsdf
		)

add_library(Acid.Ui
        ${_acid_ui_headers}
        ${_acid_ui_sources}
        ${_acid_ui_third_party_headers}
        ${_acid_ui_third_party_sources}
        )

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Ui EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Ui/Export.hpp")

find_package(Freetype 2.11.1 QUIET)
if(NOT Freetype_FOUND)
	set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_BZip2 TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_PNG TRUE)
	set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE)
	add_subdirectory(../../External/freetype freetype)
	
	set_target_properties(freetype PROPERTIES FOLDER "External")
	
	set(FREETYPE_LIBRARIES "freetype")
endif()

target_compile_features(Acid.Ui PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Ui
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_ui_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		PRIVATE
		# Since building locally from the submodules won't always create these vars.
		# We have to do a simple check if they exist or they will cause errors
		$<$<BOOL:${FREETYPE_INCLUDE_DIRS}>:${FREETYPE_INCLUDE_DIRS}>
		)
target_link_libraries(Acid.Ui
		PUBLIC
		Acid.Core
		Acid.Graphics
		PRIVATE
		# More IMPORTED
		${FREETYPE_LIBRARIES}
		)
