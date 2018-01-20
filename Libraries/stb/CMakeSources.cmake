set(STB_HEADERS_
	"include/STB/stb_image.h"
)

set(STB_SOURCES_
	"src/stb_image.c"
)

source_group("Header Files" FILES ${STB_HEADERS_})
source_group("Source Files" FILES ${STB_SOURCES_})

set(STB_SOURCES
	${STB_HEADERS_}
	${STB_SOURCES_}
)
