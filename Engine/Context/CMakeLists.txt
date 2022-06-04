set(_acid_context_headers
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
set(_acid_context_sources
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
set(_acid_context_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/ContextExport.hpp
		)
set(_acid_context_third_party_headers
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
set(_acid_context_third_party_sources
		${ACID_THIRD_PARTY_DIR}/volk/volk.c
		${ACID_THIRD_PARTY_DIR}/libjpgd/jpgd.cpp
		${ACID_THIRD_PARTY_DIR}/libspng/spng.c
		${ACID_THIRD_PARTY_DIR}/miniz/miniz.c
		${ACID_THIRD_PARTY_DIR}/stb/stb_image.c
		${ACID_THIRD_PARTY_DIR}/stb/stb_image_write.c
		${ACID_THIRD_PARTY_DIR}/tinydng/tiny_dng.cpp
		${ACID_THIRD_PARTY_DIR}/tinyexr/tiny_exr.cpp
		)

add_library(Acid.Context
        ${_acid_context_headers}
        ${_acid_context_sources}
		${_acid_context_generated_headers}
        ${_acid_context_third_party_headers}
        ${_acid_context_third_party_sources}
        )

include(GenerateExportHeader)
generate_export_header(Acid.Context EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/ContextExport.hpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_context_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_context_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_acid_context_generated_headers})
source_group(TREE "${ACID_THIRD_PARTY_DIR}" PREFIX "ThirdParty" FILES ${_acid_context_third_party_headers})
source_group(TREE "${ACID_THIRD_PARTY_DIR}" PREFIX "ThirdParty" FILES ${_acid_context_third_party_sources})

# Finds Vulkan, env "VULKAN_SDK" must be set
find_package(Vulkan REQUIRED)

#find_package(glfw3 3.3.6 QUIET)
if(NOT TARGET glfw)
	set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
	set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
	set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
	set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW")
	add_subdirectory(${ACID_THIRD_PARTY_DIR}/glfw glfw)
	set_target_properties(glfw PROPERTIES FOLDER "External/glfw")
	set_target_properties(update_mappings PROPERTIES FOLDER "External/glfw")
endif()

set_target_properties(Acid.Context PROPERTIES FOLDER "Acid")
target_compile_features(Acid.Context PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Context
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
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Context>
		)
target_link_libraries(Acid.Context
		PUBLIC
		Acid.Core
		Vulkan::Vulkan
		PRIVATE
		glfw
		)

install(TARGETS Acid.Context
		EXPORT AcidContextExport
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
install(EXPORT AcidContextExport
		FILE AcidContextConfig.cmake
		#NAMESPACE acid::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
install_headers_with_dir("Context" "${CMAKE_CURRENT_SOURCE_DIR}" "${_acid_context_headers}")
install_headers_with_dir("Context" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_acid_context_generated_headers}")
install_headers_with_dir("third_party" "${ACID_THIRD_PARTY_DIR}" "${_acid_context_third_party_headers}")
