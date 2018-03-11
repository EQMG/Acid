set(STB_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/stb/include")
add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/stb)
set(STB_LIBRARY "stb")

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${STB_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} "${STB_LIBRARY}")

if(NOT STB_LIBRARY)
	message(FATAL_ERROR "STB library not found!")
endif()
