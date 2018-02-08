set(FLOUNDEREDITOR_HEADERS_
        )

set(FLOUNDEREDITOR_SOURCES_
        "Main.cpp"
        )

source_group("Header Files" FILES ${FLOUNDEREDITOR_HEADERS_})
source_group("Source Files" FILES ${FLOUNDEREDITOR_SOURCES_})

set(FLOUNDEREDITOR_SOURCES
        ${FLOUNDEREDITOR_HEADERS_}
        ${FLOUNDEREDITOR_SOURCES_}
        )