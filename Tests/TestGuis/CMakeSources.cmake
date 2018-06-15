set(TESTGUIS_HEADERS_
        "MainRenderer.hpp"
        "MainUpdater.hpp"
        "MainWorld.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/Scene1.hpp"
        "Scenes/Uis/Navigation/ContentExit.hpp"
        "Scenes/Uis/Navigation/UiNavigation.hpp"
        "Scenes/Uis/Navigation/UiTab.hpp"
        "Scenes/Uis/OverlayDebug.hpp"
        "Scenes/Uis/UiInventory.hpp"
        )

set(TESTGUIS_SOURCES_
        "Main.cpp"
        "MainRenderer.cpp"
        "MainUpdater.cpp"
        "MainWorld.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/Scene1.cpp"
        "Scenes/Uis/Navigation/ContentExit.cpp"
        "Scenes/Uis/Navigation/UiNavigation.cpp"
        "Scenes/Uis/Navigation/UiTab.cpp"
        "Scenes/Uis/OverlayDebug.cpp"
        "Scenes/Uis/UiInventory.cpp"
        )

source_group("Header Files" FILES ${TESTGUIS_HEADERS_})
source_group("Source Files" FILES ${TESTGUIS_SOURCES_})

set(TESTGUIS_SOURCES
        ${TESTGUIS_HEADERS_}
        ${TESTGUIS_SOURCES_}
        )