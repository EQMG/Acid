set(TESTDEFERED_HEADERS_
        "Configs/ConfigManager.hpp"
        "MainRenderer.hpp"
        "MainUpdater.hpp"
        "MainWorld.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/FpsPlayer.hpp"
        "Scenes/Scene1.hpp"
        "Uis/OverlayDebug.hpp"
        )

set(TESTDEFERED_SOURCES_
        "Configs/ConfigManager.cpp"
        "Main.cpp"
        "MainRenderer.cpp"
        "MainUpdater.cpp"
        "MainWorld.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/FpsPlayer.cpp"
        "Scenes/Scene1.cpp"
        "Uis/OverlayDebug.cpp"
        )

source_group("Header Files" FILES ${TESTDEFERED_HEADERS_})
source_group("Source Files" FILES ${TESTDEFERED_SOURCES_})

set(TESTDEFERED_SOURCES
        ${TESTDEFERED_HEADERS_}
        ${TESTDEFERED_SOURCES_}
        )