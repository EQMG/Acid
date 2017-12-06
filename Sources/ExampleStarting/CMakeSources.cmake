set(EXAMPLESTARTING_HEADERS_
	"entities/EntityTreePine.hpp"
	"entities/EntitySun.hpp"
	"entities/EntityTesting.hpp"
	"uis/OverlayPaused.hpp"
	"uis/OverlayDebug.hpp"
	"FpsCamera.hpp"
	"FpsPlayer.hpp"
	"Instance.hpp"
	"ManagerRender.hpp"
	"ManagerUis.hpp"
)

set(EXAMPLESTARTING_SOURCES_
	"entities/EntityTreePine.cpp"
	"entities/EntitySun.cpp"
	"entities/EntityTesting.cpp"
	"uis/OverlayPaused.cpp"
	"uis/OverlayDebug.cpp"
	"FpsCamera.cpp"
	"FpsPlayer.cpp"
	"Instance.cpp"
	"Main.cpp"
	"ManagerRender.cpp"
	"ManagerUis.cpp"
)

source_group("Header Files" FILES ${EXAMPLESTARTING_HEADERS_})
source_group("Source Files" FILES ${EXAMPLESTARTING_SOURCES_})

set(EXAMPLESTARTING_SOURCES
	${EXAMPLESTARTING_HEADERS_}
	${EXAMPLESTARTING_SOURCES_}
)