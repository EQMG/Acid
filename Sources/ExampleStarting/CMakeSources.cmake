set(EXAMPLESTARTING_HEADERS_
	"Entities/EntitySun.hpp"
	"Entities/EntityTesting.hpp"
	"Entities/EntityTreePine.hpp"
	"ConfigTesting.hpp"
	"FpsCamera.hpp"
	"FpsPlayer.hpp"
	"Instance.hpp"
	"ManagerRender.hpp"
	"ManagerUis.hpp"
    "Uis/OverlayDebug.hpp"
	"Uis/UiInventory.hpp"
    "Uis/UiNavigation.hpp"
)

set(EXAMPLESTARTING_SOURCES_
	"Entities/EntitySun.cpp"
	"Entities/EntityTesting.cpp"
	"Entities/EntityTreePine.cpp"
	"FpsCamera.cpp"
	"FpsPlayer.cpp"
	"Instance.cpp"
	"Main.cpp"
	"ManagerRender.cpp"
	"ManagerUis.cpp"
	"Uis/OverlayDebug.cpp"
	"Uis/UiInventory.cpp"
    "Uis/UiNavigation.cpp"
)

source_group("Header Files" FILES ${EXAMPLESTARTING_HEADERS_})
source_group("Source Files" FILES ${EXAMPLESTARTING_SOURCES_})

set(EXAMPLESTARTING_SOURCES
	${EXAMPLESTARTING_HEADERS_}
	${EXAMPLESTARTING_SOURCES_}
)