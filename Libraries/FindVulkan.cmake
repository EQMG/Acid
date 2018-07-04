if(APPLE)
	find_path(METAL_INCLUDES Metal.h)
	find_library(METAL_LIBRARY NAMES Metal)

	if(NOT METAL_LIBRARY)
		message(FATAL_ERROR "Metal library not found!")
	endif()

	find_path(IOSURFACE_INCLUDES IOSurface.h)
	find_library(IOSURFACE_LIBRARY NAMES IOSurface)

	if(NOT IOSURFACE_LIBRARY)
		message(FATAL_ERROR "IOSurface library not found!")
	endif()

	find_path(QUARTZCORE_INCLUDES QuartzCore.h)
	find_library(QUARTZCORE_LIBRARY NAMES QuartzCore)

	if(NOT QUARTZCORE_LIBRARY)
		message(FATAL_ERROR "QuartzCore library not found!")
	endif()

	set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${METAL_INCLUDES} ${IOSURFACE_INCLUDES} ${QUARTZCORE_INCLUDES})
	set(LIBRARIES_LINKS ${LIBRARIES_LINKS} "${METAL_LIBRARY}" "${IOSURFACE_LIBRARY}" "${QUARTZCORE_LIBRARY}")

	find_library(VULKAN_LIBRARY NAMES MoltenVK PATHS "${PROJECT_SOURCE_DIR}/Libraries/vulkan/lib/")
else()
	find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS "${PROJECT_SOURCE_DIR}/Libraries/vulkan/lib/")
endif()

set(VULKAN_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/vulkan/include")
set(VULKAN_INCLUDE_DIR ${VULKAN_INCLUDES})

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${VULKAN_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} "${VULKAN_LIBRARY}")
message(STATUS "Vulkan: ${VULKAN_LIBRARY}")

if(NOT VULKAN_LIBRARY)
	message(FATAL_ERROR "Vulkan library not found!")
endif()

# Install
if (FLOUNDER_INSTALL)
	file(COPY "${VULKAN_INCLUDES}/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/")
	file(COPY "${VULKAN_LIBRARY}" DESTINATION "${CMAKE_INSTALL_BINDIR}/")
	file(COPY "${PROJECT_SOURCE_DIR}/Libraries/vulkan/lib/vulkan-1.lib" DESTINATION "${CMAKE_INSTALL_LIBDIR}/")
endif()
