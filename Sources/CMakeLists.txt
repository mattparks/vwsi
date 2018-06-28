include(CMakeSources.cmake)
#project(VulkanWSI)

set(WSI_INCLUDES "${PROJECT_SOURCE_DIR}/Sources/")

if(WIN32)
    add_executable(VulkanWSI WIN32 ${WSI_SOURCES})
else()
    add_executable(VulkanWSI ${WSI_SOURCES})
endif()

#add_dependencies(VulkanWSI glfw stb)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(VulkanWSI PROPERTIES PREFIX "")
endif()

target_include_directories(VulkanWSI PUBLIC ${LIBRARIES_INCLUDES} ${WSI_INCLUDES})
target_link_libraries(VulkanWSI PRIVATE ${LIBRARIES_LINKS})
