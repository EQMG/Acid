include(CMakeSources.cmake)
#project(FlounderEditor)

set(FLOUNDEREDITOR_INCLUDES
	"${PROJECT_SOURCE_DIR}/Sources/FlounderEditor/src/"
)

include_directories(${FLOUNDEREDITOR_INCLUDES})	

if(WIN32)
	add_executable(FlounderEditor WIN32 ${FLOUNDEREDITOR_SOURCES})
else()
	add_executable(FlounderEditor ${FLOUNDEREDITOR_SOURCES})
endif()

target_compile_definitions(FlounderEditor PRIVATE -DFLOUNDEREDITOR_EXPORTS)
