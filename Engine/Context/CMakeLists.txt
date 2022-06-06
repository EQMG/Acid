set(_component_name Context)
set(_component_library Acid_${_component_name})
set(_component_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Bitmap.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Dng/DngBitmap.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Exr/ExrBitmap.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Jpg/JpgBitmap.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Png/PngBitmap.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Devices/Cursor.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Devices/Joysticks.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Devices/Monitor.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Devices/Window.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Devices/Windows.hpp
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
		${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Bitmap.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Dng/DngBitmap.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Exr/ExrBitmap.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Jpg/JpgBitmap.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Bitmaps/Png/PngBitmap.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Devices/Cursor.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Devices/Joysticks.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Devices/Monitor.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Devices/Window.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Devices/Windows.cpp
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
set(_component_third_party_headers
		${THIRD_PARTY_SOURCE_DIR}/volk/volk.h
		${THIRD_PARTY_SOURCE_DIR}/libjpgd/jpgd.h
		${THIRD_PARTY_SOURCE_DIR}/libjpgd/jpgd_idct.h
		${THIRD_PARTY_SOURCE_DIR}/libspng/spng.h
		${THIRD_PARTY_SOURCE_DIR}/miniz/miniz.h
		${THIRD_PARTY_SOURCE_DIR}/stb/stb_image.h
		${THIRD_PARTY_SOURCE_DIR}/stb/stb_image_write.h
		${THIRD_PARTY_SOURCE_DIR}/tinydng/tiny_dng.h
		${THIRD_PARTY_SOURCE_DIR}/tinyexr/tiny_exr.h
		)
set(_component_third_party_sources
		${THIRD_PARTY_SOURCE_DIR}/volk/volk.c
		${THIRD_PARTY_SOURCE_DIR}/libjpgd/jpgd.cpp
		${THIRD_PARTY_SOURCE_DIR}/libspng/spng.c
		${THIRD_PARTY_SOURCE_DIR}/miniz/miniz.c
		${THIRD_PARTY_SOURCE_DIR}/stb/stb_image.c
		${THIRD_PARTY_SOURCE_DIR}/stb/stb_image_write.c
		${THIRD_PARTY_SOURCE_DIR}/tinydng/tiny_dng.cpp
		${THIRD_PARTY_SOURCE_DIR}/tinyexr/tiny_exr.cpp
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

# Finds Vulkan, env "VULKAN_SDK" must be set
find_package(Vulkan REQUIRED)

find_package(glfw3 3.3.6 QUIET)
if(NOT TARGET glfw)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
	set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
	set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW")

	add_subdirectory(${THIRD_PARTY_SOURCE_DIR}/glfw glfw)

	set_target_properties(glfw PROPERTIES FOLDER "External/glfw")
	set_target_properties(update_mappings PROPERTIES FOLDER "External/glfw")
endif()

target_compile_features(${_component_library} PUBLIC c_std_11 cxx_std_17)
target_include_directories(${_component_library}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/volk>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/libjpgd>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/libspng>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/miniz>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/stb>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/tinydng>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/tinyexr>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_component_name}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/volk>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/libjpgd>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/libspng>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/miniz>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/stb>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/tinydng>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/third_party/tinyexr>
		)
target_link_libraries(${_component_library}
		PUBLIC
		Acid::Core
		Vulkan::Vulkan
		PRIVATE
		glfw
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
