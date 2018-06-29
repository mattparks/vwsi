set(WSI_HEADERS_
        "../include/wsi/vulkan_wsi.h"
)

set(WSI_SOURCES_
        "win32_imp.c"
)

source_group("Header Files" FILES ${WSI_HEADERS_})
source_group("Source Files" FILES ${WSI_SOURCES_})

set(WSI_SOURCES
        ${WSI_HEADERS_}
        ${WSI_SOURCES_}
)
