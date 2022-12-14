cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "unit_tests")
project(${PROJECT_NAME} LANGUAGES C)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_sources(${PROJECT_NAME}
    PRIVATE
        "../src/core/types.c"
        "../tests/core/types.c"
)

target_link_libraries(${PROJECT_NAME}
    criterion
)