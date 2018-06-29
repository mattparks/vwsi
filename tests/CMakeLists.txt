#project(Test)

set(TEST_SOURCES
        "main.cpp"
        )
set(TEST_INCLUDES "${PROJECT_SOURCE_DIR}/tests/")

if(WIN32)
    add_executable(Test WIN32 ${TEST_SOURCES})
else()
    add_executable(Test ${TEST_SOURCES})
endif()

add_dependencies(Test vulkanwsi)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(Test PROPERTIES PREFIX "")
endif()

target_include_directories(Test PUBLIC ${LIBRARIES_INCLUDES} "${PROJECT_SOURCE_DIR}/include/" ${TEST_INCLUDES})
target_link_libraries(Test PRIVATE ${LIBRARIES_LINKS})
