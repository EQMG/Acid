set(STB_HEADERS_
		"include/STB/stb_image.h"
        "include/STB/stb_image_write.h"
        "include/STB/stb_vorbis.h"
)

set(STB_SOURCES_
		"src/STB/stb_image.c"
        "src/STB/stb_image_write.c"
		"src/STB/stb_vorbis.c"
)

source_group("Header Files" FILES ${STB_HEADERS_})
source_group("Source Files" FILES ${STB_SOURCES_})

set(STB_SOURCES
	${STB_HEADERS_}
	${STB_SOURCES_}
)
