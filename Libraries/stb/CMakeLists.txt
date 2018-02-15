include(CMakeSources.cmake)
#project(STB)

set(STB_INCLUDES "${PROJECT_SOURCE_DIR}/include/")

include_directories(${STB_INCLUDES})

add_library(stb ${LIB_TYPE} ${STB_SOURCES})

if(BUILD_SHARED_LIBS)
	set_target_properties(stb PROPERTIES PREFIX "")
endif()
