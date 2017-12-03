include(CMakeSources.cmake)

#project(FlounderEngine)

set(FLOUNDERENGINE_INC 
	"../../external/includes/"
	"src/"
)

include_directories(${FLOUNDERENGINE_INC})	

find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS ../../external/libs/)
find_library(GLFW_LIBRARY NAMES glfw3 glfw PATHS ../../external/libs/)
find_library(OPENAL_LIBRARY NAMES OpenAL32 openal PATHS ../../external/libs/)

add_library(FlounderEngine STATIC ${FLOUNDERENGINE_SRC}) # SHARED / STATIC
target_compile_definitions(FlounderEngine PRIVATE -DFLOUNDERENGINE_EXPORTS)

target_link_libraries(FlounderEngine PRIVATE ${VULKAN_LIBRARY})
target_link_libraries(FlounderEngine PRIVATE ${GLFW_LIBRARY})
target_link_libraries(FlounderEngine PRIVATE ${OPENAL_LIBRARY})
