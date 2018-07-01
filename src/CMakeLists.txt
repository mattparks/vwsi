set(VWSI_HEADERS
        "../include/VWSI/vulkan_wsi.h"
        )

set(VWSI_SOURCES
        "vulkan_wsi.c"
        )

set(VWSI_INCLUDES "${PROJECT_SOURCE_DIR}/include/")

add_library(VWSI ${LIB_TYPE} ${VWSI_HEADERS} ${VWSI_SOURCES})

set_target_properties(VWSI PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        FOLDER "VWSI")

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(VWSI PROPERTIES PREFIX "")
endif()

target_include_directories(VWSI PUBLIC ${LIBRARIES_INCLUDES} ${VWSI_INCLUDES})
target_link_libraries(VWSI PRIVATE ${LIBRARIES_LINKS})

# Install
if (VWSI_INSTALL)
    install(TARGETS VWSI
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )

    foreach (file ${VWSI_HEADERS})
        get_filename_component(dir ${file} DIRECTORY)
        install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VWSI/${dir})
    endforeach ()
endif ()