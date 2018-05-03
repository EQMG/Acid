set(TESTDEFERED_HEADERS_
        "Configs/ConfigManager.hpp"
        "Scenes/Scene1.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/FpsPlayer.hpp"
        "Scenes/ManagerUis.hpp"
        "MainRenderer.hpp"
        "MainUpdater.hpp"
        "Uis/OverlayDebug.hpp"
        )

set(TESTDEFERED_SOURCES_
        "Configs/ConfigManager.cpp"
        "Scenes/Scene1.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/FpsPlayer.cpp"
        "Scenes/ManagerUis.cpp"
        "Main.cpp"
        "MainRenderer.cpp"
        "MainUpdater.cpp"
        "Uis/OverlayDebug.cpp"
        )

source_group("Header Files" FILES ${TESTDEFERED_HEADERS_})
source_group("Source Files" FILES ${TESTDEFERED_SOURCES_})

set(TESTDEFERED_SOURCES
        ${TESTDEFERED_HEADERS_}
        ${TESTDEFERED_SOURCES_}
        )