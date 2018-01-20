include(CMakeSources.cmake)
#project(STB)

set(STB_INCLUDES
	"${PROJECT_SOURCE_DIR}/include/"
)

include_directories(${STB_INCLUDES})

add_library(stb STATIC ${STB_SOURCES})
