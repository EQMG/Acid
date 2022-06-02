set(_acid_context_headers
		Bitmaps/Bitmap.hpp
		Bitmaps/Dng/DngBitmap.hpp
		Bitmaps/Exr/ExrBitmap.hpp
		Bitmaps/Jpg/JpgBitmap.hpp
		Bitmaps/Png/PngBitmap.hpp
		Devices/Cursor.hpp
		Devices/Joysticks.hpp
		Devices/Monitor.hpp
		Devices/Window.hpp
		Devices/Windows.hpp
		)
set(_acid_context_sources
		Bitmaps/Bitmap.cpp
		Bitmaps/Dng/DngBitmap.cpp
		Bitmaps/Exr/ExrBitmap.cpp
		Bitmaps/Jpg/JpgBitmap.cpp
		Bitmaps/Png/PngBitmap.cpp
		Devices/Cursor.cpp
		Devices/Joysticks.cpp
		Devices/Monitor.cpp
		Devices/Window.cpp
		Devices/Windows.cpp
		)

set(_acid_context_third_party_headers
		../../External/libjpgd/jpgd.h
		../../External/libjpgd/jpgd_idct.h
		../../External/libspng/spng.h
		../../External/miniz/miniz.h
		../../External/stb/stb_image.h
		../../External/stb/stb_image_write.h
		../../External/tinydng/tiny_dng.h
		../../External/tinyexr/tiny_exr.h
		)
set(_acid_context_third_party_sources
		../../External/libjpgd/jpgd.cpp
		../../External/libspng/spng.c
		../../External/miniz/miniz.c
		../../External/stb/stb_image.c
		../../External/stb/stb_image_write.c
		../../External/tinydng/tiny_dng.cpp
		../../External/tinyexr/tiny_exr.cpp
		)
set(_acid_context_third_party_includes
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/libjpgd
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/libspng
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/miniz
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/stb
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/tinydng
		${CMAKE_CURRENT_SOURCE_DIR}/../../External/tinyexr
		)

add_library(Acid.Context
        ${_acid_context_headers}
        ${_acid_context_sources}
        ${_acid_context_third_party_headers}
        ${_acid_context_third_party_sources}
        )

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Context EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Context/Export.hpp")

# Finds Vulkan, env "VULKAN_SDK" must be set
#find_package(volk 121 QUIET)
#if(NOT volk_FOUND)
	add_subdirectory(../../External/volk volk)
	set_target_properties(volk PROPERTIES FOLDER "External")
#endif()

find_package(glfw3 3.3.6 QUIET)
if(NOT TARGET glfw)
	foreach(_glfw3_option "GLFW_BUILD_TESTS" "GLFW_BUILD_EXAMPLES" "GLFW_BUILD_DOCS" "GLFW_INSTALL")
		set(${_glfw3_option} OFF CACHE INTERNAL "")
	endforeach()
	add_subdirectory(../../External/glfw glfw)
	set_target_properties(glfw PROPERTIES FOLDER "External")
endif()

target_compile_features(Acid.Context PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Context
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_context_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		)
target_link_libraries(Acid.Context
		PUBLIC
		Acid.Core
		volk::volk
		PRIVATE
		# More IMPORTED
		glfw
		)

set_target_properties(Acid.Context PROPERTIES
		#INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
		FOLDER "Acid"
		)

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Context/Export.hpp"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Context"
		)
foreach(_header IN LISTS _acid_context_headers _acid_context_third_party_headers)
    get_filename_component(_header_dir ${_header} DIRECTORY)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_header}
			DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Context/${_header_dir}"
			)
endforeach()
# Install without an export since we're using 1 (or more) non-system libs
install(TARGETS Acid.Context
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
