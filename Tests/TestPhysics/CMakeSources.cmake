set(TESTPHYSICS_HEADERS_
        "Skybox/CelestialBody.hpp"
        "Skybox/SkyboxCycle.hpp"
        "Configs/ConfigManager.hpp"
        "MainRenderer.hpp"
        "MainUpdater.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/FpsPlayer.hpp"
        "Scenes/Scene1.hpp"
        "Uis/OverlayDebug.hpp"
        "World/World.hpp"
        )

set(TESTPHYSICS_SOURCES_
        "TestPhysics.rc"
        "Skybox/CelestialBody.cpp"
        "Skybox/SkyboxCycle.cpp"
        "Configs/ConfigManager.cpp"
        "Main.cpp"
        "MainRenderer.cpp"
        "MainUpdater.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/FpsPlayer.cpp"
        "Scenes/Scene1.cpp"
        "Uis/OverlayDebug.cpp"
        "World/World.cpp"
        )

source_group("Header Files" FILES ${TESTPHYSICS_HEADERS_})
source_group("Source Files" FILES ${TESTPHYSICS_SOURCES_})

set(TESTPHYSICS_SOURCES
        ${TESTPHYSICS_HEADERS_}
        ${TESTPHYSICS_SOURCES_}
        )