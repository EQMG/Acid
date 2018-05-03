set(TESTGUIS_HEADERS_
        "Scenes/Scene1.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/ManagerUis.hpp"
        "MainRenderer.hpp"
        "MainUpdater.hpp"
        "Uis/Navigation/ContentExit.hpp"
        "Uis/Navigation/UiNavigation.hpp"
        "Uis/Navigation/UiTab.hpp"
        "Uis/OverlayDebug.hpp"
        "Uis/UiInventory.hpp"
        )

set(TESTGUIS_SOURCES_
        "Scenes/Scene1.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/ManagerUis.cpp"
        "Main.cpp"
        "MainRenderer.cpp"
        "MainUpdater.cpp"
        "Uis/Navigation/ContentExit.cpp"
        "Uis/Navigation/UiNavigation.cpp"
        "Uis/Navigation/UiTab.cpp"
        "Uis/OverlayDebug.cpp"
        "Uis/UiInventory.cpp"
        )

source_group("Header Files" FILES ${TESTGUIS_HEADERS_})
source_group("Source Files" FILES ${TESTGUIS_SOURCES_})

set(TESTGUIS_SOURCES
        ${TESTGUIS_HEADERS_}
        ${TESTGUIS_SOURCES_}
        )