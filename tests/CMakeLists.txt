cmake_minimum_required(VERSION 3.20)

find_library(CRITERION_LIB criterion)

if (NOT CRITERION_LIB)
    message(STATUS "Unable to find criterion library, skipping tests compilation")
else()
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
            "../src/core/ring_buffer.c"
            "../tests/core/ring_buffer.c"
            "../src/termination_handler.c"
            "../src/logger.c"
            "../src/core/reader.c"
            "../tests/core/reader.c"
            "../src/core/analyzer.c"
            "../tests/core/analyzer.c"
    )

    find_package(Threads REQUIRED)

    target_link_libraries(${PROJECT_NAME}
        criterion
        Threads::Threads
    )
endif()