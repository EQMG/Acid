set(VWSI_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/vwsi/include")
set(VWSI_BUILD_TESTING OFF CACHE INTERNAL "Build the VWSI test programs")
set(VWSI_INSTALL ${FLOUNDER_INSTALL} CACHE INTERNAL "Generate installation target")
set(VWSI_SETUP_OUTPUT OFF CACHE INTERNAL "If VWSI will set it's own outputs")
set(VWSI_VULKAN_STATIC OFF CACHE INTERNAL "Use the Vulkan loader statically linked into application")
add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/vwsi)
set(VWSI_LIBRARY "VWSI")

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${VWSI_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} ${VWSI_LIBRARY})

if(NOT VWSI_LIBRARY)
	message(FATAL_ERROR "VWSI library not found!")
endif()

# Install
if (FLOUNDER_INSTALL)
	file(COPY "${VWSI_INCLUDES}/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/")
endif()
