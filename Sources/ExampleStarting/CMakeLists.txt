include(CMakeSources.cmake)
#project(ExampleStarting)

set(EXAMPLESTARTING_INCLUDES
	"${PROJECT_SOURCE_DIR}/Sources/ExampleStarting/src/"
)

include_directories(${LIBRARIES_INCLUDES} ${FLOUNDERENGINE_INCLUDES} ${EXAMPLESTARTING_INCLUDES})	

if(WIN32)
	add_executable(ExampleStarting WIN32 ${EXAMPLESTARTING_SOURCES})
else()
	add_executable(ExampleStarting ${EXAMPLESTARTING_SOURCES})
endif()

target_link_libraries(ExampleStarting FlounderEngine)
add_dependencies(ExampleStarting FlounderEngine)
