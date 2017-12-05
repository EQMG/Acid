set(GLFW_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/glfw/include")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(glfw)

# set(ALSOFT_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/openal-soft/include")
# set(ALSOFT_UTILS OFF CACHE INTERNAL "Build and install utility programs")
# set(ALSOFT_EXAMPLES OFF CACHE INTERNAL "Build and install example programs")
# set(ALSOFT_TESTS OFF CACHE INTERNAL "Build and install test programs")
# set(ALSOFT_INSTALL OFF CACHE INTERNAL "Install headers and libraries")
# add_subdirectory(openal-soft)

set(VULKAN_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/vulkan/include")
find_library(VULKAN_LIBRARY NAMES vulkan-1 PATHS "vulkan/libs/")
# add_subdirectory(vulkan)

set(LIBRARIES_INCLUDES ${GLFW_INCLUDES} ${VULKAN_INCLUDES}) # ${ALSOFT_INCLUDES} 
