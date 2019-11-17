# - Try to find OpenAL-Soft
#  and define
#
#  OPENALSOFT_FOUND          - System has OpenAL-Soft
#  OPENALSOFT_INCLUDE_DIR    - The OpenAL-Soft include directories
#  OPENALSOFT_LIBRARY        - The OpenAL-Soft libraries

IF(NOT APPLE)
  find_path(OPENALSOFT_INCLUDE_DIR al.h
    PATHS
      # Fink
      /sw
      # DarwinPorts
      /opt/local
      # Blastwave
      /opt/csw
      /opt
      # FreeBSD
      /usr/local/include/AL
      # Linux
      /usr/include/AL
      # Windows SDK Include PATH
      /include/AL
  )

  find_library(OPENALSOFT_LIBRARY
    NAMES libopenal openal libOpenAL32 OpenAL32 soft_oal.dll
    PATHS
      ${OPENALSOFT_INCLUDE_DIR}/../lib
      /usr/local/opt/openal-soft/lib
      /lib
      /usr/lib
      /usr/local/lib
  )
ENDIF(NOT APPLE)

IF (APPLE)
  SET(OPENALSOFT_INCLUDE_DIR "/usr/local/opt/openal-soft/include/")
  find_library(OPENALSOFT_LIBRARY
  NAMES libopenal.dylib openal.dylib
  PATHS
    /usr/local/opt/openal-soft/lib
  # for debugging
  message(STATUS "OPENALSOFT_INCLUDE_DIR  = '${OPENALSOFT_INCLUDE_DIR}'")
  message(STATUS "OPENALSOFT_LIBRARY      = '${OPENALSOFT_LIBRARY}'")
  )
ENDIF (APPLE)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(OPENALSOFT REQUIRED_VARS OPENALSOFT_LIBRARY OPENALSOFT_INCLUDE_DIR)

mark_as_advanced(OPENALSOFT_INCLUDE_DIR OPENALSOFT_LIBRARY)

# for debugging
#message(STATUS "OPENALSOFT_INCLUDE_DIR  = '${OPENALSOFT_INCLUDE_DIR}'")
#message(STATUS "OPENALSOFT_LIBRARY      = '${OPENALSOFT_LIBRARY}'")
