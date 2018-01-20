include(CMakeSources.cmake)
#project(FlounderEngine)

set(FLOUNDERENGINE_INCLUDES
	"${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/"
)

include_directories(${LIBRARIES_INCLUDES} ${FLOUNDERENGINE_INCLUDES})	

add_library(FlounderEngine STATIC ${FLOUNDERENGINE_SOURCES})

target_link_libraries(FlounderEngine "${STB_LIBRARY}" "${GLFW_LIBRARY}" "${OPENAL_LIBRARY}" "${VULKAN_LIBRARY}")
