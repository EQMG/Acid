include(CMakeSources.cmake)

project(FlounderEngine)

set(FlounderEngine_INC 
	"../../external/includes"
	"./src/"
)

include_directories(${FlounderEngine_INC})	

find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS ../../external/libs/)
find_library(GLFW_LIBRARY NAMES glfw3 glfw PATHS ../../external/libs/)
find_library(OPENAL_LIBRARY NAMES OpenAL32 openal PATHS ../../external/libs/)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DFLOUNDER_CONFIG_DEBUG)
add_definitions(-DFLOUNDER_PLATFORM_WINDOWS)

add_library(FlounderEngine SHARED ${FLOUNDERENGINE_SRC})

target_link_libraries(FlounderEngine PRIVATE ${VULKAN_LIBRARY})
target_link_libraries(FlounderEngine PRIVATE ${GLFW_LIBRARY})
target_link_libraries(FlounderEngine PRIVATE ${OPENAL_LIBRARY})
