set(TESTDEFERED_HEADERS_
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

set(TESTDEFERED_SOURCES_
        "TestDeferred.rc"
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

source_group("Header Files" FILES ${TESTDEFERED_HEADERS_})
source_group("Source Files" FILES ${TESTDEFERED_SOURCES_})

set(TESTDEFERED_SOURCES
        ${TESTDEFERED_HEADERS_}
        ${TESTDEFERED_SOURCES_}
        )