set(_component_name Context)
set(_component_library Acid.${_component_name})
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
		)
set(_component_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/${_component_name}Export.hpp
		)
set(_component_third_party_headers
		${ACID_THIRD_PARTY_DIR}/volk/volk.h
		${ACID_THIRD_PARTY_DIR}/libjpgd/jpgd.h
		${ACID_THIRD_PARTY_DIR}/libjpgd/jpgd_idct.h
		${ACID_THIRD_PARTY_DIR}/libspng/spng.h
		${ACID_THIRD_PARTY_DIR}/miniz/miniz.h
		${ACID_THIRD_PARTY_DIR}/stb/stb_image.h
		${ACID_THIRD_PARTY_DIR}/stb/stb_image_write.h
		${ACID_THIRD_PARTY_DIR}/tinydng/tiny_dng.h
		${ACID_THIRD_PARTY_DIR}/tinyexr/tiny_exr.h
		)
set(_component_third_party_sources
		${ACID_THIRD_PARTY_DIR}/volk/volk.c
		${ACID_THIRD_PARTY_DIR}/libjpgd/jpgd.cpp
		${ACID_THIRD_PARTY_DIR}/libspng/spng.c
		${ACID_THIRD_PARTY_DIR}/miniz/miniz.c
		${ACID_THIRD_PARTY_DIR}/stb/stb_image.c
		${ACID_THIRD_PARTY_DIR}/stb/stb_image_write.c
		${ACID_THIRD_PARTY_DIR}/tinydng/tiny_dng.cpp
		${ACID_THIRD_PARTY_DIR}/tinyexr/tiny_exr.cpp
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

# Finds Vulkan, env "VULKAN_SDK" must be set
find_package(Vulkan REQUIRED)

find_package(glfw3 3.3.6 QUIET)
if(NOT TARGET glfw)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
	set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
	set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW")

	add_subdirectory(${ACID_THIRD_PARTY_DIR}/glfw glfw)

	set_target_properties(glfw PROPERTIES FOLDER "External/glfw")
	set_target_properties(update_mappings PROPERTIES FOLDER "External/glfw")
endif()

set_target_properties(${_component_library} PROPERTIES FOLDER "Acid")
target_compile_features(${_component_library} PUBLIC c_std_11 cxx_std_17)
target_include_directories(${_component_library}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/volk>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/libjpgd>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/libspng>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/miniz>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/stb>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/tinydng>
		$<BUILD_INTERFACE:${ACID_THIRD_PARTY_DIR}/tinyexr>
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
		Acid.Core
		Vulkan::Vulkan
		PRIVATE
		glfw
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
