include(CMakeSources.cmake)
#project(FlounderEditor)

set(FLOUNDEREDITOR_INC 
	"src/"
)

include_directories(${FLOUNDEREDITOR_INC})	

if(WIN32)
	add_executable(FlounderEditor WIN32 ${FLOUNDEREDITOR_SRC})
else()
	add_executable(FlounderEditor ${FLOUNDEREDITOR_SRC})
endif()

target_compile_definitions(FlounderEditor PRIVATE -DFLOUNDEREDITOR_EXPORTS)
