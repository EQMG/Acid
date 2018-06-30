set(WSI_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/vulkanwsi/include")
set(WSI_BUILD_TESTING OFF CACHE INTERNAL "Build the WSI test programs")
set(WSI_INSTALL ${FLOUNDER_INSTALL} CACHE INTERNAL "Generate installation target")
set(WSI_SETUP_OUTPUT OFF CACHE INTERNAL "If WSI will set it's own outputs")
set(WSI_VULKAN_STATIC OFF CACHE INTERNAL "Use the Vulkan loader statically linked into application")
add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/vulkanwsi)
set(WSI_LIBRARY "vulkanwsi")

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${WSI_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} ${WSI_LIBRARY})

if(NOT WSI_LIBRARY)
	message(FATAL_ERROR "WSI library not found!")
endif()

# Install
if (FLOUNDER_INSTALL)
	file(COPY "${WSI_INCLUDES}/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/")
endif()
