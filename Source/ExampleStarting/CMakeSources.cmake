set(EXAMPLESTARTING_INC_NOFILTER
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

set(EXAMPLESTARTING_INC_MANAGERS
)

set(EXAMPLESTARTING_SRC_NOFILTER
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

set(EXAMPLESTARTING_SRC_MANAGERS
)

source_group("Header Files" FILES ${EXAMPLESTARTING_INC_NOFILTER})
source_group("Header Files//Managers" FILES ${EXAMPLESTARTING_INC_MANAGERS})
source_group("Source Files" FILES ${EXAMPLESTARTING_SRC_NOFILTER})
source_group("Source Files//Managers" FILES ${EXAMPLESTARTING_SRC_MANAGERS})

set(EXAMPLESTARTING_SRC
	${EXAMPLESTARTING_INC_NOFILTER}
	${EXAMPLESTARTING_SRC_NOFILTER}
	${EXAMPLESTARTING_INC_MANAGERS}
	${EXAMPLESTARTING_SRC_MANAGERS}
)