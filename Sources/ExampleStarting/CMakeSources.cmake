set(EXAMPLE_STARTING_HEADERS_
        "Configs/ConfigManager.hpp"
        "Scenes/Scene1.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/FpsPlayer.hpp"
        "ManagerRender.hpp"
        "Uis/ManagerUis.hpp"
        "Uis/Navigation/ContentExit.hpp"
        "Uis/Navigation/UiNavigation.hpp"
        "Uis/Navigation/UiTab.hpp"
        "Uis/OverlayDebug.hpp"
        "Uis/UiInventory.hpp"
)

set(EXAMPLE_STARTING_SOURCES_
        "Configs/ConfigManager.cpp"
        "Scenes/Scene1.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/FpsPlayer.cpp"
        "Main.cpp"
        "ManagerRender.cpp"
        "Uis/ManagerUis.cpp"
        "Uis/Navigation/ContentExit.cpp"
        "Uis/Navigation/UiNavigation.cpp"
        "Uis/Navigation/UiTab.cpp"
        "Uis/OverlayDebug.cpp"
        "Uis/UiInventory.cpp"
)

source_group("Header Files" FILES ${EXAMPLE_STARTING_HEADERS_})
source_group("Source Files" FILES ${EXAMPLE_STARTING_SOURCES_})

set(EXAMPLE_STARTING_SOURCES
        ${EXAMPLE_STARTING_HEADERS_}
        ${EXAMPLE_STARTING_SOURCES_}
)