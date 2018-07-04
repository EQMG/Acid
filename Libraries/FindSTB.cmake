set(STB_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/stb/include")
set(STB_INSTALL ${FLOUNDER_INSTALL} CACHE INTERNAL "Generate installation target")
add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/stb)
set(STB_LIBRARY "STB")

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${STB_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} "${STB_LIBRARY}")

if(NOT STB_LIBRARY)
	message(FATAL_ERROR "STB library not found!")
endif()
