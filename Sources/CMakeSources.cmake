set(WSI_HEADERS_
        "Shells/Shell.hpp"
        "Shells/ShellWin32.hpp"
)

set(WSI_SOURCES_
        "Shells/Shell.cpp"
        "Shells/ShellWin32.cpp"
        "Main.cpp"
)

source_group("Header Files" FILES ${WSI_HEADERS_})
source_group("Source Files" FILES ${WSI_SOURCES_})

set(WSI_SOURCES
        ${WSI_HEADERS_}
        ${WSI_SOURCES_}
)
