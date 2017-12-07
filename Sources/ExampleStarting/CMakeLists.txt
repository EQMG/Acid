include(CMakeSources.cmake)
#project(ExampleStarting)

set(EXAMPLESTARTING_INCLUDES
	"${PROJECT_SOURCE_DIR}/Sources/ExampleStarting/"
)

include_directories(${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/" ${EXAMPLESTARTING_INCLUDES})	# ${FLOUNDERENGINE_INCLUDES}

if(FLOUNDER_CONFIG_RELEASE AND WIN32)
	add_executable(ExampleStarting WIN32 ${EXAMPLESTARTING_SOURCES})
else()
	add_executable(ExampleStarting ${EXAMPLESTARTING_SOURCES})
endif()

target_link_libraries(ExampleStarting FlounderEngine)
add_dependencies(ExampleStarting FlounderEngine)
