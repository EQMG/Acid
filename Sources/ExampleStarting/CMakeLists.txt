include(CMakeSources.cmake)
#project(ExampleStarting)

set(EXAMPLESTARTING_INCLUDES
	"${PROJECT_SOURCE_DIR}/Sources/ExampleStarting/"
)

include_directories(${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/" ${EXAMPLESTARTING_INCLUDES})	# ${FLOUNDERENGINE_INCLUDES}

if(WIN32) # FLOUNDER_CONFIG_RELEASE AND 
	add_executable(ExampleStarting WIN32 ${EXAMPLESTARTING_SOURCES})
else()
	add_executable(ExampleStarting ${EXAMPLESTARTING_SOURCES} Uis/UiNavigation.cpp Uis/UiNavigation.hpp)
endif()

target_compile_definitions(ExampleStarting PRIVATE -DEXAMPLESTARTING_EXPORTS)

target_link_libraries(ExampleStarting FlounderEngine)
add_dependencies(ExampleStarting FlounderEngine)
