set(KOSMOS_HEADERS_
	"Configs/ConfigTesting.hpp"
	"Entities/EntitySun.hpp"
	"Entities/EntityTesting.hpp"
	"Entities/EntityTreePine.hpp"
	"FpsCamera.hpp"
	"FpsPlayer.hpp"
	"Instance.hpp"
	"ManagerRender.hpp"
	"ManagerUis.hpp"
    "Uis/OverlayDebug.hpp"
	"Uis/UiInventory.hpp"
    "Uis/UiNavigation.hpp"
)

set(KOSMOS_SOURCES_
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

source_group("Header Files" FILES ${KOSMOS_HEADERS_})
source_group("Source Files" FILES ${KOSMOS_SOURCES_})

set(KOSMOS_SOURCES
	${KOSMOS_HEADERS_}
	${KOSMOS_SOURCES_}
)