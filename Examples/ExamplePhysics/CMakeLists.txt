set(_example_executable ExamplePhysics)
set(_example_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Behaviours/HeightDespawn.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Behaviours/NameTag.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Behaviours/Rotate.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Configs/ConfigManager.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainApp.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainRenderer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/FpsCamera.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/FpsPlayer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/Scene1.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Skybox/CelestialBody.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Skybox/SkyboxCycle.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Terrain/MeshTerrain.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Terrain/Terrain.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Terrain/TerrainMaterial.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/OverlayDebug.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/World/World.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainApp.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainRenderer.hpp
		)
set(_example_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Behaviours/HeightDespawn.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Behaviours/NameTag.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Behaviours/Rotate.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Configs/ConfigManager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainApp.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainRenderer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/FpsCamera.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/FpsPlayer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Scenes/Scene1.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Skybox/CelestialBody.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Skybox/SkyboxCycle.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Terrain/MeshTerrain.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Terrain/Terrain.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Terrain/TerrainMaterial.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Uis/OverlayDebug.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/World/World.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainApp.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainRenderer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/${_example_executable}.rc
		)
set(_example_third_party_headers
		${THIRD_PARTY_SOURCE_DIR}/FastNoiseLite/FastNoiseLite.h
		)

add_executable(${_example_executable}
		${_example_headers}
		${_example_sources}
		${_example_third_party_headers}
		)

target_compile_features(${_example_executable} PUBLIC c_std_11 cxx_std_17)
target_include_directories(${_example_executable}
		PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${THIRD_PARTY_SOURCE_DIR}/FastNoiseLite>
		)
target_link_libraries(${_example_executable}
		PRIVATE
		${ACID_LIBRARIES}
		)
set_target_properties(${_example_executable} PROPERTIES FOLDER "Acid/Tests")

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(${_example_executable} PROPERTIES 
			LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
			)
elseif(UNIX AND APPLE)
	set_target_properties(${_example_executable} PROPERTIES
			#RESOURCE Icon.icns
			MACOSX_BUNDLE_BUNDLE_NAME "${_example_executable}"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			#MACOSX_BUNDLE_ICON_FILE Icon.icns
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/Info.plist.in"
			)
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_example_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_example_sources})

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS ${_example_executable}
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
