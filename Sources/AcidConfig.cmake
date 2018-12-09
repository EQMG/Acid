# This creates the IMPORTED TARGET Acid::Acid
#
# To use this...
# target_link_libraries(yourliborexe Acid::Acid)
#
# Optionally use PRIVATE/PUBLIC based on your needs like so...
# target_link_libraries(yourliborexe PRIVATE Acid::Acid)
#
# Because it's an IMPORTED TARGET, you don't need to include its headers seperately, it's handled for you

include(CMakeFindDependencyMacro)
# Since these are required for PUBLIC usage, they must be found here
# Then people don't have to pre-emptively find_package(Threads) or Vulkan
# before find_package(Acid)
find_dependency(Threads)
find_dependency(Vulkan)
# Includes the targets which are in the same dir as this file
# So CMAKE_CURRENT_LIST_DIR will be determined when calling find_package(Acid)
include(${CMAKE_CURRENT_LIST_DIR}/AcidTargets.cmake)

if(TARGET Acid::Acid)
	# Note "Found Acid: " will be automatically prefixed by find_package_handle_standard_args
	# Example output when doing find_package(Acid)...
	# -- Found Acid: /usr/lib/libAcid.so  
	get_property(_ACID_TARGET_LOCATION TARGET Acid::Acid PROPERTY LOCATION)
endif()
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Acid
	FOUND_VAR ACID_FOUND
	REQUIRED_VARS _ACID_TARGET_LOCATION
	)
