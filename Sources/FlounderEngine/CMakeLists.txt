include(CMakeSources.cmake)
#project(FlounderEngine)

set(FLOUNDERENGINE_INCLUDES
	"${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/"
)

include_directories(${LIBRARIES_INCLUDES} ${FLOUNDERENGINE_INCLUDES})	

add_library(FlounderEngine STATIC ${FLOUNDERENGINE_SOURCES}) # SHARED / STATIC

target_link_libraries(FlounderEngine "${GLFW_LIBRARY}" "${OPENAL_LIBRARY}" "${VULKAN_LIBRARY}")
