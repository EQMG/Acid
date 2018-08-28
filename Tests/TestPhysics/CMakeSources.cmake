set(TESTPHYSICS_HEADERS_
		"Configs/ConfigManager.hpp"
		"MainRenderer.hpp"
		"MainUpdater.hpp"
		"Scenes/FpsCamera.hpp"
		"Scenes/FpsPlayer.hpp"
		"Scenes/Scene1.hpp"
		"Skybox/CelestialBody.hpp"
		"Skybox/SkyboxCycle.hpp"
		"Terrain/MaterialTerrain.hpp"
		"Terrain/MeshTerrain.hpp"
		"Terrain/Terrain.hpp"
		"Uis/OverlayDebug.hpp"
		"World/World.hpp"
		)

set(TESTPHYSICS_SOURCES_
		"Configs/ConfigManager.cpp"
		"Main.cpp"
		"MainRenderer.cpp"
		"MainUpdater.cpp"
		"Scenes/FpsCamera.cpp"
		"Scenes/FpsPlayer.cpp"
		"Scenes/Scene1.cpp"
		"Skybox/CelestialBody.cpp"
		"Skybox/SkyboxCycle.cpp"
		"Terrain/MaterialTerrain.cpp"
		"Terrain/MeshTerrain.cpp"
		"Terrain/Terrain.cpp"
		"TestPhysics.rc"
		"Uis/OverlayDebug.cpp"
		"World/World.cpp"
		)

source_group("Header Files" FILES ${TESTPHYSICS_HEADERS_})
source_group("Source Files" FILES ${TESTPHYSICS_SOURCES_})

set(TESTPHYSICS_SOURCES
		${TESTPHYSICS_HEADERS_}
		${TESTPHYSICS_SOURCES_}
		)