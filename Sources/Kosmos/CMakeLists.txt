include(CMakeSources.cmake)
#project(Kosmos)

set(KOSMOS_INCLUDES
	"${PROJECT_SOURCE_DIR}/Sources/Kosmos/"
)

include_directories(${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/Sources/FlounderEngine/" ${KOSMOS_INCLUDES})	# ${FLOUNDERENGINE_INCLUDES}

#if(WIN32) # FLOUNDER_CONFIG_RELEASE AND 
#	add_executable(Kosmos WIN32 ${KOSMOS_SOURCES})
#else()
	add_executable(Kosmos ${KOSMOS_SOURCES})
#endif()

target_compile_definitions(Kosmos PRIVATE -DKOSMOS_EXPORTS)

target_link_libraries(Kosmos FlounderEngine)
add_dependencies(Kosmos FlounderEngine)
