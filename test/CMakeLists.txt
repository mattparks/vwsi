set(TEST_HEADERS
        )

set(TEST_SOURCES
        "main.cpp"
        )

set(TEST_INCLUDES "${PROJECT_SOURCE_DIR}/tests/")

add_executable(Test ${TEST_SOURCES})

add_dependencies(Test vulkanwsi)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(Test PROPERTIES PREFIX "")
endif()

target_include_directories(Test PUBLIC ${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/include/" ${TEST_INCLUDES})
target_link_libraries(Test PRIVATE ${LIBRARIES_LINKS} vulkanwsi)
