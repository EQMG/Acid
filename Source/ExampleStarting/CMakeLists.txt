include(CMakeSources.cmake)

#project(ExampleStarting)

set(EXAMPLESTARTING_INC 
	"../../external/includes/"
	"../FlounderEngine/src/"
	"src/"
)

include_directories(${EXAMPLESTARTING_INC})	

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DFLOUNDER_CONFIG_DEBUG)
add_definitions(-DFLOUNDER_PLATFORM_WINDOWS)

#add_library(ExampleStarting SHARED ${EXAMPLESTARTING_SRC})
if(WIN32)
	add_executable(ExampleStarting WIN32 ${EXAMPLESTARTING_SRC})
else()
	add_executable(ExampleStarting ${EXAMPLESTARTING_SRC})
endif()
target_compile_definitions(ExampleStarting PRIVATE -DEXAMPLE_STARTING_EXPORTS)

target_link_libraries(ExampleStarting FlounderEngine)
