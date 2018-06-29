include(CMakeSources.cmake)
#project(vulkanwsi)

set(WSI_INCLUDES "${PROJECT_SOURCE_DIR}/include/")

add_library(vulkanwsi ${LIB_TYPE} ${WSI_SOURCES})

set_target_properties(vulkanwsi PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        FOLDER "wsi")

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(vulkanwsi PROPERTIES PREFIX "")
endif()

if (BUILD_SHARED_LIBS)
    target_compile_definitions(vulkanwsi PUBLIC
            -DWSI_EXPORTS
            )
else ()
    target_compile_definitions(vulkanwsi PUBLIC
            -DWSI_STATICLIB
            )
endif ()

target_include_directories(vulkanwsi PUBLIC ${LIBRARIES_INCLUDES} ${WSI_INCLUDES})
target_link_libraries(vulkanwsi PRIVATE ${LIBRARIES_LINKS})

# Install
if (WSI_INSTALL)
    install(TARGETS vulkanwsi
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )

    foreach (file ${WSI_HEADERS_})
        get_filename_component(dir ${file} DIRECTORY)
        install(FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wsi/${dir})
    endforeach ()
endif ()