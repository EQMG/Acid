set(EXAMPLE_STARTING_HEADERS_
        "Configs/ConfigManager.hpp"
        "FpsCamera.hpp"
        "FpsPlayer.hpp"
        "Instance.hpp"
        "ManagerRender.hpp"
        "ManagerUis.hpp"
        "Uis/OverlayDebug.hpp"
        "Uis/UiInventory.hpp"
        "Uis/Navigation/UiNavigation.hpp"
        "Uis/Navigation/ContentExit.hpp"
        "Uis/Navigation/UiTab.hpp"
        )

set(EXAMPLE_STARTING_SOURCES_
        "Configs/ConfigManager.cpp"
        "FpsCamera.cpp"
        "FpsPlayer.cpp"
        "Instance.cpp"
        "Main.cpp"
        "ManagerRender.cpp"
        "ManagerUis.cpp"
        "Uis/OverlayDebug.cpp"
        "Uis/UiInventory.cpp"
        "Uis/Navigation/UiNavigation.cpp"
        "Uis/Navigation/ContentExit.cpp"
        "Uis/Navigation/UiTab.cpp"
        )

source_group("Header Files" FILES ${EXAMPLE_STARTING_HEADERS_})
source_group("Source Files" FILES ${EXAMPLE_STARTING_SOURCES_})

set(EXAMPLE_STARTING_SOURCES
        ${EXAMPLE_STARTING_HEADERS_}
        ${EXAMPLE_STARTING_SOURCES_}
        )