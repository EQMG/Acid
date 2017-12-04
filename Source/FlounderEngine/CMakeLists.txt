include(CMakeSources.cmake)
#project(FlounderEngine)

set(FLOUNDERENGINE_INC
	"../../External/includes/"
	"src/"
)

include_directories(${FLOUNDERENGINE_INC})	

find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS ../../External/libs/)
find_library(GLFW_LIBRARY NAMES glfw3 glfw PATHS ../../External/libs/)
if(WIN32)
    find_library(OPENAL_LIBRARY NAMES OpenAL32 openal PATHS ../../External/libs/)
else()
    find_library(OPENAL_LIBRARY NAMES OpenAL openal PATHS ../../External/libs/)
endif()

add_library(FlounderEngine STATIC ${FLOUNDERENGINE_SRC}) # SHARED / STATIC

target_link_libraries(FlounderEngine ${VULKAN_LIBRARY})
target_link_libraries(FlounderEngine ${GLFW_LIBRARY})
target_link_libraries(FlounderEngine ${OPENAL_LIBRARY})
