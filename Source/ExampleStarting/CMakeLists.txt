include(CMakeSources.cmake)

#project(ExampleStarting)

set(EXAMPLESTARTING_INC 
	"../../external/includes/"
	"../FlounderEngine/src/"
	"src/"
)

include_directories(${EXAMPLESTARTING_INC})	

if(WIN32)
	add_executable(ExampleStarting WIN32 ${EXAMPLESTARTING_SRC})
else()
	add_executable(ExampleStarting ${EXAMPLESTARTING_SRC})
endif()

target_compile_definitions(ExampleStarting PRIVATE -DEXAMPLESTARTING_EXPORTS)

target_link_libraries(ExampleStarting FlounderEngine)
