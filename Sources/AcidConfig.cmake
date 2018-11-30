include(CMakeFindDependencyMacro)
# Since these are required for PUBLIC usage, they must be found here
# Then people don't have to pre-emptively find_package(Threads) or Vulkan
# before find_package(Acid)
find_dependency(Threads REQUIRED)
find_dependency(Vulkan REQUIRED)
# Includes the targets which are in the same dir as this file
# So CMAKE_CURRENT_LIST_DIR will be determined when calling find_package(Acid)
include(${CMAKE_CURRENT_LIST_DIR}/AcidTargets.cmake)
