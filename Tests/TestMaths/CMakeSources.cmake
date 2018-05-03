set(TESTMATHS_HEADERS_
        )

set(TESTMATHS_SOURCES_
        "Main.cpp"
        )

source_group("Header Files" FILES ${TESTMATHS_HEADERS_})
source_group("Source Files" FILES ${TESTMATHS_SOURCES_})

set(TESTMATHS_SOURCES
        ${TESTMATHS_HEADERS_}
        ${TESTMATHS_SOURCES_}
        )