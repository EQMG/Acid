set(VULKAN_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Libraries/vulkan/Include" PARENT_SCOPE)

if(UNIX AND APPLE)
	find_path(METAL_INCLUDE_DIR Metal.h)
	find_library(METAL_LIBRARY NAMES Metal)

	find_path(IOSURFACE_INCLUDE_DIR IOSurface.h)
	find_library(IOSURFACE_LIBRARY NAMES IOSurface)

	find_path(QUARTZCORE_INCLUDE_DIR QuartzCore.h)
	find_library(QUARTZCORE_LIBRARY NAMES QuartzCore)

	find_library(VULKAN_LIBRARY NAMES MoltenVK PATHS "${PROJECT_SOURCE_DIR}/Libraries/vulkan/Lib/" PARENT_SCOPE)
else()
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS "${PROJECT_SOURCE_DIR}/Libraries/vulkan/Lib/" PARENT_SCOPE)
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
		find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS "${PROJECT_SOURCE_DIR}/Libraries/vulkan/Lib32/" PARENT_SCOPE)
	endif()
endif()

set(VULKAN_FOUND "NO")

if(VULKAN_LIBRARY AND VULKAN_INCLUDE_DIR)
	set(VULKAN_FOUND "YES")
endif()
