set(STB_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/stb/include")
add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/stb)
set(STB_LIBRARY "stb")

list(APPEND LIBRARIES_INCLUDES ${STB_INCLUDES})
list(APPEND LIBRARIES_LINKS "${STB_LIBRARY}")

if(NOT STB_LIBRARY)
	message(FATAL_ERROR "STB library not found!")
endif()
