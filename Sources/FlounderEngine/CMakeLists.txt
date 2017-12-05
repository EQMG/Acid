include(CMakeSources.cmake)
#project(FlounderEngine)

set(FLOUNDERENGINE_INCLUDES
	"${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/src/"
)

include_directories(${LIBRARIES_INCLUDES} ${FLOUNDERENGINE_INCLUDES})	

add_library(FlounderEngine STATIC ${FLOUNDERENGINE_SOURCES}) # SHARED / STATIC

target_link_libraries(FlounderEngine "glfw")
#target_link_libraries(FlounderEngine "openal-soft")
target_link_libraries(FlounderEngine ${VULKAN_LIBRARY})
