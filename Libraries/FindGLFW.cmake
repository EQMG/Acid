set(GLFW_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/glfw/include")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL ${FLOUNDER_INSTALL} CACHE INTERNAL "Generate installation target")
set(GLFW_VULKAN_STATIC OFF CACHE INTERNAL "Use the Vulkan loader statically linked into application")
add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/glfw)
set(GLFW_LIBRARY "glfw")

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${GLFW_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} ${GLFW_LIBRARY})

if(NOT GLFW_LIBRARY)
	message(FATAL_ERROR "GLFW library not found!")
endif()

# Install
if (STB_INSTALL)
	if (FLOUNDER_INSTALL)
		file(COPY "${GLFW_INCLUDES}/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/")
	endif()
endif()
