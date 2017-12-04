set(EXAMPLESTARTING_HEADERS
	"src/entities/EntityTreePine.hpp"
	"src/entities/EntitySun.hpp"
	"src/entities/EntityTesting.hpp"
	"src/uis/OverlayPaused.hpp"
	"src/FpsCamera.hpp"
	"src/FpsPlayer.hpp"
	"src/Instance.hpp"
	"src/ManagerRender.hpp"
	"src/ManagerUis.hpp"
	"src/uis/OverlayDebug.hpp"
)

set(EXAMPLESTARTING_SOURCES
	"src/entities/EntityTreePine.cpp"
	"src/entities/EntitySun.cpp"
	"src/entities/EntityTesting.cpp"
	"src/uis/OverlayPaused.cpp"
	"src/FpsCamera.cpp"
	"src/FpsPlayer.cpp"
	"src/Instance.cpp"
	"src/Main.cpp"
	"src/ManagerRender.cpp"
	"src/ManagerUis.cpp"
	"src/uis/OverlayDebug.cpp"
)

source_group("Header Files" FILES ${EXAMPLESTARTING_HEADERS})
source_group("Source Files" FILES ${EXAMPLESTARTING_SOURCES})

set(EXAMPLESTARTING_SRC
	${EXAMPLESTARTING_HEADERS}
	${EXAMPLESTARTING_SOURCES}
)